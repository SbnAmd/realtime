//
// Created by sbn on 5/6/24.
//

#include "tempertature.h"



void get_core_temperatures(float* core_temperatures) {
    FILE *fp;
    char command[] = "sensors";
    char line[MAX_LINE_LENGTH];
    int core_id;

    // Open a pipe to the sensors command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("Error opening pipe");
    }

    // Read each line of output from sensors
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        // Search for lines containing core temperature
        if (strstr(line, "Core") != NULL /*&& strstr(line, "temp") != NULL*/) {
            int core;
            float temperature;
            // Extract core ID and temperature from the line
            if (sscanf(line, "Core %d:%f°C", &core, &temperature) == 2) {
                // Store the temperature in the array
                core_temperatures[core] = temperature;
            }
        }
    }

    // Close the pipe
    pclose(fp);

}