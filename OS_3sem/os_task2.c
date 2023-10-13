#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>

typedef struct
{
    char* login;
    char* password;
    bool is_login;
    //Need to create SANCTIONS field for work with permissions
} user;

typedef enum {
    OK,
    MEMORY_ERROR,
    UNDEFINED_BEHAVIOR,
    NO_ONE_USER,
    USER_NOT_FOUND,
    INCORRECT_PASSWORD
} statuses;

void responce(int status) {
    if (status == OK) printf("OK.\n");
    if (status == MEMORY_ERROR) printf("MEMORY_ERROR.\n");
    if (status == UNDEFINED_BEHAVIOR) printf("UNDEFINED_BEHAVIOR.\n");
    if (status == NO_ONE_USER) printf("NO_ONE_USER\n");
    if (status == USER_NOT_FOUND) printf("USER_NOT_FOUND\n");
    if (status == INCORRECT_PASSWORD) printf("INCORRECT_PASSWORD\n");
}

bool is_correct_password(char* password) {
    int count_char = 0;
    for (int i = 0; i < strlen(password); ++i) {
        count_char++;
        if (password[i] < '0' || password[i] > '9') {
            return false;
        }
    }
    if (count_char > 5) {
        return false;
    }
    
    return true;
}

bool is_correct_login(char* login) {
    int count_char = 0;
    for (int i = 0; i < strlen(login); ++i) {
        count_char++;
        if (!(login[i] >= '0' && login[i] <= '9') && !(login[i] >= 'a' && login[i] <= 'z') &&\
        !(login[i] >= 'A' && login[i] <= 'Z')) {
            return false;
        }
    }
    if (count_char > 6) {
        return false;
    }
    
    return true;
}

statuses innitialize_user(user *current_user, char* login, char* password) {
    (current_user->login) = (char*)malloc(sizeof(char)*6 + 1);
    (current_user->password) = (char*)malloc(sizeof(char)*5 + 1);
    if ((current_user->login) == NULL || (current_user->password) == NULL) {
        return UNDEFINED_BEHAVIOR;
    } else {
        strcpy((current_user->login), login);
        strcpy((current_user->password), password);
    }

    if (strcmp((current_user->login), login) != 0 || strcmp((current_user->password), password) != 0) {
        return MEMORY_ERROR;
    }

    return OK;
}   

statuses find_index_by_login(user* users, int user_count, char* login, int* user_index) {
    if (user_count == 0) {
        return NO_ONE_USER;
    }
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].login, login ) == 0 ) {
            *user_index = i;
            return OK;
        }
    }
    return USER_NOT_FOUND;
}

statuses log_in(user* users, int user_count, char* login, char* password) {
    if (user_count == 0 || users == NULL) {
        return UNDEFINED_BEHAVIOR;
    }
    int user_index;
    statuses status1 = find_index_by_login(users, user_count, login, &user_index);
    if (status1 != OK) {
        return status1;
    }
    if (strcmp(users[user_index].password, password) == 0) {
        users[user_index].is_login = true;
        return OK;
    } else {
        return INCORRECT_PASSWORD;
    }
    
    
}

statuses is_user_exist(user* users, int user_count, char* login) {
    if (user_count == 0) {
        return USER_NOT_FOUND;
    }
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].login, login) == 0 ) {
            return OK;
        }
    }
    return USER_NOT_FOUND;
}

statuses log_out(user* users, int user_count, char* login) {
    if (user_count == 0) {
        return NO_ONE_USER;
    }
    for (int i = 0; i < user_count; ++i) {
        if (strcmp(users[i].login,login) == 0) {
            users[i].is_login = false;
            return OK;
        }
    }
    return USER_NOT_FOUND;
}

int main(int argc, char* argv[]) {
    int user_count = 0;

    user* users = (user*)malloc(sizeof(user));
    if (users == NULL) {
        responce(MEMORY_ERROR);
        return 1;
    }

    printf("\nPlease log in or sign in.\n");
    int command;
    while (1)
    {
        printf("\nTo log in input [ 1 ].\nTo sign in input [ 2 ].\n (Without [ ])\n");
        // create COMMAND validation, won't forget
        scanf("%d", &command);
        char* login, *password;
        bool is_match_password;
        switch (command)
        {
        case 2:
            login = (char*)malloc(sizeof(char)*128 + 1), 
            password = (char*)malloc(sizeof(char)*128 + 1);
            if (login == NULL || password == NULL) {
                printf("\nMemory allocation error.\n");
                return 1;
            }

            printf("Now you try to sign in.\n");
            printf("Create login (6 symbols or less): ");
            scanf("%s", login);

            if (!(is_correct_login(login))) {
                printf("\nInvalid login format. Try again\n");
                break;
            }
            statuses user_already_exist = is_user_exist(users, user_count, login);
            if (user_already_exist == OK) {
                printf("\nUser already exist. Try again.\n");
                break;
            }

            printf("Create your password (only numerous; 5 symbols or less): ");
            scanf("%s", password);
            if (!is_correct_password(password)) {
                printf("\nInvalid password format. Try again.\n");
                break;
            }

            user *new_user = (user*)malloc(sizeof(user));

            statuses status = innitialize_user(new_user, login, password);

            if (status != OK) {
                responce(status);
                break;
            } else {
                printf("Your login: %s\nYour password: %s\n", ((*new_user).login), ((*new_user).password));
                users[user_count] = (*new_user);
                free(new_user);
                user_count++;
                user *temp_for_realloc = realloc(users, (user_count+1)*sizeof(user));
                if (temp_for_realloc == NULL) {
                    printf("Memory allocation error.\n");
                    return 1;
                }

                users = temp_for_realloc;
            }       
            
            statuses user_sign_and_log_in = log_in(users, user_count, login, password);
            if (user_sign_and_log_in != OK) {
                responce(user_sign_and_log_in);
                return 1;
            }
            is_match_password = true;
            printf("Now, you're logged in.\n");     
            break;
        case 1:
            if (user_count == 0) {
                printf("\nThere're no any users.Please, sign in.\n");
                break;
            }
            printf("Now, you try to log in.\n");
            printf("Your login: ");

            login = (char*)malloc(sizeof(char)*128 + 1), 
            password = (char*)malloc(sizeof(char)*128 + 1);
            if (login == NULL || password == NULL) {
                printf("\nMemory allocation error.\n");
                return 1;
            }

            scanf("%s", login);
            if (!is_correct_login(login)) {
                printf("Invalid login format.\n");
                break;
            }

            printf("Your password: ");
            scanf("%s", password);
            if (!is_correct_password(password)) {
                printf("Invalid password format.\n");
                break;
            }
            is_match_password = true;
            statuses user_log_in = log_in(users, user_count, login, password);

            if (user_log_in == INCORRECT_PASSWORD) {
                responce(user_log_in);
                free(password);
                free(login);
                is_match_password = false;
                break;
            }
            printf("You're logged in.\n");;

            break;
        default:
            break;
        }
        if (!is_match_password) {
            continue;
        }
        char* request = (char*)malloc(sizeof(char)*129);
        if (request == NULL) {
            printf("Memory allocation error.\n");
            return 1;
        }
        printf("You have access to the command:\n");
        printf("<Time> \n");
        printf("<Date> \n");
        printf("<Howmach> \n");
        printf("<Logout> \n");
        scanf("%s", request);
        while (strcmp(request, "Logout") != 0)
        {
            if (strlen(request) > 15) {
                break;
            } 
            switch (request[0])
            {
            case  'T':
                break;
            case 'D':
            break;
            case 'H':
            break;
            default:
                printf("Incorrect command. Try again.\n");
                break;
            }

            free(request);
            request = (char*)calloc(129, sizeof(char));
            if (request == NULL) {
                printf("Incorrect memory allocation");
                return 1;
            }
            scanf("%s", request);
        }
        statuses logout_status = log_out(users, user_count, login);
        printf("\nYou're logged out.\n");


        free(login);
        free(password);
        free(request);

    }
    free(users);
}