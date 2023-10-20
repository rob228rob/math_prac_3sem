#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    NO_ONE_FOUND
} status;

typedef struct {
    double x;
    double y;
    int positive_cos;
} vector;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
}

status is_flag(int symb) {
    char *flags = "lrnuc";
    for (int i = 0; i < 5; ++i) {
        if (flags[i] == symb) {
            return OK;
        }
    } 
    return UNDEFINED_BEHAVIOR;
}

void get_length(char* str, int* result) {
    if (str == NULL) {
        *result = 0;
        return;
    }
    int index = 0;
    while (str[index] != '\0')
    {
        index++;
    }
    *result = index;
}

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

status string_cpy(char* source_str, char* str_cpy, int length) {
    if (str_cpy == NULL || source_str == NULL) {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < length; ++i) {
        str_cpy[i] = source_str[i];
    }
    str_cpy[length] = '\0';
    return OK;
}

double cos_of_vectors(double a_x, double a_y,double b_x, double b_y) {
    double cos = (a_x * b_x + a_y *b_y) / (pow(a_x * a_x + a_y * a_y, 0.5) + pow(b_x * b_x + b_y * b_y, 0.5));
    return cos;
}

status is_convex(int* result, int n, ...) {
    if (n < 0 || n % 2 != 0) {
        return INVALID_DATA;
    }
    va_list list;
    vector *vectors = (vector*)malloc(sizeof(vector)*n/2);
    if (vectors == NULL) {
        return MEMORY_ERROR;
    }
    va_start(list, n);
    double* arr = (double*)malloc(sizeof(int)*n);
    if (arr == NULL) {
        free(vectors);
        return MEMORY_ERROR;
    }
    for (int i = 0; i < n; ++i) {
        arr[i] = va_arg(list, double);
    }
    va_end(list);
    int index = 0;
    for (int i = 2; i < n; i += 2) {
        vectors[index].x = arr[i] - arr[i - 2];
        vectors[index].y = arr[i + 1] - arr[ i - 1];
        vectors[index].positive_cos = 0;
        if ( i == n/2 - 2) {
            vectors[index + 1].x = arr[0] - arr[i];
            vectors[index + 1].y = arr[1] - arr[1 + i];
            vectors[index + 1].positive_cos = 0;
        }
        index++;
    }
    
    for (int i = 1; i < n/2 + 1; ++i) {
        double cos;
        if (i == n/2) {
            cos = cos_of_vectors(vectors[i].x, vectors[i].y, vectors[0].x, vectors[0].y);
        } else {
        cos = cos_of_vectors(vectors[i].x, vectors[i].y, vectors[i - 1].x, vectors[i - 1].y);
        }
        if (cos >= 0) {
            vectors[i].positive_cos = 1;
        } else {
            vectors[i].positive_cos = -1;
        }


        //printf("%d\n", vectors[i].positive_cos);
    }
    
    for (int i = 1; i < index + 1; ++i) {
        if (vectors[i - 1].positive_cos + vectors[i].positive_cos == 0) {
            *result = -1;
            return OK;
        }
    }
    *result = 1;
    return OK;
}

status power(double base, int exponent, double *result) {
    if(exponent < 0) {
        return INVALID_DATA;
    }
    else if(exponent == 0) {
        *result = 1;
        return OK;
    }
    else if(exponent % 2 == 0) {
        status exp2 = power(base * base, exponent / 2, result);
    }
    else {
        status exp = power(base, exponent - 1, result);
        *result *= base;
    }
    return OK;
}


status polynom(double *result, double x, int exponent, ...) {
    if (exponent <= 0) {
        return INVALID_DATA;
    }
    double *arr = (double*)malloc(sizeof(double)*(exponent+1));
    if (arr == NULL) {
        return MEMORY_ERROR;
    }
    va_list list;
    va_start(list, exponent);
    for (int i = exponent; i >= 0; --i) {
        arr[i] = va_arg(list, double);
    }
    va_end(list);
    long double summ = 0;
    for (int i = 0; i < exponent + 1; ++i) {
        double exp;
        status stat_pow = power(x,i,&exp);
        if (stat_pow != OK) {
            return stat_pow;
        }
        //printf("%lf - summ  %d - base\n", (arr[i] * exp), i);
        summ += (arr[i] * exp);
        
    }
    
    *result = summ;
    
    return OK;
}

int main(int argc, char* argv[]) {

    int result; 
    status stat = is_convex(&result, 10, 5, 1.5, 1.9, 1.9, 0, 2, 2,0, 2, 2 );
    if (stat != OK) {
        responce(stat);
        return 1;
    }
    if (result == 1) {
        printf("is convex.\n");
    } else {
        printf("not.\n");
    }
    double result1;
    double x = 2.0;
    status poly_stat = polynom(&result1, x, 2, 1.0, -8.0, 2.0);
    if (poly_stat != OK) {
        responce(poly_stat);
        return 1;
    }
    printf("%.2lf - value of polynom in x:%.1lf\n", result1, x);
    return 0;
}