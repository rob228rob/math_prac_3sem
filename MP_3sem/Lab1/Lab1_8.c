#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>

enum statuses {
    ok,
    memory_error,
    invalid_data,
    undefined_behavior,
    open_file_error
};


int convertToDecimal(char number[], int base) {
    int decimal = 0;
    int power = 1;
    int length = strlen(number);
    
    for (int i = length - 1; i >= 0; i--) {
        int digit;
        
        if (number[i] >= '0' && number[i] <= '9') {
            digit = number[i] - '0';
        } else if (number[i] >= 'A' && number[i] <= 'Z') {
            digit = number[i] - 'A' + 10;
        }
        decimal += digit * power;
        power *= base;
    }
    
    return decimal;
}

enum statuses convertFromDecimal(int decimalNumber, int base, char** num) {
    char convertedNumber[100];
    int index = 0;
    
    while (decimalNumber != 0) {
        int remainder = decimalNumber % base;
        
        if (remainder >= 0 && remainder <= 9) {
            convertedNumber[index] = remainder + '0';
        } else if (remainder >= 10 && remainder <= 35) {
            convertedNumber[index] = remainder - 10 + 'A';
        }
        
        decimalNumber /= base;
        index++;
    }
    *num = (char*)malloc(sizeof(char)*(index+1));
    
    if (*num == NULL) {
        return memory_error;
    }

    strcpy((*num),convertedNumber);
    return ok;
}

void max_of_array(int* array, int length, int* result) {
    int max_val = array[0];
    for (int i = 0; i < length; ++i) {
        max_val = fmax(max_val, array[i]);
    }
    *result = max_val;
}

char* response(int status) {
    if (status == ok) return "ok";
    else if (status == invalid_data) return "invalid data";
    else if (status == memory_error) return "memory error";
    else if (status == undefined_behavior) return "undefined behavior";
    else if (status == open_file_error) return "open_file_error";
}

enum statuses find_next_num(char* line, int* index, char** num) {
    int start_index = *index;
    while (line[*index] != '\t' && line[*index] != '\n' &&
    line[*index] != ' ' )
    {
        (*index)++;
    }

    (*num) = (char*)malloc((*index - start_index)*sizeof(char) + 1);
    if (*num == NULL) {
        return memory_error;
    }
    while (line[start_index] != '\t' && line[start_index] != '\n' &&
    line[start_index] != ' ' )
    {
        (*num)[start_index] = line[start_index];
        start_index++;
        
    }

    printf("%s.\n", *num);
    return ok;    
}

int number_system(char* num) {
    int max_char = -1;
    bool hasChar = false;
    for (int i = 0; i < strlen(num); ++i) {
        if (num[i] >= 'a' && num[i] <= 'z') {
            max_char = fmax(max_char, num[i] - 'a' + 11);
            hasChar = true;
        }
        if (num[i] >= 'A' && num[i] <= 'Z') {
            max_char = fmax(max_char, num[i] - 'A' + 11);
            hasChar = true;
        }
        if (num[i] >= '0' && num[i] <= '9') {
            max_char = fmax(max_char, num[i] - '0' + 1);
        }
    }

    if (hasChar) {
        return fmin(max_char, 36);
    } else {
        return fmax(2, max_char);
    }
}


enum statuses write_num_to_file(char* filename, char* num) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return open_file_error;
    }
    int base = number_system(num);
    fprintf(file, "%s %d %d", num, base,convertToDecimal(num, base));
    fclose(file);
    return ok;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalig number of arguments.\n");
        return 1;
    }

    char* output_file_name = (char*)malloc(1 + sizeof(char)*strlen(argv[2]));
    char* input_file_name = (char*)malloc(1 + sizeof(char)*strlen(argv[1]));
    strcpy(input_file_name, argv[1]);
    strcpy(output_file_name, argv[2]);
    if (input_file_name == NULL || output_file_name == NULL) {
        printf("Memory allocation error.\n");
        return 1;
    }

    FILE* input_file = fopen(input_file_name, "r");
    if (input_file == NULL) {
        printf("File can't open\n");
        return 1;
    }

    char* line = (char*)calloc(512, sizeof(char));
    if (line == NULL) {
        fclose(input_file);
        printf("Memory allocation error.\n");
        return 1;
    }
    while (fgets(line, 512-1, input_file)) 
    {
        int index = 0;
        int last_index = 0;
        while (last_index < strlen(line))
        {
            char* num;
            enum statuses status1 = find_next_num(line, &index, &num);
            if (status1 != ok) {
                printf("%s.\n", response(status1));
            }
            enum statuses status2 =  write_num_to_file(output_file_name, num);
            if (status2 != ok) {
                printf("%s.\n", response(status2));
            }
            last_index += index;
            free(num);
        } 
        free(line);
        line = (char*)calloc(512,sizeof(char));
        if (line == NULL) {
            fclose(input_file);
            printf("line can't be readed.\n");
            return 1;
        }
    }
    free(line);
    fclose(input_file);
}