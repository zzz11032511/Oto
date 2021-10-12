#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "util.h"
#include "linked_list.h"
#include "../error/error.h"

struct str_list *new_str_list() {
    struct str_list *list = (struct str_list *)calloc(1, sizeof(struct str_list));
    if (list == NULL) {
        call_error(UNKNOWN_ERROR, "new_str_list()");
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

struct str_node *new_str_node(int8_t *item) {
    struct str_node *node = (struct str_node *)calloc(1, sizeof(struct str_node));
    if (node == NULL) {
        call_error(UNKNOWN_ERROR, "new_str_list()");
    }

    node->item = item;
    node->next = NULL;

    return node;
}

void free_str_list(struct str_list *list) {
    while (list->head != NULL) {
        remove_tail_str_item(list);
    }
    free(list);
}

void append_str_item(struct str_list *list, int8_t *item) {
    struct str_node *new_node = new_str_node(item);

    if (list->head == NULL) {
        list->head = list->tail = new_node;
        return;
    }

    struct str_node *node = list->head;
    while (node->next != NULL) {
        node = node->next;
    }
    node->next = list->tail = new_node;
}

void remove_head_str_item(struct str_list *list) {
    if (list->head == NULL) {
        return;
    }

    struct str_node *oldhead = list->head;
    list->head = oldhead->next;
    free(oldhead);
}

void remove_tail_str_item(struct str_list *list) {
    if (list->head == NULL) {
        return;
    }

    if (list->head->next == NULL) {
        remove_head_str_item(list);
        return;
    }

    struct str_node *prenode = list->head;
    struct str_node *node    = list->head->next;
    while (node->next != NULL) {
        prenode = node;
        node = node->next;
    }

    free(node);
    prenode->next = NULL;
    list->tail = prenode;
}

bool is_exist_str_item(struct str_list *list, int8_t *str) {
    struct str_node *node = list->head;
    while (node != NULL && node->next != NULL) {
        if (strcmp(node->item, str) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

void print_str_item_all(struct str_list *list) {
    struct str_node *node = list->head;
    size_t n = 0;
    while (node != NULL) {
        printf("[%I64d] : %s\n", n, node->item);
        node = node->next;
        n++;
    }
    printf("Number of elements : %I64d\n", n);
}

size_t count_str_num_of_item(struct str_list *list) {
    struct str_node *node = list->head;
    size_t n = 0;
    while (node != NULL) {
        node = node->next;
        n++;
    }
    return n;
}

#if 0
    int main(void) {
        struct str_list *list = new_str_list();

        append_str_item(list, "AAAA");


        TEST_EQ(is_exist_str_item(list, "AAAA"), 1);
        TEST_EQ(is_exist_str_item(list, "BBBB"), 0);

        remove_tail_str_item(list);
        remove_tail_str_item(list);
        append_str_item(list, "BBBB");

        TEST_EQ(is_exist_str_item(list, "AAAA"), 0);
        TEST_EQ(is_exist_str_item(list, "BBBB"), 1);

        append_str_item(list, "AAAA");
        append_str_item(list, "CCCC");
        print_str_item_all(list);
        remove_tail_str_item(list);
        print_str_item_all(list);
        printf("Success.\n");
    }
#endif