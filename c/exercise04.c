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

uint8_t in_use;

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

    in_use = 0;

    for (uint8_t i = 0; i < 3; i++)
    {
        printf("mode %d \n", i);
        printf("Name: %s \n", modes[i].name);
        printf("Number of iterations: %d \n", modes[i].number_of_iterations);
        printf("Input lines %d\n", modes[i].input_lines);
        printf("Output lines %d\n\n", modes[i].output_lines);
    }
    printf("mode in use: %s \n", modes[in_use].name);

    return 0;
}