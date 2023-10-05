#include <stdio.h>
#include <malloc.h>
#include <math.h>           
#include <stdlib.h>
#include <string.h>


double exponent(double x, double deg) {
    if (deg == 1.0) return x;
    if (deg == 2.0) return x*x;
    if (deg == -1.0) return 1.0/x;
    if (deg == 0) return 1.0;

}

double limit_e(double eps) {
    double n = 1;
    double x_prev = pow(1.0+(1.0/n), n);
    double x_curr = pow (1.0 + (1.0/(n+1)), n+1);
    while (fabs(x_curr - x_prev) > eps) 
    {
        n = n + 1;
        x_prev = x_curr;
        x_curr = pow(1.0 + (1.0/(n+1)), n+1);
    }
    
    return x_curr;   
}

long long fact(int num) {
  if (num == 1) return 1;
  if (num == 2) return 2;
  if (num > 20) return -1;
  long long arr_res[21];
  memset(arr_res, 0, 20);
  arr_res[0] = 1;
  arr_res[1] = 1;
  for (int i = 2; i <= num; ++i) {
    arr_res[i] = arr_res[i - 1] * i;
  }

  return arr_res[num];
}

double row_e(double eps) {
    double n = 1;
    double prev_summ = 0;
    double summ = 1.5;

    do {
        n++;
        prev_summ = summ;
        summ = summ + 1/fact(n);
    } while (fabs(summ - prev_summ) > eps);
    
    return summ;
}

int main(int argc,char* argv[]) {
    char flag = argv[1][1];
    double answer, answer2;
    double eps = atof(argv[2]);
    switch (flag)
    {
    case 'e':
        answer = limit_e(eps);
        answer2 = row_e(eps);
        printf("%lf - ans_limit. \n %lf - ans_row.\n", answer, answer2);
        break;
    
    default:
        break;
    }

    return 0;
}