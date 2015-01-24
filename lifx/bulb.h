// Copyright (c) 2014, Louis Opter <kalessin@kalessin.fr>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
//    this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

struct lgtd_lifx_gateway;

#pragma pack(push, 1)
struct lgtd_lifx_light_state {
    uint16_t    hue;
    uint16_t    saturation;
    uint16_t    brightness;
    uint16_t    kelvin;
    uint16_t    dim;
    uint16_t    power;
    char        label[LGTD_LIFX_LABEL_SIZE];
    uint64_t    tags;
};
#pragma pack(pop)

struct lgtd_lifx_bulb {
    RB_ENTRY(lgtd_lifx_bulb)        link;
    SLIST_ENTRY(lgtd_lifx_bulb)     link_by_gw;
    struct lgtd_lifx_gateway        *gw;
    uint8_t                         addr[LGTD_LIFX_ADDR_LENGTH];
    struct lgtd_lifx_light_state    state;
    lgtd_time_mono_t                last_light_state_at;
};
RB_HEAD(lgtd_lifx_bulb_map, lgtd_lifx_bulb);
SLIST_HEAD(lgtd_lifx_bulb_list, lgtd_lifx_bulb);

extern struct lgtd_lifx_bulb_map lgtd_lifx_bulbs_table;

static inline int
lgtd_lifx_bulb_cmp(const struct lgtd_lifx_bulb *a, const struct lgtd_lifx_bulb *b)
{
    return memcmp(a->addr, b->addr, sizeof(a->addr));
}

RB_GENERATE_STATIC(
    lgtd_lifx_bulb_map,
    lgtd_lifx_bulb,
    link,
    lgtd_lifx_bulb_cmp
);

struct lgtd_lifx_bulb *lgtd_lifx_bulb_get(struct lgtd_lifx_gateway *, const uint8_t *);
struct lgtd_lifx_bulb *lgtd_lifx_bulb_open(struct lgtd_lifx_gateway *, const uint8_t *);
void lgtd_lifx_bulb_close(struct lgtd_lifx_bulb *);

void lgtd_lifx_bulb_set_light_state(struct lgtd_lifx_bulb *,
                                    const struct lgtd_lifx_light_state *,
                                    lgtd_time_mono_t);
void lgtd_lifx_bulb_set_power_state(struct lgtd_lifx_bulb *, uint16_t);
