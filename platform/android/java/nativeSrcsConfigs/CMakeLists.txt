# Non functional cmake build file used to provide Android Studio editor support to the project.
cmake_minimum_required(VERSION 3.6)
project(kosmic)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(KOSMIC_ROOT_DIR ../../../..)
set(ANDROID_ROOT_DIR "${KOSMIC_ROOT_DIR}/platform/android" CACHE STRING "")
set(OPENXR_INCLUDE_DIR "${KOSMIC_ROOT_DIR}/thirdparty/openxr/include" CACHE STRING "")

# Get sources
file(GLOB_RECURSE SOURCES ${KOSMIC_ROOT_DIR}/*.c**)
file(GLOB_RECURSE HEADERS ${KOSMIC_ROOT_DIR}/*.h**)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME}
        SYSTEM PUBLIC
        ${KOSMIC_ROOT_DIR}
        ${ANDROID_ROOT_DIR}
        ${OPENXR_INCLUDE_DIR})

add_definitions(-DUNIX_ENABLED -DVULKAN_ENABLED -DANDROID_ENABLED -DGLES3_ENABLED -DTOOLS_ENABLED -DDEBUG_ENABLED)
