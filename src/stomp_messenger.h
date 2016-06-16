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
#ifndef STOMP_H_
#define STOMP_H_

#include <apr-1/apr_general.h>
#include <apr-1/apr_network_io.h>
#include <apr-1/apr_hash.h>
#include <apr-1/apr_uri.h>

#include "stomp_status.h"
#include "stomp_connection.h"
#include "stomp_engine.h"
#include "stomp_frame.h"
#include "stomp_io.h"
#include "stomp_headers.h"
#include "stomp_ex_properties.h"
#include "stomp_message.h"

typedef apr_uri_t stomp_uri_t;

/**
 * This struct provides an abstraction for some of the message exchange details
 */
typedef struct stomp_messenger_t_ {
    stomp_connection *connection;
    // TODO: not exactly the ideal place for this (nor the ideal design), but
    // will remain in this place until I come up with a better solution
    stomp_exchange_properties_t *exchange_properties;
    stomp_status_t status;
    apr_pool_t *pool;
    stomp_uri_t uri;
} stomp_messenger_t;


/**
 * Initializes the messenger object
 * @return A messenger object that can be used to exchange data via STOMP
 */
stomp_messenger_t *stomp_messenger_init();


/**
 * Destroys the messenger object. The object will be unusable after destruction
 * @param messenger An instance of the messenger object
 */
void stomp_messenger_destroy(stomp_messenger_t **messenger);


/**
 * Sets the communication endpoint for the message exchange.
 * @param messenger An instance of the messenger object
 * @param uri A connection uri in the stomp://[user]:[password]@<hostname>:<port>/<endpoint address>
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_set_endpoint(stomp_messenger_t *messenger, 
        const char *uri);


/**
 * Connects to the endpoint associated with the messenger
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_connect(stomp_messenger_t *messenger, 
                                  stomp_connection_header_t *header);


/**
 * Disconnects from the endpoint associated with the messenger
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_disconnect(stomp_messenger_t *messenger, 
                                  stomp_disconnection_header_t *header);



/**
 * Subscribes to the endpoint associated with the messenger
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_subscribe(stomp_messenger_t *messenger, 
                                  stomp_subscription_header_t *header);


/**
 * Unsubscribes from the endpoint associated with the messenger
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_unsubscribe(stomp_messenger_t *messenger, 
                                  stomp_subscription_header_t *header);


/**
 * Acknowledges the processing of a message
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_ack(stomp_messenger_t *messenger, 
                                  stomp_ack_header_t *header);


/**
 * Not-acknowledges the processing of a message
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_nack(stomp_messenger_t *messenger, 
                                  stomp_ack_header_t *header);


/**
 * Begin a transaction
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_begin(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);


/**
 * Commits a transaction
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_commit(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);


/**
 * Aborts a transaction
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_abort(stomp_messenger_t *messenger, 
                                  stomp_transaction_header_t *header);


/**
 * Sends a message
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @param message the message to exchange
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_send(stomp_messenger_t *messenger, 
                                  stomp_send_header_t *header, 
                                  stomp_message_t *message);


/**
 * Reads the exchange data from the broker. The returned exchange may be a 
 * message or an error.
 * @param messenger A pointer to an instance of the messenger object
 * @param header A pointer to an instance of the appropriate header object for 
 * the given exchange. The instance of the object must contain the required 
 * frame headers as expected by the frame.
 * @param message the message to exchange
 * @return STOMP_SUCCESS if successful or STOMP_FAILURE is failed. Upon failure, 
 * the code will set the error details on the status member of the messenger 
 * object
 */
stomp_status_code_t stomp_receive(stomp_messenger_t *messenger, 
                                  stomp_receive_header_t *header,
                                  stomp_message_t *message);

#endif /* STOMP_H_ */

