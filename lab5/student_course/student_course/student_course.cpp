#define _CRT_SECURE_NO_WARNINGS
/*
author: David
file name: student_course.cpp
file creation date: 2016-12-13 20:43:26
*/

#include "student_course.h"

void print_menu()
{
	printf("\n----------------------------------\n");
	printf("1. add student\n");
	printf("2. remove student\n");
	printf("3. modify student\n");
	printf("4. search student by name\n");
	printf("5. search student by enrollment number\n");
	printf("6. print all students\n");
	printf("7. sort students by name\n");
	printf("8. sort students by enrollment number\n");

	printf("11. add course\n");
	printf("12. remove course\n");
	printf("13. modify course\n");
	printf("14. print all courses\n");

	printf("15. add student to course\n");
	printf("16. delete student from course\n");
	printf("17. print all courses followed by a student\n");
	printf("18. print all students following a course\n");

	printf("0. exit\n");
	printf("\n----------------------------------\n");

}

void resize_students(StudentArrayT* students, int size)
{
	students->students = (StudentT**)realloc(students->students, sizeof(StudentT*) * size);
	students->size = size;
}

char* print_read(char *to_print)
{
	printf("%s: ", to_print);
	char buffer[1024];
	gets_s(buffer);
	char *to_return = (char*)malloc(sizeof(char) * strlen(buffer));
	strcpy(to_return, buffer);

	return to_return;
}

StudentT* create_student(char *name, char *enrollment_code)
{
	StudentT* student = (StudentT*)malloc(sizeof(StudentT));
	student->enrollement_code = enrollment_code;
	student->name = name;

	return student;
}

void add_student(StudentArrayT* students, StudentT* student)
{
	if (students->length >= students->size - 1)
	{
		resize_students(students, students->size * 2);
	}

	*(students->students + students->length) = student;
	students->length++;
}

void print_student(StudentT* student)
{
	printf("name: %s; enrollment code: %s\n", student->name, student->enrollement_code);
}

void print_studentarray(StudentArrayT* students)
{
	for (int i = 0; i < students->length; i++)
	{
		print_student(*(students->students + i));
	}
}

StudentT* find_student_by_name(StudentArrayT* students, char *name)
{
	for (int i = 0; i < students->length; i++)
	{
		if (strcmp((*(students->students + i))->name, name) == 0)
		{
			return *(students->students + i);
		}
	}

	return NULL;
}

StudentT* find_student_by_enrollment_code(StudentArrayT* students, char *enrollment_code)
{
	for (int i = 0; i < students->length; i++)
	{
		if (strcmp((*(students->students + i))->enrollement_code, enrollment_code) == 0)
		{
			return *(students->students + i);
		}
	}

	return NULL;

}

void delete_student(StudentArrayT* students, StudentT* student)
{
	for (int i = student - *students->students; i < students->length - 1; i++)
	{
		*(students->students + i) = *(students->students + i + 1);
	}

	students->length--;
}

int compare_student_by_name(const void *a, const void *b)
{
	return _stricmp((((StudentT*)a)->name), (((StudentT*)b)->name));
}
int compare_student_by_enrollment_code(const void *a, const void *b)
{
	return _stricmp((((StudentT*)a)->enrollement_code), (((StudentT*)b)->enrollement_code));
}