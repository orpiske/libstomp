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
#include "stomp_ex_properties.h"
#include "stomp_status.h"

void stomp_exchange_add(stomp_exchange_properties_t *properties,
	const char *name,
	const char *value) {
	apr_hash_set((apr_hash_t *) properties->hash, name, APR_HASH_KEY_STRING, value);
}

const char *stomp_exchange_get(stomp_exchange_properties_t *properties,
	const char *name) {
	return apr_hash_get((apr_hash_t *) properties->hash, name, APR_HASH_KEY_STRING);
}

void stomp_exchange_clear(stomp_exchange_properties_t *properties) {
	apr_hash_clear((apr_hash_t *) properties->hash);
}

stomp_status_code_t stomp_exchange_util_ctime(stomp_exchange_properties_t *properties,
	stomp_status_t *stat) {
	apr_time_t now = apr_time_now();
	apr_pool_t *pool = apr_hash_pool_get((apr_hash_t *) properties->hash);

	if (!pool) {
		if (stat) {
			stomp_status_set(
				stat, STOMP_FAILURE, "Invalid exchange properties structure");
		}

		return STOMP_FAILURE;
	}

	const size_t buff_size = 32;
	char *buff = apr_pcalloc(pool, buff_size);

	snprintf(buff, buff_size, "%" PRIi64 "", apr_time_as_msec(now));

	stomp_exchange_add(properties, STOMP_CREATION_TIME, buff);
	return STOMP_SUCCESS;
}