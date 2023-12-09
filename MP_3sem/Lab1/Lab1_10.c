#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <ctype.h>

enum statuses {
    ok,
    memory_error,
    invalid_data,
    undefined_behavior
};

enum statuses convert_to_base(int num, int base, char** final_result) {
    char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int index = 0;
    char result[100];
    
    if(num == 0 || base < 2 || base > 36) {
        return invalid_data;
    }
    
    while(num > 0) {
        result[index++] = digits[num % base];
        num /= base;
    }
    
    (*final_result) = (char*)malloc(sizeof(char)*strlen(result) + 1);
    
    if (*final_result == NULL) {
        return memory_error;
    }

    strcpy(*final_result, result);
    return ok;
}

bool is_valid_number(char* num, int base) {
    char valid_digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for(int i = 0; i < strlen(num); i++) {
        bool is_valid_digit = false;
        for(int j = 0; j < base; j++) {
            if(num[i] == valid_digits[j]) {
                is_valid_digit = true;
                break;
            }
        }
        
        if(!is_valid_digit) {
            return false;
        }
    }
    
    
    return true;
}

void max_of_array(int* array, int length, int* result) {
    int max_val = array[0];
    for (int i = 0; i < length; ++i) {
        max_val = fmax(max_val, array[i]);
    }
    *result = max_val;
}

char* response(int status) {
    if (status == ok) return "ok";
    else if (status == invalid_data) return "invalid data";
    else if (status == memory_error) return "memory error";
    else if (status == undefined_behavior) return "undefined behavior";
}

void result_print(int max_val) {
    int n = 9;
    printf("With base 10: %d\n",max_val);
    for (int i = 0; i < 4; ++i) {
        char* str;
        enum statuses status = convert_to_base(max_val, n, &str);
        if (status == ok) {
            printf("With base %d: %s\n",n, str);
        } else {
            printf("%s\n", response(status));
        }
        free(str);
        n += 9;
    }
}

int main() {
    int base_from;
    char terminator[] = "Stop";
    
    printf("Enter the base of the numbers: \n");
    scanf("%d", &base_from);
    if (base_from > 36 || base_from < 2) {
        printf("Inval base");
        return 1;
    }
    int* arr_abs_value = (int*)malloc(sizeof(int)*1);
    int index = 0;

    while (1)
    {
        char number[100];
        scanf("%s", number);

        if (strcmp(number,terminator) == 0) {
            break;
        } 

        if (!is_valid_number(number, base_from)) {
            printf("Invalid number.\n");
            
            free(arr_abs_value);
            return 1;
        }

        char* endptr;
        int decimal_number = strtoll(number, &endptr, base_from);
        arr_abs_value[index] = decimal_number;
        index++;
        arr_abs_value = (int*)realloc(arr_abs_value,(index+1)*sizeof(int));
    }

    int max_val;
    max_of_array(arr_abs_value, index, &max_val);
    result_print(max_val);
    free(arr_abs_value);
    
    return 0;
}