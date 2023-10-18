#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_STRING,
} status;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_STRING) printf("INVALID_STRING.\n");
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

status get_length(char* str, int* result) {
    if (str == NULL) {
        *result = 0;
        return OK;
    }
    int index = 0;
    while (str[index] != '\0')
    {
        index++;
    }
    *result = index;

    return OK;
}

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

status string_reverse(char* source_string, char* result_string) {
    int length = 0;
    status temp = get_length(source_string, &length);
    if (temp != OK) {
        return temp;
    }

    int start = 0, end = length - 1;
    while (start < end) {
        swap(&result_string[start], &result_string[end]);
        start++;
        end--;
    }
    
    return OK;

}

status to_upper_string(char** result_str, char* source_str) {
    int length = 0;
    status source_length = get_length(source_str, &length);
    if (source_length != OK) {
        return source_length;
    }

    (*result_str) = (char*)malloc(length*sizeof(char) + 1);
    if (*result_str == NULL) {
        return MEMORY_ERROR;
    }

    (*result_str)[length] = '\0';
    for (int i = 0; i < length; ++i) {
        (*result_str)[i] = source_str[i];

        if (i % 2 != 0 && source_str[i] >= 'a' ) {
            if ((source_str)[i] < 'A' || (source_str[i] > 'z')) {
                return INVALID_STRING;
            }
            (*result_str)[i] += 'A'-'a';
        }
    }
    return OK;
}

status sorted_string(char* const source_str, char** result_str) {
    int length = 0;
    status source_length = get_length(source_str, &length);
    if (source_length != OK) {
        return source_length;
    }

    (*result_str) = (char*)malloc(length*sizeof(char) + 1);
    if (*result_str == NULL) {
        return MEMORY_ERROR;
    }
    (*result_str)[length] = '\0';

    int index = 0;
    for (int i = 0; i < length; ++i) {
        if (source_str[i] >= '0' && source_str[i] <= '9') {
            (*result_str)[index] = source_str[i];
            source_str[i] = ' ';
            index++;
        }
    }

    for (int i = 0; i < length; ++i) {
        if ( (source_str[i] >= 'a' && source_str[i] <= 'z') || (source_str[i] >= 'A' && source_str[i] <= 'Z')) {
            (*result_str)[index] = source_str[i];
            source_str[i] = ' ';
            index++;
        }
    }

    for (int i = 0; i < length; ++i) {
        if (source_str[i] != ' ') {
            (*result_str)[index] = source_str[i];
            index++;
        }
    }

    return OK;
}

status string_cpy(char* source_str, char* source_cpy, int length) {
    if (source_cpy == NULL || source_str == NULL) {
        return MEMORY_ERROR;
    }
    for (int i = 0; i < length; ++i) {
        source_cpy[i] = source_str[i];
    }
    source_cpy[length] = '\0';
    return OK;
}

status concatenate_string(char* source_str, char* dest_str) {
    int length_src = 0;

    status source_length = get_length((source_str), &length_src);
    if (source_length != OK) {
        return source_length;
    }

    int length_dst = 0;
    status dst_length = get_length(dest_str, &length_dst);
    if (dst_length != OK) {
        return dst_length;
    }


    char* source_cpy = (char*)malloc(sizeof(char)*(length_src + 1));
    if (source_cpy == NULL) {
        return MEMORY_ERROR;
    }
    source_cpy[length_src] = '\0';

    status str_cpy = string_cpy(source_str, source_cpy, length_src);
    if (str_cpy != OK) {
        return str_cpy;
    }
   
    int new_length = length_dst + length_src;
    int i = 0;
    while (i < length_src) {
        (source_str)[i] = source_cpy[i];
        i++;
    }
    int j = 0;
    while (i < new_length) {
        (source_str)[i] = dest_str[j];
        i++;
        j++;
    }

    return OK;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }
    char* flag = argv[1];
    int flag_len = 0;
    status length_stat = get_length(argv[1], &flag_len);
    if (length_stat != OK) {
        responce(length_stat);
        return 1;
    }

    if (is_flag(flag[1]) != OK || flag_len != 2) {
        printf("Incorrect flag.\n");
        return 1;
    }
    char* result_str;

    if (flag[1] == 'l') {
        int result = 0;
        status string_length = get_length(argv[2], &result);
        if (string_length != OK) {
            responce(string_length);
            return 1;
        }
        printf("%d - current length of string %s.\n", result, argv[2]);
        free(result_str);
    } 
    else if (flag[1] == 'r') {
        int length;
        status length_status = get_length(argv[2], &length);

        result_str = argv[2];

        status reverse_status = string_reverse(argv[2], result_str);
        if (reverse_status != OK) {
            
            responce(reverse_status);
            return 1;
        }
        int st = get_length(result_str,&length);

        printf("%s - reversed string.\n",result_str);

    } 
    else if (flag[1] == 'u') {
        status get_new_str = to_upper_string(&result_str, argv[2]);
        if (get_new_str != OK) {
            responce(get_new_str);
            return 1;
        }
        printf("%s - new str.\n", result_str);
        free(result_str);
    }
    else if (flag[1] == 'n') {
        status sort = sorted_string(argv[2], &result_str);
        if (sort != OK) {
            responce(sort);
            return 1;
        }
        printf("%s - sorted string.\n", result_str);
        free(result_str);
    } 
    else if (flag[1] == 'c') {
        if (argc < 4) {
            printf("Incorrect number of arguments for [ c ] flag.\n");
        }
        srand(atoi(argv[2]));
        int index = 2;
        while (index + 1 < argc)
        {
            index++;
            if (rand() % 100 > 50) {
                continue;
            } else {
            char* dst_str = argv[index];
            int new_length, dst_length;
            status len_stat = get_length(result_str, &new_length);
            status dst_len = get_length(dst_str, &dst_length);
            if (new_length == 0) {
                result_str = (char*)malloc(sizeof(char)*(dst_length + 1));
                if (result_str == NULL) {
                    printf("Memory allocation error.\n");
                    return 1;
                }
                result_str[dst_length] = '\0';
            } else {
                char* temp_ptr = (char*)realloc(result_str, sizeof(char)*(new_length + dst_length + 1));
                if (temp_ptr == NULL) {
                    printf("Memory allocation error.\n");
                    return 1;
                }
                result_str = temp_ptr;
                result_str[dst_length + new_length] = '\0';
            }
            status concat_str = concatenate_string(result_str, dst_str);
            if (concat_str != OK) {
                responce(concat_str);
                return 1;
            }
            //argv[index][1] = 'F';
            printf("%s - res str\n", result_str);
            }

        }
    }
    free(result_str);

    return 0;
}