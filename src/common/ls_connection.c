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
#include "ls_connection.h"

struct ls_connection_t_ {
	gru_uri_t url;
	gru_net_socket_t socket;
};

ls_connection_t *ls_connection_new(const char *url, gru_status_t *status) {
	ls_connection_t *ret = gru_alloc(sizeof(ls_connection_t), status);
	gru_alloc_check(ret, NULL);

	ret->url = gru_uri_parse(url, status);
	if (gru_status_error(status)) {
		ls_connection_destroy(&ret);

		return NULL;
	}

	if (ret->url.port == 0) {
		ret->url.port = LS_STOMP_DEFAULT_PORT;
	}

	return ret;
}

void ls_connection_destroy(ls_connection_t **ptr) {
	ls_connection_t *connection = *ptr;

	if (!connection) {
		return;
	}

	gru_net_shutdown();

	gru_uri_cleanup(&connection->url);
	gru_dealloc((void **) ptr);
}

stomp_status_code_t ls_connection_connect(ls_connection_t *connection,
	gru_status_t *status) {
	int init = gru_net_init();

	if (init != 0) {
		gru_status_set(status, GRU_FAILURE, "Failed to initialize networking library");

		return STOMP_FAILURE;
	}

	char service[6] = {0};
	snprintf(service, sizeof(service), "%" PRIu16 "", connection->url.port);

	if (gru_net_open_socket(&connection->socket, connection->url.host, service) < 0) {
		gru_status_set(
			status, GRU_FAILURE, "Failed to open the socket and connect to the broker");

		return STOMP_FAILURE;
	}


	return STOMP_SUCCESS;
}


stomp_status_code_t ls_connection_disconnect(ls_connection_t *connection,
	gru_status_t *status) {
	gru_net_close_socket(&connection->socket);
	return STOMP_SUCCESS;
}


gru_net_socket_t ls_connection_get_socket(ls_connection_t *connection) {
	return connection->socket;
}