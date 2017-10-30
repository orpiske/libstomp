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

#ifndef STOMP_TYPES_H
#define STOMP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Message ID
 */
typedef int64_t message_id_t;

/**
 * Transaction ID
 */
typedef int64_t transaction_id_t;

/**
 * Subscription ID
 */
typedef uint16_t subscription_id_t;

/**
 * Receipt
 */
typedef uint32_t receipt_t;

#ifdef __cplusplus
}
#endif

#endif /* STOMP_TYPES_H */
