#include <stdio.h>
#include <string.h>

void rot13(char *s)
{
    printf("Function rot13 entered\n");
    printf("Input line %s\n", s);
}

void freq(char *s)
{
    printf("Function freq entered\n");
    printf("Input is %s\n", s);
}

int main( int argc, char **args )
{
    printf("\n\nProgram strating\n");
    if (argc%2 == 0)
    {
        printf("Invalid number of arguments\n");
    }
    else
    {
        // Every other command line argument is a possible command (R or F).
        // If it is, the following argument is being passed into the function.
        for (int i = 1; i < argc; i += 2)
        {
            if (strcmp (args[i], "R") == 0)
            {
                rot13(args[i+1]);
            }
            if (strcmp (args[i], "F") == 0)
            {
                freq(args[i+1]);
            }
        }
    }
}