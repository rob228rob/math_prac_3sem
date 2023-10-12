#include <stdio.h>
#include <string.h>
#include <malloc.h>

void print_buffer(char* buffer, int buffer_size);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect num of argc.\n");
    }
    
    FILE* file = fopen(argv[1], "w");
    if (file == NULL) {
        printf("Can't open file to write.\n");
        fclose(file);
        return 1;
    }

    char str[] = "31415926535";
    int count = fwrite(str, sizeof(char), strlen(str), file);
    
    if (count != strlen(str)) {
        printf("Undefined behavior.\n");
        fclose(file);
        return 1;
    }
    fclose(file);

    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file to read.\n");
        fclose(file);
        return 1;
    }
    
    char byte = fgetc(file);
    while (!feof(file) && byte != '\n') {
        printf("byte: %c;\n[ %ld ] - file->pad5; [ %hd ] - file->cur_column; [ %d ] - file->fileno;\
         [ %d ] - file->flags2  [ %d ] - file->flags;\n\n", byte, file->__pad5, file->_cur_column, file->_fileno, file->_flags2, file->_flags);
        byte = fgetc(file);
    }
    fclose(file);
    
    file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Can't open file to read.\n");
        fclose(file);
        return 1;
    }

    fseek(file, 3, SEEK_SET);
    int buffer_size = 4;
    char* buffer = (char*)malloc(sizeof(char)*buffer_size + 1);
    if (buffer == NULL) {
        printf("Memory allocation error.\n");
        fclose(file);
        return 1;
    }
    
    int size_read = fread(buffer, sizeof(char), buffer_size, file);
    if (size_read != buffer_size) {
        printf("Incorrect buffer read.\n");
        fclose(file);
        return 1;
    } else {
        print_buffer(buffer, buffer_size);
    }

    fclose(file);

    return 0;
}

void print_buffer(char* buffer, int buffer_size) {
    printf("\nBuffer output: ");
    for (int i = 0; i < 4; ++i) {
        printf("%c ", buffer[i]);
    }
    printf("\n");

    return;
}