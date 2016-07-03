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
 * from which this code is derived
 */
#ifndef STOMP_IO_H
#define STOMP_IO_H

#if defined(__DEBIAN_DISTRO__) || defined(__UBUNTU_DISTRO__)
#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_network_io.h>
#include <apr-1.0/apr_hash.h>
#include <apr-1.0/apr_env.h>
#include <apr-1.0/apr_strings.h>
#else
#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>
#include <apr-1/apr_env.h>
#include <apr-1/apr_strings.h>
#endif // defined(__DEBIAN_DISTRO__) || defined(__UBUNTU_DISTRO__)



#include <stdio.h>

#include "stomp_connection.h"
#include "stomp_frame.h"

#ifdef __cplusplus
extern "C" {
#endif
    
/**
 * @internal
 */
typedef struct data_block_list {
    char data[1024];
    struct data_block_list *next;
} data_block_list;

/**
 * Low level write to the STOMP connection
 * @param connection
 * @param frame
 * @param pool
 * @return 
 */
APR_DECLARE(apr_status_t) stomp_write(stomp_connection *connection, stomp_frame *frame, apr_pool_t *pool);

/**
 * Low level read from the STOMP connection
 * @param connection
 * @param frame
 * @param pool
 * @return 
 */
APR_DECLARE(apr_status_t) stomp_read(stomp_connection *connection, stomp_frame **frame, apr_pool_t *pool);


#ifdef __cplusplus
}
#endif

#endif /* STOMP_IO_H */

