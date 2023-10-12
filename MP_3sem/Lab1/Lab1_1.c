#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>


bool is_num(const char* str) {
  int length = strlen(str);
  for (int i = 0; i < length; ++i) {
    if (!isdigit(str[i])) {
      return false;
    }
  }
  return true;
}

bool is_flag(char str) {
  if (str == 'p' ||  str == 's' || str == 'h' || 
    str == 'e' || str == 'a' || str == 'f') {
    return true;
  }
  return false;
}

void get_multiples_nums(int current_num, int** array_answ) {
  if (current_num > 100 || current_num <= 0) {
    (*array_answ) = NULL;
    return;
  }
  if (current_num > 50) {
    (*array_answ) = NULL;
    return;
  }
  (*array_answ) = (int*)malloc(sizeof(int)*(100/current_num + 1));
  int index = 0;
  for (int i = current_num + current_num; i <= 100; i += current_num) {
    (*array_answ)[index] = i;
    index++;
  }
}

void print_array(int* array_answ, int length) {
  if (array_answ == NULL) {
    return;
  }

  for (int i = 0; i < length; ++i) {
    if (1) {
      printf("%d ", array_answ[i]);
    }
  }
  printf("\n");
}

bool is_prime(int current_num) {
  for (int i = 2; i * i < current_num; ++i) {
    if (current_num % i == 0) {
      return false;
    }
  }
  return true;
}

void cut_number(char string_num[], int** array_answ, int* count) {
  int length = strlen(string_num);
  int start_ind = 0;
  if (length == 1) {
    (*array_answ)[0] = string_num[0] - '0';
    *count = 1;
    return;
  }
  
  bool has_zero = true;
  for (int i = 0; i < length; ++i) {
    if (string_num[i] != '0') {
      break;
    }
    if (string_num[i] == '0' && has_zero) {
      start_ind++;
    }
    else {
      has_zero = false;
    }
  }
  *count = length - start_ind;
  (*array_answ) = (int*)malloc(sizeof(int) * length-start_ind);

  for (int i = start_ind; i < length; ++i) {
    (*array_answ)[i-start_ind] = string_num[i] - '0';
  }
}

int sum_to_num(int current_num) {
  if (current_num > 65500) {
    return -1;
  }
 
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
  int* result_arr;
  current_num++;
  for (int i = 1; i < current_num; ++i) {
    result_arr = (int*)calloc(10, sizeof(int));
    calculating_degree(i, result_arr);
    printf("degree:\t1\t2\t3\t4\t5\t6\t7\t8\t9\t10\n");
    printf("value:\t");
    for (int i = 0; i < 10;++i) {
      if (i != 9) {
        printf("%d\t", result_arr[i]);
      } else {
        printf("%d\n", result_arr[i]);
      }
    }
    printf("\n");

  }
  free(result_arr);
  return;
}

int main(int argc,char* argv[]) {
  if (argc != 3) {
    printf("Incorrect num of args.\n");
    return 0;
  }

  int current_num;
  char flag;
  char flag_component = argv[1][0]; 
  char* string_num;

  if (!isdigit(flag_component)) {
    flag_component = argv[1][0];
    flag = argv[1][1];
    current_num = atoi(argv[2]);
    string_num = (char*)malloc(sizeof(char)*strlen(argv[2]) + 1);
    strcpy(string_num, argv[2]);
  } else {
    flag_component = argv[2][0];
      flag = argv[2][1];
      current_num = atoi(argv[1]);
      string_num = (char*)malloc(sizeof(char)*strlen(argv[2]) + 1);
      strcpy(string_num, argv[2]);
  }
  
  if ((flag_component != '-' && flag_component != '/') || (!is_flag(flag))) {
    printf("Incorrect flag.\n");
    return 0;
  }

  if (!is_num(string_num)) {
    printf("Incorrect number.\n");
    return 0;
  }

  int* array_answ;
  int length = 0;
  bool temp;
  char* str;
  long long summ = 0;
  long long mult;

  switch (flag)
  {
  case 'h':
    get_multiples_nums(current_num, &array_answ);
    if (array_answ != NULL) {
      print_array(array_answ, 100/current_num + 1);
    } else {
      printf("There's not any number multiples to current.\n");
      free(array_answ);
      free(string_num);
      return 1;
    }

    break;

  case 'p':
    if (current_num <= 1) {
      printf("Incorrect number for this flag.\n");
      free(string_num);
      return 1;
    }
    temp = is_prime(current_num);
    if (temp) {
      printf("%d - is prime.\n", current_num);
    }
    else {
      printf("%d - isnt prime.\n", current_num);
    }
    break;
  case 's':
    cut_number(string_num, &array_answ, &length);
    if (array_answ != NULL) {
      print_array(array_answ, length);
    } else {
      printf("Something went wrong.\n");
    }
    free(array_answ);
    free(string_num);
    break;
  case 'e':
    if (current_num > 10 || current_num < 2) {
      printf("Invalid number.\n");
      return 0;
    }
    make_table(current_num);
    free(string_num);
    break;
  case 'a':
    summ = sum_to_num(current_num);
    if (summ != -1) {
      printf("Too large number.\n");
      free(string_num);
      return 1;
    }
    printf("%lld - sum up to number.\n", summ);
    free(string_num);
    break;
  case 'f':
    if (current_num >= 20 || current_num < 0) {
      printf("Isnt valid num for this flag.\n");
      free(string_num);
      return 1;
    }
    mult = fact(current_num);
    printf("%lld - factorial of %d.\n", mult, current_num);
    break;
  default:
    break;
  }
  
  return 0;
}