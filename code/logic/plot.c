/**
 * -----------------------------------------------------------------------------
 * Project: Fossil Logic
 *
 * This file is part of the Fossil Logic project, which aims to develop
 * high-performance, cross-platform applications and libraries. The code
 * contained herein is licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. You may obtain
 * a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 * Author: Michael Gene Brockus (Dreamer)
 * Date: 04/05/2014
 *
 * Copyright (C) 2014-2025 Fossil Logic. All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "fossil/data/plot.h"

#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ---------------------------------------------------------
 * Type helpers
 * --------------------------------------------------------- */

static int fossil_data_plot_is_numeric(const char *type_id)
{
    if (!type_id) return 0;

    /* signed integers */
    if (!strcmp(type_id,"i8")  ||
        !strcmp(type_id,"i16") ||
        !strcmp(type_id,"i32") ||
        !strcmp(type_id,"i64"))
        return 1;

    /* unsigned integers */
    if (!strcmp(type_id,"u8")  ||
        !strcmp(type_id,"u16") ||
        !strcmp(type_id,"u32") ||
        !strcmp(type_id,"u64") ||
        !strcmp(type_id,"size"))
        return 1;

    /* floats */
    if (!strcmp(type_id,"f32") ||
        !strcmp(type_id,"f64"))
        return 1;

    /* logical */
    if (!strcmp(type_id,"bool"))
        return 1;

    /* numeric encodings (interpretation hints) */
    if (!strcmp(type_id,"hex") ||
        !strcmp(type_id,"oct") ||
        !strcmp(type_id,"bin"))
        return 1;

    return 0;
}

static double fossil_data_plot_get_value(const void *data, size_t i, const char *type_id)
{
    /* signed integers */
    if (!strcmp(type_id,"i8"))  return ((const int8_t*)data)[i];
    if (!strcmp(type_id,"i16")) return ((const int16_t*)data)[i];
    if (!strcmp(type_id,"i32")) return ((const int32_t*)data)[i];
    if (!strcmp(type_id,"i64")) return (double)((const int64_t*)data)[i];

    /* unsigned integers */
    if (!strcmp(type_id,"u8"))  return ((const uint8_t*)data)[i];
    if (!strcmp(type_id,"u16")) return ((const uint16_t*)data)[i];
    if (!strcmp(type_id,"u32")) return ((const uint32_t*)data)[i];
    if (!strcmp(type_id,"u64")) return (double)((const uint64_t*)data)[i];
    if (!strcmp(type_id,"size"))return (double)((const size_t*)data)[i];

    /* floats */
    if (!strcmp(type_id,"f32")) return ((const float*)data)[i];
    if (!strcmp(type_id,"f64")) return ((const double*)data)[i];

    /* bool */
    if (!strcmp(type_id,"bool")) return ((const uint8_t*)data)[i] ? 1.0 : 0.0;

    /* encoded numeric hints treated as integers */
    if (!strcmp(type_id,"hex") ||
        !strcmp(type_id,"oct") ||
        !strcmp(type_id,"bin"))
        return ((const int64_t*)data)[i];

    return 0.0;
}

/* ---------------------------------------------------------
 * Line plot
 * --------------------------------------------------------- */

int fossil_data_plot_line(
    const void* y,
    size_t count,
    const char* type_id,
    const char* title_id
) {
    if (!y || !count || !fossil_data_plot_is_numeric(type_id))
        return -1;

    const size_t width  = 60;
    const size_t height = 15;

    double min = fossil_data_plot_get_value(y,0,type_id);
    double max = min;

    for (size_t i=1;i<count;i++) {
        double v = fossil_data_plot_get_value(y,i,type_id);
        if (v < min) min = v;
        if (v > max) max = v;
    }

    double range = max - min;
    if (range == 0) range = 1.0;

    const char *plot_title = (title_id && *title_id) ? title_id : "line plot";
    printf("\n=== %s ===\n", plot_title);

    for (size_t row=0; row<height; row++) {
        double threshold = max - (range * row / (height-1));

        for (size_t col=0; col<width; col++) {
            size_t idx = col * count / width;
            double v = fossil_data_plot_get_value(y,idx,type_id);
            putchar(v >= threshold ? '*' : ' ');
        }
        putchar('\n');
    }

    printf("min: %.3f  max: %.3f  n=%zu\n", min, max, count);
    return 0;
}

/* ---------------------------------------------------------
 * Histogram
 * --------------------------------------------------------- */

int fossil_data_plot_histogram(
    const void* data,
    size_t count,
    const char* type_id,
    size_t bins,
    const char* title_id
) {
    if (!data || !count || bins == 0 || !fossil_data_plot_is_numeric(type_id))
        return -1;

    double min = fossil_data_plot_get_value(data,0,type_id);
    double max = min;

    for (size_t i=1;i<count;i++) {
        double v = fossil_data_plot_get_value(data,i,type_id);
        if (v < min) min = v;
        if (v > max) max = v;
    }

    double range = max - min;
    if (range == 0) range = 1.0;

    size_t hist[bins];
    for (size_t i=0;i<bins;i++) hist[i]=0;

    for (size_t i=0;i<count;i++) {
        double v = fossil_data_plot_get_value(data,i,type_id);
        size_t b = (size_t)((v - min) / range * bins);
        if (b >= bins) b = bins-1;
        hist[b]++;
    }

    size_t max_count = 0;
    for (size_t i=0;i<bins;i++)
        if (hist[i] > max_count) max_count = hist[i];
    if (max_count == 0) max_count = 1;

    const char *plot_title = (title_id && *title_id) ? title_id : "histogram";
    printf("\n=== %s ===\n", plot_title);

    for (size_t i=0;i<bins;i++) {
        double low  = min + (range * i / bins);
        double high = min + (range * (i+1) / bins);

        printf("[%8.3f – %8.3f] | ", low, high);

        size_t bar = (hist[i] * 40) / max_count;
        for (size_t j=0;j<bar;j++) putchar('#');

        printf(" (%zu)\n", hist[i]);
    }

    printf("min: %.3f  max: %.3f  n=%zu\n", min, max, count);
    return 0;
}
