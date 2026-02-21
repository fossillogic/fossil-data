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
#include <fossil/pizza/framework.h>

#include "fossil/data/framework.h"


// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Utilites
// * * * * * * * * * * * * * * * * * * * * * * * *
// Setup steps for things like test fixtures and
// mock objects are set here.
// * * * * * * * * * * * * * * * * * * * * * * * *

// Define the test suite and add test cases
FOSSIL_SUITE(c_tensor_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_tensor_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_tensor_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_tensor_elements) {
    size_t shape1d[1] = {5};
    size_t elements = 0;
    int rc = fossil_data_tensor_elements(shape1d, 1, &elements);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_SIZE(elements, 5);

    size_t shape2d[2] = {3, 4};
    rc = fossil_data_tensor_elements(shape2d, 2, &elements);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_SIZE(elements, 12);

    size_t shape3d[3] = {2, 3, 4};
    rc = fossil_data_tensor_elements(shape3d, 3, &elements);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_SIZE(elements, 24);

    rc = fossil_data_tensor_elements(NULL, 1, &elements);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_tensor_elements(shape1d, 1, NULL);
    ASSUME_NOT_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_tensor_minmax_i32) {
    int32_t data[5] = {10, 25, 5, 30, 15};
    int32_t min_val = 0, max_val = 0;
    int rc = fossil_data_tensor_minmax(data, 5, "i32", &min_val, &max_val);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_I32(min_val, 5);
    ASSUME_ITS_EQUAL_I32(max_val, 30);

    rc = fossil_data_tensor_minmax(NULL, 5, "i32", &min_val, &max_val);
    ASSUME_NOT_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_tensor_minmax_f64) {
    double data[4] = {1.5, -2.3, 0.0, 4.7};
    double min_val = 0.0, max_val = 0.0;
    int rc = fossil_data_tensor_minmax(data, 4, "f64", &min_val, &max_val);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_F64(min_val, -2.3, 0.001);
    ASSUME_ITS_EQUAL_F64(max_val, 4.7, 0.001);
}

FOSSIL_TEST(c_test_tensor_minmax_u8) {
    uint8_t data[6] = {0, 255, 128, 64, 192, 32};
    uint8_t min_val = 0, max_val = 0;
    int rc = fossil_data_tensor_minmax(data, 6, "u8", &min_val, &max_val);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_U8(min_val, 0);
    ASSUME_ITS_EQUAL_U8(max_val, 255);
}

FOSSIL_TEST(c_test_tensor_minmax_invalid_type) {
    int32_t data[2] = {1, 2};
    int32_t min_val = 0, max_val = 0;
    int rc = fossil_data_tensor_minmax(data, 2, "invalid_type", &min_val, &max_val);
    ASSUME_NOT_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_tensor_mean_f64) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double mean = 0.0;
    int rc = fossil_data_tensor_mean(data, 5, "f64", &mean);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_F64(mean, 3.0, 0.001);
}

FOSSIL_TEST(c_test_tensor_mean_i32) {
    int32_t data[4] = {10, 20, 30, 40};
    double mean = 0.0;
    int rc = fossil_data_tensor_mean(data, 4, "i32", &mean);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_F64(mean, 25.0, 0.001);
}

FOSSIL_TEST(c_test_tensor_mean_invalid_args) {
    double data[2] = {1.0, 2.0};
    double mean = 0.0;
    int rc;

    rc = fossil_data_tensor_mean(NULL, 2, "f64", &mean);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_tensor_mean(data, 2, NULL, &mean);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_tensor_mean(data, 2, "f64", NULL);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_tensor_mean(data, 0, "f64", &mean);
    ASSUME_NOT_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_tensor_mean_f32) {
    float data[3] = {1.5f, 2.5f, 3.0f};
    double mean = 0.0;
    int rc = fossil_data_tensor_mean(data, 3, "f32", &mean);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_ITS_EQUAL_F64(mean, 2.333, 0.01);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_tensor_tests) {
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_elements);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_minmax_i32);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_minmax_f64);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_minmax_u8);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_minmax_invalid_type);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_mean_f64);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_mean_i32);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_mean_invalid_args);
    FOSSIL_TEST_ADD(c_tensor_suite, c_test_tensor_mean_f32);

    // Register the test suite
    FOSSIL_TEST_REGISTER(c_tensor_suite);
}
