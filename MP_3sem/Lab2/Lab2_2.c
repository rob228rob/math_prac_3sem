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

double binary_pow (double base, int power) {
    if (power < 0) {
        base = 1.0 / base;
        power = abs(power);
    }
    if (power == 0) return 1;
    if (power % 2 == 1) return binary_pow(base, power - 1) * base;
    else {
        double tmp = binary_pow(base, power / 2);
        return tmp * tmp;
    }
}

status average_geometric(int amount, double* result, ...) {
    if (amount <= 0) {
        return INVALID_DATA;
    }
    int bin_flag = 0;
    if (amount % 2==0) {
        bin_flag = 1;
    }
    va_list stack;
    va_start(stack, result);
    double mult = 1;
    double power = 1.0/amount;
    for (int i = 0; i < amount; ++i) {
        double num = va_arg(stack, double);

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

    double result2 = binary_pow(10.2, 2);
    printf("%.2lf - pow\n", result2);
    return 0;
}