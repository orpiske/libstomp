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

#include "ls_debug.h"

static bool log_initialized = false;

static void ls_logger_default_printer(log_level_t level, const char *msg, ...) {
	if (!gru_logger_can_log(level)) {
		return;
	}

	va_list ap;
	va_start(ap, msg);
	gru_logger_timed_do_print(level, msg, ap);
	va_end(ap);
}

void ls_log_initialization() {
	if (log_initialized) {
		return;
	}

	const char *log_level_str = getenv("LS_DEBUG");
	log_level_t log_level;
	if (!log_level_str) {
		log_level = GRU_FATAL;
	} else {
		log_level = gru_logger_get_level(log_level_str);
	}

	gru_logger_set_minimum(log_level);
	gru_logger_set(ls_logger_default_printer);

	log_initialized = true;
}