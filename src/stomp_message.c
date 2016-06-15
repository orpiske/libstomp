/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "stomp_message.h"

stomp_message_t *stomp_message_create(stomp_status_t *status) {
    stomp_message_t *message = malloc(sizeof(stomp_message_t));
    
    if (!message) {
        if (status) {
            stomp_status_set(status, STOMP_FAILURE,
                    "Unable to allocate memory for a new stomp message object");

            return NULL;
        }
    }
    
    message->body = NULL;
    message->size = -1;
    return message;
}


void stomp_message_destroy(stomp_message_t **message) {
    free((*message)->body);
    free(*message);
    *message = NULL;
}


void stomp_message_format(stomp_message_t *message, const char *data, size_t len) {
    message->body; 
    
    if (message->body) {
        free(message->body);
    }
    
    message->body = malloc(len + 1);
    bzero(message->body, (len + 1));
    strncpy(message->body, data, len);
    
    message->size = strlen(message->body) + 1;
}