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
#ifndef FOSSIL_DATA_PLOT_H
#define FOSSIL_DATA_PLOT_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Lightweight plotting utilities.
 *
 * Supports line plots and histograms using terminal or ASCII-based visualization.
 *
 * Supported type string IDs:
 *   - "i8", "i16", "i32", "i64"
 *   - "u8", "u16", "u32", "u64", "size"
 *   - "f32", "f64"
 *   - "bool"
 *   - "hex", "oct", "bin"
 *
 * Title string IDs can be arbitrary cstrings describing the plot.
 */

/**
 * @brief Plot a line chart in the terminal or ASCII-based visualization.
 *
 * @param y        Pointer to the array of y-values to plot.
 * @param count    Number of elements in the y array.
 * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
 * @param title_id String identifier for the plot title.
 * @return         0 on success, non-zero on error.
 */
int fossil_data_plot_line(
    const void* y,
    size_t count,
    const char* type_id,
    const char* title_id
);

/**
 * @brief Plot a histogram in the terminal or ASCII-based visualization.
 *
 * @param data     Pointer to the array of data to plot.
 * @param count    Number of elements in the data array.
 * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
 * @param bins     Number of bins to use in the histogram.
 * @param title_id String identifier for the plot title.
 * @return         0 on success, non-zero on error.
 */
int fossil_data_plot_histogram(
    const void* data,
    size_t count,
    const char* type_id,
    size_t bins,
    const char* title_id
);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
#include <string>

namespace fossil::data {

/**
 * @brief Plot utility class (C++ interface)
 *
 * Provides static methods for creating line plots and histograms using a simple C++ interface.
 * The methods wrap the underlying C API functions, allowing for type-safe string parameters
 * and integration with C++ codebases.
 */
class Plot {
public:

    /*
     * @brief Plot a line chart (C++ wrapper).
     * This static method wraps the C API fossil_data_plot_line, allowing C++ code to
     * create line plots using a simple interface. The y-values are provided as a pointer
     * to an array, along with the count of elements and type information. The title_id can
     * be any descriptive string for the plot title.
     * @param y        Pointer to the array of y-values to plot.
     * @param count    Number of elements in the y array.
     * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
     * @param title_id String identifier for the plot title.
     * @return         0 on success, non-zero on error.
     */
    static int line(const void* y, size_t count, const std::string& type_id,
                    const std::string& title_id) {
        return fossil_data_plot_line(y, count, type_id.c_str(), title_id.c_str());
    }

    /*
     * @brief Plot a histogram (C++ wrapper).
     * This static method wraps the C API fossil_data_plot_histogram, allowing C++ code to
     * create histograms using a simple interface. The data is provided as a pointer to an
     * array, along with the count of elements, type information, and the number of bins.
     * The title_id can be any descriptive string for the plot title.
     * @param data     Pointer to the array of data to plot.
     * @param count    Number of elements in the data array.
     * @param type_id  String identifier for the data type ("i32", "i64", "f32", "f64").
     * @param bins     Number of bins to use in the histogram.
     * @param title_id String identifier for the plot title.
     * @return         0 on success, non-zero on error.
     */
    static int histogram(const void* data, size_t count, const std::string& type_id,
                         size_t bins, const std::string& title_id) {
        return fossil_data_plot_histogram(data, count, type_id.c_str(), bins, title_id.c_str());
    }

};

} // namespace fossil::data
#endif

#endif /* FOSSIL_DATA_PLOT_H */
