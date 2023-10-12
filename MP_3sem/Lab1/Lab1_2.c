#include <stdio.h>
#include <malloc.h>
#include <math.h>           
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>                                                               


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

double limit_ln2(double eps) {
    double n = 1;
    double x_prev = n*(pow(2,1/n) - 1);
    double x_curr = (n+1) * (pow(2,1/(n+1)) - 1);
    while (fabs(x_curr - x_prev) > eps )
    {
        n++;
        x_prev = x_curr;
        x_curr = (n+1) * (pow(2,1/(n+1)) - 1);
    }
    return x_curr;
}

double limit_pi(double eps) {
    double n = 1;
    double x_prev = pow(pow(2, n) * fact(n), 4) / (n*pow(fact(2*n), 2));
    double x_curr = pow(pow(2, n+1) * fact(n+1), 4) / ((n+1)*pow(fact(2*n+1), 2));
    while (fabs(x_curr - x_prev) > eps)
    {
        n++;
        x_prev = x_curr;
        x_curr = pow(2.0, 4*(n+1))/pow(fact(2*(n+1)),2) * pow(fact(n+1), 4) / n;
        //x_curr = pow(pow(2, n+1) * fact(n+1), 4) / ((n+1)*pow(fact(2*(n+1)), 2));
    }

    return x_curr;
}

double limit_sqrt2(double eps) {
    double n = 2;
    double x_prev = -0.5;
    double x_curr = x_prev - (x_prev * x_prev / 2) + 1;
    while (fabs(x_curr - x_prev) > eps) {
        n++;
        x_prev = x_curr;
        x_curr = x_prev - (x_prev * x_prev / 2) + 1;
        
    }

    return x_curr;    
}

double C(int m, int k) {
    return fact(m)/fact(k)/fact(m-k);
}

double summ(double m) {
    double general_summ = 0;
    for (int k = 1; k < m; ++k) {
        general_summ += (C(m,k) * (pow(-1,k)/k) * log(fact(k)) );
    }
    return general_summ;
}

double limit_y(double eps) {
    double m = 3;
    double x_prev = summ(m);
    double x_curr = summ(m+1);
    printf("DDX\n");
    while (fabs(x_curr - x_prev) > eps)
    {
        m++;
        x_prev = x_curr;
        x_curr = summ(m+1);
        printf("%lf - xprev\n", x_curr);
    }
    printf("XDD\n");
    return x_prev;
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

bool is_flag(char str) {
  if (str == 'p' ||  str == 's' || str == 'l' || 
    str == 'e' || str == 'y') {
    return true;
  }
  return false;
}

int main(int argc,char* argv[]) {
    char flag = argv[1][1];
    if (!is_flag(flag)) {
        printf("Incorrect flag.\n");
        return 0;
    }
    double answer, answer2;
    double eps = atof(argv[2]);
    if (eps <= 0) {
        printf("Incorrect value of eps.\n");
        return 0;
    }
    switch (flag)
    {
    case 'e':
        answer = limit_e(eps);
        answer2 = row_e(eps);
        printf("%lf - value of e by limit.\n %lf - ans_row.\n", answer, answer2);
        break;
    case 'l':
        answer = limit_ln2(eps);
        printf("%lf - value of ln2 by limit.\n ", answer);
        break;
    case 'p':
        answer = limit_pi(eps);
        printf("%lf - value of Pi by limit.\n ", answer);
        break;
    case 's':
        answer = limit_sqrt2(eps);
        printf("%lf - value of sqrt2 by limit.\n ", answer);
        break;
    case 'y':
        answer = limit_y(eps);
        printf("%lf - value of Y by limit.\n ", answer);
        break;

    default:
        break;
    }

    return 0;
}