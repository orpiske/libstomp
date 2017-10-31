/**
 Copyright 2017 Otavio Rodolfo Piske

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
#include <common/gru_variant.h>
#include "ls_frame.h"

struct ls_frame_t_ {
  ls_command_t command;
  ls_header_t *headers;
  ls_body_t *body;
};

static char *ls_frame_headers(const ls_header_t *headers, gru_status_t *status) {
	gru_node_t *node = NULL;

	if (headers == NULL) {
		return NULL;
	}

	node = headers->root;

	char *buff = NULL;
	char *prev = NULL;
	while (node) {
		// handle(node->data, data);
		const gru_keypair_t *kp = node->data;

		if (prev) {

			if (asprintf(&buff, "%s\n%s:%s\n", prev, kp->key, kp->pair->variant.string) == -1) {
				gru_dealloc_string(&prev);

				gru_status_set(status, GRU_FAILURE, "Not enough memory to serialize headers");
				return NULL;
			}

		}
		else {
			if (asprintf(&buff, "%s:%s\n", kp->key, kp->pair->variant.string) == -1) {
				gru_status_set(status, GRU_FAILURE, "Not enough memory to serialize headers");
				return NULL;
			}
		}

		prev = buff;
		node = node->next;
	}

	return buff;
}

static char *ls_frame_serialize_connect(const ls_frame_t *frame, int *size, gru_status_t *status) {
	char *ret = NULL;
	char *header = ls_frame_headers(frame->headers, status);

	/**
	 * Note from the spec:
	 * ... Clients that use the STOMP frame instead of the CONNECT frame will only
	 * be able to connect to STOMP 1.2 servers ...
	 */
	if (header) {
		*size = asprintf(&ret, "%s\naccept-version:1.2\n%s\n", "STOMP", header);
		if (*size == -1) {
			gru_dealloc_string(&header);

			gru_status_set(status, GRU_FAILURE, "Not enough memory to serialize STOMP frame");
			return NULL;
		}

		gru_dealloc_string(&header);
		return NULL;
	}
	else {
		if (unlikely(gru_status_error(status))) {
			return NULL;
		}

		*size = asprintf(&ret, "%s\naccept-version:1.2\n\n", "STOMP");
		if (*size == -1) {
			gru_status_set(status, GRU_FAILURE, "Not enough memory to serialize STOMP frame");
			return NULL;
		}
	}

	return ret;
}

char *ls_frame_serialize(const ls_frame_t *frame, int *size, gru_status_t *status) {
	char *ret = NULL;

	switch (frame->command) {
	case STOMP_FRAME_CONNECT: {
		ret = ls_frame_serialize_connect(frame, size, status);
		break;
	}
	case STOMP_FRAME_CONNECTED:
	case STOMP_FRAME_SEND:
	case STOMP_FRAME_SUBSCRIBE:
	case STOMP_FRAME_UNSUBSCRIBE:
	case STOMP_FRAME_ACK:
	case STOMP_FRAME_NACK:
	case STOMP_FRAME_BEGIN:
	case STOMP_FRAME_COMMIT:
	case STOMP_FRAME_ABORT:
	case STOMP_FRAME_DISCONNECT:
	case STOMP_FRAME_MESSAGE:
	case STOMP_FRAME_RECEIPT:
	case STOMP_FRAME_ERROR:
	default: {
		gru_status_set(status, GRU_FAILURE, "Invalid STOMP frame value");
		break;
	}
	}

	if (ret) {
		*size += 1;
	}

	return ret;
}

stomp_status_code_t ls_frame_deserialize(ls_frame_t *frame, uint64_t size, gru_status_t *status) {
	return STOMP_FAILURE;
}

static void ls_frame_destroy_header_entries(void **ptr) {
	gru_keypair_t *kp = *ptr;

	if (!kp) {
		return;
	}

	gru_keypair_destroy(&kp);
}

void ls_frame_destroy(ls_frame_t **ptr) {
	ls_frame_t *frame = *ptr;

	if (!frame) {
		return;
	}

	if (frame->headers) {
		gru_list_clean(frame->headers, ls_frame_destroy_header_entries);
		gru_list_destroy(&frame->headers);
	}

	if (frame->body != NULL) {
		gru_dealloc_string(&frame->body);
	}
}

ls_frame_t *ls_frame_connect(gru_status_t *status) {
	ls_frame_t *ret = gru_alloc(sizeof(ls_frame_t), status);
	gru_alloc_check(ret, NULL);

	ret->command = STOMP_FRAME_CONNECT;
	ret->headers = gru_list_new(status);
	ret->body = NULL;

	return ret;
}