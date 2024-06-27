
#include "benchmark.h"



#define FILE_SIZE 40000000  // Size of the file in bytes

void file_io_benchmark() {
    FILE *file;
    char *buffer = (char *)malloc(FILE_SIZE);

    // Write data to file
    file = fopen("testfile.bin", "wb");
    if (file == NULL) {
        printf("Failed to open file for writing.\n");
        return;
    }
    fwrite(buffer, sizeof(char), FILE_SIZE, file);
    fclose(file);

    // Read data from file
    file = fopen("testfile.bin", "rb");
    if (file == NULL) {
        printf("Failed to open file for reading.\n");
        return;
    }
    fread(buffer, sizeof(char), FILE_SIZE, file);
    fclose(file);

    free(buffer);
//    printf("File I/O benchmark completed for file size %d.\n", FILE_SIZE);
}