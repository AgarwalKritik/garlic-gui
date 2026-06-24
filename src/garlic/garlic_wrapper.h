//  Copyright 2026 Kritik Agarwal
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//
// Garlic Wrapper - Interface between C++ GUI and Garlic C decompiler
// This file provides C interface functions that can be called from C++

#ifndef GARLIC_WRAPPER_H
#define GARLIC_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif

    // Main decompilation function
    // Returns 1 on success, 0 on failure
    int garlic_decompile_file(const char *input_path, const char *output_path, int thread_num);

    // Progress callback registration
    // Callback will be called with progress values from 0 to 100
    void garlic_set_progress_callback(void (*callback)(int progress));

    // File validation functions
    int garlic_is_valid_file(const char *file_path);
    int garlic_is_valid_apk(const char *file_path);
    int garlic_is_valid_dex(const char *file_path);
    int garlic_is_valid_jar(const char *file_path);
    int garlic_is_valid_class(const char *file_path);

    // Utility functions
    const char *garlic_get_version();
    const char *garlic_get_file_type_string(const char *file_path);
    int garlic_get_current_progress();

#ifdef __cplusplus
}
#endif

#endif // GARLIC_WRAPPER_H