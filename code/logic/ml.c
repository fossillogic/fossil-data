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
#include "fossil/data/ml.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* ============================================================
   Internal model definitions
   ============================================================ */

typedef enum {
    MODEL_LINEAR,
    MODEL_LOGISTIC,
    MODEL_KMEANS
} fossil_ml_model_kind_t;

typedef struct {
    fossil_ml_model_kind_t kind;
    size_t rows;
    size_t cols;
    double* weights;   /* used by regression */
    double* centers;   /* used by kmeans */
    size_t k;          /* clusters for kmeans */
} fossil_ml_model_t;

/* ============================================================
   Helpers
   ============================================================ */

static int is_numeric_type(const char* t)
{
    return t &&
        (!strcmp(t,"i8")   || !strcmp(t,"i16")  || !strcmp(t,"i32")  || !strcmp(t,"i64") ||
         !strcmp(t,"u8")   || !strcmp(t,"u16")  || !strcmp(t,"u32")  || !strcmp(t,"u64") ||
         !strcmp(t,"size") ||
         !strcmp(t,"f32")  || !strcmp(t,"f64")  ||
         !strcmp(t,"bool") ||
         !strcmp(t,"hex")  || !strcmp(t,"oct")  || !strcmp(t,"bin"));
}

static double read_value(const void* data, size_t i, const char* t)
{
    if(!strcmp(t,"f32"))   return ((const float*)data)[i];
    if(!strcmp(t,"f64"))   return ((const double*)data)[i];
    if(!strcmp(t,"i8"))    return ((const int8_t*)data)[i];
    if(!strcmp(t,"i16"))   return ((const int16_t*)data)[i];
    if(!strcmp(t,"i32"))   return ((const int32_t*)data)[i];
    if(!strcmp(t,"i64"))   return ((const int64_t*)data)[i];
    if(!strcmp(t,"u8"))    return ((const uint8_t*)data)[i];
    if(!strcmp(t,"u16"))   return ((const uint16_t*)data)[i];
    if(!strcmp(t,"u32"))   return ((const uint32_t*)data)[i];
    if(!strcmp(t,"u64"))   return ((const uint64_t*)data)[i];
    if(!strcmp(t,"size"))  return ((const size_t*)data)[i];
    if(!strcmp(t,"bool"))  return ((const bool*)data)[i] ? 1.0 : 0.0;
    if(!strcmp(t,"hex"))   return ((const uint32_t*)data)[i]; // treat as uint32_t
    if(!strcmp(t,"oct"))   return ((const uint32_t*)data)[i]; // treat as uint32_t
    if(!strcmp(t,"bin"))   return ((const uint32_t*)data)[i]; // treat as uint32_t
    return 0.0;
}

static void write_value(void* data, size_t i, const char* t, double v)
{
    if(!strcmp(t,"f32"))   ((float*)data)[i]      = (float)v;
    else if(!strcmp(t,"f64"))   ((double*)data)[i]     = v;
    else if(!strcmp(t,"i8"))    ((int8_t*)data)[i]     = (int8_t)v;
    else if(!strcmp(t,"i16"))   ((int16_t*)data)[i]    = (int16_t)v;
    else if(!strcmp(t,"i32"))   ((int32_t*)data)[i]    = (int32_t)v;
    else if(!strcmp(t,"i64"))   ((int64_t*)data)[i]    = (int64_t)v;
    else if(!strcmp(t,"u8"))    ((uint8_t*)data)[i]    = (uint8_t)v;
    else if(!strcmp(t,"u16"))   ((uint16_t*)data)[i]   = (uint16_t)v;
    else if(!strcmp(t,"u32"))   ((uint32_t*)data)[i]   = (uint32_t)v;
    else if(!strcmp(t,"u64"))   ((uint64_t*)data)[i]   = (uint64_t)v;
    else if(!strcmp(t,"size"))  ((size_t*)data)[i]     = (size_t)v;
    else if(!strcmp(t,"bool"))  ((bool*)data)[i]       = (v != 0.0);
    else if(!strcmp(t,"hex"))   ((uint32_t*)data)[i]   = (uint32_t)v;
    else if(!strcmp(t,"oct"))   ((uint32_t*)data)[i]   = (uint32_t)v;
    else if(!strcmp(t,"bin"))   ((uint32_t*)data)[i]   = (uint32_t)v;
}

/* sigmoid for logistic regression */
static double sigmoid(double x){
    return 1.0/(1.0+exp(-x));
}

/* ============================================================
   TRAIN
   ============================================================ */

int fossil_data_ml_train(
    const void* X,
    const void* y,
    size_t rows,
    size_t cols,
    const char* type_id,
    const char* model_id,
    void** model_handle)
{
    // Validate arguments
    if (!model_handle) return -1;
    *model_handle = NULL;
    if (!X || !type_id || !model_id || rows == 0 || cols == 0) return -1;
    if (!strcmp(model_id, "kmeans")) {
        // For kmeans, y can be NULL
    } else {
        if (!y) return -1;
    }
    if (!is_numeric_type(type_id)) return -2;

    fossil_ml_model_t* m = calloc(1, sizeof(*m));
    if (!m) return -3;

    m->rows = rows;
    m->cols = cols;

    /* ---------- LINEAR REGRESSION ---------- */
    if (!strcmp(model_id, "linear_regression")) {
        m->kind = MODEL_LINEAR;
        m->weights = calloc(cols, sizeof(double));
        if (!m->weights) { free(m); return -3; }

        double lr = 0.001;
        for (int iter = 0; iter < 500; iter++) {
            for (size_t j = 0; j < cols; j++) {
                double grad = 0;
                for (size_t i = 0; i < rows; i++) {
                    double pred = 0;
                    for (size_t k = 0; k < cols; k++)
                        pred += m->weights[k] * read_value(X, i * cols + k, type_id);
                    double err = pred - read_value(y, i, type_id);
                    grad += err * read_value(X, i * cols + j, type_id);
                }
                m->weights[j] -= lr * grad / rows;
            }
        }
    }

    /* ---------- LOGISTIC REGRESSION ---------- */
    else if (!strcmp(model_id, "logistic_regression")) {
        m->kind = MODEL_LOGISTIC;
        m->weights = calloc(cols, sizeof(double));
        if (!m->weights) { free(m); return -3; }

        double lr = 0.01;
        for (int iter = 0; iter < 400; iter++) {
            for (size_t j = 0; j < cols; j++) {
                double grad = 0;
                for (size_t i = 0; i < rows; i++) {
                    double z = 0;
                    for (size_t k = 0; k < cols; k++)
                        z += m->weights[k] * read_value(X, i * cols + k, type_id);
                    double p = sigmoid(z);
                    grad += (p - read_value(y, i, type_id))
                        * read_value(X, i * cols + j, type_id);
                }
                m->weights[j] -= lr * grad / rows;
            }
        }
    }

    /* ---------- KMEANS ---------- */
    else if (!strcmp(model_id, "kmeans")) {
        m->kind = MODEL_KMEANS;
        m->k = 3; // default cluster count
        m->centers = calloc(m->k * cols, sizeof(double));
        if (!m->centers) { free(m); return -3; }

        // initialize centers using first k rows
        for (size_t c = 0; c < m->k; c++)
            for (size_t j = 0; j < cols; j++)
                m->centers[c * cols + j] = read_value(X, c * cols + j, type_id);

        int iters = 20;
        int* labels = malloc(rows * sizeof(int));
        if (!labels) { free(m->centers); free(m); return -3; }

        for (int it = 0; it < iters; it++) {
            // assign
            for (size_t i = 0; i < rows; i++) {
                double best = 1e300;
                int best_id = 0;
                for (size_t c = 0; c < m->k; c++) {
                    double d = 0;
                    for (size_t j = 0; j < cols; j++) {
                        double diff =
                            read_value(X, i * cols + j, type_id) -
                            m->centers[c * cols + j];
                        d += diff * diff;
                    }
                    if (d < best) { best = d; best_id = c; }
                }
                labels[i] = best_id;
            }

            // recompute centers
            memset(m->centers, 0, sizeof(double) * m->k * cols);
            int* counts = calloc(m->k, sizeof(int));
            if (!counts) { free(labels); free(m->centers); free(m); return -3; }

            for (size_t i = 0; i < rows; i++) {
                int c = labels[i];
                counts[c]++;
                for (size_t j = 0; j < cols; j++)
                    m->centers[c * cols + j] +=
                        read_value(X, i * cols + j, type_id);
            }

            for (size_t c = 0; c < m->k; c++) {
                if (counts[c] == 0) continue;
                for (size_t j = 0; j < cols; j++)
                    m->centers[c * cols + j] /= counts[c];
            }
            free(counts);
        }
        free(labels);
    }
    else {
        free(m);
        return -4;
    }

    *model_handle = m;
    return 0;
}

/* ============================================================
   PREDICT
   ============================================================ */

int fossil_data_ml_predict(
    const void* X,
    size_t rows,
    size_t cols,
    void* y_pred,
    void* model_handle,
    const char* type_id)
{
    // Validate arguments
    if (!model_handle || !type_id || !X || !y_pred || rows == 0 || cols == 0)
        return -1;
    if (!is_numeric_type(type_id))
        return -2;

    fossil_ml_model_t* m = (fossil_ml_model_t*)model_handle;

    if (!m)
        return -3;

    /* linear or logistic */
    if (m->kind == MODEL_LINEAR || m->kind == MODEL_LOGISTIC) {
        for (size_t i = 0; i < rows; i++) {
            double z = 0;
            for (size_t j = 0; j < cols; j++)
                z += m->weights[j] * read_value(X, i * cols + j, type_id);

            if (m->kind == MODEL_LOGISTIC) {
                z = sigmoid(z);
                // For classification, output 0 or 1 (threshold 0.5)
                if (!strcmp(type_id, "i32") || !strcmp(type_id, "i64"))
                    write_value(y_pred, i, type_id, z >= 0.5 ? 1 : 0);
                else
                    write_value(y_pred, i, type_id, z);
            } else {
                write_value(y_pred, i, type_id, z);
            }
        }
    }
    /* kmeans: output cluster index as int */
    else if (m->kind == MODEL_KMEANS) {
        for (size_t i = 0; i < rows; i++) {
            double best = 1e300;
            int best_id = 0;
            for (size_t c = 0; c < m->k; c++) {
                double d = 0;
                for (size_t j = 0; j < cols; j++) {
                    double diff =
                        read_value(X, i * cols + j, type_id) -
                        m->centers[c * cols + j];
                    d += diff * diff;
                }
                if (d < best) { best = d; best_id = c; }
            }
            // Always write as int for kmeans
            write_value(y_pred, i, "i32", best_id);
        }
    }
    else return -4;

    return 0;
}

/* ============================================================
   FREE
   ============================================================ */

int fossil_data_ml_free_model(void* model_handle)
{
    if(!model_handle)
        return 0; /* treat null as already freed, not an error */

    fossil_ml_model_t* m = model_handle;

    if (m->weights)
        free(m->weights);
    if (m->centers)
        free(m->centers);

    free(m);
    return 0;
}
