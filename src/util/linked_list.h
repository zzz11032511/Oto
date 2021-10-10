#pragma once

#include <stdint.h>

struct str_node {
    int8_t *item;
    struct str_node *next;
};

struct str_list {
    struct str_node *head;
    struct str_node *tail;
};

struct str_list *new_str_list();
void free_str_list(struct str_list *list);

void append_str_item(struct str_list *list, int8_t *item);
void remove_head_str_item(struct str_list *list);
void remove_tail_str_item(struct str_list *list);
bool is_exist_str_item(struct str_list *list, int8_t *str);
void print_str_item_all(struct str_list *list);
size_t count_str_num_of_item(struct str_list *list);