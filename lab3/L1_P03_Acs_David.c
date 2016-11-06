#include <stdio.h>
#include <stdlib.h>

int inversare_biti(unsigned int numar,unsigned int poz_start,unsigned int nr_biti)
{
    //create mask
    unsigned int mask = 1;
    
    for(int i = 0; i< nr_biti - 1; i++) 
    {
        mask = mask<<1;
        mask = mask | 1;
    }
    printf("%d \n", mask);
    mask = mask<<poz_start;
    printf("mask is: %x \n", mask);
    
    //xor with the current number 
    //return result
    return numar^mask;
}

int main()
{
    unsigned int number = 0;
    unsigned int poz_start = 0;
    unsigned int nr_bites = 0;
    printf("the number: ");
    scanf("%d", &number);
    printf("the position: ");
    scanf("%d", &poz_start);
    printf("the number of bites to be inversed: ");
    scanf("%d", &nr_bites);
    printf("result: %d", inversare_biti(number, poz_start, nr_bites));
    
    return 0;
}