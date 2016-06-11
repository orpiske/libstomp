/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stomp_io.h
 * Author: otavio
 *
 * Created on June 11, 2016, 1:24 PM
 */

#ifndef STOMP_IO_H
#define STOMP_IO_H

#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>

#include "stomp_connection.h"
#include "stomp_frame.h"

#ifdef __cplusplus
extern "C" {
#endif

APR_DECLARE(apr_status_t) stomp_write(stomp_connection *connection, stomp_frame *frame, apr_pool_t *pool);
APR_DECLARE(apr_status_t) stomp_read(stomp_connection *connection, stomp_frame **frame, apr_pool_t *pool);


#ifdef __cplusplus
}
#endif

#endif /* STOMP_IO_H */

