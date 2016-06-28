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
 * The original content of this file has been modified to that it was split in 
 * different files: stomp_io.c, stomp_io.h, stomp_connection.h, stomp_frame.h. 
 * 
 * This file, itself, was renamed from stomp.c to stomp_engine.h. Additionally
 * the contents of this file were modified in order to match the project 
 * formatting guidelines and add documentation. 
 */
#ifndef STOMP_ENGINE_H
#define STOMP_ENGINE_H


#include <stdlib.h>
#include <string.h>
#ifdef __DEBIAN_DISTRO__ || __UBUNTU_DISTRO__
 #include <apr-1.0/apr.h>
 #include <apr-1.0/apr_strings.h>
 #include <apr-1.0/apr_general.h>
 #include <apr-1.0/apr_network_io.h>
 #include <apr-1.0/apr_hash.h>
#else
 #include <apr-1/apr.h>
 #include <apr-1/apr_strings.h>
 #include <apr-1/apr_general.h>
 #include <apr-1/apr_network_io.h>
 #include <apr-1/apr_hash.h>
#endif // #ifdef __DEBIAN_DISTRO__ || __UBUNTU_DISTRO__


#include "stomp_connection.h"
#include "stomp_frame.h"
#include "stomp_io.h"

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


/**
 * Connects to the broker using the low level connection engine
 * @param connection_ref
 * @param hostname
 * @param port
 * @param pool
 * @return 
 */
APR_DECLARE(apr_status_t) stomp_engine_connect(stomp_connection **connection_ref, 
                                               const char *hostname, 
                                               int port, apr_pool_t *pool);


/**
 * Disconnects from the broker using the low level connection engine
 * @param connection_ref
 * @param hostname
 * @param port
 * @param pool
 * @return 
 */
APR_DECLARE(apr_status_t) stomp_engine_disconnect(stomp_connection **connection_ref);



#ifdef __cplusplus
}
#endif

#endif  /* ! STOMP_ENGINE_H */
