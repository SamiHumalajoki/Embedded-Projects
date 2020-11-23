#include <stdio.h>
#include <string.h>
#include <stdint.h>

struct Mode
{
    char name[10];
    uint8_t number_of_iterations;
    uint8_t input_lines;
    uint8_t output_lines;
};

struct Mode *in_use;

int main()
{
    struct Mode mode1;
    struct Mode mode2;
    struct Mode mode3;
    
    strcpy (mode1.name, "Lazy mode");
    mode1.number_of_iterations = 3;
    mode1.input_lines = 1;
    mode1.output_lines = 2;

    strcpy (mode2.name, "Hazy mode");
    mode2.number_of_iterations = 13;
    mode2.input_lines = 12;
    mode2.output_lines = 24;

    strcpy (mode3.name, "Racy mode");
    mode3.number_of_iterations = 42;
    mode3.input_lines = 64;
    mode3.output_lines = 128;

    struct Mode modes[3] = {mode1, mode2, mode3};

    struct Mode *in_use = modes + 1; // point to mode2

    
    char (*namePtr)[10] = &(in_use->name);
    uint8_t *number_of_iterationsPtr = &(in_use->number_of_iterations);
    uint8_t *input_linesPtr = &(in_use->input_lines);
    uint8_t *output_linesPtr = &(in_use->output_lines);

    printf("Name: %s \n", *namePtr);
    printf("Number of iterations: %d \n", *number_of_iterationsPtr);
    printf("Input lines %d\n", *input_linesPtr);
    printf("Output lines %d\n\n", *output_linesPtr);
    
    return 0;
}