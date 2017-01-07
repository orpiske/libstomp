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
#ifndef STOMP_MESSAGE_H
#define STOMP_MESSAGE_H

#if defined(__DEBIAN_DISTRO__) || defined(__UBUNTU_DISTRO__)
#include <apr-1.0/apr_pools.h>
#else
#include <apr-1/apr_pools.h>
#endif // defined(__DEBIAN_DISTRO__) || defined(__UBUNTU_DISTRO__)

#include "stomp_status.h"

#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif

/**
 * STOMP message
 */
typedef struct stomp_message_t_ {
    /**
     * Message size
     */
    size_t size;

    /**
     * Message body
     */
    char *body;

    apr_pool_t *pool;
} stomp_message_t;


/**
 * Creates a new STOMP message object
 * @param status A pointer to a status structure that will contain error details
 * in case the message creation fails
 * @return a new STOMP message object
 */
stomp_message_t *stomp_message_create(stomp_status_t *status);

/**
 * Destroy a STOMP message object. The object will be unusable after destruction
 * @param message A pointer to a pointer of a message object
 */
void stomp_message_destroy(stomp_message_t **message);


/**
 * Writes data to the message object
 * @param message the message object instance to write to
 * @param data the data to write
 * @param len the lenght of the data
 */
void stomp_message_format(stomp_message_t *message, const char *data, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* STOMP_MESSAGE_H */

