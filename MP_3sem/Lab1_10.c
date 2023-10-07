#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>


int convertToDecimal(char number[], int base) {
    int decimalNumber = 0;
    int power = 0;
    int length = strlen(number);
    
    for (int i = length - 1; i >= 0; i--) {
        int digit;
        
        if (number[i] >= '0' && number[i] <= '9') {
            digit = number[i] - '0';
        } else if (number[i] >= 'A' && number[i] <= 'Z') {
            digit = number[i] - 'A' + 10;
        }
        
        decimalNumber += digit * pow(base, power);
        power++;
    }
    
    return decimalNumber;
}

void convertFromDecimal(int decimalNumber, int base, char** num) {
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
        return;
    }

    strcpy((*num),convertedNumber);
}

void max_of_array(int* array, int length, int* result) {
    int max_val = array[0];
    for (int i = 0; i < length; ++i) {
        max_val = fmax(max_val, array[i]);
    }
    *result = max_val;
}


void result_print(int max_val) {
    char* str1,* str2, * str3, * str4;
    convertFromDecimal(max_val, 9, &str1);
    convertFromDecimal(max_val, 18, &str2);
    convertFromDecimal(max_val, 27, &str3);
    convertFromDecimal(max_val, 36, &str4);

    printf("With base 9: %s\n", str1);
    printf("With base 10: %d\n", max_val);
    printf("With base 18: %s\n", str2);
    printf("With base 27: %s\n", str3);
    printf("With base 36: %s\n", str4);
}

int main() {
    int baseFrom, baseTo;
    char terminator[] = "Stop";
    
    printf("Enter the base of the numbers: \n");
    scanf("%d", &baseFrom);
    
    int* arr_abs_value = (int*)malloc(sizeof(int)*1);
    int index = 0;
    while (1)
    {
        //char number =(char*)malloc(sizeof(char)*100);
        char number[100];
        scanf("%s", number);
        if (strcmp(number,terminator) == 0) break;
        int decimalNumber = convertToDecimal(number, baseFrom);
        arr_abs_value[index] = decimalNumber;
        index++;
        arr_abs_value = (int*)realloc(arr_abs_value,(index+1)*sizeof(int));
        //free(number);
    }
    int max_val;
    max_of_array(arr_abs_value, index, &max_val);
    result_print(max_val);
    
    return 0;
}