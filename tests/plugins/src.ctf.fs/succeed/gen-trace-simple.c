/*
 * SPDX-License-Identifier: MIT
 *
 * Copyright (C) 2019 Philippe Proulx <pproulx@efficios.com>
 */

#include <stdint.h>
#include <babeltrace2-ctf-writer/writer.h>
#include <babeltrace2-ctf-writer/clock.h>
#include <babeltrace2-ctf-writer/clock-class.h>
#include <babeltrace2-ctf-writer/stream.h>
#include <babeltrace2-ctf-writer/event.h>
#include <babeltrace2-ctf-writer/event-types.h>
#include <babeltrace2-ctf-writer/event-fields.h>
#include <babeltrace2-ctf-writer/stream-class.h>
#include <babeltrace2-ctf-writer/trace.h>

#include "common/assert.h"

struct config {
	struct bt_ctf_writer *writer;
	struct bt_ctf_trace *trace;
	struct bt_ctf_clock *clock;
	struct bt_ctf_stream_class *sc;
	struct bt_ctf_stream *stream;
	struct bt_ctf_event_class *ec;
};

static
void fini_config(struct config *cfg)
{
	bt_ctf_object_put_ref(cfg->stream);
	bt_ctf_object_put_ref(cfg->sc);
	bt_ctf_object_put_ref(cfg->ec);
	bt_ctf_object_put_ref(cfg->clock);
	bt_ctf_object_put_ref(cfg->trace);
	bt_ctf_object_put_ref(cfg->writer);
}

static
void configure_writer(struct config *cfg, const char *path)
{
	struct bt_ctf_field_type *ft;
	int ret;

	cfg->writer = bt_ctf_writer_create(path);
	BT_ASSERT(cfg->writer);
	cfg->trace = bt_ctf_writer_get_trace(cfg->writer);
	BT_ASSERT(cfg->trace);
	cfg->clock = bt_ctf_clock_create("default");
	BT_ASSERT(cfg->clock);
	ret = bt_ctf_writer_add_clock(cfg->writer, cfg->clock);
	BT_ASSERT(ret == 0);
	ret = bt_ctf_writer_set_byte_order(cfg->writer,
		BT_CTF_BYTE_ORDER_BIG_ENDIAN);
	BT_ASSERT(ret == 0);
	cfg->sc = bt_ctf_stream_class_create("hello");
	BT_ASSERT(cfg->sc);
	ret = bt_ctf_stream_class_set_clock(cfg->sc, cfg->clock);
	BT_ASSERT(ret == 0);
	cfg->ec = bt_ctf_event_class_create("ev");
	BT_ASSERT(cfg->ec);
	ft = bt_ctf_field_type_integer_create(8);
	BT_ASSERT(ft);
	ret = bt_ctf_field_type_integer_set_is_signed(ft, BT_CTF_TRUE);
	BT_ASSERT(ret == 0);
	ret = bt_ctf_event_class_add_field(cfg->ec, ft, "first");
	BT_ASSERT(ret == 0);
	bt_ctf_object_put_ref(ft);
	ft = bt_ctf_field_type_string_create();
	BT_ASSERT(ft);
	ret = bt_ctf_event_class_add_field(cfg->ec, ft, "second");
	BT_ASSERT(ret == 0);
	bt_ctf_object_put_ref(ft);
	ret = bt_ctf_stream_class_add_event_class(cfg->sc, cfg->ec);
	BT_ASSERT(ret == 0);
	cfg->stream = bt_ctf_writer_create_stream(cfg->writer, cfg->sc);
	BT_ASSERT(cfg->stream);
}

static
void write_stream(struct config *cfg)
{
	struct bt_ctf_event *ev;
	struct bt_ctf_field *field;
	uint64_t i;
	int ret;

	for (i = 0; i < 25; i++) {
		ev = bt_ctf_event_create(cfg->ec);
		BT_ASSERT(ev);
		field = bt_ctf_event_get_payload(ev, "first");
		BT_ASSERT(field);
		ret = bt_ctf_field_integer_signed_set_value(field, -23 + i);
		BT_ASSERT(ret == 0);
		bt_ctf_object_put_ref(field);
		field = bt_ctf_event_get_payload(ev, "second");
		BT_ASSERT(field);
		ret = bt_ctf_field_string_set_value(field, "saluuuut");
		BT_ASSERT(ret == 0);
		bt_ctf_object_put_ref(field);
		ret = bt_ctf_clock_set_time(cfg->clock, 3600 + i * 5000);
		BT_ASSERT(ret == 0);
		ret = bt_ctf_stream_append_event(cfg->stream, ev);
		BT_ASSERT(ret == 0);
		bt_ctf_object_put_ref(ev);
	}

	ret = bt_ctf_stream_flush(cfg->stream);
	BT_ASSERT(ret == 0);
}

int main(int argc, char **argv)
{
	struct config cfg = {0};

	BT_ASSERT(argc >= 2);
	configure_writer(&cfg, argv[1]);
	write_stream(&cfg);
	fini_config(&cfg);
	return 0;
}
