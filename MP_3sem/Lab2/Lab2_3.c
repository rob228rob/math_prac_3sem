#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_STRING,
    OPEN_FILE_ERROR,
    NO_ONE_FOUND
} status;

typedef struct
{
    int str_number;
    int symbol;
} answer;


void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_STRING) printf("INVALID_STRING.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
}

status is_flag(int symb) {
    char *flags = "lrnuc";
    for (int i = 0; i < 5; ++i) {
        if (flags[i] == symb) {
            return OK;
        }
    } 
    return UNDEFINED_BEHAVIOR;
}

void get_length(char* str, int* result) {
    if (str == NULL) {
        *result = 0;
        return;
    }
    int index = 0;
    while (str[index] != '\0')
    {
        index++;
    }
    *result = index;
}

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

status string_cpy(char* source_str, char* str_cpy, int length) {
    if (str_cpy == NULL || source_str == NULL) {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < length; ++i) {
        str_cpy[i] = source_str[i];
    }
    str_cpy[length] = '\0';
    return OK;
}

status my_strstr(char* buf,char* substr, answer* answ) {
    int length_buf, length_substr;
    get_length(buf, &length_buf);
    get_length(substr, &length_substr);

    for (int i = 0; i <= length_buf - length_substr; ++i) {
        for (int j = 0; j < length_substr; ++j) {
            if (buf[i + j] != substr[j]) {
                break;
            }
            if (j + 1 == length_substr) {
                answ->str_number = -1;
                answ->symbol = i;
                return OK;
            }
        }
    }
    return NO_ONE_FOUND;
}

status find_and_count_substr(char* filename, char* substr, answer** answ, int* answer_length) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }
    char buf[4096];
    int str_count = 0, temp, index = 0;

    (*answ) = (answer*)malloc(sizeof(answer)*1);
    if (*answ == NULL) {
        return MEMORY_ERROR;
    }

    while (fgets(buf, 4096 , file) != NULL)
    {
        str_count++;
        status find_stat = my_strstr(buf, substr, &(*answ)[index]);
        if (find_stat == OK) {
            //printf("!!!!!!!!!!\n");
            answer* temp = (answer*)realloc(*answ, sizeof(answer)*(index+2));
            if (temp == NULL) {
                return MEMORY_ERROR;
            }
            (*answ) = temp;
            (*answ)[index].str_number = str_count;
            index++;    
        }
    }
    *answer_length = index;
    fclose(file);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Need more arguments.\n");
        return 1;
    }
    
    char* substr = argv[1]; 
    for (int i = 2; i < argc; ++i) {
        answer* answ; int answ_len;
        status substr_stat = find_and_count_substr(argv[i], substr, &answ, &answ_len);
        if (substr_stat != OK) {
            responce(substr_stat);
            return 1;
        }

        for (int i = 0; i < answ_len; ++i) {
            printf("%d - str_num; %d - symb\n", answ[i].str_number , answ[i].symbol);
        }
        free(answ);
    }
    


    return 0;
}