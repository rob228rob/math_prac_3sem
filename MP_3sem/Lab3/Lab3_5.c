#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND
} status;

void response(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND) printf("NO_ONE_FOUND.\n");
}

typedef struct {
    int id;
    char name[32];
    char surname[32];
    char group[32];
    unsigned char marks[6];
} Student;


status is_flag(int symb) {
    char *flags = "ad";
    for (int i = 0; i < 5; ++i) {
        if (flags[i] == symb) {
            return OK;
        }
    } 
    return UNDEFINED_BEHAVIOR;
}

void swap(char *a, char *b) {
    char temp = *a;
    *a = *b;
    *b = temp;
}

int is_alph(char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        if (!(str[i] >= 'a' && str[i] <= 'z')) {
            return 1;
        }
    }
    return 0;
}

int is_num(const char* str) {
    int length = strlen(str);
    for (int i = 0; i < length; ++i) {
    if (!isdigit(str[i])) {
        return 0;
    }
    }
    return 1;
}

status read_from_file(Student** students, int* count_employee, char* filename) {
    if (count_employee <= 0 || filename == NULL) {
        return INVALID_DATA;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }
    char buf[256];
    int capacity = 20, count = 0;
    *students = (Student*)malloc(sizeof(Student)*capacity);
    if (*students == NULL) {
        fclose(file);
        return MEMORY_ERROR;
    }
    
    while (fgets(buf, 255, file) != NULL)
    {
        printf("%s\n", buf);
        count++;
        if (capacity - count < 2) {
            capacity *= 2;
            Student* tmp = (Student*)realloc(*students, sizeof(Student)*capacity);
            if (tmp == NULL) {
                free(*students);
                fclose(file);
                return MEMORY_ERROR;
            }
            *students = tmp;
            tmp = NULL;
        }
        
        int ind = 0, local_ind = 0;
        char id[256], name[256], surname[256], group[256];
        unsigned char* marks = (unsigned char*)malloc(sizeof(unsigned char)*5 + 4);
        if (marks == NULL) {
            fclose(file);
            free(*students);
            return MEMORY_ERROR;
        }

        while (buf[ind] != ' ') {
            id[ind] = buf[ind];
            ind++;
        }
        id[ind] = '\0'; 
        ind++;

        if (is_num(id) == 0) {
            free(marks);
            free(*students);
            fclose(file);
            return INCORRECT_FIELD;
        }
        (*students)[count-1].id = atoi(id);
        
        local_ind = 0;
        while (buf[ind] != ' ')
        {
            name[local_ind] = buf[ind];
            ind++; local_ind++;
        }
        name[local_ind] = '\0';
        local_ind = 0; ind++;

        strcpy((*students)[count - 1].name, name);

        while (buf[ind] != ' ')
        {
            surname[local_ind] = buf[ind];
            ind++; local_ind++;
        }
        surname[local_ind] = '\0';
        local_ind = 0; ind++;

        strcpy((*students)[count - 1].surname,surname);
       
        while (buf[ind] != ' ')
        {
            group[local_ind] = buf[ind];
            local_ind++; ind++;
        }
        group[local_ind] = '\0';
        local_ind = 0; ind++;
        strcpy((*students)[count - 1].group, group);
        while (buf[ind] != '\n')
        {
            marks[local_ind] = buf[ind];
            local_ind++; ind++;
        }
        marks[local_ind] = '\0';
        if (strlen(marks) > 5 || is_num(marks) == 0) {
            free(*students);
            free(marks);
            fclose(file);
            return UNDEFINED_BEHAVIOR;
        }
        strcpy((*students)[count - 1].marks, marks);

    }
    *count_employee = count;
    fclose(file);
    return OK;
}

status find_student_by_name(Student *students, int count_student, char *name, Student **curr_student, int *curr_count) {
    if (count_student <= 0 || students == NULL || name == NULL) {
        return INVALID_DATA;
    }
    int capacity = 5;
    *curr_student = (Student*)malloc(sizeof(Student)*capacity);
    if (*curr_student == NULL) {
        return MEMORY_ERROR;
    }

    int ind = 0;
    for (int i = 0; i < count_student; ++i) {
        if (strcmp(students[i].name, name) == 0 && strlen(students[i].name) == strlen(name)) {
            (*curr_student)[ind] = students[i];
            ind++;

            if (capacity - ind < 2) {
                capacity *= 2;
                Student* tmp = (Student*)realloc(*curr_student, sizeof(Student)*capacity);
                if (tmp == NULL) {
                    free(*curr_student);
                    return MEMORY_ERROR;
                }
                *curr_student = tmp;
            }   
        }
    }
    if (ind == 0) {
        free(*curr_student);
        return NO_ONE_FOUND;
    }
    *curr_count = ind;
    return OK;
}

status find_student_by_surname(Student *students, int count_student, char *surname, Student **curr_student, int *curr_count) {
    if (count_student <= 0 || students == NULL || surname == NULL) {
        return INVALID_DATA;
    }
    int capacity = 5;
    *curr_student = (Student*)malloc(sizeof(Student)*capacity);
    if (*curr_student == NULL) {
        return MEMORY_ERROR;
    }

    int ind = 0;
    for (int i = 0; i < count_student; ++i) {
        if (strcmp(students[i].surname, surname) == 0 && strlen(students[i].surname) == strlen(surname)) {
            (*curr_student)[ind] = students[i];
            ind++;

            if (capacity - ind < 2) {
                capacity *= 2;
                Student* tmp = (Student*)realloc(*curr_student, sizeof(Student)*capacity);
                if (tmp == NULL) {
                    free(*curr_student);
                    return MEMORY_ERROR;
                }
                *curr_student = tmp;
            }   
        }
    }
    if (ind == 0) {
        free(*curr_student);
        return NO_ONE_FOUND;
    }
    *curr_count = ind;
    return OK;
}

status find_student_by_group(Student *students, int count_student, char *group, Student **curr_student, int *curr_count) {
      if (count_student <= 0 || students == NULL || group == NULL) {
        return INVALID_DATA;
    }
    int capacity = 5;
    *curr_student = (Student*)malloc(sizeof(Student)*capacity);
    if (*curr_student == NULL) {
        return MEMORY_ERROR;
    }

    int ind = 0;
    for (int i = 0; i < count_student; ++i) {
        if (strcmp(students[i].group, group) == 0 && strlen(students[i].group) == strlen(group)) {
            (*curr_student)[ind] = students[i];
            ind++;

            if (capacity - ind < 2) {
                capacity *= 2;
                Student* tmp = (Student*)realloc(*curr_student, sizeof(Student)*capacity);
                if (tmp == NULL) {
                    free(*curr_student);
                    return MEMORY_ERROR;
                }
                *curr_student = tmp;
            }   
        }
    }
    if (ind == 0) {
        free(*curr_student);
        return NO_ONE_FOUND;
    }
    *curr_count = ind;
    return OK;
}

status find_student_by_id(Student *students, int count_student, int curr_id, Student **curr_student, int *curr_count) {
    if (count_student <= 0 || students == NULL || curr_id < 0) {
        return INVALID_DATA;
    }
    int capacity = 5;
    *curr_student = (Student*)malloc(sizeof(Student)*capacity);
    if (*curr_student == NULL) {
        return MEMORY_ERROR;
    }

    int ind = 0;
    for (int i = 0; i < count_student; ++i) {
        if (students[i].id == curr_id) {
            (*curr_student)[ind] = students[i];
            ind++;

            if (capacity - ind < 2) {
                capacity *= 2;
                Student* tmp = (Student*)realloc(*curr_student, sizeof(Student)*capacity);
                if (tmp == NULL) {
                    free(*curr_student);
                    return MEMORY_ERROR;
                }
                *curr_student = tmp;
            }
            
        }
    }
    if (ind == 0) {
        free(*curr_student);
        return NO_ONE_FOUND;
    }
    *curr_count = ind;
    return OK;
}

void print_student(Student *curr_stud, int curr_count) {
    for (int i = 0; i < curr_count; ++i) {
        printf("%d %s %s %s %s\n", curr_stud[i].id, curr_stud[i].name, curr_stud[i].surname, curr_stud[i].group, curr_stud[i].marks);
    }
}

int summ(char* str, int* result) {
    if (str == NULL) {
        return -1;
    }
    int sum = 0;
    for (int i = 0; i < strlen(str); ++i) {
        sum += (str[i] - '0');
        if (!isdigit(str[i])) {
            return 1;
        }
    }
    *result = sum;
    return 0;
}

int cmp_id(const void *a, const void* b) {
    Student* stud_A = (Student*)a;
    Student* stud_B = (Student*)b;
    return stud_A->id > stud_B->id;
}

int cmp_name(const void *a, const void* b) {
    Student* stud_A = (Student*)a;
    Student* stud_B = (Student*)b;
    int len_A = strlen(stud_A->name);
    int len_B = strlen(stud_B->name);
    if (len_A != len_B) {
        return len_A < len_B;
    }
    return stud_A->name < stud_B->name;
}

int cmp_surname(const void *a, const void* b) {
    Student* stud_A = (Student*)a;
    Student* stud_B = (Student*)b;
    int len_A = strlen(stud_A->surname);
    int len_B = strlen(stud_B->surname);
    if (len_A != len_B) {
        return len_A < len_B;
    }
    return strcmp(stud_A->surname, stud_B->surname);
}

int cmp_group(const void *a, const void* b) {
    Student* stud_A = (Student*)a;
    Student* stud_B = (Student*)b;
    return strcmp(stud_A->group, stud_B->group);
}

status write_student_to_file(Student *curr_stud, int curr_count, char *filename) {
    if (curr_stud == NULL || curr_count <= 0 || filename == NULL) {
        return INVALID_DATA;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }
    for (int i = 0; i < curr_count; ++i) {
        fprintf(file, "%d %s %s %s %s\n", curr_stud[i].id, curr_stud[i].surname, curr_stud[i].name, curr_stud[i].group, curr_stud[i].marks);
    }

    fclose(file);
    return OK;
}

status write_students_with_high_marks(Student *students, int count, char* filename) {
    if (students == NULL || count <= 0 || filename == NULL) {
        return INVALID_DATA;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }
    double general_avg_mark = 0;
    for (int i = 0; i < count; ++i) {
        int tmp;
        if (summ(students[i].marks, &tmp) <= 0) {
            fclose(file);
            return INVALID_DATA;
        }
        general_avg_mark += tmp;
    }
    general_avg_mark = general_avg_mark / (5.0 * count);
    int is_exist = 0;
    for (int i = 0; i < count; ++i) {
        int str_summ;
        if (summ(students[i].marks, &str_summ) <= 0) {
            fclose(file);
            return INVALID_DATA;
        }
        double avg_mark = str_summ / 5.0;
        if (avg_mark > general_avg_mark) {
            fprintf(file, "%s %s | average mark - [ %.1lf ]\n", students[i].surname, students[i].name, avg_mark);
            is_exist = 1;
        }
    }
    if (!is_exist) {
        fclose(file);
        return NO_ONE_FOUND;
    }
    fclose(file);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }

    int count_student;
    Student* students;
    status status_stud = read_from_file(&students, &count_student, argv[1]);
    if (status_stud != OK) {
        response(status_stud);
        return 1;
    }

    char buf[4096];
    while (buf[0] != '0' || strlen(buf) <= 2 )
    {
        sleep(1);
        printf("\nInput action:\n[ 1 ] - Search by id;\n[ 2 ] - Search by surname;\n[ 3 ] - Search by name\n");
        printf("[ 4 ] - Search by group\n[ 5 ] - write info about student in <%s> file\n", argv[2]);
        printf("[ 6 ] - write students to <%s> file, which average mark higher than other\n", argv[2]);
        printf("[ 7 ] - Sort by id;\n[ 8 ] - Sort by surname;\n[ 9 ] - Sort by name\n[ 10 ] - Sort by group\n[ 0 ] - QUIT\n");

        scanf("%s", buf);

        char *request = (char*)malloc(sizeof(char)*128);
        if (request == NULL) {
            free(students);
            response(MEMORY_ERROR);
            return MEMORY_ERROR;
        }
        int act = atoi(buf);
        if (act == 1) {
            printf("Input id you found: ");
            scanf("%s", request);
            if (strlen(request) > 5 || is_num(request) > 0) {
                printf("Incorrect id.\n");
                // MAY BE LATER ADD CONTINUR INSTEAD OF RETURN 1
                free(students);
                return 1;
            }
            int current_id = atoi(request);
            Student *curr_stud; 
            int curr_count = 0;
            status find_stat = find_student_by_id(students, count_student, current_id, &curr_stud, &curr_count);
            if (find_stat!= OK) {
                response(find_stat);
                free(students);
                return 1;
            }
            print_student(curr_stud, curr_count);
            
            free(curr_stud);
        } 
        else if (act == 2) {
            printf("Input surname you found: ");
            scanf("%s", request);
            if (strlen(request) > 32 || is_alph(request) != 0) {
                printf("Invalid name.\n");
                // MAY BE LATER ADD CONTINUR INSTEAD OF RETURN 1
                free(students);
                return 1;
            }
            Student* curr_student;
            int curr_count = 0;
            status find_stat = find_student_by_surname(students, count_student, request, &curr_student, &curr_count);
            if (find_stat != OK) {
                response(find_stat);
                free(students);
                return 1;
            }
            print_student(curr_student, curr_count);
            free(curr_student);
        } 
        else if (act == 3) {
            printf("Input name you found: ");
            scanf("%s", request);
            if (strlen(request) > 32 || is_alph(request) != 0) {
                printf("Invalid surname.\n");
                // MAY BE LATER ADD CONTINUR INSTEAD OF RETURN 1
                free(students);
                return 1;
            }
            Student* curr_student;
            int curr_count;
            status find_stat = find_student_by_name(students, count_student, request, &curr_student, &curr_count);
            if (find_stat != OK) {
                response(find_stat);
                free(students);
                return 1;
            }
            print_student(curr_student, curr_count);
            free(curr_student);
            
        }
        else if (act == 4) {
            printf("Input group you found: ");
            scanf("%s", request);
            if (strlen(request) > 32) {
                printf("Invalid group.\n");
                // MAY BE LATER ADD CONTINUE INSTEAD OF RETURN 1
                free(students);
                return 1;
            }
            Student* curr_student;
            int curr_count;
            status find_stat = find_student_by_group(students, count_student, request, &curr_student, &curr_count);
            if (find_stat != OK) {
                response(find_stat);
                free(students);
                return 1;
            }
            print_student(curr_student, curr_count);
            free(curr_student);
        } 
        else if (act == 5) {
            printf("Input student id: ");
            scanf("%s", request);
            if (strlen(request) > 5 || is_num(request) > 0) {
                printf("Incorrect id.\n");
                // MAY BE LATER ADD CONTINUR INSTEAD OF RETURN 1
                free(students);
                return 1;
            }
            int current_id = atoi(request);
            Student *curr_stud; 
            int curr_count = 0;
            status find_stat = find_student_by_id(students, count_student, current_id, &curr_stud, &curr_count);
            if (find_stat!= OK) {
                response(find_stat);
                free(students);
                return 1;
            }
            status write_stat = write_student_to_file(curr_stud, curr_count, argv[2]);
            if (write_stat != OK) {
                response(write_stat);
                free(students);
                return 1;
            }
            printf("File created.\n");
            free(curr_stud);
        } 
        else if (act == 6) {
            status write_students = write_students_with_high_marks(students, count_student, argv[2]);
            if (write_students != OK) {
                response(write_students);
                free(students);
                return 1;
            }
            printf("File was created successfully.\n");
        } 
        else if (act == 7) {
            qsort(students, count_student, sizeof(Student), cmp_id);
            print_student(students, count_student);
        }
        else if (act == 8) {
           qsort(students, count_student, sizeof(Student), cmp_surname);
            print_student(students, count_student);
        } 
        else if (act == 9) {
            qsort(students, count_student, sizeof(Student), cmp_name);
            print_student(students, count_student);
        } 
        else if (act == 10) {
            qsort(students, count_student, sizeof(Student), cmp_group);
            print_student(students, count_student);           
        }
        free(request);
    }
    printf("Incorrect input or terminated zero.\n");
    
    free(students);
    return 0;
}