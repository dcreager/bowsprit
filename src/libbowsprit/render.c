/* -*- coding: utf-8 -*-
 * ----------------------------------------------------------------------
 * Copyright © 2014, RedJack, LLC.
 * All rights reserved.
 *
 * Please see the COPYING file in this distribution for license details.
 * ----------------------------------------------------------------------
 */

#include <stdio.h>

#include <libcork/core.h>
#include <libcork/ds.h>

#include "bowsprit.h"


/*-----------------------------------------------------------------------
 * Common stuffs
 */

static size_t
bws_snapshot_max_width(struct bws_snapshot *snapshot)
{
    size_t  i;
    size_t  overall_max = 0;
    for (i = 0; i < snapshot->count; i++) {
        size_t  value = snapshot->values[i].value;
        if (value > overall_max) {
            overall_max = value;
        }
    }
    return snprintf(NULL, 0, "%zu", overall_max);
}


/*-----------------------------------------------------------------------
 * Buffer renderer
 */

void
bws_snapshot_render_to_buffer(struct bws_snapshot *snapshot,
                              struct cork_buffer *dest)
{
    size_t  i;
    int  width = bws_snapshot_max_width(snapshot);
    for (i = 0; i < snapshot->count; i++) {
        struct bws_value_snapshot  *value = &snapshot->values[i];
        struct bws_measurement  *measurement = value->measurement;
        struct bws_plugin  *plugin = measurement->plugin;

        cork_buffer_append_printf
            (dest, "%*" PRIu64 " %s", width, value->value, plugin->name);
        if (plugin->instance != NULL) {
            cork_buffer_append_printf(dest, "-%s", plugin->instance);
        }
        cork_buffer_append_printf(dest, "/%s", measurement->type_name);
        if (measurement->type_instance != NULL) {
            cork_buffer_append_printf(dest, "-%s", measurement->type_instance);
        }
        if (strcmp(value->name, "value") != 0) {
            cork_buffer_append_printf(dest, "/%s", value->name);
        }
        cork_buffer_append_literal(dest, "\n");
    }
}


/*-----------------------------------------------------------------------
 * Stream renderer
 */

void
bws_snapshot_render_to_stream(struct bws_snapshot *snapshot, FILE *dest)
{
    struct cork_buffer  buf = CORK_BUFFER_INIT();
    bws_snapshot_render_to_buffer(snapshot, &buf);
    fwrite(buf.buf, 1, buf.size, dest);
}
