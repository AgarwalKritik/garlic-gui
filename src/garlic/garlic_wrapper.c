/*
 * Garlic Wrapper - Interface between C++ GUI and Garlic C decompiler
 * This file provides C interface functions that can be called from C++
 */

#include "garlic_wrapper.h"
#include "parser/class/metadata.h"
#include "jvm/jvm_decompile.h"
#include "common/str_tools.h"
#include "common/file_tools.h"
#include "libs/memory/mem_pool.h"
#include "jar/jar.h"
#include "apk/apk.h"
#include "dalvik/dex_decompile.h"
#include "dalvik/dex_smali.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifdef __linux__
#include <libgen.h>
#endif

#define JAVA_CLASS_MAGIC 0xCAFEBABE
#define JAR_FILE_MAGIC 0x504B0304
#define DEX_FILE_MAGIC 0x6465780A

typedef enum
{
    JD_FILE_TYPE_UNKNOWN = 0,
    JD_FILE_TYPE_JAVA_CLASS,
    JD_FILE_TYPE_JAR,
    JD_FILE_TYPE_DEX,
    JD_FILE_TYPE_APK,
} jd_file_type_t;

// Progress callback function pointer
static void (*progress_callback)(int progress) = NULL;
static volatile int current_progress = 0;

// Thread-safe progress update
static void update_progress(int progress)
{
    current_progress = progress;
    if (progress_callback)
    {
        progress_callback(progress);
    }
}

static jd_file_type_t magic_of_file(const char *filepath)
{
    FILE *fp = fopen(filepath, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "[garlic] Open file: %s failed\n", filepath);
        return JD_FILE_TYPE_UNKNOWN;
    }

    uint32_t magic = 0;
    size_t bytes_read = fread(&magic, 1, sizeof(magic), fp);
    fclose(fp);

    if (bytes_read != sizeof(magic))
    {
        fprintf(stderr, "[garlic] File %s read error.\n", filepath);
        return JD_FILE_TYPE_UNKNOWN;
    }

    uint32_t be_magic = ((magic & 0xFF) << 24) |
                        (((magic >> 8) & 0xFF) << 16) |
                        (((magic >> 16) & 0xFF) << 8) |
                        ((magic >> 24) & 0xFF);

    switch (be_magic)
    {
    case JAVA_CLASS_MAGIC:
        return JD_FILE_TYPE_JAVA_CLASS;
    case JAR_FILE_MAGIC:
    {
        if (str_end_with((char *)filepath, ".apk"))
        {
            return JD_FILE_TYPE_APK;
        }
        else
        {
            return JD_FILE_TYPE_JAR;
        }
    }
    case DEX_FILE_MAGIC:
        return JD_FILE_TYPE_DEX;
    default:
        fprintf(stderr, "[garlic] file: %s is not a "
                        "valid Java class/JAR/DEX file\n",
                filepath);
        return JD_FILE_TYPE_UNKNOWN;
    }
}

static void prepare_output_directory(const char *input_path, const char *output_path)
{
    if (output_path == NULL)
    {
        return;
    }
    mkdir_p((char *)output_path);
}

static int decompile_apk_file(const char *input_path, const char *output_path)
{
    update_progress(10);

    prepare_output_directory(input_path, output_path);

    update_progress(20);

    // Use 4 threads by default for GUI
    int thread_num = 4;

    update_progress(30);

    // Call Garlic's APK decompilation function
    apk_decompile_analyse((char *)input_path,
                          (char *)output_path,
                          thread_num,
                          JD_DEX_TASK_DECOMPILE);

    update_progress(100);
    return 1; // Success - Garlic functions don't return error codes
}

static int decompile_dex_file(const char *input_path, const char *output_path)
{
    update_progress(10);

    prepare_output_directory(input_path, output_path);

    update_progress(20);

    // Use 4 threads by default for GUI
    int thread_num = 4;

    update_progress(30);

    // Call Garlic's DEX decompilation function
    dex_file_analyse((char *)input_path,
                     (char *)output_path,
                     thread_num,
                     JD_DEX_TASK_DECOMPILE);

    update_progress(100);
    return 1; // Success - Garlic functions don't return error codes
}

static int decompile_jar_file(const char *input_path, const char *output_path)
{
    update_progress(10);

    prepare_output_directory(input_path, output_path);

    update_progress(20);

    // Use 4 threads by default for GUI
    int thread_num = 4;

    update_progress(30);

    // Call Garlic's JAR decompilation function
    jar_file_analyse((char *)input_path,
                     (char *)output_path,
                     thread_num);

    update_progress(100);
    return 1; // Success - Garlic functions don't return error codes
}

static int decompile_class_file(const char *input_path, const char *output_path)
{
    update_progress(10);

    mem_init_pool();

    update_progress(30);

    jclass_file *jc = parse_class_file((char *)input_path);
    if (jc == NULL)
    {
        mem_free_pool();
        return 0;
    }

    update_progress(60);

    // For single class files, we analyze and output to the specified directory
    jvm_analyse_class_file(jc->jfile);

    update_progress(90);

    mem_free_pool();

    update_progress(100);
    return 1;
}

// Main decompilation function
int garlic_decompile_file(const char *input_path, const char *output_path)
{
    if (!input_path || !output_path)
    {
        return 0; // failure
    }

    current_progress = 0;
    update_progress(0);

    jd_file_type_t file_type = magic_of_file(input_path);

    switch (file_type)
    {
    case JD_FILE_TYPE_APK:
        return decompile_apk_file(input_path, output_path);

    case JD_FILE_TYPE_DEX:
        return decompile_dex_file(input_path, output_path);

    case JD_FILE_TYPE_JAR:
        return decompile_jar_file(input_path, output_path);

    case JD_FILE_TYPE_JAVA_CLASS:
        return decompile_class_file(input_path, output_path);

    default:
        fprintf(stderr, "[garlic] Unsupported file type: %s\n", input_path);
        return 0;
    }
}

// Progress callback registration
void garlic_set_progress_callback(void (*callback)(int progress))
{
    progress_callback = callback;
}

// Utility functions
int garlic_is_valid_file(const char *file_path)
{
    if (!file_path)
        return 0;

    jd_file_type_t file_type = magic_of_file(file_path);
    return (file_type != JD_FILE_TYPE_UNKNOWN);
}

int garlic_is_valid_apk(const char *file_path)
{
    if (!file_path)
        return 0;

    jd_file_type_t file_type = magic_of_file(file_path);
    return (file_type == JD_FILE_TYPE_APK);
}

int garlic_is_valid_dex(const char *file_path)
{
    if (!file_path)
        return 0;

    jd_file_type_t file_type = magic_of_file(file_path);
    return (file_type == JD_FILE_TYPE_DEX);
}

int garlic_is_valid_jar(const char *file_path)
{
    if (!file_path)
        return 0;

    jd_file_type_t file_type = magic_of_file(file_path);
    return (file_type == JD_FILE_TYPE_JAR);
}

int garlic_is_valid_class(const char *file_path)
{
    if (!file_path)
        return 0;

    jd_file_type_t file_type = magic_of_file(file_path);
    return (file_type == JD_FILE_TYPE_JAVA_CLASS);
}

const char *garlic_get_version()
{
    return "Garlic Decompiler v1.0.0 (GUI Integration)";
}

const char *garlic_get_file_type_string(const char *file_path)
{
    if (!file_path)
        return "Unknown";

    jd_file_type_t file_type = magic_of_file(file_path);

    switch (file_type)
    {
    case JD_FILE_TYPE_APK:
        return "Android APK";
    case JD_FILE_TYPE_DEX:
        return "Android DEX";
    case JD_FILE_TYPE_JAR:
        return "Java JAR";
    case JD_FILE_TYPE_JAVA_CLASS:
        return "Java Class";
    default:
        return "Unknown";
    }
}

int garlic_get_current_progress()
{
    return current_progress;
}