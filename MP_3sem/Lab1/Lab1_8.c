#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND,
    NOT_A_NUMBER
} STATUS;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
    if (status == NOT_A_NUMBER) printf("NOT_A_NUMBER.\n");
}

STATUS to_upper(char symb, int* res) {
    if (symb >= 'a' && symb <= 'z') {
        *res = symb - 'a' + 'A';
    } else {
        *res = symb;
    }
}

STATUS convert_to_decimal(char number[], int base, int* _decimal) {
    int decimal = 0;
    int power = 1;
    int length = strlen(number);
    if (length > 8) {
        return INVALID_DATA;
    }
    
    for (int i = length - 2; i >= 0; i--) {
        int digit;
        if (number[i] >= '0' && number[i] <= '9') {
            digit = number[i] - '0';
        } else if (number[i] >= 'A' && number[i] <= 'Z') {
            digit = number[i] - 'A' + 11;
        }
        decimal += digit * power;
        power *= base;
    }
    
    *_decimal = decimal;
    return OK;
}

STATUS convert_from_decimal(int decimalNumber, int base, char** num) {
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
        return MEMORY_ERROR;
    }

    strcpy((*num),convertedNumber);
    return OK;
}

void max_of_array(int* array, int length, int* result) {
    int max_val = array[0];
    for (int i = 0; i < length; ++i) {
        max_val = fmax(max_val, array[i]);
    }
    *result = max_val;
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

STATUS find_base_and_write(char *input, char *output) {
    FILE* in = fopen(input, "r");
    if (in == NULL) {
        return OPEN_FILE_ERROR;
    }
    FILE* out = fopen(output, "w");
    if (out == NULL) {
        return OPEN_FILE_ERROR;
    }
    int ch = fgetc(in);
    while ((ch )!= EOF)
    {
        if (ch == ' ' || ch == '\n') {
            ch = fgetc(in);
            continue;
        }

        int capacity = 2, index = 0;
        char* str = (char*)malloc(sizeof(char)*(capacity+1));
        if (str == NULL) {
            return MEMORY_ERROR;
        }
        
        if (ch > '9') {
            to_upper(ch, &ch);
        }
        str[index] = ch;
        while (ch != '\n' && ch != '\n' && ch != ' ')
        {   
            if (ch == '\n' || ch == ' ' || ch == '\t') continue; 
            index++;
            ch = fgetc(in);
            if (ch > '9') {
                to_upper(ch, &ch);
            }
            if (ch == EOF) break;
            if (!isalnum(ch) && ch != '\n' && ch != '\t' && ch != ' ' && ch != EOF)  {
                return INCORRECT_FIELD;
            }
            str[index] = ch;
            if (capacity - index < 2) {
                capacity *= 2;
                char* tmp = (char*)realloc(str, capacity+1);
                if ( tmp == NULL) {
                    return MEMORY_ERROR;
                }
                str = tmp;
            }
        }
        str[strlen(str)] = '\0';
        //printf("%s\n", str);
        int base = number_system(str);
        int decimal;
        int _status = convert_to_decimal(str, base, &decimal);
        if (_status == OK) {
            fprintf(out, "converted: %d; base: %d; current: %s\n", decimal, base, str);
        } else {
            fprintf(out, "too large number: %s\n", str);
        }
        ch = fgetc(in);
        if (ch == '\n' || ch == ' ' || ch == '\t') continue; 
        if (!isalnum(ch) && !(ch == '\n' || ch == '\t' || ch != ' ' ) ) {
            return INCORRECT_FIELD;
        }
    }
    


    fclose(in);
    fclose(out);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Invalid number of arguments.\n");
        return 1;
    }
    char *in = argv[1];
    char *out = argv[2];
    STATUS status = find_base_and_write(in, out);
    if (status != OK) {
        responce(status);
        return 1;
    }
    return 0;
}