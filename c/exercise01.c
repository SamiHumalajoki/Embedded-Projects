# include <stdio.h>

int main() 
{
    for (float f = 3; f < 7; f++) 
    {
        float result = (f + 9) / f;
        printf("(%.2f + 9) / %.2f = %.2f\n", f, f, result);
    }
    return 0;
}