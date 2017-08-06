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
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>

/*
 * Valid status codes
 */
typedef enum stomp_status_code_t_ {
    STOMP_FAILURE = 0,
    STOMP_SUCCESS = 1,
    STOMP_NO_DATA = 2,
} stomp_status_code_t;

/**
 * Status type
 */
typedef struct stomp_status_t_ {
    stomp_status_code_t code;
    char *message;
} stomp_status_t;


/**
 * Sets the status
 * @param status an instance of the status object to set
 * @param code the code to set the object to
 * @param message the status message followed by any other parameter as accepted 
 * by vasprintf
 */
void stomp_status_set(stomp_status_t *status, stomp_status_code_t code, 
                      const char *message, ...);

/**
 * Resets the status
 * @param status an instance of the status object to set
 */
void stomp_status_reset(stomp_status_t *status);

/**
 * Sets the status to success
 * @param status an instance of the status object to set
 */
void stomp_status_success(stomp_status_t *status);

/**
 * Tests whether the status code for a success-related value
 */
bool stomp_success(stomp_status_code_t stat);

/**
 * Tests whether the status code for an error-related value
 */
bool stomp_error(stomp_status_code_t stat);

#ifdef __cplusplus
}
#endif

#endif /* STOMP_ERROR_H */

