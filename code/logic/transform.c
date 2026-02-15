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
#include "fossil/data/transform.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>

/* ---------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------*/

static int is_integer_type(const char* t) {
    return !strcmp(t,"i8")  || !strcmp(t,"i16") || !strcmp(t,"i32") || !strcmp(t,"i64") ||
           !strcmp(t,"u8")  || !strcmp(t,"u16") || !strcmp(t,"u32") || !strcmp(t,"u64");
}

static int is_float_type(const char* t) {
    return !strcmp(t,"f32") || !strcmp(t,"f64");
}

static int is_numeric_type(const char* t) {
    return is_integer_type(t) || is_float_type(t);
}

/* Convert element i to double */
static double read_as_double(const void* data, size_t i, const char* t) {

    if(!strcmp(t,"i8"))  return ((const int8_t*)data)[i];
    if(!strcmp(t,"i16")) return ((const int16_t*)data)[i];
    if(!strcmp(t,"i32")) return ((const int32_t*)data)[i];
    if(!strcmp(t,"i64")) return (double)((const int64_t*)data)[i];

    if(!strcmp(t,"u8"))  return ((const uint8_t*)data)[i];
    if(!strcmp(t,"u16")) return ((const uint16_t*)data)[i];
    if(!strcmp(t,"u32")) return ((const uint32_t*)data)[i];
    if(!strcmp(t,"u64")) return (double)((const uint64_t*)data)[i];

    if(!strcmp(t,"f32")) return ((const float*)data)[i];
    if(!strcmp(t,"f64")) return ((const double*)data)[i];

    return 0.0;
}

/* Write double into output buffer as same numeric type */
static void write_from_double(void* out, size_t i, const char* t, double v) {

    if(!strcmp(t,"i8"))  ((int8_t*)out)[i]  = (int8_t)v;
    else if(!strcmp(t,"i16")) ((int16_t*)out)[i] = (int16_t)v;
    else if(!strcmp(t,"i32")) ((int32_t*)out)[i] = (int32_t)v;
    else if(!strcmp(t,"i64")) ((int64_t*)out)[i] = (int64_t)v;

    else if(!strcmp(t,"u8"))  ((uint8_t*)out)[i]  = (uint8_t)v;
    else if(!strcmp(t,"u16")) ((uint16_t*)out)[i] = (uint16_t)v;
    else if(!strcmp(t,"u32")) ((uint32_t*)out)[i] = (uint32_t)v;
    else if(!strcmp(t,"u64")) ((uint64_t*)out)[i] = (uint64_t)v;

    else if(!strcmp(t,"f32")) ((float*)out)[i]  = (float)v;
    else if(!strcmp(t,"f64")) ((double*)out)[i] = v;
}

/* ---------------------------------------------------------
 * Scaling implementation
 * ---------------------------------------------------------*/

int fossil_data_transform_scale(
    const void* input,
    void* output,
    size_t count,
    const char* type_id,
    const char* method_id
) {
    /* Survive null or empty params */
    if(!input || !output || !type_id || !method_id || count == 0)
        return 0;

    if(!is_numeric_type(type_id))
        return -1;

    /* compute mean/min/max/std */
    double min=read_as_double(input,0,type_id);
    double max=min;
    double sum=0.0;

    for(size_t i=0;i<count;i++) {
        double v=read_as_double(input,i,type_id);
        if(v<min) min=v;
        if(v>max) max=v;
        sum+=v;
    }

    double mean=sum/(double)count;

    /* std */
    double var=0.0;
    for(size_t i=0;i<count;i++) {
        double d=read_as_double(input,i,type_id)-mean;
        var+=d*d;
    }
    double std=sqrt(var/(double)count);

    /* Apply transform */
    if(!strcmp(method_id,"minmax")) {

        double range=max-min;
        if(range==0.0) range=1.0;

        for(size_t i=0;i<count;i++) {
            double v=read_as_double(input,i,type_id);
            double s=(v-min)/range;
            write_from_double(output,i,type_id,s);
        }
        return 0;
    }

    if(!strcmp(method_id,"zscore")) {

        if(std==0.0) std=1.0;

        for(size_t i=0;i<count;i++) {
            double v=read_as_double(input,i,type_id);
            double s=(v-mean)/std;
            write_from_double(output,i,type_id,s);
        }
        return 0;
    }

    return -1;
}

/* ---------------------------------------------------------
 * Encoding implementation
 * ---------------------------------------------------------*/

int fossil_data_transform_encode(
    const void* input,
    void* output,
    size_t count,
    const char* type_id,
    const char* method_id
) {
    if(!input || !output || !type_id || !method_id)
        return -1;

    /* only categorical strings supported */
    if(strcmp(type_id,"cstr")!=0)
        return -1;

    const char* const* in = (const char* const*)input;

    /* ---- label encoding ---- */
    if(!strcmp(method_id,"label")) {

        int* out=(int*)output;

        /* simple dictionary */
        char** dict=(char**)malloc(count*sizeof(char*));
        size_t dict_sz=0;

        for(size_t i=0;i<count;i++) {

            size_t id=0;
            int found=0;

            for(id=0; id<dict_sz; id++) {
                if(strcmp(dict[id],in[i])==0) {
                    found=1;
                    break;
                }
            }

            if(!found) {
                dict[dict_sz]=(char*)in[i];
                id=dict_sz;
                dict_sz++;
            }

            out[i]=(int)id;
        }

        free(dict);
        return 0;
    }

    /* ---- one-hot (index only) ----
       Caller allocates integer output of size count
       This returns category index per element.
       (Full matrix expansion left to higher layer)
    */
    if(!strcmp(method_id,"onehot")) {

        int* out=(int*)output;

        char** dict=(char**)malloc(count*sizeof(char*));
        size_t dict_sz=0;

        for(size_t i=0;i<count;i++) {

            size_t id=0;
            int found=0;

            for(id=0; id<dict_sz; id++) {
                if(strcmp(dict[id],in[i])==0) {
                    found=1;
                    break;
                }
            }

            if(!found) {
                dict[dict_sz]=(char*)in[i];
                id=dict_sz;
                dict_sz++;
            }

            out[i]=(int)id;
        }

        free(dict);
        return 0;
    }

    return -1;
}
