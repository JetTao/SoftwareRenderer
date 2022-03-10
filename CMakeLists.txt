cmake_minimum_required(VERSION 3.0.0)
project(SoftwareRenderer VERSION 0.1.0)

include_directories(${PROJECT_SOURCE_DIR}/Parser)

file(GLOB SOURCE_FILES 
    "Core/Include/*.h" /
    "Core/Include/*.hpp"
    "Core/Source/*.cpp" / 
    "Parser/*.h"
    "Shaders/*.h")

add_executable(SoftwareRenderer main.cpp ${SOURCE_FILES})

set_target_properties(SoftwareRenderer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})

set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
