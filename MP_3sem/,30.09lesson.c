#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

double average(int count, ...) {
    int i;
    double summ = 0;
    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; ++i) {
        summ += va_arg(ptr, double);
    }
    return summ/count;
}

int main() {
    printf("%.2lf - avg val\n", average(3, 1.1, 2.2, 3.3));
    return 0;
}