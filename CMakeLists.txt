cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(step-writer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

add_executable(step-writer src/main.cpp src/editor.cpp src/editor.cpp src/document.cpp src/TextFileViewer.cpp)
target_link_libraries(step-writer PRIVATE ftxui::screen
                                  PRIVATE ftxui::dom
                                  PRIVATE ftxui::component)