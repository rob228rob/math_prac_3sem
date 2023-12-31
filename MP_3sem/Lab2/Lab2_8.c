#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


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

STATUS sum_in_base(char** result, int base, int count_number, ...);

int check_valid(int base, char** number);

void reverse(char** number, int len);

int is_valid_char(int ch);

void sum_of_numbers(char** result, char* number, int base, int* len_res, int len_num);

int main() {
    char* res;
    int count = 3;
    int base = 10;
    char* num1 = "1"; 
    char* num2 = "99999999998"; 
    char* num3 = "1"; 

    STATUS summ_status = sum_in_base(&res, base, count, num1, num2, num3);
    if(summ_status == OK) {
        printf("summ: %s\n", res);
    }
    else {
        responce(summ_status);
        return 1;
    }
    free(res);

    return 0;
}

STATUS sum_in_base(char ** result, int base, int count_number, ...)
{
    if(count_number <= 0 || (base < 2 || base > 36))
    {
        return INVALID_DATA;
    }

    va_list args;
    va_start(args, count_number);

    char * number_arg = va_arg(args, char*);
    char * number = strdup(number_arg);
    int len_num = strlen(number);
    if(check_valid(base, &number)) {
        va_end(args);
        free(number);
        return INVALID_DATA;
    }
    reverse(&number, len_num);

    int capacity_res = len_num * 2, len_res = len_num;
    (*result) = (char *)calloc(sizeof(char), capacity_res);
    if((*result) == NULL) {
        va_end(args);
        free(number);
        return INVALID_DATA;
    }
    strcpy(*result, number);
    free(number);

    for (int i = 0; i < count_number - 1; ++i) {
        number_arg = va_arg(args, char*);
        number = strdup(number_arg);
        len_num = strlen(number);
        if(check_valid(base, &number)) {
            va_end(args);
            free(number);
            return INVALID_DATA;
        }
        reverse(&number, len_num);
        if(len_num + 1 > capacity_res || len_res + 1 > capacity_res) {
            int max = len_res > len_num ? len_res : len_num;
            capacity_res = max * 2;
            char * tmp = (char *)realloc((*result), capacity_res * sizeof(char));
            if (tmp == NULL) {
                va_end(args);
                free(number);
                return MEMORY_ERROR;
            }
            (*result) = tmp;
        }

        sum_of_numbers(result, number, base, &len_res, len_num);
        free(number);
    }
    while(len_res > 1 && (*result)[len_res - 1] == '0')
    {
        (*result)[--len_res] = '\0';
    }
    reverse(result, len_res);
    va_end(args);
    return OK;
}

int check_valid(int base, char ** number)
{
    if((*number)[0] == '\0')
    {
        return 1;
    }
    for(int i = 0; (*number)[i] != '\0'; ++i)
    {
        if(isdigit((*number)[i]))
        {
            if( (*number)[i] - '0'>= base)
            {
                return 1;
            }
        }
        else if(isalpha((*number)[i]))
        {
            (*number)[i] = (char)toupper((*number)[i]);
            if((*number)[i]  - 'A' + 10 >= base)
            {
                return 1;
            }
        }
        else
        {
            return 1;
        }
    }
    return 0;
}

void reverse(char ** number, int len)
{
    int i = 0, j = len - 1;
    for(; (j - i) > 0; ++i, --j)
    {
        char temp = (*number)[i];
        (*number)[i] = (*number)[j];
        (*number)[j] = temp;
    }
}

void sum_of_numbers(char ** result, char * number, int base, int * len_res, int len_num)
{
    int len_sum = (*len_res > len_num) ? len_num : *len_res;
    int next_digit = 0;
    int idx = 0;
    for(; idx < len_sum; ++idx)
    {
        int first_number = isdigit((*result)[idx]) ? (*result)[idx] - '0' : (*result)[idx] - 'A' + 10;
        int second_number = isdigit(number[idx]) ? number[idx] - '0' : number[idx] - 'A' + 10;
        int sum = first_number + second_number  + next_digit;
        (*result)[idx] = (sum % base) > 9 ? (sum % base) + 'A' - 10 : (sum % base) + '0';
        next_digit = sum / base;
    }
    if(*len_res > len_num)
    {
        for(; idx < *len_res; ++idx)
        {
            int first_number = isdigit((*result)[idx]) ? (*result)[idx] - '0' : (*result)[idx] - 'A' + 10;
            int sum = first_number + next_digit;
            (*result)[idx] = (sum % base) > 9 ? (sum % base) + 'A' - 10 : (sum % base) + '0';
            next_digit = sum / base;
        }
    }
    if(len_num > *len_res)
    {
        for(; idx < len_num; ++idx)
        {
            int second_number = isdigit(number[idx]) ? number[idx] - '0' : number[idx] - 'A' + 10;
            int sum = second_number + next_digit;
            (*result)[idx] = (sum % base) > 9 ? (sum % base) + 'A' - 10 : (sum % base) + '0';
            next_digit = sum / base;
        }
    }
    if(next_digit != 0)
    {
        (*result)[idx++] = next_digit > 9 ? next_digit + 'A' - 10 : next_digit + '0';
    }
    (*result)[idx] = '\0';
    *len_res = idx;
}