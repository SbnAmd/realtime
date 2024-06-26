
#include "perf_monitor.h"

long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                     int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu,
                   group_fd, flags);
}

void get_perf_event(int core, long wait){

    int cpu_cycles_fd, cpu_instructions_fd, cpu_cache_misses_fd;
    int cpu_cache_references_fd, cpu_branch_misses_fd, cpu_branch_instructions_fd;
    int cpu_page_faults_fd, cpu_context_switches_fd, cpu_migrations_fd;

    struct perf_event_attr cpu_cycles_pe, cpu_instructions_pe, cpu_cache_misses_pe;
    struct perf_event_attr cpu_cache_references_pe, cpu_branch_misses_pe, cpu_branch_instructions_pe;
    struct perf_event_attr cpu_page_faults_pe, cpu_context_switches_pe, cpu_migrations_pe;


    long cpu_cycles;
    long cpu_instructions;
    long cpu_cache_misses;
    long cpu_cache_references;
    long cpu_branch_misses;
    long cpu_branch_instructions;
    long cpu_page_faults;
    long cpu_context_switches;
    long cpu_migrations;
    long duration;

    struct timespec start, end;
    struct timespec task_start, task_end;
    long task_duration_ns, total_ns, elapsed_ns;

    clock_gettime(CLOCK_MONOTONIC, &start);

    // CPU CYCLES
    memset(&cpu_cycles_pe, 0, sizeof(struct perf_event_attr));
    cpu_cycles_pe.type = PERF_TYPE_HARDWARE;
    cpu_cycles_pe.size = sizeof(struct perf_event_attr);
    cpu_cycles_pe.config = PERF_COUNT_HW_CPU_CYCLES;
    cpu_cycles_pe.disabled = 1;
    cpu_cycles_pe.exclude_kernel = 1;
    cpu_cycles_pe.exclude_hv = 1;

    // CPU INSTRUCTIONS
    memset(&cpu_instructions_pe, 0, sizeof(struct perf_event_attr));
    cpu_instructions_pe.type = PERF_TYPE_HARDWARE;
    cpu_instructions_pe.size = sizeof(struct perf_event_attr);
    cpu_instructions_pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    cpu_instructions_pe.disabled = 1;
    cpu_instructions_pe.exclude_kernel = 1;
    cpu_instructions_pe.exclude_hv = 1;

    // CPU CACHE MISSES
    memset(&cpu_cache_misses_pe, 0, sizeof(struct perf_event_attr));
    cpu_cache_misses_pe.type = PERF_TYPE_HARDWARE;
    cpu_cache_misses_pe.size = sizeof(struct perf_event_attr);
    cpu_cache_misses_pe.config = PERF_COUNT_HW_CACHE_MISSES;
    cpu_cache_misses_pe.disabled = 1;
    cpu_cache_misses_pe.exclude_kernel = 1;
    cpu_cache_misses_pe.exclude_hv = 1;

    // CPU CACHE REFERENCES
    memset(&cpu_cache_references_pe, 0, sizeof(struct perf_event_attr));
    cpu_cache_references_pe.type = PERF_TYPE_HARDWARE;
    cpu_cache_references_pe.size = sizeof(struct perf_event_attr);
    cpu_cache_references_pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
    cpu_cache_references_pe.disabled = 1;
    cpu_cache_references_pe.exclude_kernel = 1;
    cpu_cache_references_pe.exclude_hv = 1;

    // CPU BRANCH MISSES
    memset(&cpu_branch_misses_pe, 0, sizeof(struct perf_event_attr));
    cpu_branch_misses_pe.type = PERF_TYPE_HARDWARE;
    cpu_branch_misses_pe.size = sizeof(struct perf_event_attr);
    cpu_branch_misses_pe.config = PERF_COUNT_HW_BRANCH_MISSES;
    cpu_branch_misses_pe.disabled = 1;
    cpu_branch_misses_pe.exclude_kernel = 1;
    cpu_branch_misses_pe.exclude_hv = 1;

    // CPU BRANCH INSTRUCTIONS
    memset(&cpu_branch_instructions_pe, 0, sizeof(struct perf_event_attr));
    cpu_branch_instructions_pe.type = PERF_TYPE_HARDWARE;
    cpu_branch_instructions_pe.size = sizeof(struct perf_event_attr);
    cpu_branch_instructions_pe.config = PERF_COUNT_HW_BRANCH_INSTRUCTIONS;
    cpu_branch_instructions_pe.disabled = 1;
    cpu_branch_instructions_pe.exclude_kernel = 1;
    cpu_branch_instructions_pe.exclude_hv = 1;

    // CPU PAGE FAULTS
    memset(&cpu_page_faults_pe, 0, sizeof(struct perf_event_attr));
    cpu_page_faults_pe.type = PERF_TYPE_SOFTWARE;
    cpu_page_faults_pe.size = sizeof(struct perf_event_attr);
    cpu_page_faults_pe.config = PERF_COUNT_SW_PAGE_FAULTS;
    cpu_page_faults_pe.disabled = 1;
    cpu_page_faults_pe.exclude_kernel = 1;
    cpu_page_faults_pe.exclude_hv = 1;

    // CPU CONTEXT SWITCHES
    memset(&cpu_context_switches_pe, 0, sizeof(struct perf_event_attr));
    cpu_context_switches_pe.type = PERF_TYPE_SOFTWARE;
    cpu_context_switches_pe.size = sizeof(struct perf_event_attr);
    cpu_context_switches_pe.config = PERF_COUNT_SW_CONTEXT_SWITCHES;
    cpu_context_switches_pe.disabled = 1;
    cpu_context_switches_pe.exclude_kernel = 1;
    cpu_context_switches_pe.exclude_hv = 1;

    // CPU MIGRATIONS
    memset(&cpu_migrations_pe, 0, sizeof(struct perf_event_attr));
    cpu_migrations_pe.type = PERF_TYPE_SOFTWARE;
    cpu_migrations_pe.size = sizeof(struct perf_event_attr);
    cpu_migrations_pe.config = PERF_COUNT_SW_CPU_MIGRATIONS;
    cpu_migrations_pe.disabled = 1;
    cpu_migrations_pe.exclude_kernel = 1;
    cpu_migrations_pe.exclude_hv = 1;

    // CPU CYCLES
    cpu_cycles_fd = perf_event_open(&cpu_cycles_pe, 0, core, -1, 0);
    if (cpu_cycles_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU INSTRUCTIONS
    cpu_instructions_fd = perf_event_open(&cpu_instructions_pe, 0, core, -1, 0);
    if (cpu_instructions_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU CACHE MISSES
    cpu_cache_misses_fd = perf_event_open(&cpu_cache_misses_pe, 0, core, -1, 0);
    if (cpu_cache_misses_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU CACHE REFERENCES
    cpu_cache_references_fd = perf_event_open(&cpu_cache_references_pe, 0, core, -1, 0);
    if (cpu_cache_references_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU BRANCH MISSES
    cpu_branch_misses_fd = perf_event_open(&cpu_branch_misses_pe, 0, core, -1, 0);
    if (cpu_branch_misses_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU BRANCH INSTRUCTION
    cpu_branch_instructions_fd = perf_event_open(&cpu_branch_instructions_pe, 0, core, -1, 0);
    if (cpu_branch_instructions_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU PAGE FAULTS
    cpu_page_faults_fd = perf_event_open(&cpu_page_faults_pe, 0, core, -1, 0);
    if (cpu_page_faults_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU CONTEXT SWITCHES
    cpu_context_switches_fd = perf_event_open(&cpu_context_switches_pe, 0, core, -1, 0);
    if (cpu_context_switches_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }

    // CPU MIGRATIONS
    cpu_migrations_fd = perf_event_open(&cpu_migrations_pe, 0, core, -1, 0);
    if (cpu_migrations_fd == -1) {
        fprintf(stderr, "Error opening cpu_cycles_pe counter\n");
        exit(EXIT_FAILURE);
    }


    // Reset events
    ioctl(cpu_cycles_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_instructions_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_cache_misses_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_cache_references_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_branch_misses_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_branch_instructions_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_page_faults_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_context_switches_fd, PERF_EVENT_IOC_RESET, 0);
    ioctl(cpu_migrations_fd, PERF_EVENT_IOC_RESET, 0);

    // Enabling events
    ioctl(cpu_cycles_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_instructions_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_cache_misses_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_cache_references_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_branch_misses_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_branch_instructions_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_page_faults_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_context_switches_fd, PERF_EVENT_IOC_ENABLE, 0);
    ioctl(cpu_migrations_fd, PERF_EVENT_IOC_ENABLE, 0);


    usleep(wait);


    // Disable events
    ioctl(cpu_cycles_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_instructions_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_cache_misses_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_cache_references_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_branch_misses_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_branch_instructions_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_page_faults_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_context_switches_fd, PERF_EVENT_IOC_DISABLE, 0);
    ioctl(cpu_migrations_fd, PERF_EVENT_IOC_DISABLE, 0);


    read(cpu_cycles_fd, &cpu_cycles, sizeof(long long));
    read(cpu_instructions_fd, &cpu_instructions, sizeof(long long));
    read(cpu_cache_misses_fd, &cpu_cache_misses, sizeof(long long));
    read(cpu_cache_references_fd, &cpu_cache_references, sizeof(long long));
    read(cpu_branch_misses_fd, &cpu_branch_misses, sizeof(long long));
    read(cpu_branch_instructions_fd, &cpu_branch_instructions, sizeof(long long));
    read(cpu_page_faults_fd, &cpu_page_faults, sizeof(long long));
    read(cpu_context_switches_fd, &cpu_context_switches, sizeof(long long));
    read(cpu_migrations_fd, &cpu_migrations, sizeof(long long));
    clock_gettime(CLOCK_MONOTONIC, &end);
    // Time calc
    elapsed_ns = (end.tv_sec - start.tv_sec) * 1000000000 + (end.tv_nsec - start.tv_nsec);
    duration = elapsed_ns / 1000000; // Elapsed time in milliseconds


    close(cpu_cycles_fd);
    close(cpu_instructions_fd);
    close(cpu_cache_misses_fd);
    close(cpu_cache_references_fd);
    close(cpu_branch_misses_fd);
    close(cpu_branch_instructions_fd);
    close(cpu_page_faults_fd);
    close(cpu_context_switches_fd);
    close(cpu_migrations_fd);

    printf("cpu_cycles : %ld\n", cpu_cycles);
    printf("cpu_instructions : %ld\n", cpu_instructions);
    printf("cpu_cache_misses : %ld\n", cpu_cache_misses);
    printf("cpu_cache_references : %ld\n", cpu_cache_references);
    printf("cpu_branch_misses : %ld\n", cpu_branch_misses);
    printf("cpu_branch_instructions : %ld\n", cpu_branch_instructions);
    printf("cpu_page_faults : %ld\n", cpu_page_faults);
    printf("cpu_context_switches : %ld\n", cpu_context_switches);
    printf("cpu_migrations : %ld\n", cpu_migrations);
    printf("duration : %ld\n", duration);

}