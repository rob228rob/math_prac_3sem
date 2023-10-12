#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>


enum statuses {
    ok,
    memory_error,
    invalid_data,
    undefined_behavior,
    open_file_error,
    write_to_file_error
};

char* response(int status) {
    if (status == ok) return "ok";
    else if (status == invalid_data) return "invalid data";
    else if (status == memory_error) return "memory error";
    else if (status == undefined_behavior) return "undefined behavior";
    else if (status == open_file_error) return "open_file_error";
    else if (status == write_to_file_error) return "write_to_file_error";
}

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
  if (str == 'd' ||  str == 's' || str == 'i' || str == 'a') {
    return true;
  }
  return false;
}

void print_array(int* array_answ, int length) {
  if (array_answ == NULL) {
    return;
  }

  for (int i = 0; i < length; ++i) {
    if (array_answ[i] != 0) {
      printf("%d \n", array_answ[i]);
    }

  }
}

enum statuses write_to_file(char* file_name, char flag, char* line) {
    char output_file_name[4+strlen(file_name)];
    char pattern[] = "out_";
    printf("5.-1\n");
    strcat(pattern, file_name);

    FILE* file = fopen("output", "w");
    if (file == NULL) {
      return write_to_file_error;
    }
    fputs(line, file);
    printf("5.0\n");
    if (line == NULL) {
      printf("errrrrr\n");
      return memory_error;
    }

    switch (flag)
    {
    case 'i':

    case 's':

    break;
    case 'd':
        printf("ok5.1\n");
        //fputs(line, file);
        //fprintf(file, "%s", line);
        printf("ok5.2\n");
        break;
    break;
    case 'a':

    break;
    default:
      
      break;
    }
    fclose(file);

    printf("ok5.3\n");
    
    return ok;
}


enum statuses delete_arabic_numerals(char* input_file_name, char* line, char flag) {
    
    for (int i = 0; i < strlen(line); ++i) {
        if (line[i] >= '0' || line[i] <= '9') {
            line[i] = ' ';
        }
    }
    printf("ok3.1\n");
    enum statuses temp = write_to_file(input_file_name, flag, line);

    printf("ok3.2\n");
    if (temp != ok) {
      
        return memory_error;
    }
    printf("ok3.3\n");
    return ok;
}

int main(int argc, char* argv[]) {
    
    char* input_file_name = (char*)malloc(sizeof(char)*strlen(argv[2]) + 1);
    
    if (input_file_name == NULL) {
        printf("%s\n", response(memory_error));
        return 1;
    }

    strcpy(input_file_name, argv[2]);
    
    char flag = argv[1][1];
    char flag_char;
    char flag_component = argv[1][0]; 
    
    if (flag == 'n') {
        //output_file_name = (char*)malloc(sizeof(char)*strlen(argv[2]) + 1);
        //strcpy(output_file_name, argv[2]);
        if ((flag_component != '-' && flag_component != '/') || (!is_flag(flag)) ) {
          printf("Incorrect flag.\n");
        return 1;
        }
        flag_char = flag;
    } else {
        //output_file_name = (char*)malloc(sizeof(char)*(strlen(argv[2])) + 1);
        //strcpy(output_file_name, argv[2]);
        if ((flag_component != '-' && flag_component != '/') || (!is_flag(flag)) ) {
          printf("Incorrect flag.\n");
        return 1;
        }
        flag_char = flag;
    }

    if (argc  > 4) {
    printf("Incorrect num of args.\n");
    return 0;
    }
    
    FILE* file = fopen(input_file_name, "r");
    if (file == NULL) {
      printf("File cant open.\n");
      return 1;
    }

    printf("ok\n");
    switch (flag)
    {
    case 'i':

    break;
    case 'd':
      char  buffer[128];
      printf("ok1\n");
      while (fgets(buffer, 128, file) != NULL)
      {
          printf("ok2\n");
          enum statuses status = delete_arabic_numerals(input_file_name, buffer, flag_char);
          if (status != ok) {
              printf("%s vbhnj\n", response(status));
              fclose(file);
              return 1;
          }
          //free(buffer);
          //buffer = (char*)malloc(129*sizeof(char));
      }
      printf("last\n");
    
    
    break;
    case 's':

    break;
    case 'a':

    break;

    default:
    break;
    }


    return 0;
}