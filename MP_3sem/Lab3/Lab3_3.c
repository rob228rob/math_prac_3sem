#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

typedef enum {
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD
} status;

void response(int status) {
    if (status == OK) printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA) printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR) printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD) printf("INCORRECT_FIELD.\n");
}

typedef struct {
    int id;
    char name[256];
    char surname[256];
    double salary;

} Employee;


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


int is_float(const char* str) {
    if (str == NULL || *str == '\0') {
        return -1; 
    }
    
    int hasDigit = 0;
    int hasDot = 0;
    
    if (*str == '-' || *str == '+') {
        str++;  
    }
    
    while (*str != '\0') {
        if (*str == '.') {
            if (hasDot) {
                return 0;  
            }
            hasDot = 1;
        } else if (*str >= '0' && *str <= '9') {
            hasDigit = 1;
        } else {
            return 0; 
        }
        
        str++;
    }
    // return false if number hasn't got any numerous;
    return hasDigit; 
}

int is_digit(char* str) {
    for (int i = 0; i < strlen(str); ++i) {
        if ((str[i] < '0' || str[i] > '9')) {
            return 0;
        }
    }
    return 1;
}

int read_from_file(Employee** employee, int* count_employee, char* filename) {
    if (count_employee <= 0 || filename == NULL) {
        return INVALID_DATA;
    }
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }

    char buf[256];
    int capacity = 20, count = 0;
    *employee = (Employee*)malloc(sizeof(Employee)*capacity);
    if (*employee == NULL) {
        fclose(file);
        return MEMORY_ERROR;
    }
    
    while (fgets(buf, 255, file) != NULL)
    {
        count++;
        if (capacity - count < 2) {
            capacity *= 2;
            Employee* tmp = (Employee*)realloc(*employee, sizeof(Employee)*capacity);
            if (tmp == NULL) {
                free(*employee);
                fclose(file);
                return MEMORY_ERROR;
            }
            *employee = tmp;
        }
        
        int ind = 0, local_ind = 0;
        char id[256], name[256], surname[256], salary[256];
        while (buf[ind] != ' ') {
            id[ind] = buf[ind];
            ind++;
        }
        id[ind] = '\0'; 
        ind++;

        if (!is_digit(id)) {
            free(*employee);
            fclose(file);
            return INCORRECT_FIELD;
        }
        (*employee)[count-1].id = atoi(id);
        
        local_ind = 0;
        while (buf[ind] != ' ')
        {
            name[local_ind] = buf[ind];
            ind++; local_ind++;
        }
        name[local_ind] = '\0';
        local_ind = 0; ind++;
        strcpy((*employee)[count - 1].name, name);

        
        while (buf[ind] != ' ')
        {
            surname[local_ind] = buf[ind];
            ind++; local_ind++;
        }
        surname[local_ind] = '\0';
        local_ind = 0; ind++;
        strcpy((*employee)[count - 1].surname,surname);
       
        while (buf[ind] != '\n')
        {
            salary[local_ind] = buf[ind];
            local_ind++; ind++;
        }
        salary[local_ind] = '\0';
        local_ind = 0;
        ind = 0;
        if (!is_float(salary)) {
            free(*employee);
            fclose(file);
            return INCORRECT_FIELD;
        }
        char* endptr;
        (*employee)[count - 1].salary = strtod(salary, &endptr);
        
    }
    *count_employee = count;
    fclose(file);
    return OK;
}

int cmp1(const void* a, const void* b) {
    Employee* empA = (Employee*)a;
    Employee* empB = (Employee*)b;
    double eps = 1e-9;

    if (empA->salary != empB->salary) {
        return (empA->salary - empB->salary) < eps;
    }
    else if (strcmp(empA->name, empB->name) != 0) {
        return strcmp(empA->name, empB->name);
    } 
    else if (strcmp(empA->surname, empB->surname) != 0) {
        return strcmp(empA->surname, empB->surname);
    } 
    else if (empA->id != empB->id) {
        return empA->id < empB->id;
    } 
}

int cmp2(const void* a, const void* b) {
    Employee* empA = (Employee*)a;
    Employee* empB = (Employee*)b;
    double eps = 1e-9;

    if (empA->salary != empB->salary) {
        return (empA->salary - empB->salary) < eps;
    }
    else if (strcmp(empA->name, empB->name) != 0) {
        return -strcmp(empA->name, empB->name);
    } 
    else if (strcmp(empA->surname, empB->surname) != 0) {
        return -strcmp(empA->surname, empB->surname);
    } 
    else if (empA->id != empB->id) {
        return empA->id > empB->id;
    } 
}

int write_to_file(Employee* employee, int emp_count, char* filename) {
    if (emp_count <= 0 || employee == NULL || filename == NULL) {
        return INVALID_DATA;
    }
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        return OPEN_FILE_ERROR;
    }
    for (int i = 0; i < emp_count; ++i) {
        fprintf(file, "id:%d\tname:%s\tsurname:%s\tsalary:%.2lf\n", employee[i].id, employee[i].name, employee[i].surname, employee[i].salary);
    }
    fclose(file);
    return OK;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Incorrect number of arguments.\n");
        return 1;
    }
    char* flag = argv[2];
    int flag_len = strlen(flag);

    if (is_flag(flag[1]) != OK || flag_len != 2 || (flag[0] != '/' && flag[0] != '-')) {
        printf("Incorrect flag.\n");
        return 1;
    }
    
    Employee* employee;
    int emp_count;
    int read_status = read_from_file(&employee, &emp_count, argv[1]);
    if (read_status!= OK) {
        response(read_status);
        return read_status;
    }

    if (flag[1] == 'a') {
        qsort(employee, emp_count, sizeof(Employee), cmp1);
    } 
    else if (flag[1] == 'd') {
        qsort(employee, emp_count, sizeof(Employee), cmp2);
    }

    int write_status = write_to_file(employee, emp_count, argv[3]);
    if (write_status != OK) {
        response(write_status);
        free(employee);
        return write_status;
    }

    for (int i = 0; i < emp_count; ++i) {
        printf("id:%d name:%s surn:%s salary:%lf \n", (employee)[i].id,(employee)[i].name, (employee)[i].surname, employee[i].salary);
    }

    free(employee);
    return 0;
}