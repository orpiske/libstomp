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
    stomp_status_success(&ret->status);
    
    status = apr_pool_create(&ret->pool, NULL);
    if (status != APR_SUCCESS) {
        // TODO: collect the error from APR

        stomp_status_set(&ret->status, STOMP_FAILURE,
                "Unable to initialize the APR pool");

        return ret;
    }

    

    return ret;
}

void stomp_messenger_destroy(stomp_messenger_t **messenger)
{
    // TODO: disconnect, maybe?
    free(*messenger);
    *messenger = NULL;
}

stomp_status_code_t stomp_set_endpoint(stomp_messenger_t *messenger, const char *uri)
{
    //fprintf(stderr, "Parsing URI: %s", uri);
    apr_status_t stat = apr_uri_parse(messenger->pool, *uri, &messenger->uri);
    //fprintf(stderr, "Done: %s:%d\n", messenger->uri.hostname, messenger->uri.port);
    
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to parse the URI");

        return STOMP_FAILURE;
    }
}

stomp_status_code_t stomp_connect(stomp_messenger_t *messenger,
        stomp_connection_header_t *header)
{
    stomp_frame conn_frame;
    
    fprintf(stderr, "Connecting to %s\n", messenger->uri.hostname);
    
    apr_status_t stat = stomp_engine_connect(&messenger->connection, 
            messenger->uri.hostname, messenger->uri.port, messenger->pool);
    
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to connect to the broker service");

        return STOMP_FAILURE;
    }

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
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }


    stomp_frame *reply_frame;
    stat = stomp_read(messenger->connection, &reply_frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

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

stomp_status_code_t stomp_disconnect(stomp_messenger_t *messenger, 
                                  stomp_disconnection_header_t *header) {
    stomp_frame frame;

    frame.command = "DISCONNECT";
    frame.headers = apr_hash_make(messenger->pool);

    if (header != NULL) { 
        apr_hash_set(frame.headers, "receipt", APR_HASH_KEY_STRING,
                apr_itoa(messenger->pool, header->receipt));
    }
  
    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }
    
    if (header != NULL) {
        const char *DISCONN_REPLY_STR = "RECEIPT";

        stomp_frame *reply_frame;
        stat = stomp_read(messenger->connection, &reply_frame, messenger->pool);
        if (stat != APR_SUCCESS) {
            stomp_status_set(&messenger->status, STOMP_FAILURE,
                    "Unable to read the frame data to the underlying connection: %s", 
                    reply_frame->command);

            return STOMP_FAILURE;
        }

        if (strncmp(reply_frame->command, DISCONN_REPLY_STR, strlen(DISCONN_REPLY_STR)) != 0) {
            stomp_status_set(&messenger->status, STOMP_FAILURE,
                    "Invalid disconnection reply: %s", reply_frame->command);

            return STOMP_FAILURE;
        }
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_subscribe(stomp_messenger_t *messenger,
        stomp_subscription_header_t *header)
{
    stomp_frame frame;

    frame.command = "SUBSCRIBE";
    frame.headers = apr_hash_make(messenger->pool);

    if (!header->destination) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: null");

        return STOMP_FAILURE;
    }

    if (strlen(header->destination) == 0) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: empty");

        return STOMP_FAILURE;
    }

    apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            header->destination);
    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            apr_itoa(messenger->pool, header->id));

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_unsubscribe(stomp_messenger_t *messenger,
        stomp_subscription_header_t *header)
{
    stomp_frame frame;

    frame.command = "UNSUBSCRIBE";
    frame.headers = apr_hash_make(messenger->pool);

    if (!header->destination) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: null");

        return STOMP_FAILURE;
    }

    if (strlen(header->destination) == 0) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Invalid connection destination: empty");

        return STOMP_FAILURE;
    }

    apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING,
            header->destination);
    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            header->id);

    // TODO: handle the ACK

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_ack(stomp_messenger_t *messenger,
        stomp_ack_header_t *header)
{
    stomp_frame frame;

    frame.command = "ACK";
    frame.headers = apr_hash_make(messenger->pool);

    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            header->message_id);

    if (header->transaction_id > -1) {
        apr_hash_set(frame.headers, "transaction", APR_HASH_KEY_STRING,
                header->transaction_id);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_nack(stomp_messenger_t *messenger,
        stomp_ack_header_t *header)
{
    stomp_frame frame;

    frame.command = "NACK";
    frame.headers = apr_hash_make(messenger->pool);

    apr_hash_set(frame.headers, "id", APR_HASH_KEY_STRING,
            header->message_id);

    if (header->transaction_id > -1) {
        apr_hash_set(frame.headers, "transaction", APR_HASH_KEY_STRING,
                header->transaction_id);
    }

    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

static stomp_status_code_t stomp_transaction(stomp_messenger_t *messenger,
        stomp_transaction_header_t *header,
        const char *command)
{
    stomp_frame frame;

    frame.command = command;
    frame.headers = apr_hash_make(messenger->pool);

    apr_hash_set(frame.headers, "transaction", APR_HASH_KEY_STRING,
            header->transaction_id);


    frame.body_length = -1;
    frame.body = NULL;

    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
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
    frame.headers = apr_hash_make(messenger->pool);
    
    
    apr_hash_set(frame.headers, "destination", APR_HASH_KEY_STRING, 
            messenger->uri.path);
    

    frame.body_length = message->size;
    frame.body = message->body;
    
    if (header->transaction_id > -1) {
        apr_hash_set(frame.headers, "transaction", APR_HASH_KEY_STRING,
                header->transaction_id);
    }
    
    apr_status_t stat = stomp_write(messenger->connection, &frame, messenger->pool);
    if (stat != APR_SUCCESS) {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to write the frame data to the underlying connection");

        return STOMP_FAILURE;
    }

    return STOMP_SUCCESS;
}

stomp_status_code_t stomp_receive(stomp_messenger_t *messenger, 
                                  stomp_receive_header_t *header,
                                  stomp_message_t *message)
{
    stomp_frame *frame;
    
    apr_status_t stat = stomp_read(messenger->connection, &frame, messenger->pool);
    if (stat == APR_SUCCESS) {
        if (strncmp(frame->command, "MESSAGE", strlen("MESSAGE")) == 0) {
            strncpy(message->body, frame->body, frame->body_length);
            message->size = frame->body_length;
            
            return STOMP_SUCCESS;
        }
        else {
            // TODO: handle error condition
            strncpy(message->body, frame->body, frame->body_length);
            message->size = frame->body_length;
            
            return STOMP_FAILURE;
        }
    }
    else {
        stomp_status_set(&messenger->status, STOMP_FAILURE,
                "Unable to read the frame data to the underlying connection");

        return STOMP_FAILURE;
    }
    
}