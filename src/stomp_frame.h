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
#ifndef STOMP_FRAME_H
#define STOMP_FRAME_H

#ifdef __DEBIAN_DISTRO__ || __UBUNTU_DISTRO__
#include <apr-1.0/apr_general.h>
#include <apr-1.0/apr_time.h>
#else 
#include <apr-1/apr_general.h>
#include <apr-1/apr_hash.h>
#endif // #ifdef __DEBIAN_DISTRO__ || __UBUNTU_DISTRO__


#ifdef __cplusplus
extern "C" {
#endif

/**
 * STOMP frame reference object
 * \internal
 */
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

