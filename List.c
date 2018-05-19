#include "List.h"

List* create_list(unsigned size) {
    List *l = calloc(1, sizeof(List));
    if (l == NULL)
        check_error(OUT_OF_MEMORY_EXCEPTION);
    while (size > 0) {
        check_error(push_front(l, 0));
        size--;
    }
    return l;
}

List* string_to_list(char *str) {
    List *l = create_list(0);
    int len = strlen(str), i;
    if (len != 0 && str[len - 1] == '\n')
        len--;
    for (i=0; i<len; i++) {
        if (str[i] < 48 || str[i] > 57)
            return create_list(1);
        check_error(push_front(l, str[i] - 0x30));
    }
    return l;
}

List* list_cpy(List *original) {
    List *new_list = create_list(0);
    Node *head = original->HEAD;
    while (head) {
        push_back(new_list, head->data);
        head = head->next;
    }
    return new_list;
}

ErrorCode push_front(List *list, char data) {
    Node *new = malloc(sizeof(Node));
    if (new == NULL)
        return OUT_OF_MEMORY_EXCEPTION;
    new->data = data;
    new->next = list->HEAD;
    list->HEAD = new;
    list->count++;
    return ALL_GOOD;
}

ErrorCode push_back(List *list, char data) {
    Node **HEAD = &(list->HEAD);
    while (*HEAD)
        HEAD = &(*HEAD)->next;
    *HEAD = malloc(sizeof(**HEAD));
    if (!HEAD)
        return OUT_OF_MEMORY_EXCEPTION;
    (*HEAD)->data = data;
    (*HEAD)->next = NULL;
    list->count++;
    return ALL_GOOD;
}

char pop_front(List *list) {
    Node *tmp = list->HEAD;
    char value;
    if (!tmp)
        return LIST_EMPTY_EXCEPTION;
    list->HEAD = list->HEAD->next;
    value = tmp->data;
    free(tmp);
    list->count--;
    return value;
}

char pop_back(List *list) {
    Node **HEAD = &(list->HEAD);
    char value;
    while ((*HEAD)->next)
        HEAD = &(*HEAD)->next;
    value = (*HEAD)->data;
    free(*HEAD);
    *HEAD = NULL;
    list->count--;
    return value;
}

Node* get_by_index(List *list, unsigned index) {
    Node* result = list->HEAD;
    int i = 0;
    if (index > list->count-1 || list->count == 0 || index < 0) {
        check_error(INDEX_OUT_OF_RANGE_EXCEPTION);
        return NULL;
    }
    while (i != index && result) {
        result = result->next;
        i++;
    }
    return result;
}

ErrorCode insert(List *list, unsigned index, char data) {
    Node *new = NULL, **HEAD = &(list->HEAD);
    int i = 0;
    if (index < 0 || index >= list->count)
        return INDEX_OUT_OF_RANGE_EXCEPTION;
    while (*HEAD && i != index - 1) {
        HEAD = &(*HEAD)->next;
        i++;
    }
    new = malloc(sizeof(Node));
    if (new == NULL)
        return OUT_OF_MEMORY_EXCEPTION;
    new->data = data;
    new->next = *HEAD;
    *HEAD = new;
    list->count++;
    return ALL_GOOD;
}

ErrorCode delete_by_index(List *list, unsigned index) {
    struct Node* tmp = NULL, **HEAD = &(list->HEAD);
    int i = 0;
    if (index < 0 || index >= list->count)
        return INDEX_OUT_OF_RANGE_EXCEPTION;
    while (*HEAD && i != index - 1) {
        HEAD = &(*HEAD)->next;
        i++;
    }
    tmp = *HEAD;
    *HEAD = (*HEAD)->next;
    list->count--;
    free(tmp);
    return ALL_GOOD;
}

char* list_to_string (List *list, char optional) {
    char *str;
    int i = 0;
    if (optional != '-')
        str = malloc((list->count + 1) * sizeof(char));
    else {
        str = malloc((list->count + 2) * sizeof(char));
        i++;
        str[0] = optional;
    }
    if (str == NULL) {
        check_error(OUT_OF_MEMORY_EXCEPTION);
        return NULL;
    }
    Node **HEAD = &(list->HEAD);
    while (*HEAD) {
        str[i] = (*HEAD)->data + 0x30;
        HEAD = &(*HEAD)->next;
        i++;
    }
    str[i] = '\0';
    list = reverse_list(list);
    return str;
}

void free_list(List *list) {
    Node *head = list->HEAD, *tmp = NULL;
    while (head) {
        tmp = head->next;
        free(head);
        head = tmp;
    }
    free(list);
}

void check_error(ErrorCode code)  {
    if (code)
        exit(code);
    return;
}

List* get_longer(List *a, List *b, int *length) {
    *length = MAX(a->count, b->count);
    return (a->count == *length) ? a : b;
}

List* get_shorter(List *a, List *b, int *length) {
    *length = MIN(a->count, b->count);
    return (b->count == *length) ? b : a;
}

List* reverse_list(List *list) {
    Node *head = list->HEAD;
    Node *new_head = NULL;
    while (head) {
        Node* next = head->next;
        head->next = new_head;
        new_head = head;
        head = next;
    }
    list->HEAD = new_head;
    return list;
}

bool is_empty(List a) {
    return a.count == 0 || a.HEAD == NULL;
}

void print_list(List a) {
    Node *HEAD = a.HEAD;
    while (HEAD) {
        putchar(HEAD->data);
        HEAD = HEAD->next;
    }
}
