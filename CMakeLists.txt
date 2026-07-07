cmake_minimum_required(VERSION 3.16)

# ==============================================================================
#               Copyright 2026 Kritik Agarwal
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#          http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ==============================================================================
#
# Garlic GUI
#
# A modern Qt6-based graphical front-end for the Garlic Decompiler.
#
# The project consists of two major components:
#   • GUI Layer    (C++17 / Qt6)
#   • Garlic Core  (C99)
#
# The Garlic Core is embedded under `src/garlic` and periodically synchronized
# with the upstream Garlic repository using `scripts/sync_garlic.py`.
#
# ==============================================================================
#
# Build Overview
#
# Sections:
#   1. Project Configuration
#   2. Qt Configuration
#   3. Garlic Core Configuration
#   4. Source Collection
#   5. Build Target
#   6. Platform Configuration
#   7. Compiler Configuration
#   8. Maintenance Targets
#   9. Installation
#  10. Packaging
#
# ==============================================================================
# Project Configuration
# ==============================================================================
project(GarlicGUI VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

# ==============================================================================
# Standalone Build (Static Qt6)
#
# Enabled by default. Produces a single binary with zero runtime DLL/SO
# dependencies. Devs can turn it off with -DGARLIC_STATIC_QT=OFF for faster
# iteration with shared Qt6 DLLs.
# ==============================================================================
option(GARLIC_STATIC_QT "Statically link Qt6 for a fully standalone binary" ON)

# ==============================================================================
# Apple Platform Configuration
#
# Configure OpenGL discovery before locating Qt to prevent CMake from linking
# against deprecated Apple frameworks.
# ==============================================================================
if (APPLE)
    # Prevent CMake's FindOpenGL from adding the deprecated AGL framework
    set(OPENGL_agl_LIBRARY "" CACHE STRING "AGL framework is deprecated" FORCE)
    set(OpenGL_GL_PREFERENCE GLVND)
    # Disable deprecated frameworks
    set(CMAKE_OSX_DEPLOYMENT_TARGET 11.0)
endif ()


# ==============================================================================
# Qt Configuration
# ==============================================================================
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

get_target_property(QT_WIDGETS_LINKS Qt6::Widgets INTERFACE_LINK_LIBRARIES)
message(STATUS "Qt6::Widgets => ${QT_WIDGETS_LINKS}")

get_target_property(QT_CORE_LINKS Qt6::Core INTERFACE_LINK_LIBRARIES)
message(STATUS "Qt6::Core => ${QT_CORE_LINKS}")

get_target_property(QT_GUI_LINKS Qt6::Gui INTERFACE_LINK_LIBRARIES)
message(STATUS "Qt6::Gui => ${QT_GUI_LINKS}")

# ==============================================================================
# Qt Automatic Code Generation
#
# Enable automatic processing of:
#   • MOC (Meta-Object Compiler)
#   • UIC (User Interface Compiler)
#   • RCC (Qt Resource Compiler)
# ==============================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ==============================================================================
# Garlic Core Compiler Configuration
#
# These flags mirror the upstream Garlic project to ensure identical compiler
# behavior and optimized performance.
# ==============================================================================
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-incompatible-pointer-types")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-format")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare")

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS
        "${CMAKE_C_FLAGS} -Wno-unused-but-set-variable -Wno-misleading-indentation -Wno-type-limits -ffunction-sections -fdata-sections -Wl,--gc-sections")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux"
   AND CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
endif()

if (UNIX)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread")
endif ()

# ==============================================================================
# Garlic Core Include Directories
#
# Public include paths required by the embedded Garlic source tree.
# ==============================================================================
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/common)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/memory)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/hashmap)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/list)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/bitset)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/queue)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/str)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/zip)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/threadpool)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/libs/trie)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/jar)
include_directories(${CMAKE_SOURCE_DIR}/src/garlic/dalvik)

# ==============================================================================
# Garlic Core Source Groups
#
# Collect all C source files from each Garlic component directory into
# individual source groups. These groups are later combined into
# `GARLIC_SOURCES`.
# ==============================================================================
aux_source_directory(src/garlic/common COMMON)
aux_source_directory(src/garlic/libs/list LIST)
aux_source_directory(src/garlic/libs/hashmap HASHMAP)
aux_source_directory(src/garlic/parser/class/ CLASS_PARSER)
aux_source_directory(src/garlic/parser/dex DEX_PARSER)
aux_source_directory(src/garlic/parser/pe PE_PARSER)
aux_source_directory(src/garlic/libs/queue DQUEUE)
aux_source_directory(src/garlic/libs/str STR)
aux_source_directory(src/garlic/libs/memory MEMORY_POOL)
aux_source_directory(src/garlic/libs/zip ZIP)
aux_source_directory(src/garlic/libs/threadpool THREADPOOL)
aux_source_directory(src/garlic/libs/bitset BITSET)
aux_source_directory(src/garlic/decompiler DECOMPILER)
aux_source_directory(src/garlic/decompiler/transformer TRANSFORMER)
aux_source_directory(src/garlic/jar JAR)
aux_source_directory(src/garlic/jvm JVM)
aux_source_directory(src/garlic/apk APK)
aux_source_directory(src/garlic/dalvik DEX)
aux_source_directory(src/garlic/libs/trie TRIE)

# ==============================================================================
# Garlic Core Source Collection
#
# Source Groups:
#   • Core utilities      : COMMON, MEMORY_POOL, LIST, HASHMAP, BITSET,
#                           DQUEUE, STR, TRIE
#   • Decompiler engine   : DECOMPILER, TRANSFORMER
#   • Binary parsers      : CLASS_PARSER, DEX_PARSER, PE_PARSER
#   • Runtime support     : JVM, JAR, APK, DEX, ZIP
#   • Parallel execution  : THREADPOOL
#
# These sources form the embedded Garlic decompiler engine.
# ==============================================================================
set(GARLIC_SOURCES
    ${COMMON}
    ${MEMORY_POOL}
    ${LIST}
    ${HASHMAP}
    ${BITSET}
    ${DQUEUE}
    ${STR}
    ${TRIE}
    ${DECOMPILER}
    ${TRANSFORMER}
    ${CLASS_PARSER}
    ${DEX_PARSER}
    ${PE_PARSER}
    ${JVM}
    ${ZIP}
    ${JAR}
    ${APK}
    ${DEX}
    ${THREADPOOL}
)

# ==============================================================================
# Garlic GUI Source Files
#
# Source files comprising the Garlic GUI application.
#
# This includes:
#   • Qt widgets and dialogs
#   • Project management
#   • Decompiler interface
#   • The C wrapper bridging Qt and the Garlic core
#   • Embedded application resources
# ==============================================================================
set(GUI_SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/FileTreeWidget.cpp
    src/gui/FileTreeWidget.h
    src/gui/CodeEditorWidget.cpp
    src/gui/CodeEditorWidget.h
    src/gui/WelcomeWidget.cpp
    src/gui/WelcomeWidget.h
    src/gui/FindReplaceWidget.cpp
    src/gui/FindReplaceWidget.h
    src/gui/DecompilerInterface.cpp
    src/gui/DecompilerInterface.h
    src/gui/ProjectManager.cpp
    src/gui/ProjectManager.h
    src/gui/DecompilerProgressDialog.cpp
    src/gui/DecompilerProgressDialog.h
    src/gui/ThemeManager.cpp
    src/gui/ThemeManager.h
    src/gui/TitleBarWidget.cpp
    src/gui/TitleBarWidget.h
    src/gui/SplashWidget.cpp
    src/gui/SplashWidget.h
    src/gui/theme.qss
    src/garlic/garlic_wrapper.c
    src/garlic/garlic_wrapper.h
    resources.qrc
)

# ==============================================================================
# Verbose make is opt-in via -DCMAKE_VERBOSE_MAKEFILE=ON on the command line.
# ==============================================================================
set(CMAKE_VERBOSE_MAKEFILE OFF)

if(APPLE)
    configure_file(
        ${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
        ${CMAKE_BINARY_DIR}/Info.plist
        @ONLY
    )
endif()

# ==============================================================================
# Build Target
#
# Build the Garlic GUI executable by combining:
#   • Qt frontend
#   • Embedded Garlic core
#   • C/C++ bridge layer
# ==============================================================================
add_executable(GarlicGUI 
    ${GUI_SOURCES}
    ${GARLIC_SOURCES}
)

# ==============================================================================
# Link Required Libraries
# ==============================================================================
target_link_libraries(GarlicGUI PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

if(WIN32)
    target_link_libraries(GarlicGUI PRIVATE dwmapi)
endif()

# ==============================================================================
# Static Qt6 Plugin Imports
#
# When GARLIC_STATIC_QT is ON, Qt platform plugins must be linked directly
# and registered via Q_IMPORT_PLUGIN in main.cpp. This produces a single
# binary with zero DLL/SO runtime dependencies.
# ==============================================================================
if(GARLIC_STATIC_QT)
    set(QT_STATIC ON)
    add_definitions(-DQT_STATIC)

    if(WIN32)
        target_link_libraries(GarlicGUI PRIVATE
            Qt6::QWindowsIntegrationPlugin
        )
    elseif(APPLE)
        target_link_libraries(GarlicGUI PRIVATE
            Qt6::QCocoaIntegrationPlugin
        )
    else()
        target_link_libraries(GarlicGUI PRIVATE
            Qt6::QXcbIntegrationPlugin
        )
        if(TARGET Qt6::PlatformSupport)
            target_link_libraries(GarlicGUI PRIVATE Qt6::PlatformSupport)
        endif()
        if(TARGET Qt6::GlxSupport)
            target_link_libraries(GarlicGUI PRIVATE Qt6::GlxSupport)
        endif()
    endif()

    message(STATUS "Static Qt6: ON — building standalone binary")
else()
    message(STATUS "Static Qt6: OFF — using shared Qt6 libraries")
endif()

# ==============================================================================
# Platform-Specific Configuration
#
# Configure platform-specific libraries and executable properties for
# Linux, Windows, and macOS.
# ==============================================================================
if (UNIX)
    target_link_libraries(GarlicGUI PRIVATE pthread)
endif ()

if (WIN32)
    target_link_libraries(GarlicGUI PRIVATE ws2_32)
    set_target_properties(GarlicGUI PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

if (APPLE)
    set_target_properties(GarlicGUI PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST "${CMAKE_BINARY_DIR}/Info.plist"
        MACOSX_BUNDLE_GUI_IDENTIFIER "com.agarwalkritik.garlicgui"
        MACOSX_BUNDLE_BUNDLE_NAME "Garlic Decompiler"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
        MACOSX_BUNDLE_ICON_FILE "garlic.icns"
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "-"
    )
    # Bundle the .icns icon into the .app Resources directory
    set(MACOS_ICON "${CMAKE_SOURCE_DIR}/icon/garlic.icns")
    if (EXISTS "${MACOS_ICON}")
        target_sources(GarlicGUI PRIVATE "${MACOS_ICON}")
        set_source_files_properties("${MACOS_ICON}" PROPERTIES
            MACOSX_PACKAGE_LOCATION "Resources")
    endif ()
endif ()

# ==============================================================================
# Compiler Warning Configuration
#
# Configure compiler warnings separately for:
#   • MSVC
#   • GCC / Clang
# ==============================================================================
if(MSVC)
    target_compile_options(GarlicGUI PRIVATE /W3)
else()
    # C++ flags
    target_compile_options(GarlicGUI PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wall>)
    target_compile_options(GarlicGUI PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-Wextra>)
    target_compile_options(GarlicGUI PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>)
    # C flags (without C++ specific options)
    target_compile_options(GarlicGUI PRIVATE $<$<COMPILE_LANGUAGE:C>:-Wall>)
    target_compile_options(GarlicGUI PRIVATE $<$<COMPILE_LANGUAGE:C>:-Wextra>)
endif()

# ==============================================================================
# Post-Build Optimization
#
# Strip debug symbols from Release builds on Unix platforms.
# ==============================================================================
# Strip debug symbols on Linux only.
# Excluded on macOS: stripping before code signing invalidates the signature.
if (UNIX AND NOT APPLE)
    add_custom_command(
        TARGET GarlicGUI POST_BUILD
        COMMAND strip $<TARGET_FILE:GarlicGUI>
    )
endif()

# ==============================================================================
# Upstream Garlic Synchronization
#
# Synchronizes the embedded Garlic source tree by:
#   • Fetching the latest upstream Garlic repository
#   • Updating `shell/`
#   • Updating `src/garlic/`
#   • Preserving Garlic GUI wrapper files
#   • Applying Garlic GUI-specific patches
#
# This target is intended for project maintenance and should be executed
# manually whenever the upstream Garlic repository is updated.
#
# Usage:
#   cmake --build build --target sync_garlic
# ==============================================================================
find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_custom_target(sync_garlic
    COMMAND ${Python3_EXECUTABLE}
            ${CMAKE_SOURCE_DIR}/scripts/sync_garlic.py
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Synchronizing Garlic core from upstream..."
)

# ==============================================================================
# Installation
#
# Install the GarlicGUI executable and, on Windows, deploy the required Qt
# runtime libraries for redistribution.
# ==============================================================================
# On macOS, use BUNDLE DESTINATION to preserve the full .app bundle structure.
# On other platforms, install the binary to bin/.
install(TARGETS GarlicGUI
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

if(WIN32 AND NOT GARLIC_STATIC_QT)
    # Run windeployqt during install step so CPack includes the Qt DLLs
    install(CODE "execute_process(COMMAND windeployqt \"\${CMAKE_INSTALL_PREFIX}/bin/GarlicGUI.exe\")")
endif()

if(APPLE AND NOT GARLIC_STATIC_QT)
    # Run macdeployqt to embed Qt frameworks and platform plugins into the .app.
    find_program(
        MACDEPLOYQT_EXECUTABLE
        NAMES macdeployqt
        HINTS
            "${Qt6_DIR}/../../../bin"
            "$ENV{QTDIR}/bin"
            "/opt/homebrew/bin"
            "/usr/local/bin"
    )
    if (MACDEPLOYQT_EXECUTABLE)
        install(CODE [[
            execute_process(
                COMMAND "${MACDEPLOYQT_EXECUTABLE}"
                        "${CMAKE_INSTALL_PREFIX}/GarlicGUI.app"
                        -always-overwrite
            )
        ]])
    else()
        message(WARNING "macdeployqt not found. Qt frameworks will not be bundled automatically. "
                        "Run macdeployqt manually on the .app after install.")
    endif()
endif()