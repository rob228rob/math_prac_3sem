#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>

const double err = 1e10;
const int c = 3;

enum statuses {
    ok,
    memory_error,
    invalid_data,
    undefined_behavior,
    only_one_root,
    negative_discr
};

void response(int status) {
    if (status == ok) printf("ok\n");
    else if (status == invalid_data) printf("invalid data\n");
    else if (status == memory_error) printf("memory error\n");
    else if (status == undefined_behavior) printf("undefined behavior\n");
    else if (status == only_one_root) printf("only_one_root\n");
    else if (status == negative_discr) printf("negative_discr\n");
}

bool is_int(const char* str) {
  size_t length = strlen(str);
  for (int i = 0; i < length; ++i) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool is_float(const char* num) {
  size_t length = strlen(num);
  for (int i = 0; i < length; ++i) {
    if ((num[i] < '0' || num[i] > '9') && (num[i] != '.' && num[0] != '-')) {
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


bool is_mult(int first_number, int second_number) {
  if (first_number % second_number == 0) {
    return true;
  }
  return false;
}

bool is_triangle(double a, double b, double c, double eps) {
  double mx = fabs(fmax(a,fmax(b,c)));
  if (fabs(mx - (a+b+c - mx)) < eps && (a >  eps && b > eps && c > eps)) {
    return true;
  }
  return false;
}

enum statuses solve_equation(double a, double b, double c, double eps, double* res1, double* res2) {
  if (a == 0 && c != 0) {
    *res1 = -b/c;
    return only_one_root;
  } else if (a == 0 && c == 0) {
    *res1 = 0;
    return only_one_root;
  }

  double discr = b*b-4*a*c;
  if (discr < eps) {
    return negative_discr;
  } else if (discr - eps == 0) {
    *res1 = -b/(2*a);
    return only_one_root;
  } else {
    double sqrt_d = pow(discr, 1/2) ;
    *res1 = (-b + sqrt_d)/(2 * a) ;
    *res2 = (-b - sqrt_d) /(2 * a) ;
    return ok;
  }
}

int main(int argc,char* argv[]) {

  if ((argv[1][0] != '-' && argv[1][0] != '/') || (!is_flag(argv[1][1]))) {
    printf("Please, input flag correctly.\n");
    return 0;
  }

  char flag = argv[1][1];
  double* array_answ;
  int length = 0;
  long long summ = 0;
  long long mult;

  switch (flag)
  {
  case 'q':
    if (argc != 6) {
      printf("Incorrect num of args for [-q] flag\n");
      return 1;
    }

    double a = (double) atof(argv[3]);
    double b = (double) atof(argv[4]);
    double c = (double) atof(argv[5]);
    if (strlen(argv[2]) > 15 || strlen(argv[3]) > 15 || strlen(argv[4]) > 15 || strlen(argv[5]) > 15) {
      printf("overflowed\n");
      return 1;
    }
    double eps = (double)atof(argv[2]);
    if (eps <= 0) {
      printf("Incorrect eps.\n");
      return 1;
    }
    
    if (!is_float(argv[3]) || !is_float(argv[4]) || !is_float(argv[5]) ) {
      printf("Invalid coeffs. \n");
      return 1;
    }
    
    double* coeff = (double*)malloc(sizeof(double)*3);
    if (coeff == NULL) {
      printf("Allocation memory error.\n");
      return 1;
    }

    coeff[0] = a, coeff[1] = b; coeff[2] = c;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        for (int k = 0; k < 3; ++k) {
          if (i == j || j == k || i == k) {
            continue;
          }
          double res1, res2;     
          enum statuses status = solve_equation(coeff[i], coeff[j], coeff[k], eps, &res1, &res2);
          if ( status == only_one_root) {
            //response(status);
            printf("[ %.3lf ] - once root\n", res1); 
          } else if (status == negative_discr) {
            response(status);
          } else if (status == ok) {
            printf("[ %.3lf ] - first root.\n[ %.3lf ] - second root.\n", res1, res2);
          }
          printf("---------------\n");
        }
      }
    }

    break;
  case 'm':
    if (argc != 4) {
      printf("Incorrect num of args for [-m] flag\n");
      return 0;
    }

    int first_num, second_num;
    if (!is_int(argv[2]) || !is_int(argv[3])) {
      printf("Incorrect input. Number must be integer.\n");
      return 1;
    }

    first_num = atoi(argv[2]);
    second_num = atoi(argv[3]);

    if (first_num == 0 || second_num == 0) {
      printf("Incorrect input. There're must be two correct integer.\n");
      return 0;
    }
    if (!is_mult(first_num, second_num)) {
      printf("First number isnt multiple to second.\n");
    } else {
      printf("[ %d ] multiple to [ %d ].\n", first_num, second_num);
      return 0;
    }

    break;

  case 't':
    if (argc != 6) {
      printf("Incorrect num of args for [-t] flag\n"); 
      return 0;
    }

    char* ptr;
    double epsilon = strtod(argv[2], &ptr), 
    first_side = strtod(argv[3], &ptr),
    second_side = strtod(argv[4], &ptr),
    third_side = strtod(argv[5], &ptr);
    if (!is_float(argv[2]) || !is_float(argv[3]) || !is_float(argv[4]) || 
    !is_float(argv[5])) {
      printf("Incorrect input.\n");
      return 1;
    }

    if (epsilon <= 0) {
      printf("Incorrect value of epsilon.\n");
      return 1;
    }

    bool tmp = is_triangle(first_side, second_side, third_side, epsilon);
    if (tmp) {
      printf("sides [%.2lf; %.2lf; %.2lf] \ncan be a triangle.\n", first_side, second_side, third_side);
    } else {
      printf("sides [%.2lf; %.2lf;% .2lf] \n isn't triangle.\n", first_side, second_side, third_side);
    }
    break;

  default:
    printf("Something went wrong.\n");
    break;
  }
  
  return 0;
}








