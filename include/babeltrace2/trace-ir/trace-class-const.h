#ifndef BABELTRACE_TRACE_IR_TRACE_CLASS_CONST_H
#define BABELTRACE_TRACE_IR_TRACE_CLASS_CONST_H

/*
 * Copyright 2017-2018 Philippe Proulx <pproulx@efficios.com>
 * Copyright 2014 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * The Common Trace Format (CTF) Specification is available at
 * http://www.efficios.com/ctf
 */

#include <stdint.h>

/*
 * For bt_bool, bt_uuid, bt_trace_class, bt_stream_class,
 * bt_field_class, bt_value
 */
#include <babeltrace2/types.h>

/* For __BT_FUNC_STATUS_* */
#define __BT_FUNC_STATUS_ENABLE
#include <babeltrace2/func-status.h>
#undef __BT_FUNC_STATUS_ENABLE

#ifdef __cplusplus
extern "C" {
#endif

typedef void (* bt_trace_class_destruction_listener_func)(
		const bt_trace_class *trace_class, void *data);

extern bt_bool bt_trace_class_assigns_automatic_stream_class_id(
		const bt_trace_class *trace_class);

extern uint64_t bt_trace_class_get_stream_class_count(
		const bt_trace_class *trace_class);

extern const bt_stream_class *
bt_trace_class_borrow_stream_class_by_index_const(
		const bt_trace_class *trace_class, uint64_t index);

extern const bt_stream_class *bt_trace_class_borrow_stream_class_by_id_const(
		const bt_trace_class *trace_class, uint64_t id);

typedef enum bt_trace_class_add_listener_status {
	BT_TRACE_CLASS_ADD_LISTENER_STATUS_MEMORY_ERROR	= __BT_FUNC_STATUS_MEMORY_ERROR,
	BT_TRACE_CLASS_ADD_LISTENER_STATUS_OK		= __BT_FUNC_STATUS_OK,
} bt_trace_class_add_listener_status;

extern bt_trace_class_add_listener_status
bt_trace_class_add_destruction_listener(
        const bt_trace_class *trace_class,
        bt_trace_class_destruction_listener_func listener,
        void *data, uint64_t *listener_id);

typedef enum bt_trace_class_remove_listener_status {
	BT_TRACE_CLASS_REMOVE_LISTENER_STATUS_MEMORY_ERROR	= __BT_FUNC_STATUS_MEMORY_ERROR,
	BT_TRACE_CLASS_REMOVE_LISTENER_STATUS_OK		= __BT_FUNC_STATUS_OK,
} bt_trace_class_remove_listener_status;

extern bt_trace_class_remove_listener_status
bt_trace_class_remove_destruction_listener(
        const bt_trace_class *trace_class, uint64_t listener_id);

extern void bt_trace_class_get_ref(const bt_trace_class *trace_class);

extern void bt_trace_class_put_ref(const bt_trace_class *trace_class);

#define BT_TRACE_CLASS_PUT_REF_AND_RESET(_var)		\
	do {						\
		bt_trace_class_put_ref(_var);		\
		(_var) = NULL;				\
	} while (0)

#define BT_TRACE_CLASS_MOVE_REF(_var_dst, _var_src)	\
	do {						\
		bt_trace_class_put_ref(_var_dst);	\
		(_var_dst) = (_var_src);		\
		(_var_src) = NULL;			\
	} while (0)

#ifdef __cplusplus
}
#endif

#include <babeltrace2/undef-func-status.h>

#endif /* BABELTRACE_TRACE_IR_TRACE_CLASS_CONST_H */
