#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

const double err = 1e10;
const int c = 3;

bool correct_num(const char* str) {
  size_t length = strlen(str);
  for (int i = 0; i < length; ++i) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool is_flag(char str) {
  if (str == 'q' ||  str == 't' || str == 'm') {
    return true;
  }
  return false;
}

void print_array(int* array_answ, int length) {
  for (int i = 0; i < length; ++i) {
    printf("%d \n", array_answ[i]);
  }
}


long long fact(int num) {
  int arr_res[20];
  memset(arr_res, 0, 20);
  arr_res[0] = 1;
  arr_res[1] = 1;
  for (int i = 2; i <= num; ++i) {
    arr_res[i] = arr_res[i - 1] * i;
  }

  return arr_res[num];
}

int exponent(int num, int degree) {
  int sum = 1;
  for (int i = 0; i < degree; ++i) {
    sum *= num;
  }
  return sum;
}

void calculating_degree(int degree, int* res_arr) {
  for (int i = 0; i < 10; ++i) {
    res_arr[i] = exponent(i + 1, degree);
  }
}

void get_result_of_equation(double* array_answ, double discr,
    double a, double b, double c, int i, double eps) {
    if (fabs(discr) < eps) {
      array_answ[i] = err;
      array_answ[i+1] = err;
      return;
    }
    if (a == 0) {
      array_answ[i] = -b/c;
      array_answ[i+1] = -b/c;
      return;
    }

    double square_discr = pow(discr, 1/2);
    array_answ[i]  =  (-b + square_discr)/2*a;
    array_answ[i+1] = (-b - square_discr)/2*a;
    
}

void solve_equation(double first_coeff, double second_coeff, double third_coeff, double* array_answ, double eps) {
    double a = first_coeff;
    double b = second_coeff;
    double c = third_coeff;
    double e = eps;
    double* discr = (double*)malloc(sizeof(double) * 3);
    discr[0] = a*a - 4 * b * c;
    discr[1]= b*b - 4*c*a;
    discr[2] = c*c - 4 * a*b;


    for (int i = 0; i < 6; i += 2) { //&!&&!&!&!&!&!&1!!!
        get_result_of_equation(array_answ, discr[i], a, b, c, i, e);
        double temp = a;
        a = b;
        b = c;
        c = temp;
    }
    
    return;
}

bool is_mult(int first_number, int second_number) {
  if (first_number % second_number == 0) {
    return true;
  }
  return false;
}

bool is_triangle(double a, double b, double c, double eps) {
  if (fabs(a - (b+c)) <= eps ||
      fabs(b - (a+c)) <= eps ||
      fabs(c - (b+a)) <= eps) {
    return false;
  }
  return true;
}

int zero_quantity(double a, double b, double c) {
  int num_of_zero = 0;
  double* arr = (double*)malloc(sizeof(double)*3);
  arr[0] = a; arr[1] = b; arr[2] = c;
  for (int i = 0; i < 3; ++i) {
    if (arr[i] == 0) num_of_zero++;
  }
  return num_of_zero;
}

int main(int argc,char* argv[]) {

  if ((argv[1][0] != '-' && argv[1][0] != '/') || (!is_flag(argv[1][1]))) {
    printf("Please, input flag correctly.\n");
    return 0;
  }

  char flag = argv[1][1];
  double* array_answ;
  int length = 0;
  bool temp;
  char* str;
  long long summ = 0;
  long long mult;

  switch (flag)
  {
  case 'q':
    if (argc != 6) {
    printf("Incorrect num of args for [-q] flag\n");
    return 0;
    }
    array_answ = (double*)malloc(sizeof(double) * 6);
    double epsilon = (double)atof(argv[2]);
    if (epsilon <= 0) {
      printf("Incorrect value of epsilon\n");
      return 0;
    }

    double first_coeff = (double) atof(argv[3]);
    double second_coeff = (double) atof(argv[4]);
    double third_coeff = (double) atof(argv[5]);

    int temp = zero_quantity(first_coeff, second_coeff, third_coeff);
    if (temp == 3) {
      printf("Input correct coefficients.\n");
      return 0;
    } else if (temp == 2) {
      printf("There's only one root - [ 0 ].\n");
      return 0;
    }

    solve_equation(first_coeff, second_coeff, third_coeff, array_answ, epsilon);
    int arr_length = sizeof(array_answ)/sizeof(double);
    for (int i = 0; i < 6; ++i) {
      if (array_answ[i] == err) {
        printf("Incorrect coefficients. Discriminant less than 0.\n");
      } else if (array_answ[i] == -err) {
        printf("Its a default equation. X = %lf\n", array_answ[i]);
      } else {
        printf("[ %.1lf ] - solve of equation\n", array_answ[i]);
      }
      if (i % 2 != 0) printf("\n");
    }
    free(array_answ);
    break;
  case 'm':
    if (argc != 4) {
      printf("Incorrect num of args for [-m] flag\n");
      return 0;
    }
    int first_num, second_num;
    first_num = atoi(argv[2]);
    second_num = atoi(argv[3]);

    if (first_num == 0 || second_num == 0) {
      printf("Incorrect input.\nBoth of number must not be 0.\n");
      return 0;
    }
    if (!is_mult(first_num, second_num)) {
      printf("First number isnt multiple to second.\n");
    } else {
      printf("First number multiple to second.\n");
    }
    break;
  case 't':
    if (argc != 6) {
      printf("Incorrect num of args for [-t] flag\n"); 
      return 0;
    }

    double eps = atof(argv[2]), 
    first_side = atof(argv[3]),
    second_side = atof(argv[4]),
    third_side = atof(argv[5]);

    if (eps <= 0) {
      printf("Incorrect value of epsilon\n");
      return 0;
    }

    bool tmp = is_triangle(first_side, second_side, third_side, eps);
    if (tmp) {
      printf("sides [%.2lf; %.2lf; %.2lf] \ncan be a triangle.\n", first_side, second_side, third_side);
    } else {
      printf("sides [%.2lf; %.2lf;% .2lf] \nCAN NOT be a triangle.\n", first_side, second_side, third_side);
    }
    break;

  default:
    printf("Something went wrong.\n");
    break;
  }
  
  return 0;
}