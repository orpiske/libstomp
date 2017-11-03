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
#include "ls_io.h"

static stomp_status_code_t ls_io_read_buffer(ls_connection_t *connection, char **out, gru_status_t *status) {
	gru_net_socket_t socket = ls_connection_get_socket(connection);

	gru_net_set_t set;

	gru_net_init_socket_set(&set);
	if (gru_net_add_socket_to_set(&socket, &set) < 0) {
		gru_status_set(status, GRU_FAILURE, "Unable to initialize socket set");
	}

	int sock_stat = gru_net_check_socket_set(&set, 0.5);
	if (sock_stat < 0) {
		gru_status_set(status, GRU_FAILURE, "Unable to check the socket status: %s", gru_net_get_last_error());

		return STOMP_FAILURE;
	}
	else {
		if (sock_stat == 0) {
			return STOMP_SUCCESS | STOMP_NO_DATA;
		}
	}

	logger_t logger = gru_logger_get();

	int ret = 0;
	char buff[4096] = {0};

	do {
		char *frame = realloc(frame, 4096);
		if (!frame) {

		}
		ret = gru_net_recv(&socket, &buff, sizeof(buff));

		logger(GRU_DEBUG, "Received (%d):\n%s", ret, buff);



	} while (ret == sizeof(buff));

	return STOMP_SUCCESS;
}

stomp_status_code_t ls_io_read_frame(ls_connection_t *connection, ls_frame_t *frame, gru_status_t *status) {

}

stomp_status_code_t ls_io_write_frame(ls_connection_t *connection, ls_frame_t *frame, gru_status_t *status) {
	int size = 0;
	char *data = ls_frame_serialize(frame, &size, status);

	if (!data || size == -1) {
		return STOMP_FAILURE;
	}

	logger_t logger = gru_logger_get();
	logger(GRU_TRACE, "Sending frame (%d):\n%s", size, data);

	gru_net_socket_t socket = ls_connection_get_socket(connection);
	if (gru_net_send(&socket, data, size) != 0) {
		gru_status_set(status, GRU_FAILURE, gru_net_get_last_error());

		return STOMP_FAILURE;
	}

	return STOMP_SUCCESS;
}