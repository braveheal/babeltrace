#ifndef BABELTRACE_GRAPH_PRIVATE_CONNECTION_NOTIFICATION_ITERATOR_H
#define BABELTRACE_GRAPH_PRIVATE_CONNECTION_NOTIFICATION_ITERATOR_H

/*
 * Copyright 2017 Philippe Proulx <pproulx@efficios.com>
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
 */

/* For enum bt_notification_iterator_status */
#include <babeltrace/graph/notification-iterator.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bt_component;
struct bt_notification;
struct bt_notification_iterator;

extern struct bt_component *bt_private_connection_notification_iterator_get_component(
		struct bt_notification_iterator *iterator);

extern enum bt_notification_iterator_status
bt_private_connection_notification_iterator_next(
		struct bt_notification_iterator *iterator,
		bt_notification_array *notifs, uint64_t *count);

#ifdef __cplusplus
}
#endif

#endif /* BABELTRACE_GRAPH_PRIVATE_CONNECTION_NOTIFICATION_ITERATOR_H */