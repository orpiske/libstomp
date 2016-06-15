/**
 Copyright 2016 Otavio Rodolfo Piske
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */
#ifndef STOMP_HEADERS_H
#define STOMP_HEADERS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct stomp_credentials_t_
{
    char *username;
    char *password;
} stomp_credentials_t;

typedef struct stomp_heartbeat_t_
{
    uint16_t send;
    uint16_t receive;
} stomp_heartbeat_t;


typedef struct stomp_common_header_t_ {
    size_t content_lenght;
    char *content_type; 
} stomp_common_header_t;


typedef struct stomp_connection_header_t_
{
    stomp_common_header_t common;
    stomp_credentials_t *credentials;
    stomp_heartbeat_t heartbeat;
} stomp_connection_header_t;

typedef uint32_t receipt_t;

typedef struct stomp_disconnection_header_t_
{
    stomp_common_header_t common;
    receipt_t receipt;
} stomp_disconnection_header_t;

typedef uint16_t subscription_id_t;

typedef enum stomp_subscription_ack_t_
{
    AUTO,
    CLIENT,
    CLIENT_INDIVIDUAL,
} stomp_subscription_ack_t;

typedef struct stomp_subscription_header_t_
{
    stomp_common_header_t common;
    receipt_t receipt;
    subscription_id_t id;
    stomp_subscription_ack_t ack;
} stomp_subscription_header_t;

typedef int64_t message_id_t;
typedef int64_t transaction_id_t;

typedef struct stomp_ack_header_t_
{
    stomp_common_header_t common;
    receipt_t receipt;
    message_id_t message_id;
    transaction_id_t transaction_id;
} stomp_ack_header_t;

typedef struct stomp_transaction_header_t_
{
    stomp_common_header_t common;
    transaction_id_t transaction_id;
    receipt_t receipt;
} stomp_transaction_header_t;

typedef struct stomp_send_header_t_
{
    stomp_common_header_t common;
    char *destination;
    transaction_id_t transaction_id;
    receipt_t receipt;
} stomp_send_header_t;


typedef struct stomp_receive_header_t_
{
    
} stomp_receive_header_t;


#ifdef __cplusplus
}
#endif

#endif /* STOMP_HEADERS_H */

