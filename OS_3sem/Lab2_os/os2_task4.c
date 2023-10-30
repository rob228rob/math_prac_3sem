#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_SEARCH_LENGTH 256

typedef enum {
    ZERO_STATUS,
    MEMORY_ERROR,
    UNDEFINED_BEHAVIOR,
    STRING_NOT_FOUND,
    STRING_FOUND,
    OPEN_FILE_ERROR
} statuses;

void responce(int status) {
    if (status == ZERO_STATUS) printf("ZERO_STATUS.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR\n");
    if (status == STRING_NOT_FOUND) printf("STRING_NOT_FOUND\n");
    if (status == STRING_FOUND) printf("STRING_FOUND\n");
}

typedef struct {
    char filename[MAX_FILENAME_LENGTH];
    char search_str[MAX_SEARCH_LENGTH];
    int status;
    int result_line;
} thread_args;

void *search_in_file(void *arg) {
    thread_args *args = (thread_args *)arg;
    char *filename = args->filename;
    char *searchStr = args->search_str;
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        args->status = OPEN_FILE_ERROR;
        pthread_exit(NULL);
    }
    
    args->status = 2;
    char line[256];
    int line_num = 0;
    while (fgets(line, sizeof(line), file)) {
        line_num++;
        if (strstr(line, searchStr) != NULL) {
            args->status = STRING_FOUND;
            args->result_line = line_num;
            fclose(file);
            pthread_exit(NULL);
        }
    }

    args->status = STRING_NOT_FOUND;
    fclose(file);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Please, input source file.\n");
        return 0;
    }
    
    const char *path_to_file_list = argv[1];
    FILE *file_list = fopen(path_to_file_list, "r");
    if (file_list == NULL) {
        printf("Cannot open Source file: %s\n", path_to_file_list);
        return 0;
    }
    
    char filename[MAX_FILENAME_LENGTH];
    char search_str[MAX_SEARCH_LENGTH];
    
    pthread_t tid;
    thread_args args;
    int counter = 0;

    while (fscanf(file_list, "%s", filename) != EOF) {
        printf("Search in file: %s\n", filename);
        
        FILE *file = fopen(filename, "r");
        if (file == NULL) {
            printf("Cannot open file\n");
            continue;
        }
        strcpy(args.filename, filename);
        strcpy(args.search_str,"!!!!");
        
        // Create new thread
        if (pthread_create(&tid, NULL, search_in_file, &args) != 0) {
            printf("Thread create error\n");
            continue;
        }

        //expected that creating thread was finished
        pthread_join(tid, NULL);
        if (args.status != STRING_FOUND) {
            responce(args.status);
        } else {
            printf("String was found %s, line: %d\n", filename, args.result_line);
        }
        if (args.status == STRING_FOUND) {
            counter++;
        }
        
        fclose(file);
    }
    fclose(file_list);

    if (counter == 0) {
        printf("\nString was not found in Source file: %s\n",path_to_file_list);
    }
    return 0;
}