#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>


typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
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
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
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

int get_length(char* str) {
    if (str == NULL) {
        return 0;
    }
    int index = 0;
    while (str[index] != '\0')
    {
        index++;
    }
    return index;
}

status find_n_count_substr(answer** answ, int* answ_count, char* substr, int count, ...) {
    va_list args;
    va_start(args, count);
    int sub_length = get_length(substr);
    if (count <= 0) {
        return INVALID_DATA;
    }
    int ans_index = 0;
    (*answ) = (answer*)malloc(sizeof(answer));
    if (*answ == NULL) {
        return MEMORY_ERROR;
    }
    //printf("TESTOK\n");
    while (count > 0)
    {
        count--;
        char* filename = va_arg(args, char*);
        FILE* file = fopen(filename, "r");
        if (file == NULL) {
            return OPEN_FILE_ERROR;
        }
        //printf("%s\n", filename);
        int curr_char;
        int found = 0;
        int row = 1;
        int column = 1;
        while ((curr_char = fgetc(file)) != EOF) 
        {
            column++;
            int sub_ind = 0;
            //printf("%c\n", curr_char);
            if (curr_char == '\n') {
                row++;
                column = 1;
            }
            for (int i = 0; i < sub_length; ++i) {
                //curr_char = getc(file);
                if (curr_char == substr[sub_ind]) {
                    //printf("%c - FOUND\n", curr_char);
                    curr_char = fgetc(file);
                    sub_ind++;
                } else {
                    break;
                }
            }
            if (sub_ind == sub_length) {
                (*answ)[ans_index].symbol = column - 1;
                (*answ)[ans_index].str_number = row;
                answer* temp = (answer*)realloc(*answ, sizeof(answer)*(ans_index+2));
                if (temp == NULL) {
                    return MEMORY_ERROR;
                }
                (*answ) = temp;
                ans_index++;

                int arrival = fseek(file, -sub_length, SEEK_CUR);
                //printf("STR FOUND\n");
            } else {
                int arrival = fseek(file, -sub_ind, SEEK_CUR);
            }

            
        }
        fclose(file);

    }
    *answ_count = ans_index;
    va_end(args);
    return OK;
}

int main(int argc, char* argv[]) {
    answer* answ;
    int answ_count;
    char* substr = "ab";

    status find_status = find_n_count_substr(&answ, &answ_count,substr, 1, "1.txt");
    if (find_status != OK) {
        responce(find_status);
        return 1;
    }

    for (int i = 0; i < answ_count; ++i) {
        printf("str [%s] was found.[ line: %d ] [ column: %d ]\n", substr, answ[i].str_number, answ[i].symbol);
    }

    free(answ);

    return 0;
}