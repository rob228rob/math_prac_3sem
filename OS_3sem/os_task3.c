#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    FILE* copy_from = fopen(argv[1], "r");
    if (copy_from == NULL) {
        printf("Source file can't open.\n");
        return 1;
    }

    FILE* copy_to = fopen(argv[2], "w");
    if (copy_to == NULL) {
        printf("Target file can't open to write.\n");
        return 1;
    }

    int ch;
    while(!feof(copy_from))
    {
        ch=getc(copy_from);
        if ( !feof(copy_from)) {
            putc(ch, copy_to);
        }
    }
    
    fclose(copy_from);
    fclose(copy_to);
    
    return 0;    
}