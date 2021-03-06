/**
 Copyright 2016 Otavio Rodolfo Piske

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#include "stomp_messenger.h"

stomp_messenger_t *stomp_messenger_init()
{
    stomp_messenger_t *ret = malloc(sizeof (stomp_messenger_t));
    if (!ret) {
        fprintf(stderr, "Unable to allocate memory for messenger structure");

        return NULL;
    }


    apr_status_t status = apr_initialize();
    if (status != APR_SUCCESS) {
        // TODO: collect the error from APR

        stomp_status_set(&ret->status, STOMP_FAILURE,
                "Unable to initialize the Apache Portal Runtime");

        return ret;
    }

    ret->connection = NULL;
    ret->status.message = NULL;
    stomp_status_success(&ret->status);

    status = apr_pool_create(&ret->pool, NULL);
    if (status != APR_SUCCESS) {
        // TODO: collect the error from APR

        stomp_status_set(&ret->status, STOMP_FAILURE,
                "Unable to initialize the APR pool");

        return ret;
    }

    ret->exchange_properties = malloc(sizeof(stomp_exchange_properties_t));
    if (!ret->exchange_properties) {
        stomp_status_set(&ret->status, STOMP_FAILURE,
                "Unable to initialize the outer exchange properties structure");

        return ret;
    }


    ret->exchange_properties->hash = (apr_hash_t *) apr_hash_make(ret->pool);
    if (!ret->exchange_properties->hash) {
        stomp_status_set(&ret->status, STOMP_FAILURE,
                "Unable to initialize the exchange properties structure");

        return ret;
    }

    // For Apache Artemis complains about / on the beginning of the path
    ret->options |= STOMP_OPT_STRIP_ROOT;
    return ret;
}

void stomp_messenger_destroy(stomp_messenger_t **messenger)
{
    free((*messenger)->exchange_properties);

    stomp_status_reset(&(*messenger)->status);

    if ((*messenger) != NULL) {
        apr_pool_destroy((*messenger)->pool);
    }

    // TODO: disconnect, maybe?
    free(*messenger);
    *messenger = NULL;

    apr_terminate();
}

void stomp_messenger_set_timeout(stomp_messenger_t *messenger,
                                              int32_t timeout)
{
#if defined(apr_time_from_msec)
    stomp_engine_set_timeout(messenger->connection, apr_time_from_msec(timeout));
#else
    stomp_engine_set_timeout(messenger->connection, (timeout) * 1000);
#endif
}

stomp_status_code_t stomp_set_endpoint(stomp_messenger_t *messenger, const char *uri)
{
    //fprintf(stderr, "Parsing URI: %s\n", uri);
    apr_status_t stat = apr_uri_parse(messenger->pool, uri, &messenger->uri);
    fprintf(stderr, "Done: %s:%d [%s]\n", messenger->uri.hostname, messenger->uri.port,
	    messenger->uri.path);

    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to parse the URI");

        return STOMP_FAILURE;
    }

    if (!messenger->uri.path) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: null");

        return STOMP_FAILURE;
    }

    if (strlen(messenger->uri.path) == 0) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: empty");

        return STOMP_FAILURE;
    }


    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_connect(stomp_messenger_t *messenger,
        stomp_connection_header_t *header, int32_t timeout)
{
    stomp_frame conn_frame;

    fprintf(stderr, "Connecting to %s\n", messenger->uri.hostname);

    apr_status_t stat = stomp_engine_connect(&messenger->connection,
            messenger->uri.hostname, messenger->uri.port, messenger->pool);

    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to connect to the broker service: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    stomp_messenger_set_timeout(messenger, timeout);

    conn_frame.command = "CONNECT";
    conn_frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) {
        if (header->credentials != NULL) {
            apr_hash_set(conn_frame.headers, "login", APR_HASH_KEY_STRING,
                    header->credentials->username);
            apr_hash_set(conn_frame.headers, "passcode", APR_HASH_KEY_STRING,
                    header->credentials->password);
        }

        /*
         * For now we will just silently ignore the heartbeats until I can
         * investigate further how to implement this properly
         */
    }

    apr_hash_set(conn_frame.headers, "accept-version", APR_HASH_KEY_STRING,
            "1.2");

    conn_frame.body = NULL;
    conn_frame.body_length = -1;

    stat = stomp_write(messenger->connection, &conn_frame,
            messenger->pool);

    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (!stomp_io_can_read(messenger->connection)) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Timed out while trying to read data");

        return STOMP_FAILURE;
    }


    stomp_frame *reply_frame;
    stat = stomp_read(messenger->connection, &reply_frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to read the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    const char *CONN_REPLY_STR = "CONNECTED";

    if (strncmp(reply_frame->command, CONN_REPLY_STR, strlen(CONN_REPLY_STR)) != 0) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection reply: %s", reply_frame->command);

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

static stomp_status_code_t stomp_process_receipt(stomp_messenger_t *messenger) {
    fprintf(stderr, "Waiting for receipt\n");
    const char *DISCONN_REPLY_STR = "RECEIPT";

    if (!stomp_io_can_read(messenger->connection)) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Timed out while trying to read data");

        return STOMP_FAILURE;
    }

    stomp_frame *reply_frame;
    apr_status_t stat = stomp_read(messenger->connection, &reply_frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to read the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (strncmp(reply_frame->command, DISCONN_REPLY_STR, strlen(DISCONN_REPLY_STR)) != 0) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid disconnection reply: %s (%s)", reply_frame->command,
		(reply_frame->body != NULL ? reply_frame->body : "null"));

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

static inline void stomp_write_receipt(stomp_messenger_t *messenger,
        stomp_frame *frame, receipt_t receipt) {
     if (receipt > 0) {
        apr_hash_set(frame->headers, "receipt", APR_HASH_KEY_STRING,
                apr_itoa(messenger->pool, receipt));
    }
}

stomp_status_code_t stomp_disconnect(stomp_messenger_t *messenger,
                                  stomp_disconnection_header_t *header) {
    stomp_frame frame;

    frame.command = "DISCONNECT";
    frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) {
        stomp_write_receipt(messenger, &frame, header->receipt);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_subscribe(stomp_messenger_t *messenger,
        stomp_subscription_header_t *header)
{
    stomp_frame frame;

    frame.command = "SUBSCRIBE";
    frame.headers = apr_hash_make(messenger->pool);


    if (messenger->options & STOMP_OPT_STRIP_ROOT) {
	apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            &messenger->uri.path[1]);
    }
    else {
	apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            messenger->uri.path);
    }

    if (header == NULL) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "A STOMP subscription requires a subscription ID");

        return STOMP_FAILURE;
    }


    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            apr_itoa(messenger->pool, header->id));

    stomp_write_receipt(messenger, &frame, header->receipt);


    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_unsubscribe(stomp_messenger_t *messenger,
        stomp_subscription_header_t *header)
{
    stomp_frame frame;

    frame.command = "UNSUBSCRIBE";
    frame.headers = apr_hash_make(messenger->pool);

    apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            messenger->uri.path);
    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            apr_itoa(messenger->pool, header->id));

    if (header != NULL) {
        stomp_write_receipt(messenger, &frame, header->receipt);
    }

    // TODO: handle the ACK

    frame.body_length = -1;
    frame.body = NULL;


    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

static inline void stomp_write_message_id(stomp_messenger_t *messenger,
        stomp_frame *frame,
        message_id_t message_id)
{
    if (message_id > -1) {
        apr_hash_set(frame->headers, "id", APR_HASH_KEY_STRING,
                apr_itoa(messenger->pool, message_id));
    }
}
static inline void stomp_write_transaction_id(stomp_messenger_t *messenger,
        stomp_frame *frame,
        transaction_id_t transaction_id)
{
    if (transaction_id > -1) {
        apr_hash_set(frame->headers, "transaction", APR_HASH_KEY_STRING,
                apr_itoa(messenger->pool, transaction_id));
    }
}

stomp_status_code_t stomp_ack(stomp_messenger_t *messenger,
        stomp_ack_header_t *header)
{
    stomp_frame frame;

    frame.command = "ACK";
    frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) {
        stomp_write_message_id(messenger, &frame, header->message_id);
        stomp_write_transaction_id(messenger, &frame, header->transaction_id);
        stomp_write_receipt(messenger, &frame, header->receipt);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_nack(stomp_messenger_t *messenger,
        stomp_ack_header_t *header)
{
    stomp_frame frame;

    frame.command = "NACK";
    frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) {
        stomp_write_message_id(messenger, &frame, header->message_id);
        stomp_write_transaction_id(messenger, &frame, header->transaction_id);
        stomp_write_receipt(messenger, &frame, header->receipt);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

static stomp_status_code_t stomp_transaction(stomp_messenger_t *messenger,
        stomp_transaction_header_t *header,
        char *command)
{
    stomp_frame frame;

    frame.command = command;
    frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) {
        stomp_write_transaction_id(messenger, &frame, header->transaction_id);
        stomp_write_receipt(messenger, &frame, header->receipt);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_begin(stomp_messenger_t *messenger,
        stomp_transaction_header_t *header)
{
    return stomp_transaction(messenger, header, "BEGIN");
}

stomp_status_code_t stomp_commit(stomp_messenger_t *messenger,
        stomp_transaction_header_t *header)
{
    return stomp_transaction(messenger, header, "COMMIT");
}

stomp_status_code_t stomp_abort(stomp_messenger_t *messenger,
        stomp_transaction_header_t *header)
{
    return stomp_transaction(messenger, header, "ABORT");
}

stomp_status_code_t stomp_send(stomp_messenger_t *messenger,
                                  stomp_send_header_t *header,
                                  stomp_message_t *message)
{
    stomp_frame frame;
    frame.command = "SEND";
    frame.headers = apr_hash_copy(message->pool, messenger->exchange_properties->hash);


    if (messenger->options & STOMP_OPT_STRIP_ROOT) {
	apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            &messenger->uri.path[1]);
    }
    else {
	apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            messenger->uri.path);
    }

    frame.body_length = message->size;
    frame.body = message->body;

    if (header != NULL) {
        stomp_write_transaction_id(messenger, &frame, header->transaction_id);
        stomp_write_receipt(messenger, &frame, header->receipt);
    }


    apr_status_t stat = stomp_write(messenger->connection, &frame, message->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_t engine_status = stomp_engine_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection: %s",
                         engine_status.message);

        return STOMP_FAILURE;
    }

    if (header != NULL && header->receipt > 0) {
        return stomp_process_receipt(messenger);
    }


    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_receive(stomp_messenger_t *messenger,
                                  stomp_receive_header_t *header,
                                  stomp_message_t *message)
{
    stomp_frame *frame;

    /* 
     * If timed out reading data, maybe there's nothing on the broker. So try
     * ignore it and leave for the application to handle
     */
    if (!stomp_io_can_read(messenger->connection)) {
        stomp_status_set(&messenger->status, STOMP_SUCCESS | STOMP_NO_DATA,
                "Timed out while trying to read data");

        return STOMP_SUCCESS | STOMP_NO_DATA;
    }

    apr_status_t stat = stomp_read(messenger->connection, &frame, message->pool);
    if (stat == APR_SUCCESS) {
        stomp_message_format(message, frame->body, frame->body_length);
        messenger->exchange_properties->hash = apr_hash_overlay(messenger->pool,
                frame->headers, messenger->exchange_properties->hash);

        if (strncmp(frame->command, "MESSAGE", strlen("MESSAGE")) == 0) {
            return STOMP_SUCCESS;
        }

        return STOMP_FAILURE;
    }
    else {
        if (stat == APR_TIMEUP) {
            return STOMP_SUCCESS;
        }

	stomp_status_t io_status = stomp_io_last_status();

        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to read the frame data from the underlying connection: %s",
			 io_status.message);

        return STOMP_FAILURE;
    }

}