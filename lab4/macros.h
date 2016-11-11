#ifndef MACRO_H_DEFINED
#define MACRO_H_DEFINED

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define TESTEZA(i, j, n) (( (i) > 0  && (i) < (n) && (j) > 0 && (j) < (n) ) ? 1 : 0 )
#define MEDIA(a, b, c) ( ((double)(a) + (b) + (c)) / 3 ) 
#define POLINOM(x) ( 2*pow((x), 5) + 3*pow((x), 4) - pow((x), 3)  + 2*pow((x), 2) - 8*(x) + 5 )

void myFucntion();
#endif
