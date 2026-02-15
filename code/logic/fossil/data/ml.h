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
#ifndef FOSSIL_DATA_ML_H
#define FOSSIL_DATA_ML_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Machine learning utilities for training and inference.
 *
 * Implements basic ML tasks (regression, classification, clustering) using internal model logic.
 *
 * Internal model definitions:
 *   - fossil_ml_model_kind_t: MODEL_LINEAR, MODEL_LOGISTIC, MODEL_KMEANS
 *   - fossil_ml_model_t: stores model kind, shape, weights (regression), centers (kmeans), cluster count
 *
 * Supported type string IDs:
 *   - "i8", "i16", "i32", "i64"
 *   - "u8", "u16", "u32", "u64", "size"
 *   - "f32", "f64"
 *   - "bool"
 *   - "hex", "oct", "bin"
 *
 * Supported model string IDs:
 *   - "linear_regression" (MODEL_LINEAR)
 *   - "logistic_regression" (MODEL_LOGISTIC)
 *   - "kmeans" (MODEL_KMEANS)
 *
 * Model handles are opaque pointers managed by the library.
 */

/**
 * @brief Train a machine learning model.
 *
 * This function trains a machine learning model using the provided input data and labels.
 * The type of data and model are specified by string IDs. The resulting model is returned
 * as an opaque handle through the model_handle pointer.
 *
 * @param X            Pointer to the input feature matrix (row-major order).
 * @param y            Pointer to the target labels or values.
 * @param rows         Number of samples (rows) in the input data.
 * @param cols         Number of features (columns) in the input data.
 * @param type_id      String ID specifying the data type ("i32", "i64", "f32", "f64").
 * @param model_id     String ID specifying the model type ("linear_regression", etc.).
 * @param model_handle Output pointer to the trained model handle (opaque pointer).
 * @return             0 on success, non-zero on failure.
 */
int fossil_data_ml_train(
    const void* X,
    const void* y,
    size_t rows,
    size_t cols,
    const char* type_id,
    const char* model_id,
    void** model_handle
);

/**
 * @brief Make predictions using a trained machine learning model.
 *
 * This function uses a previously trained model to make predictions on new input data.
 * The predictions are written to the y_pred buffer.
 *
 * @param X            Pointer to the input feature matrix (row-major order).
 * @param rows         Number of samples (rows) in the input data.
 * @param cols         Number of features (columns) in the input data.
 * @param y_pred       Output buffer for predicted values or labels.
 * @param model_handle Opaque pointer to the trained model.
 * @param type_id      String ID specifying the data type ("i32", "i64", "f32", "f64").
 * @return             0 on success, non-zero on failure.
 */
int fossil_data_ml_predict(
    const void* X,
    size_t rows,
    size_t cols,
    void* y_pred,
    void* model_handle,
    const char* type_id
);

/**
 * @brief Free the resources associated with a trained machine learning model.
 *
 * This function releases all memory and resources held by a model previously
 * created by fossil_data_ml_train.
 *
 * @param model_handle Opaque pointer to the trained model to be freed.
 * @return             0 on success, non-zero on failure.
 */
int fossil_data_ml_free_model(void* model_handle);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>

namespace fossil::data {

class ML {
public:
    /**
     * @brief Train a machine learning model (C++ wrapper).
     *
     * This static method wraps the C API fossil_data_ml_train, allowing C++ code to
     * train a machine learning model using type-safe std::string parameters.
     * Returns an opaque pointer to the trained model on success, or nullptr on failure.
     *
     * @param X        Pointer to the input feature matrix (row-major order).
     * @param y        Pointer to the target labels or values.
     * @param rows     Number of samples (rows) in the input data.
     * @param cols     Number of features (columns) in the input data.
     * @param type_id  String specifying the data type ("i32", "i64", "f32", "f64").
     * @param model_id String specifying the model type ("linear_regression", etc.).
     * @return         Opaque pointer to the trained model, or nullptr on failure.
     */
    static void* train(
        const void* X,
        const void* y,
        size_t rows,
        size_t cols,
        const std::string& type_id,
        const std::string& model_id
    ) {
        void* model_handle = nullptr;
        int result = fossil_data_ml_train(
            X, y, rows, cols, type_id.c_str(), model_id.c_str(), &model_handle
        );
        return (result == 0) ? model_handle : nullptr;
    }

    /**
     * @brief Make predictions using a trained machine learning model (C++ wrapper).
     *
     * This static method wraps the C API fossil_data_ml_predict, allowing C++ code to
     * make predictions using a previously trained model. The predictions are written
     * to the y_pred buffer.
     *
     * @param X            Pointer to the input feature matrix (row-major order).
     * @param rows         Number of samples (rows) in the input data.
     * @param cols         Number of features (columns) in the input data.
     * @param y_pred       Output buffer for predicted values or labels.
     * @param model_handle Opaque pointer to the trained model.
     * @param type_id      String specifying the data type ("i32", "i64", "f32", "f64").
     * @return             0 on success, non-zero on failure.
     */
    static int predict(
        const void* X,
        size_t rows,
        size_t cols,
        void* y_pred,
        void* model_handle,
        const std::string& type_id
    ) {
        return fossil_data_ml_predict(
            X, rows, cols, y_pred, model_handle, type_id.c_str()
        );
    }

    /**
     * @brief Free the resources associated with a trained machine learning model (C++ wrapper).
     *
     * This static method wraps the C API fossil_data_ml_free_model, releasing all memory
     * and resources held by a model previously created by train().
     *
     * @param model_handle Opaque pointer to the trained model to be freed.
     */
    static void free_model(void* model_handle) {
        fossil_data_ml_free_model(model_handle);
    }
};

} // namespace fossil::data
#endif

#endif /* FOSSIL_DATA_ML_H */
