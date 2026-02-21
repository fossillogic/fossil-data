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
#include "fossil/data/tensor.h"
#include <string.h>
#include <stdint.h>
#include <float.h>
#include <limits.h>
#include <stdio.h>

// static size_t type_size(const char* type_id) {
//     if (strcmp(type_id, "i8") == 0 || strcmp(type_id, "u8") == 0 || strcmp(type_id, "bool") == 0) return 1;
//     if (strcmp(type_id, "i16") == 0 || strcmp(type_id, "u16") == 0) return 2;
//     if (strcmp(type_id, "i32") == 0 || strcmp(type_id, "u32") == 0 || strcmp(type_id, "f32") == 0) return 4;
//     if (strcmp(type_id, "i64") == 0 || strcmp(type_id, "u64") == 0 || strcmp(type_id, "size") == 0 || strcmp(type_id, "f64") == 0) return 8;
//     if (strcmp(type_id, "hex") == 0 || strcmp(type_id, "oct") == 0 || strcmp(type_id, "bin") == 0) return sizeof(uint64_t); // treat as u64
//     return 0; // unknown type
// }

int fossil_data_tensor_elements(const size_t* shape, size_t rank, size_t* out_elements) {
    if (!shape || !out_elements) return -1;
    size_t total = 1;
    for (size_t i = 0; i < rank; ++i) total *= shape[i];
    *out_elements = total;
    return 0;
}

int fossil_data_tensor_minmax(const void* data, size_t count, const char* type_id, void* out_min, void* out_max) {
    if (!data || !type_id || !out_min || !out_max) return -1;

    if (strcmp(type_id, "i8") == 0) {
        const int8_t* d = data;
        int8_t min = INT8_MAX, max = INT8_MIN;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(int8_t*)out_min = min; *(int8_t*)out_max = max;
    } else if (strcmp(type_id, "i16") == 0) {
        const int16_t* d = data;
        int16_t min = INT16_MAX, max = INT16_MIN;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(int16_t*)out_min = min; *(int16_t*)out_max = max;
    } else if (strcmp(type_id, "i32") == 0) {
        const int32_t* d = data;
        int32_t min = INT32_MAX, max = INT32_MIN;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(int32_t*)out_min = min; *(int32_t*)out_max = max;
    } else if (strcmp(type_id, "i64") == 0) {
        const int64_t* d = data;
        int64_t min = INT64_MAX, max = INT64_MIN;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(int64_t*)out_min = min; *(int64_t*)out_max = max;
    } else if (strcmp(type_id, "u8") == 0) {
        const uint8_t* d = data;
        uint8_t min = UINT8_MAX, max = 0;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(uint8_t*)out_min = min; *(uint8_t*)out_max = max;
    } else if (strcmp(type_id, "u16") == 0) {
        const uint16_t* d = data;
        uint16_t min = UINT16_MAX, max = 0;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(uint16_t*)out_min = min; *(uint16_t*)out_max = max;
    } else if (strcmp(type_id, "u32") == 0) {
        const uint32_t* d = data;
        uint32_t min = UINT32_MAX, max = 0;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(uint32_t*)out_min = min; *(uint32_t*)out_max = max;
    } else if (strcmp(type_id, "u64") == 0 || strcmp(type_id, "size") == 0 || strcmp(type_id, "hex") == 0 || strcmp(type_id, "oct") == 0 || strcmp(type_id, "bin") == 0) {
        const uint64_t* d = data;
        uint64_t min = UINT64_MAX, max = 0;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(uint64_t*)out_min = min; *(uint64_t*)out_max = max;
    } else if (strcmp(type_id, "f32") == 0) {
        const float* d = data;
        float min = FLT_MAX, max = -FLT_MAX;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(float*)out_min = min; *(float*)out_max = max;
    } else if (strcmp(type_id, "f64") == 0) {
        const double* d = data;
        double min = DBL_MAX, max = -DBL_MAX;
        for (size_t i = 0; i < count; i++) { if (d[i] < min) min = d[i]; if (d[i] > max) max = d[i]; }
        *(double*)out_min = min; *(double*)out_max = max;
    } else return -1; // unknown type
    return 0;
}

int fossil_data_tensor_mean(const void* data, size_t count, const char* type_id, double* out_mean) {
    if (!data || !type_id || !out_mean || count == 0) return -1;
    double sum = 0.0;

    if (strcmp(type_id, "i8") == 0) { const int8_t* d = data; for (size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "i16") == 0) { const int16_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "i32") == 0) { const int32_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "i64") == 0) { const int64_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "u8") == 0) { const uint8_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "u16") == 0) { const uint16_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "u32") == 0) { const uint32_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id, "u64") == 0 || strcmp(type_id,"size")==0 || strcmp(type_id,"hex")==0 || strcmp(type_id,"oct")==0 || strcmp(type_id,"bin")==0) { const uint64_t* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id,"f32")==0) { const float* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else if (strcmp(type_id,"f64")==0) { const double* d=data; for(size_t i=0;i<count;i++) sum+=d[i]; }
    else return -1;

    *out_mean = sum / (double)count;
    return 0;
}
