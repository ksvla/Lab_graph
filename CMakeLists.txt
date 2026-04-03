cmake_minimum_required(VERSION 3.14)
project(GraphLab VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходники
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")

# Основное приложение
add_executable(lab_graph ${SOURCES} ${HEADERS})
target_include_directories(lab_graph PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Тесты (Catch2)
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(catch2)

enable_testing()
add_executable(graph_tests tests/test_main.cpp src/Graph.cpp src/Algorithms.cpp src/Generators.cpp)
target_include_directories(graph_tests PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(graph_tests PRIVATE Catch2::Catch2WithMain)

add_test(NAME GraphTests COMMAND graph_tests)
