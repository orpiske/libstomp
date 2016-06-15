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
#ifndef STOMP_H_
#define STOMP_H_

#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>
#include <apr-1/apr_uri.h>

#include "stomp_status.h"
#include "stomp_connection.h"
#include "stomp_engine.h"
#include "stomp_frame.h"
#include "stomp_io.h"
#include "stomp_headers.h"

typedef apr_uri_t stomp_uri_t;

typedef struct stomp_messenger_t_ {
    stomp_connection *connection;
    stomp_status_t status;
    apr_pool_t *pool;
    stomp_uri_t uri;
} stomp_messenger_t;

typedef struct stomp_message_t_ {
    size_t size; 
    char *body;
} stomp_message_t;


stomp_messenger_t *stomp_messenger_init();
void stomp_messenger_destroy(stomp_messenger_t **messenger);

stomp_status_code_t stomp_set_endpoint(stomp_messenger_t *messenger, 
        const char *uri);

stomp_status_code_t stomp_connect(stomp_messenger_t *messenger, 
                                  stomp_connection_header_t *header);

stomp_status_code_t stomp_disconnect(stomp_messenger_t *messenger, 
                                  stomp_disconnection_header_t *header);

stomp_status_code_t stomp_subscribe(stomp_messenger_t *messenger, 
                                  stomp_subscription_header_t *header);

stomp_status_code_t stomp_unsubscribe(stomp_messenger_t *messenger, 
                                  stomp_subscription_header_t *header);

stomp_status_code_t stomp_ack(stomp_messenger_t *messenger, 
                                  stomp_ack_header_t *header);

stomp_status_code_t stomp_nack(stomp_messenger_t *messenger, 
                                  stomp_ack_header_t *header);

stomp_status_code_t stomp_begin(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);

stomp_status_code_t stomp_commit(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);

stomp_status_code_t stomp_abort(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);

stomp_status_code_t stomp_send(stomp_messenger_t *messenger, 
                                  stomp_send_header_t *header, 
                                  stomp_message_t *message);

stomp_status_code_t stomp_message(stomp_messenger_t *messenger, 
                                  stomp_receive_header_t *header,
                                  stomp_message_t *message);

stomp_status_code_t stomp_receive(stomp_messenger_t *messenger, 
                                  stomp_receive_header_t *header,
                                  stomp_message_t *message);

#endif /* STOMP_H_ */

