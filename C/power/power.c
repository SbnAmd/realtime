//
// Created by sbn on 5/6/24.
//

#include "power.h"


unsigned long get_cores_power(){

    FILE *fp;

    char buffer[MAX_BUF_SIZE];

    // Open the file for reading
    fp = fopen(RAPL_PATH, "r");
    if (fp == NULL) {
        perror("Error opening file");
    }

    // Read the contents of the file
    if (fgets(buffer, MAX_BUF_SIZE, fp) == NULL) {
        perror("Error reading file");
        fclose(fp);
    }

    // Close the file
    fclose(fp);

    // Convert the string to an integer (energy in microjoules)
    return strtoul(buffer, NULL, 10);
}


void get_energy_consumption(unsigned long* energy_uj){

    *energy_uj = get_cores_power();
}

void get_power_and_energy(double* power,unsigned long* energy_uj){

    static unsigned long e1 =0 ;
    static unsigned long e2 =0 ;

    get_energy_consumption(&e2);
    if(e1 == 0){
        *energy_uj = e2;
        *power = 1;
    } else{
        *power = (double )(e2-e1)/20000;
        *energy_uj = e2;
    }

}

