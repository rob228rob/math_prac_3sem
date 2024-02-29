#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int tdata;

typedef struct list_node {
    tdata data;
    struct list_node* next;
    struct list_node* prev;
} list_node;

typedef struct linked_list {
    size_t size;
    struct list_node* head;
    struct list_node* tail;
} linked_list;

list_node * create_node(tdata data) {
    list_node * new_node = (list_node*)malloc(sizeof(list_node));
    if (new_node == NULL) return NULL;
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;

    return new_node;
}

linked_list * create_list() {
    linked_list * new_list = (linked_list *)malloc(sizeof(linked_list));
    if (new_list == NULL) {
        return NULL;
    }
    new_list->size = 0;
    new_list->head = NULL;
    new_list->tail = NULL;

    return new_list;
}

int is_empty(linked_list* list) {
    return (list->size == 0);
}

int push_front(linked_list* list, tdata data)  {
    if (list == NULL) {
        return 2;
    }

    if (list->size == 0) {
        list_node* node = create_node(data);
        if (node == NULL) return 1;
        list->size++;
        list->head = node;
        list->tail = node;

        return 0;
    }

    list_node* node = create_node(data);
    if (node == NULL) return 1;

    list->size++;
    list->head->prev = node;   
    node->next = list->head;
    list->head = node;

    return 0;
}

int push_back(linked_list* list, tdata data) {
    if (list == NULL) {
        return 2;
    }

    if (list->size == 0) {
        list_node* node = create_node(data);
        if (node == NULL) return 1;
        list->size++;
        list->head = node;
        list->tail = node;

        return 0;
    }

    list_node* node = create_node(data);
    if (node == NULL) return 1;

    list->size++;
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;

    return 0;
}

int pop_first(linked_list* list) {
    if (list == NULL || list->size == 0) {
        return 2;
    }

    if (list->size == 1) {
        list_node* to_del = list->tail;
        free(to_del);
        list->head = NULL;
        list->tail = NULL;
        list->size--;

        return 0;
    }

    list_node* to_delete = list->head;
    list->head = list->head->next; 
    list->size--;
    free(to_delete);

    return 0;
}

int pop_last(linked_list* list) {
    if (list == NULL || list->size == 0) {
        return 2;
    }

    if (list->size == 1) {
        list_node* to_del = list->tail;
        free(to_del);
        list->head = NULL;
        list->tail = NULL;
        list->size--;
        return 0;
    }
    //TODO: fix pop error(seg fault);
    //RESULT: fixed; problem was in (size++) in pushback
    list_node* to_delete = list->tail;
    list->tail = list->tail->prev;
    list->tail->next = NULL; 
    list->size--;
    free(to_delete);
    return 0;
}

int delete_by_value(linked_list* list, tdata data) {
    if (list == NULL) return 2;

    list_node* to_delete = list->head;
    while (to_delete->next != list->tail->next)
    {
        if (to_delete->data == data && to_delete != list->tail && to_delete != list->head) {
            to_delete->prev->next = to_delete->next;
            to_delete->next->prev = to_delete->prev;
            list->size--;
            free(to_delete);

            return 0;
        } 
        else if (to_delete->data == data && to_delete == list->head) {
            list->head = to_delete->next;
            list->head->prev = NULL;
            list->size--;
            free(to_delete);
            
            return 0;
        }
        to_delete = to_delete->next;
    }

    if (to_delete->data == data && to_delete == list->tail) {
        list->tail = to_delete->prev;
        list->tail->next = NULL;
        list->size--;
        free(to_delete);

        return 0;
    }

    //NO ONE FOUND
    return 1;
}

int delete_list(linked_list* list) {
    if (list == NULL) return 1;

    list_node* curr = list->head;
    while (curr->next != list->tail->next)
    {
        list_node* to_delete = curr;
        curr = curr->next;
        free(to_delete);
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return 0;
}


void print_list(linked_list* list) {
    if (list == NULL) return;
    if (list->size == 0) {
        printf("List is empty\n");
        return;
    }
    list_node* curr = list->head;
    while (curr->next != list->tail->next)
    {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("%d\n", curr->data);
}

int main() {
    linked_list* list = create_list();
    for (int i = 0; i < 10; ++i) {
        push_front(list, i*i);
    }
    print_list(list);
    int temp = delete_by_value(list, 36);
    if (temp == 1) printf("NO ONE FOUND\n");
    delete_by_value(list, 49);
    delete_by_value(list, 50);
    delete_by_value(list, 81);
    delete_by_value(list, 0);
    delete_by_value(list, 1);
    push_back(list, 123);
    pop_first(list);
    push_front(list, 8111);
    pop_last(list);
    print_list(list);
    delete_list(list);
    if (is_empty(list)) {
        printf("list is empty\n, size: %ld ", list->size);
    }
    
    free(list);
    return 0;
}