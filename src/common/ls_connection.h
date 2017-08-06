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

#include <network/gru_net.h>
#include "../../../../../../../usr/include/stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ls_connection_t_ {
	char *host;
  	uint16_t port;
	gru_net_socket_t socket;


} ls_connection_t;

#ifdef __cplusplus
}
#endif

#endif //LITESTOMP_LS_CONNECTION_H
