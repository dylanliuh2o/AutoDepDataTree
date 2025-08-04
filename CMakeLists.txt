cmake_minimum_required(VERSION 3.10)
project(AutoDepDataTree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/src)

# add subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/tabulate)

file(GLOB SOURCE_FILES src/*.cpp)

# add executable and link libraries
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE tabulate)
