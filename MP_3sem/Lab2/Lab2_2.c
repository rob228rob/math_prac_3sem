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

double old_approximate_power(double b, double e) {
    union {
        double d;
        long long i;
    } u = { b };
    u.i = (long long)(4606853616395542500L + e * (u.i - 4606853616395542500L));
    return u.d;
}

status average_geometric(int amount, double* result, ...) {
    if (amount <= 0) {
        return INVALID_DATA;
    }
    
    va_list stack;
    va_start(stack, result);
    __int64_t mult = 1;
    for (int i = 0; i < amount; ++i) {
        mult *= (__int64_t)va_arg(stack, int);
    }
    
    *result = pow(mult, 0.5);
    va_end(stack);
    return OK;
}

int main(int argc, char* argv[]) {
    double result;
    status geom_status = average_geometric(6, &result, 10, 2, 10,  10, 20, 15);
    if (geom_status != OK) {
        responce(geom_status);
        return 1;
    }
    printf("res is: %.2lf\n", result);

    double result2;
    status rec_pow = power(10, 4, &result2);
    if (rec_pow != OK) {
        responce(rec_pow);
        return 1;
    }
    printf("%.2lf - pow\n", result2);
    return 0;
}