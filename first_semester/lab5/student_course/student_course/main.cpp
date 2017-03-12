#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: main.cpp
file creation date: 2016-12-13 20:42:42
*/

#include "student_course.h"

int main()
{
	StudentArrayT* students = (StudentArrayT*)malloc(sizeof(StudentArrayT));
	students->length = 0;
	students->size = 1;
	students->students = (StudentT**)malloc(sizeof(StudentT*));
	while (true)
	{
		print_menu();
		int input;
		scanf("%d", &input);
		while (getchar() != '\n');
		switch (input)
		{
			case 1:
			{
				char *name = print_read("student name");
				char *enrollment_code = print_read("enrollment code");
				StudentT* student = create_student(name, enrollment_code);
				add_student(students, student);

				break;
			}
			case 2:
			{
				char *name = print_read("student name");
				StudentT* result = find_student_by_name(students, name);
				if (result != NULL)
				{
					delete_student(students, result);
				}
				else 
				{
					printf("student not found\n");
				}
				
				
				break;
			}
			case 3:
			{

				break;
			}
			case 4:
			{
				char *name = print_read("student name");
				StudentT* result = find_student_by_name(students, name);
				if (result != NULL)
				{
					print_student(result);
				}
				else 
				{
					printf("student not found\n");
				}

				break;
			}
			case 5:
			{
				char *enrollment_code = print_read("enrollment_code");
				StudentT* result = find_student_by_enrollment_code(students, enrollment_code);
				if (result != NULL)
				{
					print_student(result);
				}
				else 
				{
					printf("student not found\n");
				}

				break;
			}
			case 6:
			{
				print_studentarray(students);

				break;
			}
			case 7:
			{
				//TODO random bug: students is not sorted, increment counter for number of hours wasted here: 2
				qsort(students->students, students->length, sizeof(StudentT*), compare_student_by_name);
				print_studentarray(students);
				break;
			}
			case 8:
			{
				qsort(students->students, students->length, sizeof(StudentT*), compare_student_by_enrollment_code);
				print_studentarray(students);
				break;

				break;
			}
			case 11:
			{

				break;
			}
			case 12:
			{

				break;
			}
			case 13:
			{

				break;
			}
			case 14:
			{

				break;
			}
			case 15:
			{

				break;
			}
			case 16:
			{

				break;
			}
			case 17:
			{

				break;
			}
			case 18:
			{

				break;
			}
			case 0:
			{
				system("pause");
				exit(0);
			}
			default:
				printf("not a valid option\n");
				break;
		}
	}
	system("pause");
	return 0;
}