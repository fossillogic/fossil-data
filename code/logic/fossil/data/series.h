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
#ifndef FOSSIL_DATA_SERIES_H
#define FOSSIL_DATA_SERIES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file fossil_data_series.h
 * @brief Sequence and time series operations.
 *
 * Provides cumulative sum, rolling mean, and other sequence-level transformations.
 *
 * Supported type string IDs:
 *   - "i32", "i64", "f32", "f64"
 */

/**
 * @brief Computes the cumulative sum of a sequence.
 *
 * This function calculates the cumulative sum (prefix sum) of the input sequence
 * and stores the result in the output buffer. The operation is performed according
 * to the type specified by the type_id string ("i32", "i64", "f32", "f64").
 *
 * @param input    Pointer to the input data array.
 * @param output   Pointer to the output data array (must be pre-allocated).
 * @param count    Number of elements in the input/output arrays.
 * @param type_id  String identifier for the data type.
 * @return         0 on success, non-zero on error (e.g., unsupported type).
 */
int fossil_data_series_cumsum(
    const void* input,
    void* output,
    size_t count,
    const char* type_id
);

/**
 * @brief Computes the rolling mean (moving average) of a sequence.
 *
 * This function calculates the rolling mean over a fixed window size for the input
 * sequence and stores the result in the output buffer. The operation is performed
 * according to the type specified by the type_id string ("i32", "i64", "f32", "f64").
 * The window parameter specifies the number of elements to include in each mean.
 *
 * @param input    Pointer to the input data array.
 * @param output   Pointer to the output data array (must be pre-allocated).
 * @param count    Number of elements in the input/output arrays.
 * @param window   Size of the rolling window (must be > 0).
 * @param type_id  String identifier for the data type.
 * @return         0 on success, non-zero on error (e.g., unsupported type or invalid window).
 */
int fossil_data_series_rolling_mean(
    const void* input,
    void* output,
    size_t count,
    size_t window,
    const char* type_id
);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>

namespace fossil::data {

class Series {
public:
    /**
     * @brief Computes the cumulative sum (prefix sum) of a sequence.
     *
     * This static method wraps the C API function fossil_data_series_cumsum,
     * allowing C++ code to compute the cumulative sum of a sequence.
     * The input and output pointers must reference arrays of the type
     * specified by the type_id string ("i32", "i64", "f32", "f64").
     *
     * @param input    Pointer to the input data array.
     * @param output   Pointer to the output data array (must be pre-allocated).
     * @param count    Number of elements in the input/output arrays.
     * @param type_id  String identifier for the data type.
     * @return         0 on success, non-zero on error (e.g., unsupported type).
     */
    static int cumsum(const void* input, void* output, size_t count, const std::string& type_id) {
        return fossil_data_series_cumsum(input, output, count, type_id.c_str());
    }

    /**
     * @brief Computes the rolling mean (moving average) of a sequence.
     *
     * This static method wraps the C API function fossil_data_series_rolling_mean,
     * allowing C++ code to compute the rolling mean over a fixed window size.
     * The input and output pointers must reference arrays of the type
     * specified by the type_id string ("i32", "i64", "f32", "f64").
     * The window parameter specifies the number of elements to include in each mean.
     *
     * @param input    Pointer to the input data array.
     * @param output   Pointer to the output data array (must be pre-allocated).
     * @param count    Number of elements in the input/output arrays.
     * @param window   Size of the rolling window (must be > 0).
     * @param type_id  String identifier for the data type.
     * @return         0 on success, non-zero on error (e.g., unsupported type or invalid window).
     */
    static int rolling_mean(const void* input, void* output, size_t count,
                            size_t window, const std::string& type_id) {
        return fossil_data_series_rolling_mean(input, output, count, window, type_id.c_str());
    }
};

} // namespace fossil::data
#endif

#endif /* FOSSIL_DATA_SERIES_H */
