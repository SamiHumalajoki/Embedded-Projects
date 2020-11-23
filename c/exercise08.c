#include <stdio.h>
#include <string.h>

void rot13(char *s)
{
    printf("Function rot13 entered\n");
    printf("Input is %s\n", s);
    for (int i = 0; i < strlen(s); i++)
    {
        // Check if the letter is A-M or a-m
        if ((s[i] >= 'A' && s[i] <= 'M') || (s[i] >= 'a' && s[i] <= 'm'))
        {
            s[i] = s[i] + 13;
        }
        // Check if the letter is N-Z or n-z
        else if ( (s[i] >= 'N' && s[i] <= 'Z') || (s[i] >= 'n' && s[i] <= 'z') )
        {
            s[i] = s[i] - 13;
        }
    }
    printf("Converted string is %s\n", s);
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
        // If it is, the following irgument is being passed into the function.        
        for (int i = 1; i < argc; i = i + 2)
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