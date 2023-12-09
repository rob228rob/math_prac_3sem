#include <stdio.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND,
    ALREADY_EXIST
} STATUS;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
    if (status == ALREADY_EXIST) printf("ALREADY_EXIST.\n");
}

void build_tree(FILE * input, FILE * output)
{
    char c = fgetc(input);
    if(c == '\n' || c == EOF) {
        fprintf(output, "\\_\n");
    }

    int count = 0;
    while (c != '\n' && c != EOF) {
        if(c == '(')
        {
            count++;
        }
        else if(c == ')')
        {
            count--;
        }
        else if (c == ' ' || c == ',')
        {
            c = fgetc(input);
            continue;
        }
        else
        {
            for(int i = 0; i < count * 3; ++i)
            {
                fprintf(output, " ");
            }
            fprintf(output, "\\_%c\n", c);
        }
        c = fgetc(input);
    }
}


int main(int argc, char* argv[]) {
    if(argc != 3) {
        responce(INVALID_DATA);
        return 1;
    }

    FILE * input = fopen(argv[1], "r");
    if (input == NULL) {
        responce(OPEN_FILE_ERROR);
        return 1;
    }

    FILE * output = fopen(argv[2], "w");
    if (output == NULL) {
        fclose(input);
        responce(OPEN_FILE_ERROR);
        return 1;
    }

    int count = 1;
    while (!feof(input))
    {
        fprintf(output, "tree number [%d]\n", count);
        build_tree(input, output);
        fprintf(output, "\n");
        count++;
    }

    printf("tree was created\n");

    fclose(input);
    fclose(output);

    return 0;
}