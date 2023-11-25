#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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


int is_final(double epsilon, int system, char*** results, int amount, ...)
{
    va_list args;
    va_start(args, amount);
    *results = (char**)malloc(sizeof(char*) * amount);
    if( *results == NULL) {
        va_end(args);
        return MEMORY_ERROR;
    }
    double number;

    for(int i = 0; i < amount; i++)
    {
        int length = 2;
        int capacity = 4;
        (*results)[i] = (char*)malloc(sizeof(char)*(length+1));
        if((*results)[i] == NULL)
        {
            for(int j = 0; j <= i; j++)
            {
                free((*results)[j]);
            }
            free(*results);
            va_end(args);
            return MEMORY_ERROR;
        }
        (*results)[i][0] = '0';
        (*results)[i][1] = '.';
        (*results)[i][2] = '\0';
        number = va_arg(args, double);
        double fraction = number;
        int itter_limit = 50;
        int iterations = 0;
        while(fraction > epsilon)
        {
            if(iterations == itter_limit) {
                strcpy((*results)[i], "Could not be a final representative.\0");
                break;
            }
            length++;
            if(capacity == length+1) {
                capacity *= 2;
                char* tmp = (char*)realloc((*results)[i], sizeof(char)*capacity);
                if (tmp == NULL)
                {
                    free(*results);
                    va_end(args);
                    return MEMORY_ERROR;
                }
                (*results)[i] = tmp;
            }
            fraction *= system;
            int digit = (int)fraction;
            fraction -= (int)fraction;
            (*results)[i][length-1] = (digit < 10) ? digit + '0' : digit + 'A' - 10;
            (*results)[i][length] = '\0';
            iterations++;
        }
    }

    va_end(args);
    return OK;
}

int main() {
    char** results;
    double eps = 1e-3;
    int base = 10; 
    int amount = 4;
    STATUS status_code = is_final(eps, base , &results, amount, 0.60004883632064, 0.020408093929291, 0.6711110475328233506580634, 0.357);
    if (status_code != OK) {
        free(results);
        responce(status_code);
        return status_code;
    }
    for (int i = 0; i < amount; i++) {
        printf("%s\n", results[i]);
    }
    for(int j = 0; j < amount; j++) {
        free((results)[j]);
    }
    free(results);

    return 0;
}
