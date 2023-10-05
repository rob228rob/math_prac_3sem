#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

const double err = 1e10;

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
  if (str == 'q' ||  str == 'p' || str == 'm') {
    return true;
  }
  return false;
}

void get_multiples_nums(int current_num, int* array_answ, int* count) {
  if (current_num > 100) {
    array_answ = NULL;
    return;
  }
  array_answ = (int*)malloc(sizeof(int) * (100 / current_num + 1));
  int index = 0;
  for (int i = current_num; i <= 100; i += current_num) {
    array_answ[index] = i;
    index++;
  }
  *count = index;
  
}

void print_array(int* array_answ, int length) {
  for (int i = 0; i < length; ++i) {
    printf("%d \n", array_answ[i]);
  }
}

bool is_prime(int current_num) {

  for (int i = 2; i * i < current_num; ++i) {
    if (current_num % i == 0) {
      return false;
    }
  }
  return true;
}

void cut_number(char string_num[], int* array_answ, int* count) {
  int length = strlen(string_num);
  int start_ind = 0;
  if (length == 1) {
    array_answ[0] = atoi(string_num);
    return;
  }
  
  bool has_zero = true;
  for (int i = 0; i < strlen(string_num); ++i) {
    if (string_num[i] == '0' && has_zero) {
      start_ind++;
    }
    else {
      has_zero = false;
    }
  }
  *count = length - start_ind;
  array_answ = (int*)malloc(sizeof(int) * length-start_ind);
  for (int i = start_ind; i < length; ++i) {
    char str[2];
    str[0] = string_num[i];
    str[1] = '\0';
    array_answ[i-start_ind] =atoi(str);
  }
}

int sum_to_num(int current_num) {
  return (double)(1 + current_num) / 2 * current_num;
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

void make_table(int current_num) {
  for (int i = 1; i < current_num; ++i) {
    int* result_arr = (int*)malloc(sizeof(int)*10);
    calculating_degree(i, result_arr);
    print_array(result_arr, sizeof(result_arr)/sizeof(int));
  }
}

void get_result_of_equation(double* array_answ, double discr,
    double a, double b, double c, int i) {
    if (discr < 0) {
      array_answ[i] = err;
      array_answ[i+1] = err;
      return;
    }
    if (a == 0) {
      array_answ[i] = -b/c;
      array_answ[i+1] = -err;
      return;
    }

    double square_discr = pow(discr, 1/2);
    array_answ[i]  =  (-b + square_discr)/2*a;
    array_answ[i+1] = (-b - square_discr)/2*a;
    
}

void solve_equation(double first_coeff,double second_coeff, double third_coeff, double* array_answ) {
    double a = first_coeff;
    double b = second_coeff;
    double c = third_coeff;
    double* discr = (double*)malloc(sizeof(double) * 3);
    discr[0] = a*a - 4 * b * c;
    discr[1]= b*b - 4*c*a;
    discr[2] = c*c - 4 * a*b;


    for (int i = 0; i < 6; i += 2) { //&!&&!&!&!&!&!&1!!!
        get_result_of_equation(array_answ, discr[i], a, b, c, i);
        double temp = a;
        a = b;
        b = c;
        c = temp;
    }
    

    return;
}

int main(int argc,char* argv[]) {

  if ((argv[1][0] != '-' && argv[1][0] != '/') || (!is_flag(argv[1][1]))) {
    printf("Please, input flag correctly.\n");
    return 0;
  }

  char flag = argv[1][1];
  //int current_num = atoi(argv[1]);
  //char string_num[] = argv[1];
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
    double first_coeff = (double) atof(argv[3]);
    double second_coeff = (double) atof(argv[4]);
    double third_coeff = (double) atof(argv[5]);

    solve_equation(first_coeff, second_coeff, third_coeff, array_answ);
    int arr_length = sizeof(array_answ)/sizeof(double);
    for (int i = 0; i < 6; ++i) {
      if (array_answ[i] == err) {
        printf("Incorrect coefficienrs. Discriminant less than 0.\n");
      } else if (array_answ[i] == -err) {
        printf("Its a default equation. X = %lf\n", array_answ[i]);
      }
        printf("[ %lf ] - solve of equation\n", array_answ[i]);
    }
    free(array_answ);
    break;
  case 'm':
    if (argc != 4) {
    printf("Incorrect num of args for [-m] flag\n");
    return 0;
    }
    break;
  case 't':
    if (argc != 5) {
    printf("Incorrect num of args for [-t] flag\n"); 
    }
    return 0;
    break;

  default:
    break;
  }

  
  return 0;
}