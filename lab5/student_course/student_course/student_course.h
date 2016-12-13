/*
author: Acs David
file name: student_course.h
file creation date: 2016-11-20 23:26:24
*/

#ifndef STUDENT_COURSE_H
#define STUDENT_COURSE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
typedef struct StudentTag {
	char *name;
	char *enrollement_code;
}StudentT;

typedef struct CourseTag {
	int id;
	char *name;
}CourseT;

typedef struct StudentArrayTag {
	StudentT **students;
	int length;
	int size;
}StudentArrayT;

// define your functions here
void print_menu();
char* print_read(char *to_print);
StudentT* create_student(char *name, char *enrollment_code);
void add_student(StudentArrayT* students, StudentT* student);
void print_student(StudentT* student);
void print_studentarray(StudentArrayT* students);
StudentT* find_student_by_name(StudentArrayT* students, char *name);
StudentT* find_student_by_enrollment_code(StudentArrayT* students, char *enrollment_code);
void delete_student(StudentArrayT* students, StudentT* student);
int compare_student_by_name(const void *a, const void *b);
int compare_student_by_enrollment_code(const void *a, const void *b);

#endif
