#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <unistd.h>
#include <stdbool.h>

typedef enum
{
    OK,
    UNDEFINED_BEHAVIOR,
    MEMORY_ERROR,
    INVALID_DATA,
    OPEN_FILE_ERROR,
    INCORRECT_FIELD,
    NO_ONE_FOUND,
    ALREADY_EXIST
} STATUS;

void response(int status)
{
    if (status == OK)
        printf("OK.\n");
    if (status == UNDEFINED_BEHAVIOR)
        printf("UNDEFINED_BEHAVIOR.\n");
    if (status == MEMORY_ERROR)
        printf("MEMORY_ERROR.\n");
    if (status == INVALID_DATA)
        printf("INVALID_DATA.\n");
    if (status == OPEN_FILE_ERROR)
        printf("OPEN_FILE_ERROR.\n");
    if (status == INCORRECT_FIELD)
        printf("INCORRECT_FIELD.\n");
    if (status == NO_ONE_FOUND)
        printf("NO_ONE_FOUND.\n");
    if (status == ALREADY_EXIST)
        printf("ALREADY_EXIST.\n");
}

bool is_str(char *str)
{
    int length = strlen(str) - 1;
    for (int i = 0; i < length; ++i)
    {
        if (!isalpha(str[i]))
        {
            return false;
        }
    }
    return true;
}

bool is_num(char *str)
{
    for (int i = 0; i < strlen(str) - 1; ++i)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }
    return true;
}

int is_float(const char* str) {
    if (str == NULL || *str == '\0') {
        return -1; 
    }
    
    int has_digit = 0;
    int has_dot = 0;
    
    if (*str == '-' || *str == '+') {
        str++;  
    }
    
    while (*str != '\0') {
        if (*str == '.') {
            if (has_dot) {
                return 0;  
            }
            has_dot = 1;
        } else if (*str >= '0' && *str <= '9') {
            has_digit = 1;
        } else {
            return 0; 
        }
        
        str++;
    }
    // return false if number hasn't got any numerous;
    return has_digit; 
}

typedef struct
{
    int size;
    char *str;
} String;

STATUS init_str(String *string, char *str)
{
    if (str == NULL)
    {
        return INVALID_DATA;
    }
    int len = strlen(str);
    string->size = len;

    string->str = (char *)malloc(sizeof(char) * (len + 1));
    if (string->str == NULL)
    {
        return MEMORY_ERROR;
    }
    strcpy(string->str, str);
    return OK;
}

void print_str(String *string)
{
    printf("%s", string->str);
}

void clear_str(String *string)
{
    free(string->str);
    string->size = 0;
    string->str = NULL;
}

int is_equal(String str1, String str2)
{   
    if (str1.str == NULL || str2.str == NULL) {
        return 0;
    }

    int differ = fabs(str1.size - str2.size);
    if (differ != 0)
    {
        return 1;
    }
    for (int i = 0; i < str1.size; ++i)
    {
        if (str1.str[i] - str2.str[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}

__int64_t to_int(String string)
{
    return atol(string.str);
}

int string_cmp(const void *a, const void *b)
{
    const String *str1 = (const String *)a;
    const String *str2 = (const String *)b;

    return str1->size < str2->size;
}

char* set_time(int differ)
{
    time_t rawtime;
    struct tm *timeinfo;
    char *buffer = (char *)malloc(sizeof(char) * 52);
    if (buffer == NULL)
    {
        return NULL;
    }
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int past_mon = timeinfo->tm_mon;
    if (timeinfo->tm_mon + differ > 11)
    {
        timeinfo->tm_mon = (past_mon + differ) % 11 - 1;
        timeinfo->tm_year += (past_mon + differ) / 11;
    }
    strftime(buffer, sizeof(char) * 52, "%d/%m/%Y %X", timeinfo);

    return buffer;
}

STATUS string_cpy(String *dest, String *src)
{
    if (src->str != NULL)
    {
        return INVALID_DATA;
    }
    strcpy(dest->str, src->str);
    return OK;
}

STATUS concatenate_string(String str1, String str2, String *string_res)
{   
    if (str1.str == NULL || str2.str == NULL) return INVALID_DATA;

    int len1 = str1.size;
    int len2 = str2.size;
    int new_len = len1 + len2;
    string_res->str = (char *)malloc(sizeof(char) * (new_len + 1));
    if (string_res->str == NULL)
    {
        return MEMORY_ERROR;
    }
    string_res->str[new_len] = '\0';

    int i = 0;
    while (i < len1)
    {
        (string_res->str)[i] = str1.str[i];
        i++;
    }
    int j = 0;
    while (i < new_len)
    {
        (string_res->str)[i] = str2.str[j];
        i++;
        j++;
    }
    string_res->size = strlen(string_res->str);
    return OK;
}

typedef struct
{
    String city;
    String street;
    String building;
    int house_number;
    String person_id;
} Adress;

void clear_adress(Adress *adress)
{
    clear_str(&(adress->building));
    clear_str(&(adress->street));
    clear_str(&(adress->person_id));
    clear_str(&(adress->city));
}

typedef struct
{
    Adress adress;
    double weight;
    bool is_delete;
    String mail_id;
    String time_create;
    String time_deliver;
} Mail;

typedef struct
{
    Adress current_adress;
    int size;
    int capacity;
    Mail *mails;
} Post;

int diff_time(char *time1, char *time2)
{
    if (time1 == NULL || time2 == NULL) {
        return -1;
    }
    char year1[4], year2[4];
    for (int i = 6; i < 10;)
    {
        year1[i - 6] = time1[i];
        year2[i - 6] = time2[i];
    }
    int diff_y = atoi(year1) - atoi(year2);
    if (diff_y != 0)
    {
        return diff_y;
    }
    char mon1[2], mon2[2];
    for (int i = 3; i < 5; ++i)
    {
        mon1[i - 3] = time1[i];
        mon2[i - 3] = time2[i];
    }
    int diff_mon = atoi(mon1) - atoi(mon2);
    if (diff_mon != 0)
    {
        return diff_mon;
    }
    char day1[2], day2[2];
    for (int i = 0; i < 2; ++i)
    {
        day1[i] = time1[i];
        day2[i] = time2[i];
    }
    int diff_day = atoi(day1) - atoi(day2);
    if (diff_day != 0)
    {
        return diff_day;
    }

    char hour1[2], hour2[2];
    for (int i = 11; i < 13; ++i)
    {
        day1[i - 11] = time1[i];
        day2[i - 11] = time2[i];
    }
    int diff_hour = atoi(hour1) - atoi(hour2);
    if (diff_hour != 0)
    {
        return diff_hour;
    }

    char min1[2], min2[2];
    for (int i = 14; i < 16; ++i)
    {
        min1[i = 14] = time1[i];
        min2[i - 14] = time2[i];
    }
    int diff_min = atoi(min1) - atoi(min2);
    if (diff_min != 0)
    {
        return diff_min;
    }

    char sec1[2], sec2[2];
    for (int i = 17; i < 19; ++i)
    {
        sec1[i - 17] = time1[i];
        sec2[i - 17] = time2[i];
    }
    int diff_sec = atoi(sec1) - atoi(sec2);
    if (diff_sec != 0)
    {
        return diff_sec;
    }

    return 0;
}

int parse_date(char *time1)
{
    char year1[4];
    for (int i = 6; i < 10; ++i)
    {
        year1[i - 6] = time1[i];
    }
    char mon1[2];
    for (int i = 3; i < 5; ++i)
    {
        mon1[i - 3] = time1[i];
    }
    char day1[2];
    for (int i = 0; i < 2; ++i)
    {
        day1[i] = time1[i];
    }
    char hour1[2];
    for (int i = 11; i < 13; ++i)
    {
        day1[i - 11] = time1[i];
    }
    char min1[2];
    for (int i = 14; i < 16; ++i)
    {
        min1[i - 14] = time1[i];
    }
    char sec1[2];
    for (int i = 17; i < 19; ++i)
    {
        sec1[i - 17] = time1[i];
    }

    int day = atoi(day1);
    int month = atoi(mon1);
    int year = atoi(year1);
    int hour = atoi(hour1);
    int minute = atoi(min1);
    int second = atoi(sec1);

    int days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (year % 400 == 0 || (year % 100 != 0 && year % 4 == 0))
    {
        days_in_month[1] = 29;
    }

    struct tm timeinfo = {.tm_sec = second, .tm_min = minute, .tm_hour = hour, .tm_mday = day, .tm_mon = month - 1, .tm_year = year - 1900};
    time_t time = mktime(&timeinfo);

    return time;
}

int mail_cmp(const void *a, const void *b)
{
    const Mail *mail1 = (const Mail *)a;
    const Mail *mail2 = (const Mail *)b;

    int p_id1 = to_int(mail1->adress.person_id);
    int p_id2 = to_int(mail2->adress.person_id);

    if (p_id1 != p_id2)
    {
        return p_id1 < p_id2;
    }

    int mail_id1 = to_int(mail1->mail_id);
    int mail_id2 = to_int(mail2->mail_id);

    return mail_id1 < mail_id2;
}

int time_cmp(const void *a, const void *b)
{
    const Mail *mail1 = (const Mail *)a;
    const Mail *mail2 = (const Mail *)b;

    int time1 = parse_date(mail1->time_create.str);
    int time2 = parse_date(mail2->time_create.str);

    return time1 > time2;
}

STATUS post_constr(Post *post, Adress adress)
{

    post->current_adress = adress;
    post->capacity = 15;
    post->size = 0;
    post->mails = (Mail *)malloc(sizeof(Mail) * post->capacity);
    if (post->mails == NULL)
    {
        return MEMORY_ERROR;
    }
    return OK;
}

STATUS add_mail(Post *post, Mail mail)
{
    post->mails[post->size] = mail;
    post->size++;
    if (post->size + 1 >= post->capacity)
    {
        post->capacity *= 2;
        Mail *tmp = (Mail *)realloc(post->mails, sizeof(Mail) * post->capacity);
        if (tmp == NULL)
        {
            return MEMORY_ERROR;
        }
        post->mails = tmp;
    }
    qsort(post->mails, post->size, sizeof(Mail), mail_cmp);
    return OK;
}

void free_mail(Mail *mail)
{
    mail->is_delete = 1;
    clear_str(&(mail->time_create));
    clear_str(&(mail->time_deliver));
    clear_str(&(mail->mail_id));
    clear_adress(&(mail->adress));
}

int binary_search_by_pers_id(Post post, int key)
{
    if (post.size == 0)
    {
        return -1;
    }
    if (post.size < 5)
    {
        for (int i = 0; i < post.size; ++i)
        {
            if (post.mails[i].is_delete == 1)
            {
                continue;
            }
            if (to_int(post.mails[i].adress.person_id) == key)
            {
                return i;
            }
        }
        return -1;
    }
    int left = 0;
    int right = post.size;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (post.mails[mid].is_delete == 1)
            continue;
        if (to_int(post.mails[mid].adress.person_id) == key)
        {
            return mid;
        }
        else if (to_int(post.mails[mid].adress.person_id) < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    // if element doesn't exist
    return -1;
}

int binary_search_by_mail_id(Post post, __int64_t key)
{
    if (post.size == 0)
    {
        return -1;
    }
    if (post.size < 5)
    {
        for (int i = 0; i < post.size; ++i)
        {
            if (post.mails[i].is_delete == 1)
            {
                continue;
            }

            if (to_int(post.mails[i].mail_id) == key)
            {
                return i;
            }
        }
        return -1;
    }
    int left = 0;
    int right = post.size;
    while (left <= right)
    {
        int mid = left + (right - left) / 2;
        if (post.mails[mid].is_delete == 1)
            continue;

        if (to_int(post.mails[mid].mail_id) == key)
        {
            return mid;
        }
        else if (to_int(post.mails[mid].mail_id) < key)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }

    // if element doesn't exist
    return -1;
}

STATUS delete_mail_by_personal_id(Post *post, char *_id)
{
    if (strlen(_id) != 6)
    {
        return INVALID_DATA;
    }
    if (post->size == 0)
    {
        return NO_ONE_FOUND;
    }
    int id = atoi(_id);
    int index_to_delete = binary_search_by_pers_id(*post, id);
    if (index_to_delete == -1)
    {
        return NO_ONE_FOUND;
    }
    free_mail(&(post->mails[index_to_delete]));
    for (int i = index_to_delete; i < post->size - 1; ++i)
    {
        post->mails[i] = post->mails[i + 1];
    }
    post->size--;
    return OK;
}

STATUS find_mail_by_mail_id(Post *post, char *_id, Mail *mail)
{
    if (post == NULL || strlen(_id) != 14) {
        return INVALID_DATA;
    }

    if (post->size == 0)
    {
        return NO_ONE_FOUND;
    }
    __int64_t id = atol(_id);
    int found_index = binary_search_by_mail_id(*post, id);
    if (found_index == -1)
    {
        return NO_ONE_FOUND;
    }
    *mail = post->mails[found_index];
    return OK;
}

STATUS read_adress(Post post, Adress *adress)
{

    char *city = (char *)malloc(sizeof(char) * 32 + 1);
    if (city == NULL)
    {
        return MEMORY_ERROR;
    }
    char *street = (char *)malloc(sizeof(char) * 32 + 1);
    if (street == NULL)
    {
        return MEMORY_ERROR;
    }
    char *campus = (char *)malloc(sizeof(char) * 32 + 1);
    if (campus == NULL)
    {
        return MEMORY_ERROR;
    }
    char *personal_id = (char *)malloc(sizeof(char) * 32 + 1);
    if (personal_id == NULL)
    {
        return MEMORY_ERROR;
    }
    char *home_number = (char *)malloc(sizeof(char) * 32 + 1);
    if (home_number == NULL)
    {
        return MEMORY_ERROR;
    }
    printf("Input your city:\n");
    fscanf(stdin, "%s", city);
    if (is_str(city) == false)
    {
        free(city);
        free(street);
        free(campus);
        free(home_number);
        free(personal_id);
        return INVALID_DATA;
    }
    init_str(&(adress->city), city);
    free(city);

    printf("Input street:\n");
    fscanf(stdin, "%s", street);
    if (is_str(street) == false)
    {
        free(street);
        free(campus);
        free(home_number);
        free(personal_id);
        return INVALID_DATA;
    }
    init_str(&(adress->street), street);
    free(street);

    printf("Input building:\n");
    fscanf(stdin, "%s", campus);
    if (is_str(campus) == false)
    {
        free(campus);
        free(home_number);
        free(personal_id);
        return INVALID_DATA;
    }
    init_str(&(adress->building), campus);
    free(campus);

    printf("Input home number:\n");
    fscanf(stdin, "%s", home_number);
    if (is_num(home_number) == false || atoi(home_number) <= 0 || strlen(home_number) > 4)
    {
        free(home_number);
        free(personal_id);
        return INVALID_DATA;
    }
    adress->house_number = atoi(home_number);
    free(home_number);

    printf("Input personal id (only 6 symbols):\n");
    fscanf(stdin, "%s", personal_id);
    if (is_num(personal_id) == false || strlen(personal_id) != 6)
    {
        free(personal_id);
        return INVALID_DATA;
    }
    if (post.size > 0)
    {
        int is_exist = binary_search_by_pers_id(post, atoi(personal_id));
        if (is_exist != -1)
        {
            free(personal_id);
            return ALREADY_EXIST;
        }
    }
    init_str(&(adress->person_id), personal_id);
    free(personal_id);

    return OK;
}

STATUS read_mail(Post post, Mail *mail)
{
    mail->is_delete = 0;
    printf("Firstly, you need to input your adress.\n");
    int stat = read_adress(post, &(mail->adress));
    if (stat != OK)
    {
        return stat;
    }

    char *mail_id = (char *)malloc(sizeof(char) * 32 + 1);
    if (mail_id == NULL)
    {
        return MEMORY_ERROR;
    }
    printf("Input mail id(only 14 symbols):\n");
    fscanf(stdin, "%s", mail_id);
    if (is_num(mail_id) != 0 && strlen(mail_id) != 14)
    {
        free(mail_id);
        return INVALID_DATA;
    }
    int is_exist = binary_search_by_mail_id(post, atol(mail_id));
    if (is_exist != -1)
    {
        free(mail_id);
        return ALREADY_EXIST;
    }
    init_str(&(mail->mail_id), mail_id);
    free(mail_id);

    double weight;
    printf("Input weight:\n");
    fscanf(stdin, "%lf", &weight);
    if (weight > 1000.0)
    {
        return INVALID_DATA;
    }
    mail->weight = weight;
    char *time1 = set_time(0), *time2 = set_time(2);
    init_str(&(mail->time_create), time1);
    free(time1);
    init_str(&(mail->time_deliver), time2);
    free(time2);

    return OK;
}

STATUS print_mail(Mail mail)
{
    printf("Mail:\nmail_id: %s\nweight: %.2lf\ntime create: %s\n", mail.mail_id.str, mail.weight, mail.time_create.str);
    printf("time deliver: %s\n", mail.time_deliver.str);
    printf("personal id: %s city: %s  street: %s, %s,%d\n", mail.adress.person_id.str, mail.adress.city.str, mail.adress.street.str, mail.adress.building.str, mail.adress.house_number);
    return OK;
}

int main(int argc, char *argv[])
{
    // String string1, string2;
    // init_str(&string1, "abcd");
    // string_cpy(&string2, &string1);
    // if (is_equal(string1, string2) == 0) {
    //     printf("str is equal\n");
    // }
    // String* string3 = (String*)malloc(sizeof(String));
    // if (string3 == NULL) {
    //     response(MEMORY_ERROR);
    //     return 1;
    // }
    // status stat = concatenate_string(string1, string2, string3);
    // if (stat != 0) {
    //     return 1;
    // }
    // clear_str(&string2);
    // printf("S: %s; %d - len\n", string3->str, string3->size);
    // printf("S2: %s; %d - len\n", string2.str, string2.size);

    Post post;
    Adress adress;
    post_constr(&post, adress);
    printf("Input your adress: ");
    char buf[4096];
    while (1)
    {
        printf("\nInput action:\n[ 1 ] - add mail to Post;\n[ 2 ] - delete mail from Post;\n[ 3 ] - Find mail by mail_id\n");
        printf("[ 4 ] - find mail that alredy has delivered;\n[ 5 ] - find mail that hasn't delivered yet;\n");
        printf("[ 0 ] - QUIT\n");
        fscanf(stdin, "%s", buf);
        if (buf[0] == '0')
        {
            break;
        }
        if (strlen(buf) > 2)
        {
            printf("\nINVALID ACTION.\n");
            continue;
        }
        int fl = buf[0] - '0';
        if (fl == 1)
        {
            Mail mail;
            printf("Input your Mail.\n");
            STATUS read_m = read_mail(post, &mail);
            if (read_m != OK)
            {
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                response(read_m);
                return 1;
            }

            STATUS stat = add_mail(&post, mail);
            if (stat != OK)
            {
                response(stat);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            printf("Mail was added.");
        }
        else if (fl == 2)
        {
            if (post.size == 0)
            {
                printf("\nPost is empty.\n");
                continue;
            }
            printf("Please, input personal id mail, you want to delete:\n");
            char *personal_id = (char *)malloc(sizeof(char) * (32 + 1));
            if (personal_id == NULL)
            {
                response(MEMORY_ERROR);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            scanf("%s", personal_id);
            if (strlen(personal_id) != 6 || !is_num(personal_id))
            {
                response(INVALID_DATA);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                free(personal_id);
                return 1;
            }

            STATUS del_mail = delete_mail_by_personal_id(&post, personal_id);
            if (del_mail != OK)
            {
                response(del_mail);
                free(personal_id);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            printf("Mail with pers_id:[%s] delete successfully.\n", personal_id);

            free(personal_id);
        }
        else if (fl == 3)
        {
            if (post.size == 0)
            {
                printf("\nPost is empty.\n");
                continue;
            }
            printf("Please, input [mail id], you want to find:\n");
            char *mail_id = (char *)malloc(sizeof(char) * (32 + 1));
            if (mail_id == NULL)
            {
                response(MEMORY_ERROR);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            fscanf(stdin, "%s", mail_id);
            if (strlen(mail_id) != 14 || !is_num(mail_id))
            {
                response(INVALID_DATA);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            Mail mail;
            STATUS find_by_id = find_mail_by_mail_id(&post, mail_id, &mail);
            if (find_by_id != OK && find_by_id != NO_ONE_FOUND)
            {
                response(find_by_id);
                for (int i = 0; i < post.size; ++i)
                {
                    free_mail(&(post.mails[i]));
                }
                free(post.mails);
                return 1;
            }
            if (find_by_id == NO_ONE_FOUND)
            {
                response(find_by_id);
                continue;
            }
            print_mail(mail);
            free(mail_id);
        }
        else if (fl == 4)
        {
            if (post.size == 0)
            {
                printf("\nPost is empty.\n");
                continue;
            }
            int no_one = 1;
            for (int i = 0; i < post.size; ++i) 
            {
                if (post.mails[i].is_delete == 1)
                {
                    continue;
                }
                char *deliver_time = post.mails[i].time_deliver.str;
                char *curr_time = set_time(0);
                int sec1 = parse_date(curr_time);
                free(curr_time);
                if (sec1 == INVALID_DATA)
                {
                    response(INVALID_DATA);
                    continue;
                }
                int sec2 = parse_date(deliver_time);
                if (sec2 == INVALID_DATA)
                {
                    printf("inval data\n");
                    continue;
                }
                int differ = sec1 - sec2;
                if (differ > 0)
                {
                    no_one = 0;
                    print_mail(post.mails[i]);
                }
            }
            if (no_one)
            {
                printf("\nNo one posts was delivered\n");
            }
        }
        else if (fl == 5)
        {
            if (post.size == 0)
            {
                printf("\nPost is empty.\n");
                continue;
            }
            Post _post = post;
            qsort(_post.mails, _post.size, sizeof(Mail), time_cmp);
            for (int i = 0; i < _post.size; ++i)
            {
                if (_post.mails[i].is_delete == 1)
                {
                    continue;
                }
                char *deliver_time = post.mails[i].time_deliver.str;
                char *curr_time = set_time(0);
                int sec1 = parse_date(curr_time);
                free(curr_time);
                if (sec1 == INVALID_DATA)
                {
                    printf(INVALID_DATA);
                    continue;
                }
                int sec2 = parse_date(deliver_time);
                if (sec2 == INVALID_DATA)
                {
                    response(INVALID_DATA);
                    continue;
                }
                int differ = sec1 - sec2;
                if (differ <= 0)
                {
                    print_mail(_post.mails[i]);
                    printf("\n");
                }
            }
        }
    }

    for (int i = 0; i < post.size; ++i)
    {
        free_mail(&(post.mails[i]));
    }
    free(post.mails);
    response(OK);
    return 0;
}