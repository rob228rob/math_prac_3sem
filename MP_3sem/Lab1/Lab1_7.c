#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include<stdlib.h>

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

STATUS validation(char** str)
{
    if(strlen(*str) != 2) return INVALID_DATA;
    if((*str)[0] !='-' && (*str)[0] != '/') return INVALID_DATA;
    if((*str)[1] !='r' && (*str)[1] != 'a') return INVALID_DATA;
    return OK;
}

int get_length_of_number_in_base(int n, int base)
{
    return (int)(log(n)/log(base)) + 1;
}

void skip_dividers(int* c, FILE* f)
{
    do
    {
        if(*c == EOF || isalnum(*c)) break;
        *c = fgetc(f);
    }while(*c == ' ' || *c== '\t' || *c=='\n');
    fseek(f, -1, SEEK_CUR);
}

void go_to_divider(int* c, FILE* f)
{
    do
    {
        if(*c == EOF) break;
        *c =fgetc(f);
    } while (isalnum((*c)));
    fseek(f, -1, SEEK_CUR);
}

char* gorner(int n, int base)
{
    int length = get_length_of_number_in_base(n, base);
    char* result = (char*)malloc(sizeof(char) * (length + 1));
    if(result == NULL) return NULL;
    result[length] = '\0';
    if(n < base)
    {
       if(n >= 10)   result[0] = 'a' + (n - 10);
       else result[0] = '0' + n;
    }
    while(n >= base)
    {
        if(n % base < 10) 
        {
            result[--length] = '0' + n % base;
        }
        else 
        {
            result[--length] = 'a' + (n%base - 10);
        }
        n /= base;
        if(n < base)
        {
            if(n >= 10)   result[0] = 'a' + (n - 10);
            else result[0] = '0' + n;
        }
    }
    return result;
}

STATUS write_from_2_files(char *file1, char* file2, char* file3)
{
    FILE* f1 = fopen(file1, "r");
    if (f1 == NULL) {
        return OPEN_FILE_ERROR;
    }

    FILE* f2 = fopen(file2, "r");
    if (f2 == NULL) {
        return OPEN_FILE_ERROR;
    }

    FILE* f3 = fopen(file3, "w");
    if (f3 == NULL) {
        return OPEN_FILE_ERROR;
    }

    int number_of_liter = 1;
    int a = '1', b = '1';
    do
    {
        if(number_of_liter % 2 == 1)
        {
            do
            {
                a = fgetc(f1);
                if(a == EOF) break;
                fputc(a, f3);
            } while(a != ' ' && a != '\n' && a != '\t');

            skip_dividers(&a, f1);
            number_of_liter++;
        }
        else
        {
            do
            {
                b = fgetc(f2);
                if(b == EOF) break;
                fputc(b, f3);
            } while(b != ' ' && b != '\n' && b != '\t');

            skip_dividers(&b, f2);
            number_of_liter++;
        }

    } while((a != EOF) && (b != EOF));

    fputc(' ', f3);

    if(b != EOF)
    {
        while((b = fgetc(f2)) != EOF)
        {
            fputc(b, f3);
            if (b == ' ' && b == '\t' && b == '\n')  skip_dividers(&b, f2);
        }
    }
    else if(a != EOF)
    {
        while((a = fgetc(f1)) != EOF)
        {
            fputc(a, f3);
            if (a == ' ' && a == '\t' && a == '\n')  skip_dividers(&a, f1);
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    return OK;
}

STATUS key_a(char* file1, char* file2)
{
    FILE* f1 = fopen(file1, "r");
    if (f1 == NULL) {
        return OPEN_FILE_ERROR;
    } 

    FILE* output = fopen(file2, "w");
    if (output == NULL) {
        return OPEN_FILE_ERROR;
    }

    int c;
    int number_of_lexeme = 1;
    do
    {
        c= fgetc(f1);
        if(c == EOF) break;
        if(number_of_lexeme % 10 == 0)
        {
            do
            {
                c = fgetc(f1);
                if (c == EOF) break;
                char* converted = gorner(c, 4);
                if (converted == NULL) {
                    return MEMORY_ERROR;
                }
                fprintf(output, "%s", converted);
            } while(c != ' ' && c != '\n' && c != '\t');
            
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else if(number_of_lexeme % 2 ==0 && number_of_lexeme % 10 != 0)
        {
            do
            {
                if(c==EOF) break;
                char tmp = tolower(c);
                fputc(tmp, output);
                c = fgetc(f1);
            } while(c != ' ' && c != '\n' && c != '\t');
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else if(number_of_lexeme % 5 == 0 && number_of_lexeme % 10 != 0)
        {
            do
            {
                if(c==EOF) break;
                char* converted = gorner(c, 8);
                if(converted == NULL) return MEMORY_ERROR;
                fprintf(output, "%s", converted);
                c = fgetc(f1);
            } while(c != ' ' && c != '\n' && c != '\t');
            skip_dividers(&c, f1);
            number_of_lexeme++;  
        }
        else
        {
            if(c == ' ' || c=='\t' || c=='\n')  skip_dividers(&c, f1);
            else
            {
                go_to_divider(&c, f1);
                skip_dividers(&c, f1);
            }
            number_of_lexeme++;
        }
        fputc(' ', output);
    } while(c != EOF);
    
    fclose(f1);
    fclose(output);
    return OK;
}

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("INCORRECT NUMBER OF ARGUMENTS\n");
        return 1;
    }
    char* key = argv[1];
    int check_key = validation(&key);
    if (check_key != OK) {
        responce(check_key);
        return 1;
    }

    char* dir1;
    char* dir2;
    char* output;

    switch(key[1])
    {
        case 'r':
            if(argc != 5)
            {
                printf("INCORRECT NUMBER OF ARGUMENTS\n");
                return 1;
            }
            dir1 = argv[2];
            dir2 = argv[3];
            output = argv[4];
            STATUS write_to_file = write_from_2_files(dir1, dir2, output);
            if (write_to_file != OK) {
                responce(write_to_file);
                return 1;
            }
            
            printf("File1: %s\nFile2 : %s\nWERE CONCATENATED INTO FILE : %s\n", dir1, dir2, output);
            break;
        case 'a':
            if(argc != 4)
            {
                printf("INCORRECT NUMBER OF ARGUMENTS\n");
                return 1;
            }
            dir1 = argv[2];
            output = argv[3];
            STATUS key_a_stat = key_a(dir1, output);
            if (key_a_stat != OK) {
                responce(key_a_stat);
                return 1;
            }

            printf("File with directory: %s\nHas been rewritten according to key 'а' in file with directory: %s\n", dir1, output);

            break;
    }

    return 0;
}












