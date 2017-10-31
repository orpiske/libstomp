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
#ifndef LITESTOMP_LS_FRAME_H
#define LITESTOMP_LS_FRAME_H

#include <stdint.h>

#include <collection/gru_list.h>
#include <common/gru_alloc.h>
#include <common/gru_status.h>
#include <common/gru_variant.h>

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

/**
 * Serialize a STOMP frame
 * @param frame the frame to serialize
 * @param size the number of bytes that were serialized
 * @param status status structure
 * @return a pointer of chars or NULL in case of errors
 */
char *ls_frame_serialize(const ls_frame_t *frame, int *size, gru_status_t *status);

/**
 * Deserialize a STOMP frame
 * @param frame
 * @param size
 * @param status
 * @return
 */
stomp_status_code_t ls_frame_deserialize(ls_frame_t *frame, uint64_t size, gru_status_t *status);

/**
 * Destroys a STOMP frame
 * @param ptr pointer to the frame
 */
void ls_frame_destroy(ls_frame_t **ptr);

/**
 * Create a new STOMP connect frame
 * @param status status structure
 * @return a new STOMP connect frame or false otherwise
 */
ls_frame_t *ls_frame_connect(gru_status_t *status);

/**
 * Set a header into the frame object
 * @param frame frame object to set the header onto
 * @param kp keypair structure
 * @param status status structure in case of error
 * @return true if successfully set the header or false otherwise
 */
bool ls_frame_set_header(ls_frame_t *frame, const gru_keypair_t *kp, gru_status_t *status);

#endif // LITESTOMP_LS_FRAME_H
