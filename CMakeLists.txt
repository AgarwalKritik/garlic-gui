cmake_minimum_required(VERSION 3.16)
project(GarlicGUI VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Garlic C compiler flags (adapted from original Garlic CMakeLists.txt)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden -Wall")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter -Wno-unused-but-set-variable")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration -Wno-incompatible-pointer-types")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-misleading-indentation -Wno-format")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-sign-compare -Wno-type-limits -Wno-old-style-declaration")

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ffunction-sections -fdata-sections")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--gc-sections")
endif ()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

if(NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-ignored-optimization-argument")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto")
endif ()

if (UNIX)
    SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -pthread")
endif ()

# Include directories (matching Garlic's structure)
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

# Garlic source files (matching original structure)
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

# Combine all Garlic sources
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

# GUI sources
set(GUI_SOURCES
    src/main.cpp
    src/gui/MainWindow.cpp
    src/gui/MainWindow.h
    src/gui/FileTreeWidget.cpp
    src/gui/FileTreeWidget.h
    src/gui/CodeEditorWidget.cpp
    src/gui/CodeEditorWidget.h
    src/gui/DecompilerInterface.cpp
    src/gui/DecompilerInterface.h
    src/gui/ProjectManager.cpp
    src/gui/ProjectManager.h
    src/gui/DecompilerProgressDialog.cpp
    src/gui/DecompilerProgressDialog.h
    src/garlic/garlic_wrapper.c
    src/garlic/garlic_wrapper.h
)

# Create executable
add_executable(GarlicGUI 
    ${GUI_SOURCES}
    ${GARLIC_SOURCES}
)

# Link Qt6
target_link_libraries(GarlicGUI Qt6::Core Qt6::Widgets)

# Platform specific libraries (matching Garlic's requirements)
if (UNIX)
    target_link_libraries(GarlicGUI pthread)
endif ()

if (WIN32)
    target_link_libraries(GarlicGUI ws2_32)
    set_target_properties(GarlicGUI PROPERTIES WIN32_EXECUTABLE TRUE)
endif ()

# Compiler specific settings
if(MSVC)
    target_compile_options(GarlicGUI PRIVATE /W3)
else()
    target_compile_options(GarlicGUI PRIVATE -Wall -Wextra)
endif()

# Strip binary on Unix (like original Garlic)
if (UNIX)
    add_custom_command(
        TARGET GarlicGUI POST_BUILD
        COMMAND strip $<TARGET_FILE:GarlicGUI>
    )
endif()

# Install
install(TARGETS GarlicGUI DESTINATION bin)