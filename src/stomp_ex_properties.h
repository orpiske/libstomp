/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   stomp_ex_properties.h
 * Author: opiske
 *
 * Created on June 16, 2016, 4:23 PM
 */

#ifndef STOMP_EX_PROPERTIES_H
#define STOMP_EX_PROPERTIES_H

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include <apr_hash.h>
#include <apr_time.h>

#include "stomp_status.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define STOMP_CREATION_TIME "creation"

typedef apr_hash_t stomp_hash_t;

/**
 * This type holds application properties set at the exchange level (in other 
 * words, it sets properties that get appended as STOMP headers in the 
 * transaction). The exchange properties are durable throughout the message 
 * exchange lifetime, therefore they should be cleared if undesired for some 
 * particular transaction.
 */
typedef struct stomp_exchange_properties_t_
{
    stomp_hash_t *hash;
} stomp_exchange_properties_t;


/**
 * Adds a property to the exchange
 * @param properties the properties data structure to be added
 * @param name the name of the property
 * @param value the value of the property
 */
void stomp_exchange_add(stomp_exchange_properties_t *properties,
                        const char *name, const char *value);


/**
 * Gets a property from the exchange
 * @param properties the properties data structure to be read
 * @param name the name of the property
 * @return value the value of the property
 */
const char *stomp_exchange_get(stomp_exchange_properties_t *properties,
                               const char *name);


/**
 * Clears the exchange properties
 * @param properties the properties data structure to be read
 */
void stomp_exchange_clear(stomp_exchange_properties_t *properties);


/**
 * Appends the creation time to the exchange properties. It will be added in the
 * transaction as "creation"
 * @param properties the properties data structure to be read
 */
stomp_status_code_t stomp_exchange_util_ctime(stomp_exchange_properties_t *properties,
                                              stomp_status_t *stat);

#ifdef __cplusplus
}
#endif

#endif /* STOMP_EX_PROPERTIES_H */

