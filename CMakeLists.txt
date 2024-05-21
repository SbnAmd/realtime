cmake_minimum_required(VERSION 3.26)
project(realtime C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(realtime
        C/threads/init.c
        C/threads/init.h
        C/tasks/benchmark_tasks.h
        C/tasks/bit_counts/bitarray.c
        C/tasks/bit_counts/bitcnt_1.c
        C/tasks/bit_counts/bitcnt_2.c
        C/tasks/bit_counts/bitcnt_3.c
        C/tasks/bit_counts/bitcnt_4.c
        C/tasks/bit_counts/bitfiles.c
        C/tasks/bit_counts/bitstrng.c
        C/tasks/bit_counts/bstr_i.c
        C/tasks/basic_math/cubic.c
        C/tasks/basic_math/isqrt.c
        C/tasks/basic_math/rad2deg.c
#        server/server.h
#        server/server.c
        C/performance_counter/performance_counter.h
        C/performance_counter/performance_counter.c
        C/power/power.h
        C/power/power.c
#        C/temperature/tempertature.h
#        C/temperature/tempertature.c
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
        C/function_types/function_types.h
        Test/perf_count.h
        Test/perf_count.c
        Test/task_times.h
        Test/task_times.c
        Test/kernel_power/kernel_power_measurement.h
        Test/kernel_power/kernel_power_measurement.c
        C/tasks/string_search/bmhasrch.c
        C/tasks/string_search/bmhisrch.c
        C/tasks/string_search/bmhsrch.c
        C/tasks/string_search/search.h
        C/tasks/qsort/qsort.c
        C/tasks/qsort/qsort.h
        C/tasks/bit_counts/bitcount.c
        C/tasks/bit_counts/bitcount.h
        C/tasks/basic_math/basicmath.c
        C/tasks/basic_math/basicmath.h
        C/tasks/string_search/stringsearch.c
        C/tasks/string_search/stringsearch.h
        C/tasks/fft/ddc.h
        C/tasks/fft/ddcmath.h
        C/tasks/fft/fftmisc.c
        C/tasks/fft/fourier.h
        C/tasks/fft/fourier.c
        C/tasks/fft/fft.c
        C/tasks/fft/fft.h
        C/tasks/crc32/crc.h
        C/tasks/crc32/crc.c
        Test/task_duration/task_duration.c
        Test/task_duration/task_duration.h
        C/tasks/sniptype.h
        C/threads/temperature.c
        C/threads/temperature.h
)
add_link_options("-pthread")
target_link_libraries(realtime PRIVATE m sensors cjson)