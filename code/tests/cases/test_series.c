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
FOSSIL_SUITE(c_series_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_series_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_series_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_series_cumsum_i32) {
    int32_t input[5] = {1, 2, 3, 4, 5};
    int32_t output[5] = {0};
    int rc = fossil_data_series_cumsum(input, output, 5, "i32");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_I32(1, output[0]);
    ASSUME_ITS_EQUAL_I32(3, output[1]);
    ASSUME_ITS_EQUAL_I32(6, output[2]);
    ASSUME_ITS_EQUAL_I32(10, output[3]);
    ASSUME_ITS_EQUAL_I32(15, output[4]);
}

FOSSIL_TEST(c_test_series_cumsum_f64) {
    double input[4] = {0.5, 1.5, 2.0, 3.0};
    double output[4] = {0};
    int rc = fossil_data_series_cumsum(input, output, 4, "f64");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_F64(0.5, output[0], 1e-9);
    ASSUME_ITS_EQUAL_F64(2.0, output[1], 1e-9);
    ASSUME_ITS_EQUAL_F64(4.0, output[2], 1e-9);
    ASSUME_ITS_EQUAL_F64(7.0, output[3], 1e-9);
}

FOSSIL_TEST(c_test_series_rolling_mean_i32) {
    int32_t input[5] = {10, 20, 30, 40, 50};
    int32_t output[5] = {0};
    int rc = fossil_data_series_rolling_mean(input, output, 5, 3, "i32");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_I32(10, output[0]); // (10)/1
    ASSUME_ITS_EQUAL_I32(15, output[1]); // (10+20)/2
    ASSUME_ITS_EQUAL_I32(20, output[2]); // (10+20+30)/3
    ASSUME_ITS_EQUAL_I32(30, output[3]); // (20+30+40)/3
    ASSUME_ITS_EQUAL_I32(40, output[4]); // (30+40+50)/3
}

FOSSIL_TEST(c_test_series_rolling_mean_f32) {
    float input[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float output[4] = {0};
    int rc = fossil_data_series_rolling_mean(input, output, 4, 2, "f32");
    ASSUME_ITS_EQUAL_I32(0, rc);
    ASSUME_ITS_EQUAL_F32(1.0f, output[0], 1e-6f); // (1)/1
    ASSUME_ITS_EQUAL_F32(1.5f, output[1], 1e-6f); // (1+2)/2
    ASSUME_ITS_EQUAL_F32(2.5f, output[2], 1e-6f); // (2+3)/2
    ASSUME_ITS_EQUAL_F32(3.5f, output[3], 1e-6f); // (3+4)/2
}

FOSSIL_TEST(c_test_series_cumsum_invalid_args) {
    int rc;
    int32_t input[2] = {1, 2};
    int32_t output[2] = {0};
    rc = fossil_data_series_cumsum(NULL, output, 2, "i32");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_cumsum(input, NULL, 2, "i32");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_cumsum(input, output, 0, "i32");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_cumsum(input, output, 2, "badtype");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_series_rolling_mean_invalid_args) {
    int rc;
    double input[2] = {1.0, 2.0};
    double output[2] = {0.0};
    rc = fossil_data_series_rolling_mean(NULL, output, 2, 1, "f64");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_rolling_mean(input, NULL, 2, 1, "f64");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_rolling_mean(input, output, 0, 1, "f64");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_rolling_mean(input, output, 2, 0, "f64");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_series_rolling_mean(input, output, 2, 1, "badtype");
    ASSUME_ITS_TRUE(rc != 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_series_tests) {
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_cumsum_i32);
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_cumsum_f64);
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_rolling_mean_i32);
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_rolling_mean_f32);
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_cumsum_invalid_args);
    FOSSIL_TEST_ADD(c_series_suite, c_test_series_rolling_mean_invalid_args);

    // Register the test suite
    FOSSIL_TEST_REGISTER(c_series_suite);
}
