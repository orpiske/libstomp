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
#include "stomp_status.h"

void stomp_status_set(stomp_status_t *status, stomp_status_code_t code, 
        const char *message, ...) 
{
    va_list ap;
    char *ret = NULL; // This needs to free'd later

    if (message != NULL) {
        va_start(ap, message);
        vasprintf(&ret, message, ap);
        va_end(ap);
    }
    
    status->code = code;
    status->message = message;
}

void stomp_status_reset(stomp_status_t *status) {
    free(status->message);
    status->message = NULL;
}

void stomp_status_success(stomp_status_t *status) {
    stomp_status_set(status, STOMP_SUCCESS, NULL);
}