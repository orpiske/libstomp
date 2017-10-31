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
#include <common/gru_alloc.h>

#include <common/ls_connection.h>
#include <common/ls_io.h>
#include <common/ls_frame.h>
#include <common/ls_debug.h>


static int connect_frame_test(int argc, char **argv) {
	ls_log_initialization();

	gru_status_t status = gru_status_new();
	ls_connection_t *ls_connection = ls_connection_new(argv[1], &status);

	if (!ls_connection) {
		fprintf(stderr, "Unable to create new connect: %s\n", status.message);

		return EXIT_FAILURE;
	}

	stomp_status_code_t ret = ls_connection_connect(ls_connection, &status);
	if (ret != STOMP_SUCCESS) {
		fprintf(stderr, "Connection failure: %s\n", status.message);

		return EXIT_FAILURE;
	}

	printf("Connected\n");

	ls_frame_t *frame = ls_frame_connect(&status);
	if (!frame) {
		fprintf(stderr, "Unable to create a new frame structure: %s\n", status.message);

		return EXIT_FAILURE;
	}

	gru_keypair_t *host = gru_keypair_new(&status);
	if (!host) {
		fprintf(stderr, "Unable to create a new keypair for setting the host: %s\n", status.message);

		goto err_exit_0;
	}

	gru_keypair_set_key(host, "host");
	gru_variant_set_string(host->pair, ls_connection_get_host(ls_connection));

	if (!ls_frame_set_header(frame, host, &status)) {
		fprintf(stderr, "Unable to set the frame header: %s\n", status.message);

		goto err_exit_1;
	}


	printf("Sending hello frame\n");
	ret = ls_io_write_frame(ls_connection, frame, &status);
	if (ret != STOMP_SUCCESS) {
		fprintf(stderr, "Error sending data: %s\n", status.message);

		goto err_exit_1;
	}

	printf("Sent hello frame\n");
	ret = ls_io_read_frame(ls_connection, NULL, &status);
	if (ret != STOMP_SUCCESS) {
		fprintf(stderr, "Error receiving data: %s\n", status.message);

		goto err_exit_1;
	}

	ret = ls_connection_disconnect(ls_connection, &status);
	if (ret != STOMP_SUCCESS) {
		fprintf(stderr, "Disconnect failure: %s\n", status.message);

		goto err_exit_1;
	}

	ls_frame_destroy(&frame);
	return EXIT_SUCCESS;

	err_exit_1:
	gru_keypair_destroy(&host);

	err_exit_0:
	ls_frame_destroy(&frame);
	return EXIT_FAILURE;
}


int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Missing test case name\n");

		return EXIT_FAILURE;
	}

	if (strcmp(argv[1], "connect") == 0) {
		if (!connect_frame_test(argc, &argv[1])) {
			return EXIT_FAILURE;
		}
	} else {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}