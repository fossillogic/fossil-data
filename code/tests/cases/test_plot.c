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
FOSSIL_SUITE(c_plot_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_plot_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_plot_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_plot_line_f64) {
    double y[5] = {1.0, 2.0, 3.0, 2.0, 1.0};
    int rc = fossil_data_plot_line(y, 5, "f64", "Test Line Plot f64");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_plot_line_i32) {
    int y[6] = {0, 1, 4, 9, 16, 25};
    int rc = fossil_data_plot_line(y, 6, "i32", "Test Line Plot i32");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_plot_line_invalid_args) {
    double y[3] = {1.0, 2.0, 3.0};
    int rc;
    rc = fossil_data_plot_line(NULL, 3, "f64", "Null Data");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_plot_line(y, 0, "f64", "Zero Count");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_plot_line(y, 3, "badtype", "Bad Type");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_plot_histogram_f64) {
    double data[8] = {1.0, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5, 5.0};
    int rc = fossil_data_plot_histogram(data, 8, "f64", 4, "Test Histogram f64");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_plot_histogram_i32) {
    int data[10] = {1, 2, 2, 3, 3, 3, 4, 4, 5, 5};
    int rc = fossil_data_plot_histogram(data, 10, "i32", 5, "Test Histogram i32");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_plot_histogram_invalid_args) {
    double data[2] = {1.0, 2.0};
    int rc;
    rc = fossil_data_plot_histogram(NULL, 2, "f64", 2, "Null Data");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_plot_histogram(data, 0, "f64", 2, "Zero Count");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_plot_histogram(data, 2, "badtype", 2, "Bad Type");
    ASSUME_ITS_TRUE(rc != 0);
    rc = fossil_data_plot_histogram(data, 2, "f64", 0, "Zero Bins");
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(c_test_plot_line_bool) {
    uint8_t y[8] = {0, 1, 1, 0, 1, 0, 0, 1};
    int rc = fossil_data_plot_line(y, 8, "bool", "Test Line Plot bool");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

FOSSIL_TEST(c_test_plot_histogram_bool) {
    uint8_t data[6] = {1, 0, 1, 1, 0, 0};
    int rc = fossil_data_plot_histogram(data, 6, "bool", 2, "Test Histogram bool");
    ASSUME_ITS_EQUAL_I32(0, rc);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_plot_tests) {
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_line_f64);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_line_i32);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_line_invalid_args);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_histogram_f64);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_histogram_i32);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_histogram_invalid_args);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_line_bool);
    FOSSIL_TEST_ADD(c_plot_suite, c_test_plot_histogram_bool);


    // Register the test suite
    FOSSIL_TEST_REGISTER(c_plot_suite);
}
