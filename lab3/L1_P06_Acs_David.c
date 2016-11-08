#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("enter n=");
    int n = 0;
    scanf("%d", &n);
    long long next_value = n;
    while(next_value > 1)
    {
        printf("%ld, ", next_value);
        if(next_value & 1)
        {
            next_value = (next_value << 1) + next_value + 1;
        }
        else{
            next_value = next_value >> 1;
        }
    }
    if(next_value != 1)
    {
        printf("overflow!\n");
    }
    printf("%ld", next_value);
    return 0;
}