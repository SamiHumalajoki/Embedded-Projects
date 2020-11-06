#include <stdio.h>
#include <stdint.h>

int main()
{
    for (uint8_t n = 5; n < 16; n++)
    {
        uint8_t m = n;
        for (uint8_t i = 0; i < 8; i++)
        {
            printf("%d", m/128);
            m = m << 1;
        }
        printf("\n");
    }
    return 0;
}