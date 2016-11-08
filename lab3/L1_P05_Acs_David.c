#include <stdio.h>
#include <stdlib.h>

const int INT_SIZE = 32;
const int ARRAY_SIZE = 32;


// sets defined, o
int *a = NULL;
int *b = NULL;
int *c = NULL;

void print_menu()
{
    printf("1. insert a new set\n");
    printf("2. print elements of a set\n");
    printf("3. print cardinality of a set\n");
    printf("4. reunion of two set\n");
    printf("5. intersection of two sets\n");
    printf("6. difference of two sets\n");
    printf("7. check if number is part of a set\n");
    printf("8. show menu\n");
    printf("0. exit\n");
}

int* get_set()
{
    printf("choose a set from a, b or c: ");
    char aux;
    scanf("%*c%c", &aux);
    printf("you chose %c\n", aux);
    switch(aux){
        case 'a':
            return a;
        case 'b':
            return b;
        case 'c':
            return c;
        default:
            printf("set not existing, using default set a");
    }
    return a;
}

void print_elements(int *bit_set)
{
    printf("printing elements of a set\n");
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        int copy = bit_set[i];
        for(int j = 0; j < INT_SIZE && copy != 0; j++)
        {
            if(copy & 1)
            {
                printf("%d ", i * ARRAY_SIZE + j);
            }
            copy = copy >> 1;
            
        }
    }
    printf("\n");
}

void print_cardinality(int *bit_set)
{
    printf("printing cardinality of a set\n");
    int sum = 0;
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        int copy = bit_set[i];
        for(int j = 0; j < INT_SIZE && copy != 0; j++)
        {
            if(copy & 1)
            {
                sum++;
            }
            copy = copy >> 1;
        }
    }
    printf("the cardinality of the set is :%d\n", sum);
}

void insert_number_in_set(int *bit_set, int number)
{
    int index_in_array = number / INT_SIZE;
    int index_in_int = number % INT_SIZE;
    int mask = 1;
    mask = mask << index_in_int;
    bit_set[index_in_array] = bit_set[index_in_array] | mask;
}

void insert_set(int *bit_set)
{
    printf("inserting a set\n");
    printf("insert the number of elements in the set: ");
    int n = 0;
    scanf("%d", &n);
    int k = 0;
    int aux;
    int *array = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i<n;i++)
    {
        printf("set[%d]=", i);
        scanf("%d", &aux);
        if(aux>=0 && aux<ARRAY_SIZE * INT_SIZE)
        {
            array[k++] = aux;
        }else{
            printf("only accepitng elements in the interval [0,%d)\n", ARRAY_SIZE * INT_SIZE);
            printf("ommiting insereted value\n");
        }
    }
    
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        bit_set[i] = 0;
    }
    
    for(int i = 0; i < k; i++)
    {
        insert_number_in_set(bit_set, array[i]);
    }
}

void reunion_sets(int *first, int *second, int *result)
{
    printf("reunion of two sets\n");
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        result[i] = first[i] | second[i];
    }
}

void intersection_sets(int *first, int *second, int *result)
{
    printf("intersection of two sets\n");
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        result[i] = first[i] & second[i];
    }
}

void difference_sets(int *first, int *second, int *result)
{
    printf("difference of two sets\n");
    for(int i = 0; i < ARRAY_SIZE; i++)
    {
        result[i] = first[i] & (~second[i]);
    }
}

void check_number_in_set(int *bit_set, int number)
{
    printf("check if number is in set\n");
    int index_in_array = number / INT_SIZE;
    int index_in_int = number % INT_SIZE;
    int mask = 1;
    mask = mask << index_in_int;
    if(mask & bit_set[index_in_array])
    {
        printf("number %d is in the set\n", number);
    }else{
        printf("number %d in not in the set\n", number);
    }
}

void get_input()
{
    int response = 0;
    printf("choose a menu option: ");
    scanf("%d", &response);
    if(response<0 || response >8)
    {
        printf("not a valid option\n");
        print_menu();
        return;
    }
    
    int *first = NULL;
    int *second = NULL;
    int *result = NULL;
    switch(response){
        case 0:
            exit(0);
            break;
        case 1:
            first = get_set();
            insert_set(first);
            break;
        case 2:
            first = get_set();
            print_elements(first);
            break;
        case 3:
            first = get_set();
            print_cardinality(first);
            break;
        case 4:
            printf("first set:\n");
            first = get_set();
            printf("second set:\n");
            second = get_set();
            printf("result set:\n");
            result = get_set();
            reunion_sets(first, second, result);
            break;
        case 5:
            printf("first set:\n");
            first = get_set();
            printf("second set:\n");
            second = get_set();
            printf("result set:\n");
            result = get_set();
            intersection_sets(first, second, result);
            break;
        case 6:
            printf("first set:\n");
            first = get_set();
            printf("second set:\n");
            second = get_set();
            printf("result set:\n");
            result = get_set();
            difference_sets(first, second, result);
            break;
        case 7:
            first = get_set();
            printf("what number do you want to check? :");
            int aux = 0;
            scanf("%d", &aux);
            check_number_in_set(first, aux);
            break;
        case 8:
            print_menu();
            break;
        default:
            printf("what?");      
    }
}

int main()
{
    a = (int*)calloc(ARRAY_SIZE, sizeof(int));
    b = (int*)calloc(ARRAY_SIZE, sizeof(int));
    c = (int*)calloc(ARRAY_SIZE, sizeof(int));
    print_menu();
    
    while(1)
    {
        get_input();
    }
    
    return 0;
}