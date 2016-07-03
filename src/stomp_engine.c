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
 * formatting guidelines. Also replaced the CHECK_SUCCESS macro with regular 
 * code and added error handling.
 */

#include "stomp_engine.h"
#include "stomp_status.h"

static stomp_status_t error = { .message = NULL, .code = STOMP_SUCCESS };

stomp_status_t stomp_engine_last_status() {
    return error;
}

/********************************************************************************
 * 
 * Used to establish a connection
 *
 ********************************************************************************/
APR_DECLARE(apr_status_t) stomp_engine_connect(stomp_connection **connection_ref, 
                                               const char *hostname, int port, 
                                               apr_pool_t *pool)
{
    apr_status_t rc;
    int socket_family;
    stomp_connection *connection = NULL;

    //
    // Allocate the connection and a memory pool for the connection.
    //
    connection = apr_pcalloc(pool, sizeof (stomp_connection));
    if (connection == NULL) {
        stomp_status_set(&error, STOMP_FAILURE, 
                         "Not enough memory to create a connection");
        return APR_ENOMEM;
    }

    // Look up the remote address
    rc = apr_sockaddr_info_get(&connection->remote_sa, hostname, APR_UNSPEC, port, 0, pool);
    if (rc != APR_SUCCESS) { 
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to resolve hostname (%s): %s", 
                         hostname, msgbuf);
        return rc; 
    }

    // Create and Connect the socket.
    socket_family = connection->remote_sa->sa.sin.sin_family;
    rc = apr_socket_create(&connection->socket, socket_family, SOCK_STREAM, APR_PROTO_TCP, pool);
    if (rc != APR_SUCCESS) {
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to create socket: %s", 
                         hostname, msgbuf);
        return rc; 
    }
    
     
    rc = apr_socket_connect(connection->socket, connection->remote_sa);
    if (rc != APR_SUCCESS && !APR_STATUS_IS_EAGAIN(rc)) { 
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to connect: %s", 
                         msgbuf);
        return rc; 
    }
    
    
    // Get the Socket Info
    rc = apr_socket_addr_get(&connection->remote_sa, APR_REMOTE, connection->socket);
    if (rc != APR_SUCCESS) { 
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to get socket information: %s", 
                         hostname, msgbuf);
        return rc; 
    }
    
    //rc = apr_sockaddr_ip_get(&connection->remote_ip, connection->remote_sa);
    //CHECK_SUCCESS;
    
    connection->remote_ip = connection->remote_sa->hostname;
    rc = apr_socket_addr_get(&connection->local_sa, APR_LOCAL, connection->socket);
    if (rc != APR_SUCCESS) {
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to get the address associated with the socket: %s", 
                         hostname, msgbuf);
        return rc; 
    }
    
    rc = apr_sockaddr_ip_get(&connection->local_ip, connection->local_sa);
    if (rc != APR_SUCCESS) {
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to resolve socket address: %s", 
                         hostname, msgbuf);
        return rc; 
    }

    *connection_ref = connection;
    return rc;
}

APR_DECLARE(apr_status_t) stomp_engine_set_timeout(stomp_connection *connection,
                                                    apr_int64_t timeout) {
    return apr_socket_timeout_set(connection->socket, timeout);
}

APR_DECLARE(apr_status_t) stomp_engine_disconnect(stomp_connection **connection_ref)
{
    apr_status_t result, rc;
    stomp_connection *connection = *connection_ref;

    if (connection_ref == NULL || *connection_ref == NULL) {
        stomp_status_set(&error, STOMP_FAILURE, "invalid connection object");
        
        return APR_EGENERAL;
    }

    result = APR_SUCCESS;
    rc = apr_socket_shutdown(connection->socket, APR_SHUTDOWN_WRITE);
    if (result != APR_SUCCESS) {
        result = rc;
        
        char msgbuf[80] = {0};
        
        apr_strerror(rc, msgbuf, sizeof(msgbuf));
        
        stomp_status_set(&error, STOMP_FAILURE, 
                         "unable to connect: %s", 
                         msgbuf);
    }

    if (connection->socket != NULL) {
        rc = apr_socket_close(connection->socket);
        if (result != APR_SUCCESS) {
            result = rc;
        }
        
        connection->socket = NULL;
    }
    *connection_ref = NULL;
    return rc;
}

