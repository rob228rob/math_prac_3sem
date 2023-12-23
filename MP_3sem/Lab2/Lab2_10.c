#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

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


double solve_polynom(double argument, double coefs[], double power) {
    double result = 0;
    double current_power = 1;
    for (int i = 0; i < power + 1; i++) {
        current_power *= argument;
        result += current_power * coefs[i];
    }
    return result;
}

void derivative(double coefs[], double power, int iteration) {
    for (int i = 0; i < power + 1; i++) {
        coefs[i] *= i - iteration;
    }
}

STATUS new_coefs(double argument, double** coefs_result, int power, ...) {
    if (power < 0) {
        return INVALID_DATA;
    }
    (*coefs_result) = (double *)malloc(sizeof(double) * (power + 1));
    if (!(*coefs_result)) {
        return MEMORY_ERROR;
    }
    
    double coefs_old[power + 1];

    va_list args;
    va_start(args, power);
    for (int i = 0; i < power + 1; i++) {
        coefs_old[i] = va_arg(args, double);
    }
    va_end(args);
    if (argument == 0) {
        for (int i = 0; i < power; ++i) {
        (*coefs_result)[i] = coefs_old[i];
        
        }   
        return OK;     
    }
    double multiply = 1.0;
    for (int i = 0; i < power + 1; i++) {
        (*coefs_result)[i] = solve_polynom(argument, coefs_old, power);
        if (i > 1) {
            multiply *= i;
            (*coefs_result)[i] /= multiply; 
        }
        derivative(coefs_old, power, i);
    }
    return OK;
}

int main(int argc, char * argv[]) {
    double* coefs;
    int power = 3;
    int  stat = new_coefs(0, &coefs, power, 1.0, 1.0, 1.0, 1.0);
    if (stat != OK) {
        responce(stat);
        return 1;
    }

    for (int i = 0; i < power; ++i) {
        printf("%lf\n", coefs[i]);
    }
    free(coefs);
    return 0;
}



