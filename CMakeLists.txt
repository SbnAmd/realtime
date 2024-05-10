cmake_minimum_required(VERSION 3.26)
project(realtime C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(realtime
        scheduler.c
        scheduler.h
        tasks/tasks.h
        tasks/tasks.c
        tasks/bitarray.c
        tasks/bitcnt_1.c
        tasks/bitcnt_2.c
        tasks/bitcnt_3.c
        tasks/bitcnt_4.c
        tasks/bitfiles.c
        tasks/bitstrng.c
        tasks/bstr_i.c
        tasks/cubic.c
        tasks/isqrt.c
        tasks/rad2deg.c
        server/server.h
        server/server.c
        performance_counter/performance_counter.h
        performance_counter/performance_counter.c
        power/power.h
        power/power.c
        temperature/tempertature.h
        temperature/tempertature.c
        realtime.h
        serdes/serdes.c
        serdes/serdes.h
        shared_memory/shared_memory.h
        shared_memory/shared_memory.c
        utils/utils.h
        utils/utils.c
)
add_link_options("-pthread")
target_link_libraries(realtime PRIVATE m sensors cjson)