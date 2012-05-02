/*
 * Copyright (C) 2010-2012, Andreas Sandberg
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <assert.h>
#include <stdint.h>

#include "expect.h"
#include "util.h"
#include "perf_file.h"

int
ctrs_write_header(ctr_list_t *ctrs, int fd)
{
    uint16_t no_counters;
    assert(ctrs);

    no_counters = (uint16_t)ctrs_len(ctrs);
    write_all(fd, MAGIC, sizeof(MAGIC));
    write_all(fd, &no_counters, sizeof(no_counters));
    for (ctr_t *c = ctrs->head; c; c = c->next)
        write_all(fd, &c->attr, sizeof(c->attr));

    return 1;
}

int
ctrs_read_header(ctr_list_t *ctrs, int fd)
{
    uint16_t no_counters;
    char magic[sizeof(MAGIC)];

    assert(ctrs);

    ctrs->head = NULL;
    ctrs->tail = NULL;

    EXPECT(read_all(fd, magic, sizeof(magic)) != 0);
    EXPECT(memcmp(magic, MAGIC, sizeof(magic)) == 0);

    EXPECT(read_all(fd, &no_counters, sizeof(no_counters)) != 0);

    for (int i = 0; i < no_counters; i++) {
        ctr_t *c = malloc(sizeof(ctr_t));
        c->fd = -1;
        c->next = NULL;

        EXPECT(read_all(fd, &c->attr, sizeof(c->attr)) != 0);
        ctrs_add(ctrs, c);
    }

    return 1;
}

/*
 * Local Variables:
 * mode: c
 * c-basic-offset: 4
 * indent-tabs-mode: nil
 * c-file-style: "k&r"
 * End:
 */
