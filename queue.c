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

    element_t *q_head = NULL, *q_next = NULL;

    list_for_each_entry_safe(q_head, q_next, head, list)
        q_release_element(q_head);
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
    if (!item->value) {
        fprintf(stderr, "malloc failed\n");
        free(item);
        return false;
    }

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
    // strdup also allocates memory space
    // Need to check if allocation fails after calling strdup()
    if (!item->value) {
        fprintf(stderr, "malloc failed\n");
        free(item);
        return false;
    }

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
    q_release_element(remove_entry);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
err:
    return false;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head) {
        return false;
    }

    element_t *it = NULL, *safe = NULL;
    struct list_head *pending = malloc(sizeof(struct list_head));
    bool flag = false;

    INIT_LIST_HEAD(pending);
    list_for_each_entry_safe(it, safe, head, list) {
        if (&safe->list != head && strcmp(it->value, safe->value) != 0) {
            if (flag) {
                list_del(&it->list);
                list_add_tail(&it->list, pending);
                flag = false;
            }
            continue;
        }
        if (it->list.next != head) {
            list_del(&it->list);
            list_add_tail(&it->list, pending);
            flag = true;
        }
    }

    q_free(pending);
    /*list_for_each_entry_safe(it, safe, pending, list) {
        q_release_element(it);
    }*/

    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    return true;
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    struct list_head *l1 = NULL, *l2 = NULL;

    if (!head) {
        return;
    }

    list_for_each_safe(l1, l2, head) {
        if (l2 == head) {
            break;
        }
        list_del(l1);
        list_add(l1, l2);
        l2 = l1->next;
    }

    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head)) {
        return;
    }
    const struct list_head *list_first = head->next;
    struct list_head *move_node = head->prev;
    struct list_head *ptr = head;  // Pointer to the list_head structure after
                                   // which to add the new node.

    while (move_node != list_first) {
        list_del(move_node);
        list_add(move_node, ptr);
        ptr = move_node;
        move_node = head->prev;
    }
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    LIST_HEAD(tmp_list);
    LIST_HEAD(final_list);

    struct list_head *it, *safe;
    int cnt = 0;
    list_for_each_safe(it, safe, head) {
        cnt++;
        if (cnt == k) {
            list_cut_position(&tmp_list, head, it);
            q_reverse(&tmp_list);
            list_splice_tail_init(&tmp_list, &final_list);
            cnt = 0;
        }
    }
    list_splice(&final_list, head);

    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Get the last node for left partition*/
struct list_head *get_middle_list_node(struct list_head *head)
{
    if (list_empty(head) || list_is_singular(head)) {
        return head;
    }
    struct list_head *left, *right;
    left = head->next;
    right = head->prev;

    while (right != left && left->next != right) {
        left = left->next;
        right = right->prev;
    }
    return left;
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head) || list_is_singular(head)) {
        return;
    }

    struct list_head *middle_node = get_middle_list_node(head);

    // Do partition and set middle node as the last node of left part
    LIST_HEAD(left);
    LIST_HEAD(right);
    list_cut_position(&left, head, middle_node);
    list_splice_init(head, &right);

    q_sort(&left, descend);
    q_sort(&right, descend);

    // Merge 2 partitions
    while (!list_empty(&left) && !list_empty(&right)) {
        element_t *element_in_A = list_first_entry(&left, element_t, list);
        element_t *element_in_B = list_first_entry(&right, element_t, list);
        if (descend) {
            if (strcmp(element_in_A->value, element_in_B->value) >= 0) {
                list_move_tail(&element_in_A->list, head);
            } else {
                list_move_tail(&element_in_B->list, head);
            }
        } else {
            if (strcmp(element_in_A->value, element_in_B->value) <= 0) {
                list_move_tail(&element_in_A->list, head);
            } else {
                list_move_tail(&element_in_B->list, head);
            }
        }
    }
    if (!list_empty(&left)) {
        list_splice_tail_init(&left, head);
    } else if (!list_empty(&right)) {
        list_splice_tail_init(&right, head);
    }
}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    element_t *left = NULL, *right = NULL;
    struct list_head *pending;

    if (!head || list_empty(head) || list_is_singular(head)) {
        return 0;
    }

    pending = malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(pending);

    list_for_each_entry_safe(left, right, head, list) {
        while (&left->list != head && &right->list != head &&
               strcmp(right->value, left->value) < 0) {
            list_del(&left->list);
            list_add_tail(&left->list, pending);
            left = list_entry(right->list.prev, element_t, list);
        }
    }

    q_free(pending);

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    element_t *left = NULL, *right = NULL;
    struct list_head *pending;

    if (!head || list_empty(head) || list_is_singular(head)) {
        return 0;
    }

    pending = malloc(sizeof(struct list_head));

    INIT_LIST_HEAD(pending);

    list_for_each_entry_safe(left, right, head, list) {
        while (&left->list != head && &right->list != head &&
               strcmp(right->value, left->value) > 0) {
            list_del(&left->list);
            list_add_tail(&left->list, pending);
            left = list_entry(right->list.prev, element_t, list);
        }
    }

    q_free(pending);

    return q_size(head);
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
