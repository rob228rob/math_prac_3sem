#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef enum {
    OK,
    INVALID_DATA,
    NO_ROOT,
    MEMORY_ERROR
} status;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == NO_ROOT) printf("NO_ROOT.\n");
}

status power(int base, int exponent, double *result) {
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

status average_geometric(int amount, double* result, ...) {
    if (amount <= 0) {
        return INVALID_DATA;
    }
    int bin_flag = 0;
    if (amount%2==0) {
        bin_flag = 1;
    }
    va_list stack;
    va_start(stack, result);
    double mult = 1;
    double power = 1.0/amount;
    for (int i = 0; i < amount; ++i) {
        double num = va_arg(stack, double);
        //printf("num: %lf; pow: %lf\n", num, power);
        if (bin_flag == 1 && num < 0) {
            va_end(stack);
            return INVALID_DATA;
        }
        mult *= pow(num, power);
    }

    *result = mult;
    va_end(stack);
    return OK;
}

int main(int argc, char* argv[]) {
    double result;
    status geom_status = average_geometric(6, &result, 10000.0, 2.0, 10.0,  100.0, 20.0, 15.0);
    if (geom_status != OK) {
        responce(geom_status);
        return 1;
    }
    printf("res is: %lf\n", result);

    double result2;
    status rec_pow = power(10, 4, &result2);
    if (rec_pow != OK) {
        responce(rec_pow);
        return 1;
    }
    printf("%.2lf - pow\n", result2);
    return 0;
}