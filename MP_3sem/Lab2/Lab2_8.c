#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

typedef struct {
    double x;
    double y;
    int positive_cos;
} vector;

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND,
    NOT_A_NUMBER
} STATUS;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
    if (status == NOT_A_NUMBER) printf("NOT_A_NUMBER.\n");
}

STATUS to_upper(char symb, int* res) {
    if (symb >= 'a' && symb <= 'z') {
        *res = symb - 'a' + 'A';
    } else {
        *res = symb;
    }
}

STATUS convert_to_decimal(char number[], int base, int* _decimal) {
    if (base == 10) {
        *_decimal = atoi(number);
        return OK;
    }
    int decimal = 0;
    int power = 1;
    int length = strlen(number);
    if (length > 8) {
        return INVALID_DATA;
    }
    
    for (int i = length - 2; i >= 0; i--) {
        int digit;
        if (number[i] >= '0' && number[i] <= '9') {
            digit = number[i] - '0';
        } else if (number[i] >= 'A' && number[i] <= 'Z') {
            digit = number[i] - 'A' + 11;
        }
        decimal += digit * power;
        power *= base;
    }
    
    *_decimal = decimal;
    return OK;
}

STATUS convert_from_decimal(int decimalNumber, int base, char** num) {
    char convertedNumber[100];
    int index = 0;
    
    while (decimalNumber != 0) {
        int remainder = decimalNumber % base;
        
        if (remainder >= 0 && remainder <= 9) {
            convertedNumber[index] = remainder + '0';
        } else if (remainder >= 10 && remainder <= 35) {
            convertedNumber[index] = remainder - 10 + 'A';
        }
        
        decimalNumber /= base;
        index++;
    }
    *num = (char*)malloc(sizeof(char)*(index+1));
    if (*num == NULL) {
        return MEMORY_ERROR;
    }

    strcpy((*num),convertedNumber);
    return OK;
}
STATUS is_flag(int symb) {
    char *flags = "lrnuc";
    for (int i = 0; i < 5; ++i) {
        if (flags[i] == symb) {
            return OK;
        }
    } 
    return UNDEFINED_BEHAVIOR;
}

// return length of str
int get_length(char* str) {
    if (str == NULL) {
        return 0;
    }
    
    int length = 0;
    while (*(str + length) != '\0') {
        length++;
    }
    
    return length;
}


void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

STATUS string_cpy(char* source_str, char* str_cpy, int length) {
    if (str_cpy == NULL || source_str == NULL) {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < length; ++i) {
        str_cpy[i] = source_str[i];
    }
    str_cpy[length] = '\0';
    return OK;
}

STATUS power(double base, int exponent, double *result) {
    if(exponent < 0) {
        return INVALID_DATA;
    }
    else if(exponent == 0) {
        *result = 1;
        return OK;
    }
    else if(exponent % 2 == 0) {
        STATUS exp2 = power(base * base, exponent / 2, result);
    }
    else {
        STATUS exp = power(base, exponent - 1, result);
        *result *= base;
    }
    return OK;
}



int char_to_number(char c) {
    if (isdigit(c)) {
        return c - '0';
    } 
    else {
        return toupper(c) - 'A' + 10;
    }
}

char number_to_char(int num) {
    if (num < 10 || num >= 0) {
        return num + '0';
    }
    else {
        return num - 10 + 'A';
    }
}

// Summ number in the num syst with current base
STATUS sum_by_numeral_syst(int *_result, int base, int count, ...) {
    
    if (base < 2 || base > 36) {
        return INVALID_DATA; 
    }
    if (count <= 0) {
        return INVALID_DATA;
    }

    va_list args;
    va_start(args, count);

    int result = 0;
    for (int i = 0; i < count; i++) {
        char* num = va_arg(args, char*);
        int temp_int;
        STATUS status = convert_to_decimal(num, base, &temp_int);
        if (status != OK) {
            return status;
        }
        result += temp_int;
    }
    
    va_end(args);
    
    (*_result) = result;
    return OK;
}



int main(int argc, char* argv[]) {

    int base = 2; 
    int count = 3; 
    char* num1 = "01"; 
    char* num2 = "01"; 
    char* num3 = "01"; 
    int result = -1;
    STATUS sum_stat = sum_by_numeral_syst(&result,base, count, num1, num2, num3);
    if (sum_stat != OK) {
        responce(sum_stat);
        return 1;
    }
    //char* result = addNumbers(base, count, num1, num2, num3);
    
    printf("Результат: %d\n", result);
    
    //free(result); // Освобождение памяти
    
    return 0;
}
