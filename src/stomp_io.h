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


#include <apr_general.h>
#include <apr_network_io.h>
#include <apr_hash.h>
#include <apr_env.h>
#include <apr_strings.h>
#include <apr_support.h>

#include <stdio.h>
#include <stdbool.h>

#include "stomp_connection.h"
#include "stomp_frame.h"
#include "stomp_status.h"

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

/**
 * Lower level check that returns whether there's data to be read from the 
 * connection
 * @param connection
 * @return true if there's data to read or false otherwise
 */
bool stomp_io_can_read(stomp_connection *connection);

/**
 * Gets the last stomp engine status
 * @return The last status 
 */
stomp_status_t stomp_io_last_status();

#ifdef __cplusplus
}
#endif

#endif /* STOMP_IO_H */

