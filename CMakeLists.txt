cmake_minimum_required(VERSION 3.26)
project(realtime C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(realtime
        C/scheduler.c
        C/scheduler.h
        C/tasks/tasks.h
        C/tasks/tasks.c
        C/tasks/bitarray.c
        C/tasks/bitcnt_1.c
        C/tasks/bitcnt_2.c
        C/tasks/bitcnt_3.c
        C/tasks/bitcnt_4.c
        C/tasks/bitfiles.c
        C/tasks/bitstrng.c
        C/tasks/bstr_i.c
        C/tasks/cubic.c
        C/tasks/isqrt.c
        C/tasks/rad2deg.c
#        server/server.h
#        server/server.c
        C/performance_counter/performance_counter.h
        C/performance_counter/performance_counter.c
        C/power/power.h
        C/power/power.c
        C/temperature/tempertature.h
        C/temperature/tempertature.c
        C/realtime.h
        C/serdes/serdes.c
        C/serdes/serdes.h
        C/shared_memory/shared_memory.h
        C/shared_memory/shared_memory.c
        C/utils/utils.h
        C/utils/utils.c
        C/threads/tick.h
        C/threads/tick.c
        C/threads/realtime_t.h
        C/threads/realtime_t.c
        C/threads/manager.h
        C/threads/manager.c
        main.c
)
add_link_options("-pthread")
target_link_libraries(realtime PRIVATE m sensors cjson)