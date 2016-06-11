/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stomp_connection.h
 * Author: otavio
 *
 * Created on June 11, 2016, 1:27 PM
 */

#ifndef STOMP_CONNECTION_H
#define STOMP_CONNECTION_H

#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>

#ifdef __cplusplus
extern "C" {
#endif

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

