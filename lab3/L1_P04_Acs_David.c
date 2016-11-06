#include <stdio.h>
#include <stdlib.h>

char dictionary[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

char* reverse_string(char* string)
{
    char* reversed = (char*)malloc(sizeof(char)*1024);
    int length = strlen(string);
    for(int i = length - 1; i>=0; i--)
    {
        reversed[length - 1 - i] = string[i];
    }
    reversed[length] = '\0';
    return reversed;
}

void itob(int numar, char *sir, int base)
{
    char* new_number = (char*) malloc(sizeof(char)*1024);
    if(base<2 || base>16)
    {
        printf("base too small or too large (1<b<17)");
        exit(1);
    }
    int i = 0;
    while(numar>0)
    {
        new_number[i++] = dictionary[numar % base];
        numar /= base;
    }
    new_number[i] = '\0';
    
    strcpy(sir, reverse_string(new_number));
    
}

int main()
{
    char *sir = (char*)malloc(sizeof(char)*1024);
    int number = 0;
    int base = 0;
    printf("the number to be converted: ");
    scanf("%d", &number);
    printf("the base to be converted into: ");
    scanf("%d", &base);
    itob(number, sir, base);
    printf("%s", sir);
}