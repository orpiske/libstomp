/**
 *
 * Copyright 2005 LogicBlaze Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
#ifndef STOMP_H
#define STOMP_H


#include <stdlib.h>
#include <string.h>
#include <apr-1/apr.h>
#include <apr-1/apr_strings.h>
#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>

#include "stomp_connection.h"
#include "stomp_frame.h"
#include "stomp_io.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


APR_DECLARE(apr_status_t) stomp_engine_connect(stomp_connection **connection_ref, const char *hostname, int port, apr_pool_t *pool);
APR_DECLARE(apr_status_t) stomp_engine_disconnect(stomp_connection **connection_ref);



#ifdef __cplusplus
}
#endif

#endif  /* ! STOMP_H */
