#include <stdio.h>
#include <malloc.h>
#include <math.h>           
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>   
#include <ctype.h>


typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND,
    ALREADY_EXIST
} STATUS;

void response(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
    if (status == ALREADY_EXIST) printf("ALREADY_EXIST.\n");
}

int add(int a, int b) {
    int res = 0, carry = 0;
    res = a ^ b;
    carry = (a & b) << 1;
    while (carry)
    {
        int temp = res;
        res = res ^ carry;
        carry = (temp & carry) << 1;
    }

    return res;
}

int negative(int n) {
    return add(~n, 1);
}

int substract(int a, int b) {
    return add(a, negative(b));
}

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

void reverse(char* result_string) {
    int length = strlen(result_string);
    int start = 0, end = length - 1;
    while (start < end) {
        swap(&result_string[start], &result_string[end]);
        start++;
        end--;
    }
}

int division(int a, int b) {
    int quotient = 0;
    int remainder = 0;

    while (a >= b) {
        int shift = 0;
        while ((b << add(shift, 1)) <= a) {
            shift = add(shift, 1);
        }
        quotient = add(quotient, (1 << shift));
        a = substract(a, b << shift);
    }
    remainder = a;
    return quotient;
}

int decimal_to_base(int src_num, int r, char** final_result) {
    if (r < 1 || r > 5) {
        return INVALID_DATA;
    }
    if (src_num == 0) {
        *final_result = (char*)malloc(sizeof(char)*(1+1));
        if (*final_result == NULL) {
            return MEMORY_ERROR;
        }
        strcpy(*final_result, "0");
        return OK;
    }
    char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int num = src_num;
    if (src_num < 0) num = -num;
    int index = 0;
    char* result = (char*)malloc(sizeof(char) * (pow(2,r) + 1));
    if (result == NULL) {
        return MEMORY_ERROR;
    }
    int base = pow(2, r);

    while (num != 0) {
        result[index] = digits[abs(num) % base];
        index = add(index, 1);
        num = division(num, base);
    }
    if (src_num < 0) {
        result[index] = '-';
        index = add(index, 1);
    }
    result[index] = '\0';
    (*final_result) = (char*)malloc(sizeof(char) * (strlen(result) + 1));
    if (*final_result == NULL) {
        free(result);
        return MEMORY_ERROR;
    }

    strcpy(*final_result, result);
    free(result);
    reverse(*final_result);
    return OK;
}

int main() {
    int number = -5;
    int r = 2;
    int base = pow(2, r);

    char* res2;
    int stat_1 = decimal_to_base(number, r, &res2);
    if (stat_1 != OK) {
        response(stat_1);
        return stat_1;
    }
    printf("Res by bitwise op: %s\n", res2);
    free(res2);

    return 0;
}