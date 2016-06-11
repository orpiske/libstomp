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
#ifndef STOMP_ERROR_H
#define STOMP_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

typedef enum stomp_status_code_t_ {
    STOMP_SUCCESS,
    STOMP_FAILURE,
} stomp_status_code_t;

typedef struct stomp_status_t_ {
    stomp_status_code_t code;
    char *message;
} stomp_status_t;

void stomp_status_set(stomp_status_t *status, stomp_status_code_t code, const char *message, ...);
void stomp_status_reset(stomp_status_t *status);
void stomp_status_success(stomp_status_t *status);

#ifdef __cplusplus
}
#endif

#endif /* STOMP_ERROR_H */

