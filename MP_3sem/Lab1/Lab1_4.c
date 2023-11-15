#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <ctype.h>
#include <stdlib.h>


typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND
} STATUS;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
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

STATUS generate_name(char** filename, char* old_name) {
  *filename = (char*)malloc((strlen(old_name) + 5) * sizeof(char));
  if (*filename == NULL) {
    return MEMORY_ERROR;
  }
  for (int i = 4; i < strlen(old_name) + 4; ++i) {
    (*filename)[i] = old_name[i-4];
  }
  (*filename)[0] = 'o';
  (*filename)[1] = 'u';
  (*filename)[2] = 't';
  (*filename)[3] = '_';
  (*filename)[strlen(old_name) + 4] = '\0';

  return OK;
}

STATUS delete_arabic_numerals(char* input_filename, char* output_filename) {
  if (input_filename == NULL || output_filename == NULL) {
    return INVALID_DATA;
  }
  FILE* in = fopen(input_filename, "r");
  if (in == NULL) {
    return OPEN_FILE_ERROR;
  }
  FILE* out = fopen(output_filename, "w");
  if (out == NULL) {
    fclose(in);
    return OPEN_FILE_ERROR;
  }
  int ch = fgetc(in);
  while (1)
  {
    if (isdigit(ch)) {
      fputc(' ', out);
    } else {
      fputc(ch, out);
    }
    ch = fgetc(in);
    if (ch == '\n') {
      continue;
    }
    if (ch == EOF) {
      break;
    }
  }
  fclose(in);
  fclose(out);
  return OK;   
}

STATUS count_latin_letters(char* input_filename, char* output_filename)
{
  if (input_filename == NULL || output_filename == NULL) {
    return INVALID_DATA;
  }
  FILE* in = fopen(input_filename, "r");
  if (in == NULL) {
    return OPEN_FILE_ERROR;
  }
  FILE* out = fopen(output_filename, "w");
  if (out == NULL) {
    fclose(in);
    return OPEN_FILE_ERROR;
  }
    int count = 0;
    int c = 0;
    while((c = fgetc(in)))
    {
        if((c >= 'a' && c <= 'z') || (c >='A' && c <= 'Z')) count ++;
        else if(c == '\n' || c==EOF)
        {
            if(fprintf(out, "%d\n", count) == EOF) return UNDEFINED_BEHAVIOR;
            count = 0;
        }
        if(c == EOF) break;
    }
    fclose(in);
    fclose(out);
    return OK;
}

STATUS count_extra_symbols(char* input_filename, char* output_filename)
{  
  if (input_filename == NULL || output_filename == NULL) {
    return INVALID_DATA;
  }
  FILE* in = fopen(input_filename, "r");
  if (in == NULL) {
    return OPEN_FILE_ERROR;
  }
  FILE* out = fopen(output_filename, "w");
  if (out == NULL) {
    fclose(in);
    return OPEN_FILE_ERROR;
  }
    int count = 0;
    int c;
    do
    {
        c = fgetc(in);
        if(!isalnum(c))
        {        
        count++;
        if(c == '\n' || c==EOF)
        {
            fprintf(out, "%d\n", count - 1);
            count = 0;
        }
        }
    } while(c != EOF);

    fclose(in);
    fclose(out);
    return OK;
} 

STATUS convert_into_ascii(char* input_filename, char* output_filename)
{
  if (input_filename == NULL || output_filename == NULL) {
    return INVALID_DATA;
  }
  FILE* in = fopen(input_filename, "r");
  if (in == NULL) {
    return OPEN_FILE_ERROR;
  }
  FILE* out = fopen(output_filename, "w");
  if (out == NULL) {
    fclose(in);
    return OPEN_FILE_ERROR;
  }
    int c;
    while((c = fgetc(in)) != EOF)
    {
        if(!(c >= '0' && c <= '9'))
        {
            if(c == '\n' || c == EOF)
            {
                if(fprintf(out, "\n") == EOF) return UNDEFINED_BEHAVIOR;
            }
            else
            {
                if(fprintf(out, "%X", c) == EOF) return UNDEFINED_BEHAVIOR;
            }
        }
        else
        {
            if(fprintf(out, "%c", c) == EOF) return UNDEFINED_BEHAVIOR;
        }
    }
    fclose(in);
    fclose(out);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) {
      printf("Incorrect num of args.\n");
      return 1;
    }
    char *output;
    int changed = false;
    if (argc == 3) {
      generate_name(&output, argv[2]);
      changed = true;
    } else if (argc == 4) {
      output = argv[3];
    }
    //printf("%s %s\n", output, argv[2]);
    
    char flag = argv[1][1];
    char flag_char;
    char flag_component = argv[1][0]; 
    if (flag == 'n') {
        if ((flag_component != '-' && flag_component != '/') || (!is_flag(flag)) ) {
          printf("Incorrect flag.\n");
          if (changed) free(output); 
          return 1;
        }
        flag_char = flag;
    } else {
        if ((flag_component != '-' && flag_component != '/') || (!is_flag(flag)) ) {
          printf("Incorrect flag.\n");
          if (changed) free(output); 
          return 1;
        }
        flag_char = flag;
    }

    switch (flag)
    {
    case 'i':
      STATUS status1 = count_latin_letters(argv[2], output);
      if (status1 != OK) {
        responce(status1);
        if (changed) free(output); 
        return 1;
      }
    break;
    case 'd':
      printf("-d fl\n");
      STATUS status = delete_arabic_numerals(argv[2], output);
      if (status != OK) {
        responce(status);
        if (changed) free(output); 
        return 1;
      }
    break;
    case 's':
      STATUS status2 = count_extra_symbols(argv[2], output);
      if (status2 != OK) {
        responce(status2);
        if (changed) free(output); 
        return 1;
      }
    break;
    case 'a':
      STATUS status3 = convert_into_ascii(argv[2], output);
      if (status3 != OK) {
        responce(status3);
        if (changed) free(output); 
        return 1;
      }
    break;

    default:
    break;
    }


    return 0;
}