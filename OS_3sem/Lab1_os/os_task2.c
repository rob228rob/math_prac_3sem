#include <stdio.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    char* login;
    char* password;
    bool is_login;
    int sanctions;
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
        !(login[i] >= 'A' && login[i] <= 'Z') || (login[i] == ' ')) {
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
    if ((current_user->login) == NULL) {
        return UNDEFINED_BEHAVIOR;
    } 
    if (current_user->password == NULL) {
        return UNDEFINED_BEHAVIOR;
    }
    strcpy((current_user->login), login);
    strcpy((current_user->password), password);

    current_user->sanctions = -1;
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
        if (strcmp(users[i].login, login ) == 0 && strlen(login) == strlen(users[i].login)) {
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
    if (strcmp(users[user_index].password, password) == 0 && strlen(users[user_index].password) == strlen(password) ) {
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

statuses set_limit_request(user* users,int user_count, char* login, int* user_index, int count_sanctions) {
    if (users == NULL) {
        return UNDEFINED_BEHAVIOR;
    }

    statuses status1 = find_index_by_login(users, user_count, login, user_index);
    if (status1 != OK) {
        return status1;
    }
    users[*user_index].sanctions = count_sanctions;
    
    return OK;
}



int main(int argc, char* argv[]) {
    int user_count = 0;

    user* users = (user*)malloc(sizeof(user));
    if (users == NULL) {
        responce(MEMORY_ERROR);
        return 1;
    }

    printf("\nPlease log in or sign in.\n");
    char str_command[128];
    while (1)
    {
        printf("\nMAIN MENU:\n");
        printf("To log in input [ 1 ].\nTo sign in input [ 2 ].\nTo quit input [ 3 ] \n (Without [ ])\n");
        scanf("%127s", str_command);
        // create COMMAND validation, won't forget
        if (strlen(str_command) > 1) {
            printf("Command isn't correct.\n");
            continue;
        }
        int command = atoi(str_command);
        if (command == 3) {
            break;
        }
        char* login, *password;
        bool is_match_password;
        bool is_log_in = false;
        switch (command)
        {
        case 2:
            login = (char*)malloc(sizeof(char)*128 + 1), 
            password = (char*)malloc(sizeof(char)*128 + 1);
            if (login == NULL) {
                printf("\nMemory allocation error.\n");
                free(users);
                return 1;
            }
            if (password == NULL) {
                printf("\nMemory allocation error.\n");
                free(users);
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
            if (new_user == NULL) {
                free(users);
                return 1;
            }

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
                    free(users);
                    return 1;
                }

                users = temp_for_realloc;
            }       
            
            statuses user_sign_and_log_in = log_in(users, user_count, login, password);
            if (user_sign_and_log_in != OK) {
                responce(user_sign_and_log_in);
                free(users);
                return 1;
            }
            is_match_password = true;
            is_log_in = true;
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
                free(users);
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
                is_log_in = false;
                break;
            } else if (user_log_in != OK) {
                is_log_in = false;
                break;
            }
            printf("You're logged in.\n");
            is_log_in = true;

            break;
        default:
            break;
        }
        if (!is_log_in) {
            printf("\nIncorrect login or password.\n\n");
            free(login);
            free(password);
            continue;;
        }   
        if (!is_match_password) {
            continue;
        }

        printf("\nSession started.\n");
        char* request = (char*)malloc(sizeof(char)*129);
        if (request == NULL) {
            printf("Memory allocation error.\n");
            free(users);
            return 1;
        }
        printf("You have access to the command:\n\n");
        printf("<Time> \n");
        printf("<Date> \n");
        printf("<Howmach> \n");
        printf("<Logout> \n");
        printf("<Sanctions \n");
        scanf("%s", request);
        int user_index;
        statuses find_user_index = find_index_by_login(users, user_count, login, &user_index);
        if (find_user_index != OK) {
            free(users);
            free(request);
            free(login);
            free(password);
            responce(find_user_index);
            return 1;
        }
        
        int limit_request = users[user_index].sanctions;
        while (strcmp(request, "Logout") != 0 && (limit_request != 0))
        {
            if (limit_request > 0) {
                limit_request--;
            } 
            if (strlen(request) > 15) {
                break;
            } 
            
            switch (request[0])
            {
            case  'T':
                if (strcmp(request, "Time") != 0) {
                    printf("Incorrect command. Try again.\n");
                    break;
                }
                time_t now_time = time(NULL);
                struct tm *_time = localtime(&now_time);
                if (_time == NULL) {
                    printf("Time can't be obtained.\n");
                    break;
                }
                printf("\ntime now: %02d:%02d:%02d\n", _time->tm_hour, _time->tm_min, _time->tm_sec);
                break;

            case 'D':
                if (strcmp(request, "Date") != 0) {
                    printf("Incorrect command. Try again.\n");
                    break;
                }
                time_t now_date = time(NULL);
                struct tm *date = localtime(&now_date);
                if (time == NULL) {
                    printf("Time can't be obtained.\n");
                    break;
                }
                printf("\ndate now: %02d:%02d:%04d\n", date->tm_mday , date->tm_mon + 1, date->tm_year + 1900);

                break;
            case 'H':
                if (strcmp(request, "Howmach") != 0) {
                    printf("Incorrect command. Try again.\n");
                    break;
                }
                char buf[128], flag[128];
                printf("Input time(dd.mm.yyyy) and flag(-h, -m, -s, y): ");
                scanf("%127s", buf);
                scanf("%s", flag);
                if (strlen(flag) > 2 || !(flag[0] == '-') || !(flag[1] == 's' || flag[1] == 'm' || flag[1] == 'h' || \
                flag[1] == 'y') || strlen(buf) != 10) {
                    printf("Incorrect flag or date.\n");
                    break;
                }
                
                char day[3];
                char month[3];
                char year[5];
                day[0] = buf[0], day[1] = buf[1], day[2] = '\0';
                month[0] = buf[3], month[1] = buf[4], month[2] = '\0';
                year[0] = buf[6], year[1] = buf[7], year[2] = buf[8], year[3] = buf[9], year[4] = '\0';
                int year_int = atoi(year), month_int = atoi(month), day_int = atoi(day);
                struct tm user_time = {0};
                user_time.tm_year = year_int - 1900;
                user_time.tm_mday = day_int;
                user_time.tm_mon = month_int - 1;
                time_t now = time(NULL);
                struct tm *current_time = localtime(&now);
                double seconds_passed = fabs(difftime(mktime(&user_time), mktime(current_time)));
                double minutes_passed = seconds_passed/60;
                double hours_passed = minutes_passed/60;
                //printf("%s\n%s\n%s\n", day, month, year);
                switch (flag[1])
                {
                case 's':
                    printf("Seconds from %s: %.0lf\n", buf, seconds_passed );
                    break;
                case 'm':
                printf("Minutes from %s: %.0lf\n", buf, minutes_passed);
                    break;
                case 'h':
                    printf("Hours from %s: %.0lf\n", buf, hours_passed);
                    break;
                case 'y':
                    printf("Year from %s: %d\n", buf, (int) current_time->tm_year + 1900 - atoi(year));
                    break;
                default:
                    break;
                }

                break;
            case 'S':
                if (strcmp(request, "Sanctions") != 0) {
                    printf("Incorrect command. Try again.\n");
                    break;
                }
                printf("Input user login and limit commands for this user.\n<login number>\n");
                char _login[128], _number[128];
                scanf("%127s", _login);
                scanf("%127s", _number);
                if (!is_correct_login(login) || !is_correct_password(_number)) {
                    printf("Invalid values. Try again.\n");
                    break;
                }
                int count_sanctions = atoi(_number);
                if (count_sanctions > 50 || count_sanctions < 0) {
                    printf("Too large number.\n");
                    break;
                }
                
                statuses sanction_status = set_limit_request(users, user_count, login, &user_index, count_sanctions);
                if (sanction_status != OK) {
                    free(users);
                    free(login);
                    free(password);
                    free(request);
                    responce(sanction_status);
                    return 1;
                }
                char temp_password[128];
                printf("Input ADMIN password: ");
                scanf("%127s", temp_password);
                if (strcmp(temp_password, "12345") != 0) {
                    printf("Permission denied.\n");
                    break;
                }
                limit_request = users[user_index].sanctions;
                printf("Ok. User [%s] had only %d commands.\n", (users[user_index]).login, count_sanctions);

                break;
            default:
                printf("Incorrect command. Try again.\n");
                break;
            }
            if (limit_request == 0) {
                free(request);
                break;
            }
            free(request);
            request = (char*)calloc(129, sizeof(char));
            if (request == NULL) {
                printf("Incorrect memory allocation");
                free(users);
                return 1;
            }
            scanf("%s", request);
        }
        statuses logout_status = log_out(users, user_count, login);
        printf("\nYou're logged out.\n");


        free(login);
        free(password);
        //free(request);
        printf("Session finished.\n");
    }
    free(users);
}