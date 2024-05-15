//
// Created by sbn on 5/6/24.
//

#include "tempertature.h"

extern int kill_flag;

void get_core_temperatures(float* core_temperatures) {
    FILE *fp;
    char command[] = "sudo rdmsr -p 0 -u -0 -f22:16 -a 0x19c";
    char line[MAX_LINE_LENGTH];
    long n;
    int core = 0;

    // Open a pipe to the sensors command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
    }


    // Read and print each line from the command output
    while (fgets(line, sizeof(line), fp) != NULL) {

        n = strtol(line, (char**)NULL, 10);
        core_temperatures[core] = 100 - n;
        core += 1;
    }


    // Close the pipe
    pclose(fp);


}