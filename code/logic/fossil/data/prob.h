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
#ifndef FOSSIL_DATA_PROB_H
#define FOSSIL_DATA_PROB_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file fossil_data_prob.h
 * @brief Probability and statistics utilities.
 *
 * Supports mean, standard deviation, and random sampling from distributions.
 *
 * Supported type string IDs:
 *   - "i32", "i64", "f32", "f64"
 *
 * Supported distribution string IDs:
 *   - "normal"
 *   - "uniform"
 *   - "binomial" (integer parameters)
 */

/**
 * @brief Computes the mean (average) of an array of values.
 *
 * @param data     Pointer to the input data array.
 * @param count    Number of elements in the data array.
 * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
 * @param result   Pointer to a double where the computed mean will be stored.
 * @return         0 on success, non-zero on error (e.g., unsupported type or count == 0).
 */
int fossil_data_prob_mean(
    const void* data,
    size_t count,
    const char* type_id,
    double* result
);

/**
 * @brief Computes the standard deviation of an array of values.
 *
 * @param data     Pointer to the input data array.
 * @param count    Number of elements in the data array.
 * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
 * @param result   Pointer to a double where the computed standard deviation will be stored.
 * @return         0 on success, non-zero on error (e.g., unsupported type or count < 2).
 */
int fossil_data_prob_std(
    const void* data,
    size_t count,
    const char* type_id,
    double* result
);

/**
 * @brief Samples random values from a specified probability distribution.
 *
 * @param output   Pointer to the output array where sampled values will be stored.
 * @param count    Number of values to sample.
 * @param dist_id  String identifier for the distribution ("normal", "uniform", "binomial").
 * @param type_id  String identifier for the output data type ("i32", "i64", "f32", "f64").
 * @param params   Pointer to distribution-specific parameters (structure depends on dist_id).
 * @return         0 on success, non-zero on error (e.g., unsupported distribution or type).
 */
int fossil_data_prob_sample(
    void* output,
    size_t count,
    const char* dist_id,
    const char* type_id,
    const void* params
);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

#include <string>
#include <limits>

namespace fossil::data {

class Prob {
public:
    /**
     * @brief Computes the mean (average) of an array of values.
     *
     * This static method wraps the C API fossil_data_prob_mean, providing
     * a C++ interface. It computes the mean of the input data array,
     * given the number of elements and a string identifier for the data type.
     * If the computation fails (e.g., due to unsupported type or count == 0),
     * the method returns NaN.
     *
     * @param data     Pointer to the input data array.
     * @param count    Number of elements in the data array.
     * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
     * @return         The computed mean as a double, or NaN on error.
     */
    static double mean(const void* data, size_t count, const std::string& type_id) {
        double result;
        if (fossil_data_prob_mean(data, count, type_id.c_str(), &result) != 0) {
            // Handle error (e.g., throw exception or return NaN)
            return std::numeric_limits<double>::quiet_NaN();
        }
        return result;
    }

    /**
     * @brief Computes the standard deviation of an array of values.
     *
     * This static method wraps the C API fossil_data_prob_std, providing
     * a C++ interface. It computes the standard deviation of the input data array,
     * given the number of elements and a string identifier for the data type.
     * If the computation fails (e.g., due to unsupported type or count < 2),
     * the method returns NaN.
     *
     * @param data     Pointer to the input data array.
     * @param count    Number of elements in the data array.
     * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
     * @return         The computed standard deviation as a double, or NaN on error.
     */
    static double std(const void* data, size_t count, const std::string& type_id) {
        double result;
        if (fossil_data_prob_std(data, count, type_id.c_str(), &result) != 0) {
            // Handle error (e.g., throw exception or return NaN)
            return std::numeric_limits<double>::quiet_NaN();
        }
        return result;
    }

    /**
     * @brief Samples random values from a specified probability distribution.
     *
     * This static method wraps the C API fossil_data_prob_sample, providing
     * a C++ interface. It fills the output array with random samples drawn from
     * the specified distribution, using the provided distribution and type identifiers,
     * and distribution-specific parameters.
     *
     * @param output   Pointer to the output array where sampled values will be stored.
     * @param count    Number of values to sample.
     * @param dist_id  String identifier for the distribution ("normal", "uniform", "binomial").
     * @param type_id  String identifier for the output data type ("i32", "i64", "f32", "f64").
     * @param params   Pointer to distribution-specific parameters.
     * @return         0 on success, non-zero on error (e.g., unsupported distribution or type).
     */
    static int sample(void* output, size_t count, const std::string& dist_id,
                      const std::string& type_id, const void* params) {
        return fossil_data_prob_sample(output, count, dist_id.c_str(), type_id.c_str(), params);
    }

};

} // namespace fossil::data
#endif

#endif /* FOSSIL_DATA_PROB_H */
