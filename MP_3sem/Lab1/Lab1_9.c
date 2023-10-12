#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

enum random {
    ok,
    invalid_value,
    undefined_behavior,
    memory_error,
    incorrect_memory_allocation
};

bool is_num(const char* str) {
  int length = strlen(str);
  for (int i = 0; i < length; ++i) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

char* response(enum random value) {
    if (value == ok) return "ok";
    if (value == invalid_value) return "invalid_value";
    if (value == memory_error) return "memory_error";
    if (value == incorrect_memory_allocation) return "incorrect_memory_allocation";
    if (value == undefined_behavior) return "undefined_behavior";
}

void is_switched(int* a, int* b) {
    if ((*a) > (*b)) {
        int tmp = *b;
        *b = *a;
        *a = tmp;
    }
}

enum random random_value_of_arr(int* arr, int length, int A, int B) {
    if (arr == NULL) {
        return memory_error;
    }

    srand(time(NULL));
    for (int i = 0; i < length; ++i) {
        arr[i] = rand() % (B - A) + A;
    }
    return ok;
}

enum random find_and_replace(int* arr, int length) {
    if (arr == NULL) return memory_error;
    int max_val = arr[0], min_val = arr[0], max_val_index = 0, min_val_index = 0;
    for (int i = 0; i < length; ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
            max_val_index = i;
        }
        if (arr[i] < min_val) {
            min_val = arr[i];
            min_val_index = i;
        }
        if (i == length - 1) {
            //printf("Max value: %d; Max_val_index: %d;\tMin value: %d; Min value index: %d\n", arr[max_val_index], max_val_index, arr[min_val_index], min_val_index);
            arr[max_val_index] = min_val;
            arr[min_val_index] = max_val;
            return ok;
            //printf("\n");
            //printf("Max value: %d; Max_val_index: %d;\tMin value: %d; Min value index: %d\n", arr[max_val_index], max_val_index, arr[min_val_index], min_val_index);
        }
    }
    return undefined_behavior;
}

int get_nearest(int* arr, int length, int value) {
    int answer = arr[0];
    for (int i = 0; i < length; ++i) {
        if (fabs(arr[i] - value) < fabs(answer - value)) {
            answer = arr[i];
        }
    }
    return answer;
}

void print_arr(int* arr, int length) {
    for (int i = 0; i < length; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

enum random generate_array_C(int** arrC, int* arrA,int* arrB, int lengthA, int lengthB) {
    (*arrC) = (int*)malloc(sizeof(int)*lengthA);
    if (*arrC == NULL) {
        return memory_error;
    }
    if (arrC == NULL) return incorrect_memory_allocation;
    if (arrA == NULL || arrB == NULL) return memory_error;

    for (int i = 0; i < lengthA; ++i) {
        (*arrC)[i] = arrA[i] + get_nearest(arrB, lengthB, arrA[i]);
    }
    return ok;
}

int main(int argc,char* argv[]) {
    srand(time(NULL));
    if (argc != 3) {
        printf("Incorrect num of args.\n");
        return 0;
    }
    int A = atoi(argv[1]);
    int B = atoi(argv[2]);

    is_switched(&A, &B);
    const int length = 50;
    int* arr = (int*)malloc(sizeof(int) * length);
    if (arr == NULL) {
        printf("Incorrect memory allocation.\n");
        return 0;
    }
    enum random func = random_value_of_arr(arr, length, A, B);
    print_arr(arr, 50);

    if (func != ok) {
        printf("Something went wrong: %s\n", response(func));
        return 0;
    }

    enum random temp = find_and_replace(arr, length);
    if (temp != ok ) {
        printf("Something went wrong: %s\n", response(temp));
        return 0;
    }
    //print_arr(arr, 50);
    free(arr);

    const int length1 = rand() %  (10000 + 10) - 10;
    //printf("%d\n", length1);
    const int length2 = rand() % (10000 + 10) - 10;

    int* arrA = (int*)malloc(sizeof(int)*length1);
    int* arrB = (int*)malloc(sizeof(int)*length2);

    random_value_of_arr(arrA, length1, -1000, 1000);
    random_value_of_arr(arrB, length2, -1000, 1000);
    //print_arr(arrA, length1);
    //print_arr(arrB, length2);

    int* arrC;
    enum random value = generate_array_C(&arrC, arrA, arrB, length1, length2);
    if (value != 0) {
        printf("Something went wrong: %s\n", response(value));
    }
    //print_arr(arrC, length1);
    
    free(arrA);
    free(arrB);
    free(arrC);
    return 0;
}