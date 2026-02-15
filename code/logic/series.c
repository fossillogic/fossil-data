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
#include "fossil/data/series.h"

#include <stdint.h>
#include <string.h>

/* ---------------------------------------------------------
 * Type helpers
 * --------------------------------------------------------- */

static int fossil_data_series_is_numeric(const char *type_id)
{
    return
        !strcmp(type_id,"i8")   || !strcmp(type_id,"i16") ||
        !strcmp(type_id,"i32")  || !strcmp(type_id,"i64") ||
        !strcmp(type_id,"u8")   || !strcmp(type_id,"u16") ||
        !strcmp(type_id,"u32")  || !strcmp(type_id,"u64") ||
        !strcmp(type_id,"f32")  || !strcmp(type_id,"f64") ||
        !strcmp(type_id,"size") ||
        !strcmp(type_id,"bool") ||
        !strcmp(type_id,"hex")  ||
        !strcmp(type_id,"oct")  ||
        !strcmp(type_id,"bin");
}

static double fossil_data_series_get(const void *data, size_t i, const char *type_id)
{
    if (!strcmp(type_id,"i8"))   return ((const int8_t*)data)[i];
    if (!strcmp(type_id,"i16"))  return ((const int16_t*)data)[i];
    if (!strcmp(type_id,"i32"))  return ((const int32_t*)data)[i];
    if (!strcmp(type_id,"i64"))  return (double)((const int64_t*)data)[i];

    if (!strcmp(type_id,"u8"))   return ((const uint8_t*)data)[i];
    if (!strcmp(type_id,"u16"))  return ((const uint16_t*)data)[i];
    if (!strcmp(type_id,"u32"))  return ((const uint32_t*)data)[i];
    if (!strcmp(type_id,"u64"))  return (double)((const uint64_t*)data)[i];

    if (!strcmp(type_id,"hex") ||
        !strcmp(type_id,"oct") ||
        !strcmp(type_id,"bin"))
        return (double)((const uint64_t*)data)[i];

    if (!strcmp(type_id,"f32"))  return ((const float*)data)[i];
    if (!strcmp(type_id,"f64"))  return ((const double*)data)[i];

    if (!strcmp(type_id,"bool")) return ((const uint8_t*)data)[i] ? 1.0 : 0.0;
    if (!strcmp(type_id,"size")) return (double)((const size_t*)data)[i];

    return 0.0;
}

static void fossil_data_series_store(void *out, size_t i, const char *type_id, double v)
{
    if (!strcmp(type_id,"f64")) ((double*)out)[i] = v;
    else if (!strcmp(type_id,"f32")) ((float*)out)[i] = (float)v;
    else if (!strcmp(type_id,"i32")) ((int32_t*)out)[i] = (int32_t)v;
    else if (!strcmp(type_id,"i64")) ((int64_t*)out)[i] = (int64_t)v;
    else if (!strcmp(type_id,"u32")) ((uint32_t*)out)[i] = (uint32_t)(v < 0 ? 0 : v);
    else if (!strcmp(type_id,"u64")) ((uint64_t*)out)[i] = (uint64_t)(v < 0 ? 0 : v);
    else if (!strcmp(type_id,"bool")) ((uint8_t*)out)[i] = v > 0.5;
    else if (!strcmp(type_id,"size")) ((size_t*)out)[i] = (size_t)(v < 0 ? 0 : v);
    else ((double*)out)[i] = v; /* fallback */
}

/* ---------------------------------------------------------
 * Cumulative sum
 * --------------------------------------------------------- */

int fossil_data_series_cumsum(
    const void* input,
    void* output,
    size_t count,
    const char* type_id
){
    if (!input || !output || count == 0 || !fossil_data_series_is_numeric(type_id))
        return -1;

    long double sum = 0.0;

    for (size_t i=0;i<count;i++) {
        sum += fossil_data_series_get(input,i,type_id);
        fossil_data_series_store(output,i,type_id,(double)sum);
    }

    return 0;
}

/* ---------------------------------------------------------
 * Rolling mean
 * --------------------------------------------------------- */

int fossil_data_series_rolling_mean(
    const void* input,
    void* output,
    size_t count,
    size_t window,
    const char* type_id
){
    if (!input || !output || count == 0 || window == 0 ||
        !fossil_data_series_is_numeric(type_id))
        return -1;

    long double sum = 0.0;
    size_t start = 0;

    for (size_t i=0;i<count;i++) {

        sum += fossil_data_series_get(input,i,type_id);

        /* Remove element leaving the window */
        if (i >= window) {
            sum -= fossil_data_series_get(input,start,type_id);
            start++;
        }

        size_t denom = (i+1 < window) ? (i+1) : window;
        fossil_data_series_store(output,i,type_id,(double)(sum/denom));
    }

    return 0;
}
