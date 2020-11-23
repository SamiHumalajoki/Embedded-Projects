#include <stdio.h>
#include <string.h>

void rot13(char *s)
{
    printf("Function rot13 entered\n");
    printf("Input is %s\n", s);
    for (int i = 0; i < strlen(s); i++)
    {
        // Check if the letter is A-M or a-m. If it is, the ciphered letter
        // is just original + 13;
        if ((s[i] >= 'A' && s[i] <= 'M') || (s[i] >= 'a' && s[i] <= 'm'))
        {
            s[i] = s[i] + 13;
        }
        // Check if the letter is N-Z or n-z.If it is, the ciphered letter
        // is just original - 13 (because of rolling over);
        else if ((s[i] >= 'N' && s[i] <= 'Z') || (s[i] >= 'n' && s[i] <= 'z'))
        {
            s[i] = s[i] - 13;
        }
    }
    printf("Converted stirng is %s\n", s);
}

void freq(char *s)
{
    printf("Function freq entered\n");
    printf("Input is %s\n", s);
    // Initialize the histogram array
    // Each location in the array corresponds a letter from a to z
    int histogram[26] = {0};
    for (int i = 0; i < strlen(s); i++)
    {
        // if a letter from a to z is found, 
        // the corresponding integer is incremented
        if (s[i] >= 'a' || s[i] <= 'z')
        {
            histogram[s[i] - 'a']++;
        }
    }
    // Find the maximum occurance of one letter
    int max = 0;
    for (int i = 0; i < 26; i++)
    {
        if ( histogram[i] > max )
        {
            max = histogram[i];
        }
    }
    printf("Histogram\n\n");
    // Print the histogram. 
    for (int i = max; i > 0; i--)
    {
        for (int j = 0; j < 26; j++)
        {
            if (histogram[j] >= i)
            {
                printf("*");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
    // Print the alphabets from a to z
    for (char c = 'a'; c <= 'z'; c++)
    {
        printf("%c", c);    
    }
    printf("\n");
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