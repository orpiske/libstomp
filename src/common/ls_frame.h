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
#ifndef LITESTOMP_LS_FRAME_H
#define LITESTOMP_LS_FRAME_H

#include <stdint.h>

#include <collection/gru_list.h>
#include <common/gru_alloc.h>
#include <common/gru_status.h>

#include "stomp_status.h"
#include "ls_connection.h"
#include "ls_frame.h"

typedef char ls_body_t;
typedef gru_list_t ls_header_t;

typedef enum ls_command_t_ {
  STOMP_FRAME_CONNECT,
  STOMP_FRAME_CONNECTED,
  STOMP_FRAME_SEND,
  STOMP_FRAME_SUBSCRIBE,
  STOMP_FRAME_UNSUBSCRIBE,
  STOMP_FRAME_ACK,
  STOMP_FRAME_NACK,
  STOMP_FRAME_BEGIN,
  STOMP_FRAME_COMMIT,
  STOMP_FRAME_ABORT,
  STOMP_FRAME_DISCONNECT,
  STOMP_FRAME_MESSAGE,
  STOMP_FRAME_RECEIPT,
  STOMP_FRAME_ERROR,
} ls_command_t;

typedef struct ls_frame_t_ ls_frame_t;

char *ls_frame_serialize(const ls_frame_t *frame, int *size, gru_status_t *status);
stomp_status_code_t ls_frame_deserialize(ls_frame_t *frame, uint64_t size, gru_status_t *status);


ls_frame_t *ls_frame_connect(gru_status_t *status);

#endif // LITESTOMP_LS_FRAME_H
