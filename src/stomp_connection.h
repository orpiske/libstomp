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
 * 
 * ********************************************************************
 * The contents of this file are part of the original stomp.c and stomp.h file 
 * from which this code is derived. The contents of the file have been modified 
 * to comply with project formatting, as well as additions for connection 
 * credentials
 */
#ifndef STOMP_CONNECTION_H
#define STOMP_CONNECTION_H

#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>

#include "stomp_frame.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Stomp connection reference object
 */
typedef struct stomp_connection
{
    apr_socket_t *socket;
    apr_sockaddr_t *local_sa;
    char *local_ip;
    apr_sockaddr_t *remote_sa;
    char *remote_ip;
} stomp_connection;

#ifdef __cplusplus
}
#endif

#endif /* STOMP_CONNECTION_H */

