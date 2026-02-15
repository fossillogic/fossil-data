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
FOSSIL_SUITE(c_ml_suite);

// Setup function for the test suite
FOSSIL_SETUP(c_ml_suite) {
    // Setup code here
}

// Teardown function for the test suite
FOSSIL_TEARDOWN(c_ml_suite) {
    // Teardown code here
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Cases
// * * * * * * * * * * * * * * * * * * * * * * * *
// The test cases below are provided as samples, inspired
// by the Meson build system's approach of using test cases
// as samples for library usage.
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST(c_test_ml_linear_regression_f64) {
    // Simple linear regression: y = 2x + 1
    double X[4] = {1.0, 2.0, 3.0, 4.0};
    double y[4] = {3.0, 5.0, 7.0, 9.0};
    void* model = NULL;
    int rc = fossil_data_ml_train(X, y, 4, 1, "f64", "linear_regression", &model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_NOT_CNULL(model);

    double X_test[2] = {5.0, 6.0};
    double y_pred[2] = {0};
    rc = fossil_data_ml_predict(X_test, 2, 1, y_pred, model, "f64");
    ASSUME_ITS_EQUAL_I32(rc, 0);
    printf("Linear Regression Predictions: %f %f\n", y_pred[0], y_pred[1]);
    ASSUME_ITS_EQUAL_F64(y_pred[0], 11.0, 0.5);
    ASSUME_ITS_EQUAL_F64(y_pred[1], 13.0, 0.8);

    rc = fossil_data_ml_free_model(model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_ml_logistic_regression_i32) {
    // Simple binary classification: x < 3 => 0, x >= 3 => 1
    int X[6] = {1, 2, 3, 4, 5, 6};
    int y[6] = {0, 0, 1, 1, 1, 1};
    void* model = NULL;
    int rc = fossil_data_ml_train(X, y, 6, 1, "i32", "logistic_regression", &model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_NOT_CNULL(model);

    int X_test[2] = {2, 5};
    int y_pred[2] = {0};
    rc = fossil_data_ml_predict(X_test, 2, 1, y_pred, model, "i32");
    ASSUME_ITS_EQUAL_I32(rc, 0);
    printf("Logistic Regression Predictions: %d %d\n", y_pred[0], y_pred[1]);
    ASSUME_ITS_WITHIN_RANGE_I32(y_pred[0], 0, 1);
    ASSUME_ITS_WITHIN_RANGE_I32(y_pred[1], 0, 1);

    rc = fossil_data_ml_free_model(model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_ml_kmeans_f32) {
    // Two clusters: [1,2,1.5] and [8,9,10]
    float X[6] = {1.0f, 2.0f, 1.5f, 8.0f, 9.0f, 10.0f};
    int y[6] = {0}; // Not used for kmeans
    void* model = NULL;
    int rc = fossil_data_ml_train(X, y, 6, 1, "f32", "kmeans", &model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
    ASSUME_NOT_CNULL(model);

    float X_test[2] = {1.2f, 9.5f};
    int y_pred[2] = {0};
    rc = fossil_data_ml_predict(X_test, 2, 1, y_pred, model, "i32");
    ASSUME_ITS_EQUAL_I32(rc, 0);
    printf("KMeans Predictions: %d %d\n", y_pred[0], y_pred[1]);
    ASSUME_ITS_WITHIN_RANGE_I32(y_pred[0], 0, 1);
    ASSUME_ITS_WITHIN_RANGE_I32(y_pred[1], 0, 1);

    rc = fossil_data_ml_free_model(model);
    ASSUME_ITS_EQUAL_I32(rc, 0);
}

FOSSIL_TEST(c_test_ml_invalid_args) {
    double X[2] = {1.0, 2.0};
    double y[2] = {1.0, 2.0};
    void* model = NULL;
    int rc;

    rc = fossil_data_ml_train(NULL, y, 2, 1, "f64", "linear_regression", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_ml_train(X, NULL, 2, 1, "f64", "linear_regression", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_ml_train(X, y, 0, 1, "f64", "linear_regression", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_ml_train(X, y, 2, 0, "f64", "linear_regression", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_ml_train(X, y, 2, 1, "badtype", "linear_regression", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    rc = fossil_data_ml_train(X, y, 2, 1, "f64", "badmodel", &model);
    ASSUME_NOT_EQUAL_I32(rc, 0);

    // Only run predict tests if model is not NULL
    if (model != NULL) {
        rc = fossil_data_ml_predict(NULL, 2, 1, y, model, "f64");
        ASSUME_NOT_EQUAL_I32(rc, 0);

        rc = fossil_data_ml_predict(X, 0, 1, y, model, "f64");
        ASSUME_NOT_EQUAL_I32(rc, 0);

        rc = fossil_data_ml_predict(X, 2, 1, NULL, model, "f64");
        ASSUME_NOT_EQUAL_I32(rc, 0);

        rc = fossil_data_ml_predict(X, 2, 1, y, NULL, "f64");
        ASSUME_NOT_EQUAL_I32(rc, 0);
    }

    fossil_data_ml_free_model(NULL);
}

// * * * * * * * * * * * * * * * * * * * * * * * *
// * Fossil Logic Test Pool
// * * * * * * * * * * * * * * * * * * * * * * * *

FOSSIL_TEST_GROUP(c_ml_tests) {
    FOSSIL_TEST_ADD(c_ml_suite, c_test_ml_linear_regression_f64);
    FOSSIL_TEST_ADD(c_ml_suite, c_test_ml_logistic_regression_i32);
    FOSSIL_TEST_ADD(c_ml_suite, c_test_ml_kmeans_f32);
    FOSSIL_TEST_ADD(c_ml_suite, c_test_ml_invalid_args);

    // Register the test suite
    FOSSIL_TEST_REGISTER(c_ml_suite);
}
