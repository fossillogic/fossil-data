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
FOSSIL_SUITE(c_transform_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_transform_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_transform_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_transform_scale_minmax_f64) {
    double input[5] = {2.0, 4.0, 6.0, 8.0, 10.0};
    double output[5] = {0};
    int rc = fossil_data_transform_scale(input, output, 5, "f64", "minmax");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_F64(0.0, output[0], 1e-8);
    ASSUME_ITS_EQUAL_F64(0.25, output[1], 1e-8);
    ASSUME_ITS_EQUAL_F64(0.5, output[2], 1e-8);
    ASSUME_ITS_EQUAL_F64(0.75, output[3], 1e-8);
    ASSUME_ITS_EQUAL_F64(1.0, output[4], 1e-8);
}

FOSSIL_TEST(c_test_transform_scale_zscore_i32) {
    int input[3] = {1, 2, 3};
    int output[3] = {0};
    int rc = fossil_data_transform_scale(input, output, 3, "i32", "zscore");
    ASSUME_ITS_EQUAL_I32(0, rc);
    // mean = 2, std = 0.81649658
    ASSUME_ITS_EQUAL_I32((int)((1-2)/0.81649658), output[0]);
    ASSUME_ITS_EQUAL_I32((int)((2-2)/0.81649658), output[1]);
    ASSUME_ITS_EQUAL_I32((int)((3-2)/0.81649658), output[2]);
}

FOSSIL_TEST(c_test_transform_scale_invalid_type) {
    double input[2] = {1.0, 2.0};
    double output[2] = {0};
    int rc = fossil_data_transform_scale(input, output, 2, "badtype", "minmax");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_transform_scale_invalid_method) {
    double input[2] = {1.0, 2.0};
    double output[2] = {0};
    int rc = fossil_data_transform_scale(input, output, 2, "f64", "badmethod");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_transform_scale_null_args) {
    double input[2] = {1.0, 2.0};
    double output[2] = {0};
    int rc = fossil_data_transform_scale(NULL, output, 2, "f64", "minmax");
    // Per implementation, returns 0 (no-op) for null/empty params
    ASSUME_ITS_EQUAL_I32(0, rc);
    rc = fossil_data_transform_scale(input, NULL, 2, "f64", "minmax");
    ASSUME_ITS_EQUAL_I32(0, rc);
    rc = fossil_data_transform_scale(input, output, 0, "f64", "minmax");
    ASSUME_ITS_EQUAL_I32(0, rc);
    rc = fossil_data_transform_scale(input, output, 2, NULL, "minmax");
    ASSUME_ITS_EQUAL_I32(0, rc);
    rc = fossil_data_transform_scale(input, output, 2, "f64", NULL);
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_transform_encode_label) {
    const char* input[4] = {"cat", "dog", "cat", "bird"};
    int output[4] = {0};
    int rc = fossil_data_transform_encode(input, output, 4, "cstr", "label");
    ASSUME_ITS_EQUAL_I32(0, rc);
    // "cat" and "dog" and "bird" should get unique labels
    ASSUME_ITS_TRUE(output[0] == output[2]);
    ASSUME_ITS_TRUE(output[0] != output[1]);
    ASSUME_ITS_TRUE(output[1] != output[3]);
}

FOSSIL_TEST(c_test_transform_encode_onehot) {
    const char* input[3] = {"red", "blue", "red"};
    int output[3] = {0};
    int rc = fossil_data_transform_encode(input, output, 3, "cstr", "onehot");
    ASSUME_ITS_EQUAL_I32(0, rc);
    // "red" and "blue" should get unique indices
    ASSUME_ITS_TRUE(output[0] == output[2]);
    ASSUME_ITS_TRUE(output[0] != output[1]);
}

FOSSIL_TEST(c_test_transform_encode_invalid_type) {
    const char* input[2] = {"a", "b"};
    int output[2] = {0};
    int rc = fossil_data_transform_encode(input, output, 2, "f64", "label");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_transform_encode_invalid_method) {
    const char* input[2] = {"a", "b"};
    int output[2] = {0};
    int rc = fossil_data_transform_encode(input, output, 2, "cstr", "badmethod");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_transform_encode_null_args) {
    const char* input[2] = {"a", "b"};
    int output[2] = {0};
    int rc = fossil_data_transform_encode(NULL, output, 2, "cstr", "label");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_transform_encode(input, NULL, 2, "cstr", "label");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_transform_encode(input, output, 2, NULL, "label");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_transform_encode(input, output, 2, "cstr", NULL);
    ASSUME_ITS_TRUE(rc != 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_transform_tests) {
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_scale_minmax_f64);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_scale_zscore_i32);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_scale_invalid_type);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_scale_invalid_method);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_scale_null_args);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_encode_label);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_encode_onehot);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_encode_invalid_type);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_encode_invalid_method);
    FOSSIL_TEST_ADD(c_transform_suite, c_test_transform_encode_null_args);

    // Register the test suite
    FOSSIL_TEST_REGISTER(c_transform_suite);
}
