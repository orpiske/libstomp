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
#include "stomp_message.h"
#include "stomp_status.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    stomp_status_code_t stat;
    stomp_messenger_t *messenger = stomp_messenger_init(); 
    
    if (!messenger) {
        fprintf(stderr, "Unable to initialize stomp messenger\n");
                
        goto failure;
    }
    
    stat = stomp_set_endpoint(messenger, "stomp://localhost:61613/queue/test.stomp.queue");
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, messenger->status.message);
        
        goto failure;
    }
   
    
    stat = stomp_connect(messenger, NULL);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, messenger->status.message);
        
        goto failure;
    }
    
    
    stomp_subscription_header_t sub_header;
    
    sub_header.id = 1;
    sub_header.receipt = 123;
    stat = stomp_subscribe(messenger, &sub_header);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, "%s\n", messenger->status.message);
        
        goto failure;
    }

    stomp_message_t *message = stomp_message_create(NULL);
    
    stomp_receive_header_t receive_header; 
    
    stat = stomp_receive(messenger, &receive_header, message);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, messenger->status.message);
        
        goto failure;
    }
    fprintf(stdout, "%s\n", message->body);

    
    stat = stomp_disconnect(messenger, NULL);
    if (stat != STOMP_SUCCESS) {
        fprintf(stderr, messenger->status.message);
        
        goto failure;
    }
    
    
    stomp_message_destroy(&message);
    stomp_messenger_destroy(&messenger);
    return EXIT_SUCCESS;
    
    failure:
    stomp_message_destroy(&message);
    stomp_messenger_destroy(&messenger);
    return EXIT_FAILURE;

}