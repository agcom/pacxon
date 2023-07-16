#ifndef UTILS_C_INCLUDED
#define UTILS_C_INCLUDED

#ifdef WINDOWS
#include <windows.h>
#define SLEEP_MS(x) Sleep(x)
#else
#include <unistd.h>
#define SLEEP_MS(x) usleep((x) * 1000)
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "kbhit.c"

// AKC = Arrow Key Code
#define LEFT_AKC KEY_LEFT
#define RIGHT_AKC KEY_RIGHT
#define UP_AKC KEY_UP
#define DOWN_AKC KEY_DOWN

#define SHOULD_NOT_REACH_HERE (-1)

bool rand_bool() {
	return rand() % 2;
}

void print_2d_str_arr(const int rows, const int columns, const char *arr[rows][columns]) {
	int i;
	for (i = 0; i < rows; i++) {
		int j;
		for (j = 0; j < columns; j++) {
			printf("%s", arr[i][j]);
		}
		printf("\r\n");
	}
}

void print_int_arr(const int size, const int arr[size]) {
	int i;
	for (i = 0; i < size; i++) {
		printf("%d", arr[i]);
		if (i != size - 1) printf(", ");
	}
}

int replace_2d_int_arr(const int rows, const int columns, int arr[rows][columns], const int key, const int replace) {
	int replaceds = 0;
	
	int i;
	for (i = 0; i < rows; i++) {
		int j;
		for (j = 0; j < columns; j++) {
			if (arr[i][j] == key) {
				arr[i][j] = replace;
				replaceds++;
			}
		}
	}
	
	return replaceds;
}

int linear_int_arr_search(const int size, const int arr[size], const int key) {
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == key) return i;
	}
	
	return -1;
}

void clear_screen() {
#ifdef WINDOWS
	system("cls");
#else
	system("clear");
#endif
}

void purge_keys() {
	while (kbhit()) getch();
}

int find_common_elements_of_int_arrs(
		const int res_size, const int arr1_size, const int arr2_size, int res[res_size],
		const int arr1[arr1_size], const int arr2[arr2_size]
) {
	
	int rai = 0; // Result Append Index
	bool has_space = res_size != 0;
	
	// Set the smaller length array as base
	if (arr1_size < arr2_size) {
		int i;
		for (i = 0; i < arr1_size; i++) {
			if (linear_int_arr_search(arr2_size, arr2, arr1[i]) != -1) { // Found a common int
				if (has_space) res[rai] = arr1[i];
				rai++;
				
				if (rai == res_size) has_space = false;
			}
		}
	} else {
		int i;
		for (i = 0; i < arr2_size; i++) {
			if (linear_int_arr_search(arr1_size, arr1, arr2[i]) != -1) { // Found a common int
				if (has_space) res[rai] = arr2[i];
				rai++;
				
				if (rai == res_size) has_space = false;
			}
		}
	}
	
	return rai;
}

void insertion_sort_int_arr(int size, int arr[size]) {
	int i;
	for (i = 1; i < size; i++) {
		int j;
		for (j = i; arr[j] < arr[j - 1] && j > 0; j--) {
			const int t = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = t;
		}
	}
}

bool shift_left_int_arr(const int size, int arr[size], const int remove_index) {
	
	if (remove_index < 0 || remove_index >= size) return false;
	else if (remove_index == size - 1) return true;
	
	bool executed = false;
	
	int i;
	for (i = remove_index + 1; i < size; i++) {
		const int t = arr[i];
		arr[i] = arr[i - 1];
		arr[i - 1] = t;
		
		if (!executed) executed = true;
	}
	
	return executed;
}

void sleep_ms(const long ms) {
	SLEEP_MS(ms);
}

void bug_alert(const char *msg) {
	printf("Bug: %s", msg);
	sleep_ms(1000);
}

#endif // UTILS_C_INCLUDED