cmake_minimum_required(VERSION 3.26)
project(realtime C)

set(CMAKE_C_STANDARD 11)

include_directories("benchmarks/inc")

file(GLOB SOURCES "benchmarks/src/*.c" "benchmarks/*.c")

add_executable(realtime
        ${SOURCES}
        benchmarks/src/simd_vec_add.c
        benchmarks/src/simd_mm.c
        benchmarks/src/simd_dot.c
        benchmarks/src/simd_image_conv.c
        benchmarks/perf_monitor.c
        benchmarks/perf_monitor.h

)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -msse4.1 -mavx2")
add_compile_options("-march=native -mavx2")
add_link_options("-pthread")
target_link_libraries(realtime PRIVATE m sensors cjson ssl crypto)