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
#include "fossil/data/prob.h"

#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


/* ---------------------------------------------------------
 * Numeric type helpers
 * --------------------------------------------------------- */

static int fossil_data_prob_is_numeric(const char *type_id)
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

static double fossil_data_prob_get(const void *data, size_t i, const char *type_id)
{
    /* signed */
    if (!strcmp(type_id,"i8"))   return ((const int8_t*)data)[i];
    if (!strcmp(type_id,"i16"))  return ((const int16_t*)data)[i];
    if (!strcmp(type_id,"i32"))  return ((const int32_t*)data)[i];
    if (!strcmp(type_id,"i64"))  return (double)((const int64_t*)data)[i];

    /* unsigned */
    if (!strcmp(type_id,"u8"))   return ((const uint8_t*)data)[i];
    if (!strcmp(type_id,"u16"))  return ((const uint16_t*)data)[i];
    if (!strcmp(type_id,"u32"))  return ((const uint32_t*)data)[i];
    if (!strcmp(type_id,"u64"))  return (double)((const uint64_t*)data)[i];

    /* integer encodings */
    if (!strcmp(type_id,"hex") ||
        !strcmp(type_id,"oct") ||
        !strcmp(type_id,"bin"))
        return (double)((const uint64_t*)data)[i];

    /* floats */
    if (!strcmp(type_id,"f32"))  return ((const float*)data)[i];
    if (!strcmp(type_id,"f64"))  return ((const double*)data)[i];

    /* logical/meta */
    if (!strcmp(type_id,"bool")) return ((const uint8_t*)data)[i] ? 1.0 : 0.0;
    if (!strcmp(type_id,"size")) return (double)((const size_t*)data)[i];

    return 0.0;
}

static void fossil_data_prob_store(void *out, size_t i, const char *type_id, double v)
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
 * Mean
 * --------------------------------------------------------- */

int fossil_data_prob_mean(
    const void* data,
    size_t count,
    const char* type_id,
    double* result
){
    if (!data || !result || count == 0 || !fossil_data_prob_is_numeric(type_id))
        return -1;

    long double sum = 0.0;

    for (size_t i=0;i<count;i++)
        sum += fossil_data_prob_get(data,i,type_id);

    *result = (double)(sum / count);
    return 0;
}

/* ---------------------------------------------------------
 * Standard deviation (population)
 * --------------------------------------------------------- */

int fossil_data_prob_std(
    const void* data,
    size_t count,
    const char* type_id,
    double* result
){
    if (!data || !result || count == 0 || !fossil_data_prob_is_numeric(type_id))
        return -1;

    double mean;
    if (fossil_data_prob_mean(data,count,type_id,&mean))
        return -1;

    long double var = 0.0;

    for (size_t i=0;i<count;i++) {
        double d = fossil_data_prob_get(data,i,type_id) - mean;
        var += d*d;
    }

    var /= count;
    *result = sqrt((double)var);
    return 0;
}

/* ---------------------------------------------------------
 * RNG helpers
 * --------------------------------------------------------- */

static double fossil_rand_unit(void)
{
    return (double)rand() / (double)RAND_MAX;
}

/* Box–Muller transform */
static double fossil_rand_normal(double mean, double stddev)
{
    double u1 = fossil_rand_unit();
    double u2 = fossil_rand_unit();
    double r = sqrt(-2.0 * log(u1));
    double theta = 2.0 * M_PI * u2;
    return mean + stddev * r * cos(theta);
}

static int fossil_rand_binomial(int n, double p)
{
    int successes = 0;
    for (int i=0;i<n;i++)
        if (fossil_rand_unit() < p)
            successes++;
    return successes;
}

/* ---------------------------------------------------------
 * Sampling
 * --------------------------------------------------------- */

typedef struct {
    double a;
    double b;
} fossil_uniform_params_t;

typedef struct {
    double mean;
    double std;
} fossil_normal_params_t;

typedef struct {
    int n;
    double p;
} fossil_binomial_params_t;

int fossil_data_prob_sample(
    void* output,
    size_t count,
    const char* dist_id,
    const char* type_id,
    const void* params
){
    if (!output || !dist_id || !type_id || !params)
        return -1;

    if (!fossil_data_prob_is_numeric(type_id))
        return -2;

    /* ---- uniform ---- */
    if (!strcmp(dist_id,"uniform")) {

        const fossil_uniform_params_t *p = params;

        for (size_t i=0;i<count;i++) {
            double v = p->a + fossil_rand_unit() * (p->b - p->a);
            fossil_data_prob_store(output,i,type_id,v);
        }
        return 0;
    }

    /* ---- normal ---- */
    if (!strcmp(dist_id,"normal")) {

        const fossil_normal_params_t *p = params;

        for (size_t i=0;i<count;i++) {
            double v = fossil_rand_normal(p->mean,p->std);
            fossil_data_prob_store(output,i,type_id,v);
        }
        return 0;
    }

    /* ---- binomial ---- */
    if (!strcmp(dist_id,"binomial")) {

        const fossil_binomial_params_t *p = params;

        for (size_t i=0;i<count;i++) {
            double v = fossil_rand_binomial(p->n,p->p);
            fossil_data_prob_store(output,i,type_id,v);
        }
        return 0;
    }

    return -3; /* unknown distribution */
}
