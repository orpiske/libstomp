/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stomp_frame.h
 * Author: otavio
 *
 * Created on June 11, 2016, 1:31 PM
 */

#ifndef STOMP_FRAME_H
#define STOMP_FRAME_H

#include <apr-1/apr_general.h>
#include <apr-1/apr_hash.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stomp_frame
{
    char *command;
    apr_hash_t *headers;
    char *body;
    apr_size_t body_length;
} stomp_frame;


#ifdef __cplusplus
}
#endif

#endif /* STOMP_FRAME_H */

