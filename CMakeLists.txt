cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(step-writer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
# For Windows: Prevent overriding the parent project's compiler/linker settings
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
target_include_directories(step-writer PRIVATE include)
target_link_libraries(step-writer PRIVATE ftxui::screen
                                   PRIVATE ftxui::dom
                                   PRIVATE ftxui::component)

# Enable testing
enable_testing()

# Define the test executable
add_executable(all-test tests/RowTest.cpp 
                        tests/DocumentTest.cpp
                        src/Row.cpp
                        src/Document.cpp)
target_include_directories(all-test PRIVATE include)
target_link_libraries(all-test PRIVATE GTest::gtest_main)

# Include GoogleTest CMake functions
include(GoogleTest)
gtest_discover_tests(all-test)