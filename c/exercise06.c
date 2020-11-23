#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main()
{
    char s_one[100] = "C programming is fun.";
    char s_two[100] = "IOT is the future.";
    char s_three[100] = "Our teachers are the best.";
    char s_four[100] = "I will get 5 from this course.";

    char s_all[4][100];
    strcpy(s_all[0], s_one);
    strcpy(s_all[1], s_two);
    strcpy(s_all[2], s_three);
    strcpy(s_all[3], s_four);

    printf("Task 3:\n");
    printf("%s\n\n", strcat(strcat(s_one, s_two), s_three));
    printf("Task 4:\n");
    printf("%d \n\n", strlen(s_three) + strlen(s_four));
    
    printf("Task 5:\n");
    for (uint8_t i = 0; i < 4; i++)
    {
        if ((s_all[i][0] == 'r') || (s_all[i][1] == 'r') || (s_all[i][2] == 'r'))
        {
            printf("%s\n", s_all[i]);
        }
    }

    return 0;
}