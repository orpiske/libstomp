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
#include "stomp_connection.h"
#include "stomp_messenger.h"
#include "stomp_ex_properties.h"
#include "stomp_message.h"
#include "stomp_status.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    stomp_status_code_t stat;

    /*
     * Initializes the messenger
     */
    stomp_messenger_t *messenger = stomp_messenger_init();

    if (!messenger) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_without_message;
    }

    /*
     * Sets the endpoint address
     */
    stat = stomp_set_endpoint(messenger, "stomp://localhost:61613/queue/test.stomp.queue");
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_without_message;
    }


    /*
     * Connects to the endpoint
     */
    stat = stomp_connect(messenger, NULL, 1000);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_without_message;
    }


    /*
     * Creates a message to be sent
     */
    stomp_message_t *message = stomp_message_create(&messenger->status);
    if (!message) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_with_message;
    }


    /*
     * Formats the message
     */
    char *text = "HIGH LEVEL API TEST";
    stomp_message_format(message, text, strlen(text));

    stomp_send_header_t send_header;

    send_header.transaction_id = -1;
    send_header.receipt = 124;

    // Sets an arbitrary property to the exchange
    stomp_exchange_add(messenger->exchange_properties, "test", "123");

    stat = stomp_exchange_util_ctime(messenger->exchange_properties,
                                     &messenger->status);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, messenger->status.message);

        goto failure_with_message;
    }

    /*
     * Sends the message
     */
    stat = stomp_send(messenger, &send_header, message);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_with_message;
    }


    /*
     * Disconnects from the broker after receiving the receipt response
     */
    stomp_disconnection_header_t disconn;
    disconn.receipt = 124;
    stat = stomp_disconnect(messenger, &disconn);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, "%s\n", messenger->status.message);

        goto failure_with_message;
    }


    /*
     * Cleanup the objects
     */
    stomp_message_destroy(&message);
    stomp_messenger_destroy(&messenger);
    return EXIT_SUCCESS;
    
    failure_with_message:
    stomp_message_destroy(&message);
    
    failure_without_message:
    stomp_messenger_destroy(&messenger);
    return EXIT_FAILURE;

}