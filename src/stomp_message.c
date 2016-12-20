/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include <apr-1/apr_pools.h>

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

    apr_status_t stat = apr_pool_create(&message->pool, NULL);
    if (stat != APR_SUCCESS) {
        free(message);
        stomp_status_set(status, STOMP_FAILURE,
                    "Unable to allocate memory for internal message pool");

        return NULL;
    }

    return message;
}


void stomp_message_destroy(stomp_message_t **message) {
    apr_pool_destroy((*message)->pool);
    free((*message)->body);
    free(*message);
    *message = NULL;
}


void stomp_message_format(stomp_message_t *message, const char *data, size_t len) {
    if (message->body) {
        free(message->body);
    }

    message->body = malloc(len + 1);
    bzero(message->body, (len + 1));
    strncpy(message->body, data, len);

    message->size = strlen(message->body) + 1;
}