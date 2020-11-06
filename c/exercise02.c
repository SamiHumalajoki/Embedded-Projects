# include <stdio.h>

int main() 
{
    for (int i = 1; i < 89; i += 3)
    {
        if (i % 8 == 0)
        {
            printf("%i ", i);
        }
    }
    printf("\n");
    return 0;
}