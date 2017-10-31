/**
 *    Copyright 2017 Otavio Rodolfo Piske
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */
#ifndef LITESTOMP_LS_CONNECTION_H
#define LITESTOMP_LS_CONNECTION_H

#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include <network/gru_net.h>
#include <network/gru_uri.h>

#include "stomp_status.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LS_STOMP_DEFAULT_PORT 61613

/**
 * A STOMP connection object
 */
typedef struct ls_connection_t_ ls_connection_t;

/**
 * Creates a new connection
 * @param url the URL to the Stomp server (ie.: stomp://hostname/)
 * @param status a status object
 * @return a new stomp connection object or NULL in case of failure
 */
ls_connection_t *ls_connection_new(const char *url, gru_status_t *status);

/**
 * Destroys a connection
 * @param ptr A pointer to a connection pointer
 */
void ls_connection_destroy(ls_connection_t **ptr);

/**
 * Connects the connection
 * @param connection the connection to connect
 * @param status a status object
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE otherwise
 */
stomp_status_code_t ls_connection_connect(ls_connection_t *connection,
	gru_status_t *status);


/**
 * Disconnects the connection
 * @param connection the connection to disconnect
 * @param status a status object
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE otherwise
 */
stomp_status_code_t ls_connection_disconnect(ls_connection_t *connection,
	gru_status_t *status);


/**
 * Get the connection socket
 * @param connection
 * @return
 */
gru_net_socket_t ls_connection_get_socket(ls_connection_t *connection);


/**
 * Get the connection hostname/IP
 * @param connection the connection object
 * @return The connection hostname
 */
const char *ls_connection_get_host(ls_connection_t *connection);

#ifdef __cplusplus
}
#endif

#endif // LITESTOMP_LS_CONNECTION_H
