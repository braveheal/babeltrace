/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright 2019 EfficiOS Inc.
 */
#ifndef BABELTRACE_PLUGINS_COMMON_PARAM_VALIDATION_PARAM_VALIDATION_H
#define BABELTRACE_PLUGINS_COMMON_PARAM_VALIDATION_PARAM_VALIDATION_H

#include <babeltrace2/babeltrace.h>
#include <glib.h>
#include <stdbool.h>

#include <stdio.h> /* For __MINGW_PRINTF_FORMAT. */

#include <common/macros.h>

struct bt_param_validation_context;
struct bt_param_validation_value_descr;

#define BT_PARAM_VALIDATION_MAP_VALUE_ENTRY_END { NULL, 0, {} }

struct bt_param_validation_map_value_descr {
	const struct bt_param_validation_map_value_entry_descr *entries;
};

#define BT_PARAM_VALIDATION_INFINITE UINT64_MAX

struct bt_param_validation_array_value_descr {
	uint64_t min_length;
	uint64_t max_length;  /* Use BT_PARAM_VALIDATION_INFINITE if there's no max. */
	const struct bt_param_validation_value_descr *element_type;
};

struct bt_param_validation_string_value_descr {
	/* NULL-terminated array of choices. Unused if NULL. */
	const char **choices;
};

enum bt_param_validation_status {
	BT_PARAM_VALIDATION_STATUS_OK = 0,
	BT_PARAM_VALIDATION_STATUS_MEMORY_ERROR = -1,
	BT_PARAM_VALIDATION_STATUS_VALIDATION_ERROR = -2,
};

typedef enum bt_param_validation_status
	(bt_param_validation_func)(const bt_value *value,
			struct bt_param_validation_context *);

struct bt_param_validation_value_descr {
	bt_value_type type;

	/* Additional checks dependent on the type. */
	union {
		struct bt_param_validation_array_value_descr array;
		struct bt_param_validation_map_value_descr map;
		struct bt_param_validation_string_value_descr string;
	};

	/*
	 * If set, call this function, which is responsible of validating the
	 * value. The other fields are ignored.
	 *
	 * If validation fails, this function must call
	 * `bt_param_validation_error` with the provided context
	 * to set the error string.
	 */
	bt_param_validation_func *validation_func;
};

#define BT_PARAM_VALIDATION_MAP_VALUE_ENTRY_OPTIONAL true
#define BT_PARAM_VALIDATION_MAP_VALUE_ENTRY_MANDATORY false

struct bt_param_validation_map_value_entry_descr {
	const char *key;
	bool is_optional;

	const struct bt_param_validation_value_descr value_descr;
};

BT_HIDDEN
enum bt_param_validation_status bt_param_validation_validate(
		const bt_value *params,
		const struct bt_param_validation_map_value_entry_descr *entries,
		gchar **error);

BT_HIDDEN __BT_ATTR_FORMAT_PRINTF(2, 3)
enum bt_param_validation_status bt_param_validation_error(
		struct bt_param_validation_context *ctx,
		const char *format, ...);

#endif /* BABELTRACE_PLUGINS_COMMON_PARAM_VALIDATION_PARAM_VALIDATION_H */
