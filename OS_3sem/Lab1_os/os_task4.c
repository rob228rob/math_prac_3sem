#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD
} STATUS;

void response(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
}


int convert_str_to_int (const char *str, unsigned int * result, int base)
{
    char *endptr;
    errno = 0;
    *result = strtol(str, &endptr, base);

    if (errno == ERANGE && *result == UINT_MAX)
    {
        return INVALID_DATA;
    } else if (errno != 0 && *result == 0) {
        return INVALID_DATA;
    } else if (*endptr != '\0') {
        return INVALID_DATA;
    }

    return OK;
}

int xor8_file(FILE* input, unsigned int * result)
{
    *result = 0;
    unsigned char c;
    while(fread(&c, sizeof(unsigned char), sizeof(c), input))
    {
        *result ^= c;
    }

    return OK;
}

int xor32_file(FILE* input, unsigned char ** group, size_t size_group)
{
    for(size_t i = 0; i < size_group; ++i)
    {
        (*group)[i] = 0;
    }
    size_t size_buffer = size_group;
    unsigned char* buffer = (unsigned char *)malloc(sizeof(unsigned char) * size_buffer);
    if(buffer == NULL)
    {
        return MEMORY_ERROR;
    }
    for(size_t i = 0; i < size_buffer; ++i)
    {
        buffer[i] = 0;
    }
    while( fread(buffer, sizeof(unsigned char), size_buffer, input) > 0)
    {
        for(size_t i = 0; i < size_group; ++i)
        {
            (*group)[i] ^= buffer[i];
        }
    }
    if(ferror(input))
    {
        free(buffer);
        return OPEN_FILE_ERROR;
    }
    free(buffer);
    return OK;
}

int count_xor_mask_file(FILE* input, unsigned int mask, unsigned int number, int * count_result)
{
    *count_result = 0;
    number = 0;
    while(fread(&number, sizeof(unsigned int), 1, input) > 0)
    {
        if((number & mask) > 0)
        {
            (*count_result)++;
        }
    }
    if(ferror(input))
    {
        return OPEN_FILE_ERROR;
    }
    return OK;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        response(INVALID_DATA);
        return INVALID_DATA;
    }

    FILE * input = fopen(argv[1], "rb");
    if(input == NULL)
    {
        response(OPEN_FILE_ERROR);
        return OPEN_FILE_ERROR;
    }
    if(strcmp(argv[2], "xor8") == 0)
    {
        if(argc != 3)
        {
            response(INVALID_DATA);
            fclose(input);
            return INVALID_DATA;
        }
        unsigned int result_xor8;
        if (xor8_file(input, &result_xor8) != OK)
        {
            response(OPEN_FILE_ERROR);
            fclose(input);
            return OPEN_FILE_ERROR;
        }
        printf("xor8 = %d", result_xor8);
    }
    else if(strcmp(argv[2], "xor32") == 0)
    {
        if(argc != 3)
        {
            response(INVALID_DATA);
            fclose(input);
            return INVALID_DATA;
        }
        size_t size_group = 4;
        unsigned char* group = (unsigned char *)malloc(sizeof(unsigned char) * size_group);
        if(group == NULL)
        {
            fclose(input);
            return MEMORY_ERROR;
        }
        if (xor32_file(input, &group, size_group) != OK)
        {
            response(OPEN_FILE_ERROR);
            fclose(input);
            free(group);
            return OPEN_FILE_ERROR;
        }
        printf("xor8 = ");
        for(size_t i = 0; i < size_group; ++i)
        {
            printf("%u ", group[i]);
        }
        printf("\n");
        free(group);
    }
    else if (strcmp(argv[2], "mask") == 0)
    {
        if(argc != 4)
        {
            response(INVALID_DATA);
            fclose(input);
            return INVALID_DATA;
        }
        unsigned int mask;
        if(convert_str_to_int(argv[3],&mask, 16) != OK)
        {
            response(INVALID_DATA);
            fclose(input);
            return INVALID_DATA;
        }
        int count_result;
        unsigned int number = 0;
        if(count_xor_mask_file(input, mask, number, &count_result) != OK)
        {
            response(OPEN_FILE_ERROR);
            fclose(input);
            return OPEN_FILE_ERROR;
        }
        printf("количество групп в файле, которые соответствуют маске %s: %u\n", argv[3],  count_result);
    }
    else
    {
        response(INVALID_DATA);
        fclose(input);
        return INVALID_DATA;
    }

    if(input != NULL) fclose(input);

    return 0;
}