#include<stdio.h>
#include<stdlib.h>
#include<math.h>

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


double f1(double x, double epsilon)
{
    return log(1 + x) / x;
}
double f2(double x, double epsilon)
{
    return exp(-x*x/2);
}
double f3(double x, double epsilon)
{
    return log(1/(1 - x));
}


double _pow(double x, double st)
{
    double tmp = x;
    for(int i = 2; i <= st; ++i) {
        tmp*=x;
    }
    return tmp;
}

double _root(double x, double exponent, double eps){
    exponent = 1/exponent;
    if(exponent - 1.0 <=eps) { 
        return x;
    }
    double value_current = 1;
    double value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ _pow(value_current, exponent-1));
    while(fabs(value_current - value_next) > eps){
        value_current = value_next;
        value_next = (1.0/exponent)*((exponent - 1) * value_current + x/ _pow(value_current, exponent-1));
    }
    return value_next;
}

double f4(double x, double epsilon)
{
    return(_root(x, x, epsilon));
}
STATUS integral(double(*f)(double, double), double a, double b, double epsilon, double* res)
{
    if ( b <= a || epsilon <= 0 || f == NULL) {
        return INVALID_DATA;
    }
    double trapezoids = (b - a)/epsilon;
    double x0 = a + epsilon;
    double int_sum = 0;
    for(int i = 1; i < trapezoids; ++i)
    {
       int_sum += (f(x0, epsilon) + f(x0 + epsilon, epsilon))/2 * epsilon;
       x0 += epsilon; 
    }
    if (isnan(int_sum)) {
        return NOT_A_NUMBER;
    }
    *res = int_sum;
    return OK;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        printf("INVALID NUMBER OF ARGUMENTS\n");
        return 1;
    }
    char* end;
    double epsilon;
    epsilon= strtod(argv[1], &end);
    if (epsilon <= 0) {
        return 1;
    }
    double res, res2, res3, res4;

    int status1 = integral(f1, 0, 1, epsilon, &res);
    if (status1 != OK) {
        responce(status1);
        return 1;
    }
    printf("First: %.4f\n", res);
    int status2 = integral(f2, 0, 1, epsilon, &res2);
    if (status2 != OK) {
        responce(status2);
        return 1;
    }
        printf("Second: %.4f\n", res2);
    int status3 = integral(f3, 0, 1, epsilon, &res3);
    if (status3 != OK) {
        responce(status3);
        return 1;
    }

    printf("Third: %.4f\n", res3);
    int status4 = integral(f4, 0, 1, epsilon, &res4);
    if (status4 != OK) {
        responce(status4);
        return 1;
    }   
    printf("Fourth: %.4f\n", res4);
    return 0;
}