cmake_minimum_required(VERSION 3.24.0)
project(BoardBee LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

# == Files to Compile ==

add_subdirectory(libs)

set(structures "src/structures/board.cc" "libs/time/date_time.cc"
    "src/structures/event.cc" "src/structures/flags.cc"
    "src/structures/item.cc")

set(src "src/main.cc" ${structures})

# == Compilation ==

add_executable(main ${src})

# == Linking ==

target_link_libraries(main PUBLIC ansi json)
target_include_directories(main PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}" "libs")
