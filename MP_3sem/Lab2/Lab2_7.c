#include <stdio.h>
#include <stdlib.h>
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

typedef double (*Equation)(double);

status find_root_by_bisection(double left_border, double right_border, double epsilon, Equation function, double* result) {
    if (epsilon <= 0) {
        return INVALID_DATA;
    }

    double fl = function(left_border);
    double fr = function(right_border);
    
    if (fl * fr >= epsilon) {
        return NO_ROOT; 
    }
    
    double x;
    while (fabs(left_border - right_border) > epsilon) 
    {   
        x = (left_border + right_border) / 2.0;
        double fx = function(x);
        if (fabs(fx - epsilon) == 0 ) {
            x = fx;
            break;
        } else if (fx * fl < 0) {
            right_border = x; 
        } else {
            left_border = x;
        }
    }
    
    *result = x;
    return OK;
}


double equation1(double x) {
    return -x;
}

double equation2(double x) {
    return x - 20;
}

double equation3(double x) {
    return x*x*x - 9;
}
int main(int argc, char* argv[]) {
    double result;
    status method_status = find_root_by_bisection(-10, 10, 1e-10, equation1, &result );
    if (method_status != OK) {
        responce(method_status);
        return 1;
    } else {
        printf("%.2lf - root of equation.\n", result);
    }

    status method_status2 = find_root_by_bisection(-100, 0, 1e-10, equation2, &result );
    if (method_status2 != OK) {
        responce(method_status2);
        return 1;
    } else {
        printf("%.2lf - root of equation.\n", result);
    }

    status method_status3 = find_root_by_bisection(-100, 100, 1e-10, equation3, &result );
    if (method_status3 !=  OK) {
        responce(method_status3);
        return 1;
    } else {
        printf("%.2lf - root of equation.\n", result);
    }

    return 0;
}