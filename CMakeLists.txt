cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(step-writer LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

include(FetchContent)

# Fetch and include FTXUI
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# Fetch and include GoogleTest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Define the main executable
add_executable(step-writer
    src/main.cpp
    src/Editor.cpp
    src/Document.cpp
    src/Row.cpp
    src/TextFileViewer.cpp
    src/DataFile.cpp
    src/DirectoryFile.cpp
)

# Add Tree-sitter C++ grammar
add_library(ts-cpp STATIC
    third-party/tree-sitter-cpp/src/parser.c
    third-party/tree-sitter-cpp/src/scanner.c
)

# Include directories for Tree-sitter C++ grammar
target_include_directories(ts-cpp PUBLIC
    third-party/tree-sitter-cpp/src
)

# Add Tree-sitter library
add_library(ts-lib STATIC
    third-party/tree-sitter/lib/src/lib.c
)

# Include directories for Tree-sitter
target_include_directories(ts-lib PUBLIC
    third-party/tree-sitter/lib/include
    third-party/tree-sitter/lib/src
)

target_include_directories(step-writer PRIVATE include)

# Link libraries to the main executable
target_link_libraries(step-writer
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE ts-lib
    PRIVATE ts-cpp
)

# Enable testing
enable_testing()

# Define the test executable
add_executable(all-test
    tests/RowTest.cpp 
    tests/DocumentTest.cpp
    src/Row.cpp
    src/Document.cpp
)

target_include_directories(all-test PRIVATE include)
# Link libraries to the test executable
target_link_libraries(all-test PRIVATE GTest::gtest_main
                               PRIVATE ftxui::component
                               PRIVATE ts-lib
                               PRIVATE ts-cpp)

# Include GoogleTest CMake functions
include(GoogleTest)
gtest_discover_tests(all-test)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()
