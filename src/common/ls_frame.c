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
#include "ls_frame.h"

enum ls_command_t_ {
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
};

struct ls_frame_t_ {
	ls_command_t command;
};