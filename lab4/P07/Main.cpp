#include "L2_P07_Acs_David.h"

int main()
{
	//testare modului

	VectorT vector = citire_tastatura();
	afisare_elementelor(vector);
	inserare_pozitie(vector, 2, 9);
	afisare_elementelor(vector);
	printf("%d\n", max_element(vector));
	printf("%d\n", min_element(vector));
	ordonare(vector);
	afisare_elementelor(vector);
	VectorT vector2 = citire_tastatura();
	ordonare(vector2);
	VectorT vector_intercalat = intercalare(vector, vector2);
	afisare_elementelor(vector_intercalat);
	
	return 0;
}