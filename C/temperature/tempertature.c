//
// Created by sbn on 5/6/24.
//

#include "tempertature.h"

#define MAX_FILE_SIZE 128
extern int kill_flag;

void get_core_temperatures(float* core_temperatures) {
    FILE *fp;
    char buffer[MAX_FILE_SIZE];
    char command[] = "sudo rdmsr -p 0 -u -0 -f22:16 -a 0x19c";
//    char line[MAX_LINE_LENGTH];
    long n;
    int core = 0;
    char *line = buffer;
#ifdef DEBUG
    struct timespec lend, lstart;
    long elapsed_ns;
#endif

    // Open a pipe to the sensors command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
    }
    clock_gettime(CLOCK_MONOTONIC, &lstart);
    size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, fp);
    char *end = buffer + bytesRead;

    // Read and print each line from the command output
    while (line < end) {

        char *next_line = strchr(line, '\n');
        if (next_line) {
            *next_line = '\0'; // Replace newline with null terminator
        }

        int n = strtol(line, NULL, 10);
        core_temperatures[core] = 100 - n;
        core += 1;

        if (!next_line) break;
        line = next_line + 1;
    }

#ifdef DEBUG
    clock_gettime(CLOCK_MONOTONIC, &lend);
    elapsed_ns = (lend.tv_sec-lstart.tv_sec) * 1000000000 + (lend.tv_nsec-lstart.tv_nsec);
    printf("temp took %f \n", elapsed_ns / 1000000.0);
#endif
    // Close the pipe
    pclose(fp);


}