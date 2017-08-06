/*
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
#include "ls_transport_plain.h"

stomp_status_code_t ls_transport_plain_connect(ls_connection_t *connection, gru_status_t *status) {
	int init = gru_net_init();

	if (init != 0) {
		gru_status_set(status, GRU_FAILURE, "Failed to initialize networking library");

		return STOMP_FAILURE;
	}

	char service[6] = {0};
	snprintf(service, sizeof(service), "%"PRIu16"", connection->port);


	if (gru_net_open_socket(&connection->socket, connection->host, service) < 0) {
		gru_status_set(status, GRU_FAILURE, "Failed to open the socket and connect to the broker");
	}


	return STOMP_SUCCESS;
}