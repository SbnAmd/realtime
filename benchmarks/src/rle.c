
#include "benchmark.h"

#define INPUT_SIZE 100000

void run_length_encoding(const char *input, char *output) {
    int count = 1;
    char current = input[0];
    int j = 0;
    for (int i = 1; i < strlen(input); i++) {
        if (input[i] == current) {
            count++;
        } else {
            j += sprintf(&output[j], "%c%d", current, count);
            current = input[i];
            count = 1;
        }
    }
    sprintf(&output[j], "%c%d", current, count);
}

void rle() {
    char *input = (char *)malloc(INPUT_SIZE + 1);
    char *output = (char *)malloc(2 * INPUT_SIZE);

    // Fill the input with a pattern for testing
    for (int i = 0; i < INPUT_SIZE; i++) {
        input[i] = 'A' + (i % 26);
    }
    input[INPUT_SIZE] = '\0';

    // Perform run-length encoding
    run_length_encoding(input, output);

    free(input);
    free(output);
//    printf("Run-length encoding completed for input size %d.\n", INPUT_SIZE);

}