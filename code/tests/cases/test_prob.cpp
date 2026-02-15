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
FOSSIL_SUITE(cpp_prob_suite);

// Setup function for the test suite
FOSSIL_SETUP(cpp_prob_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(cpp_prob_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(cpp_test_prob_mean_f64) {
    double data[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double result = fossil::data::Prob::mean(data, 5, "f64");
    ASSUME_ITS_EQUAL_F64(3.0, result, 1e-9);
}

FOSSIL_TEST(cpp_test_prob_mean_i32) {
    int32_t data[4] = {10, 20, 30, 40};
    double result = fossil::data::Prob::mean(data, 4, "i32");
    ASSUME_ITS_EQUAL_F64(25.0, result, 1e-9);
}

FOSSIL_TEST(cpp_test_prob_mean_invalid_type) {
    double data[2] = {1.0, 2.0};
    double result = fossil::data::Prob::mean(data, 2, "badtype");
    ASSUME_ITS_TRUE(std::isnan(result));
}

FOSSIL_TEST(cpp_test_prob_mean_zero_count) {
    double data[2] = {1.0, 2.0};
    double result = fossil::data::Prob::mean(data, 0, "f64");
    ASSUME_ITS_TRUE(std::isnan(result));
}

FOSSIL_TEST(cpp_test_prob_std_f64) {
    double data[4] = {2.0, 4.0, 4.0, 4.0};
    double result = fossil::data::Prob::std(data, 4, "f64");
    ASSUME_ITS_EQUAL_F64(0.8660254037844386, result, 1e-9);
}

FOSSIL_TEST(cpp_test_prob_std_i32) {
    int32_t data[5] = {1, 2, 3, 4, 5};
    double result = fossil::data::Prob::std(data, 5, "i32");
    ASSUME_ITS_EQUAL_F64(sqrt(2.0), result, 1e-9);
}

FOSSIL_TEST(cpp_test_prob_std_invalid_type) {
    double data[2] = {1.0, 2.0};
    double result = fossil::data::Prob::std(data, 2, "badtype");
    ASSUME_ITS_TRUE(std::isnan(result));
}

FOSSIL_TEST(cpp_test_prob_std_zero_count) {
    double data[2] = {1.0, 2.0};
    double result = fossil::data::Prob::std(data, 0, "f64");
    ASSUME_ITS_TRUE(std::isnan(result));
}

FOSSIL_TEST(cpp_test_prob_sample_uniform_f32) {
    float output[10] = {0};
    struct { double a; double b; } params = {2.0, 5.0};
    int rc = fossil::data::Prob::sample(output, 10, "uniform", "f32", &params);
    ASSUME_ITS_EQUAL_I32(0, rc);
    for (int i = 0; i < 10; ++i) {
        ASSUME_ITS_TRUE(output[i] >= 2.0f && output[i] <= 5.0f);
    }
}

FOSSIL_TEST(cpp_test_prob_sample_normal_f64) {
    double output[100] = {0};
    struct { double mean; double std; } params = {10.0, 2.0};
    int rc = fossil::data::Prob::sample(output, 100, "normal", "f64", &params);
    ASSUME_ITS_EQUAL_I32(0, rc);
    double sum = 0.0;
    for (int i = 0; i < 100; ++i) sum += output[i];
    double mean = sum / 100.0;
    ASSUME_ITS_TRUE(mean > 8.0 && mean < 12.0);
}

FOSSIL_TEST(cpp_test_prob_sample_binomial_i32) {
    int32_t output[20] = {0};
    struct { int n; double p; } params = {10, 0.5};
    int rc = fossil::data::Prob::sample(output, 20, "binomial", "i32", &params);
    ASSUME_ITS_EQUAL_I32(0, rc);
    for (int i = 0; i < 20; ++i) {
        ASSUME_ITS_TRUE(output[i] >= 0 && output[i] <= 10);
    }
}

FOSSIL_TEST(cpp_test_prob_sample_invalid_dist) {
    double output[2] = {0};
    int rc = fossil::data::Prob::sample(output, 2, "bad_dist", "f64", NULL);
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(cpp_test_prob_sample_invalid_type) {
    double output[2] = {0};
    struct { double a; double b; } params = {0.0, 1.0};
    int rc = fossil::data::Prob::sample(output, 2, "uniform", "badtype", &params);
    ASSUME_ITS_TRUE(rc != 0);
}

FOSSIL_TEST(cpp_test_prob_sample_null_params) {
    double output[2] = {0};
    int rc = fossil::data::Prob::sample(output, 2, "uniform", "f64", NULL);
    ASSUME_ITS_TRUE(rc != 0);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(cpp_prob_tests) {
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_mean_f64);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_mean_i32);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_mean_invalid_type);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_mean_zero_count);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_std_f64);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_std_i32);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_std_invalid_type);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_std_zero_count);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_uniform_f32);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_normal_f64);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_binomial_i32);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_invalid_dist);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_invalid_type);
    FOSSIL_TEST_ADD(cpp_prob_suite, cpp_test_prob_sample_null_params);

    // Register the test suite
    FOSSIL_TEST_REGISTER(cpp_prob_suite);
}
