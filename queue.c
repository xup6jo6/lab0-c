#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head =
        (struct list_head *) malloc(sizeof(struct list_head));

    if (!head)
        return NULL;

    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    if (list_empty(head)) {
        free(head);
        return;
    }
    // element_t *q_head = list_entry(head, element_t, struct list_head);
    element_t *q_head = NULL, *q_next = NULL;

    list_for_each_entry_safe(q_head, q_next, head, list)
        free(q_head);
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    element_t *item = malloc(sizeof(element_t));

    if (!item)
        return false;

    if (!head) {
        free(item);
        return false;
    }

    item->value = strdup(s);

    list_add(&item->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    element_t *item = malloc(sizeof(element_t));
    if (!item)
        return false;

    if (!head) {
        free(item);
        return false;
    }

    item->value = strdup(s);

    list_add_tail(&item->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *tmp_remove = list_first_entry(head, element_t, list);
    list_del(&tmp_remove->list);
    if (sp) {
        strncpy(sp, tmp_remove->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return tmp_remove;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head)) {
        return NULL;
    }
    element_t *tmp_remove = list_last_entry(head, element_t, list);
    list_del(&tmp_remove->list);
    if (sp) {
        strncpy(sp, tmp_remove->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    return tmp_remove;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    struct list_head *iter = NULL;
    int cnt = 0;

    if (!head || list_empty(head)) {
        return cnt;
    }

    list_for_each(iter, head) {
        cnt++;
    }

    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    struct list_head *left, *right;
    element_t *remove_entry;

    if (!head || list_empty(head)) {
        goto err;
    }

    right = head->prev;
    left = head->next;
    while (right != left && left->next != right) {
        left = left->next;
        right = right->prev;
    }
    list_del(right);
    remove_entry = list_entry(right, element_t, list);
    free(remove_entry);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
err:
    return false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) {}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
