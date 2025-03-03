#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *new_queue = malloc(sizeof(struct list_head));

    /* check malloc */
    if (!new_queue) {
        return NULL;
    }

    INIT_LIST_HEAD(new_queue);
    return new_queue;
}


/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    if (!head) {
        return;
    }

    struct list_head *node, *safe;

    list_for_each_safe (node, safe, head) {
        list_del(node);
        q_release_element(container_of(node, element_t, list));
    }
    free(head);
}


/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_el = malloc(sizeof(element_t));
    if (!new_el)
        return false;
    INIT_LIST_HEAD(&new_el->list);
    new_el->value = strdup(s);
    if (!new_el->value) {
        free(new_el);
        return false;
    }
    list_add(&(new_el->list), head);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head || !s)
        return false;
    element_t *new_el = malloc(sizeof(element_t));
    if (!new_el)
        return false;
    INIT_LIST_HEAD(&new_el->list);
    new_el->value = strdup(s);
    if (!new_el->value) {
        free(new_el);
        return false;
    }
    list_add_tail(&(new_el->list), head);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->next == head)
        return NULL;
    element_t *el = container_of(head->next, element_t, list);
    if (!sp && !el->value)
        return NULL;
    strncpy(sp, el->value, bufsize);
    list_del(&(el->list));
    return el;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->prev == head)
        return NULL;
    element_t *el = container_of(head->prev, element_t, list);
    if (!sp && !el->value)
        return NULL;
    strncpy(sp, el->value, bufsize);
    list_del(&(el->list));
    return el;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int cnt = 0;
    struct list_head *pos;
    list_for_each (pos, head)
        cnt++;
    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (head == NULL || head->next == head)
        return false;
    struct list_head *slow = head->next;
    struct list_head *fast = head->next;
    while (fast != head && fast->next != head) {
        fast = fast->next->next;
        slow = slow->next;
    }

    element_t *el = container_of(slow, element_t, list);
    list_del(slow);
    q_release_element(el);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (head == NULL || list_empty(head)) {
        return false;
    }
    bool del = false;
    struct list_head *cur, *nxt;
    list_for_each_safe (cur, nxt, head) {
        element_t *cur_el = container_of(cur, element_t, list);
        const element_t *nxt_el = container_of(nxt, element_t, list);
        // last node is duplicate
        if (nxt == head) {
            if (del) {
                list_del(cur);
                free(cur_el->value);
                free(cur_el);
            }
            break;
        }
        // detect duplicate
        if (strcmp(cur_el->value, nxt_el->value) == 0) {
            del = true;
            list_del(cur);
            free(cur_el->value);
            free(cur_el);
        }
        // last duplicate
        else if (del) {
            list_del(cur);
            free(cur_el->value);
            free(cur_el);
            del = false;
        }
    }
    return true;
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *cur = head->next;
    while (cur != head && cur->next != head) {
        struct list_head *nxt = cur->next->next;
        struct list_head *adj = cur->next;
        list_move(adj, cur->prev);
        cur = nxt;
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    if (!head || head->next == head)
        return;
    struct list_head *current = head;
    struct list_head *temp;

    do {
        temp = current->next;
        current->next = current->prev;
        current->prev = temp;
        current = current->prev;
    } while (current != head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // Handle edge cases
    if (head == NULL || list_empty(head) || k <= 1) {
        return;
    }

    // Prepare for the first group
    struct list_head *group_prev = head;
    struct list_head *group_start = head->next;

    // Iterate through the list processing k nodes at a time
    while (1) {
        // Check if we have k nodes left
        struct list_head *check = group_start;
        int count = 0;
        while (check != head && count < k) {
            check = check->next;
            count++;
        }

        // If we don't have k nodes left, stop
        if (count < k)
            break;
        // Initialize pointers for reversing
        struct list_head *group_end = group_start;
        struct list_head *current = group_start;
        struct list_head *next;

        // Move to the end of the current group
        for (int i = 0; i < k - 1; i++) {
            group_end = group_end->next;
        }

        // Next group will start after this group
        struct list_head *group_next = group_end->next;

        // Reverse the k nodes in this group
        for (int i = 0; i < k; i++) {
            next = current->next;
            list_move(current, group_prev);
            // Move to the next node
            current = next;
        }

        // Update group_prev and group_start for the next group
        group_prev = group_start;  // After reversal, the original group_start
                                   // is now at the end of the reversed group
        group_start = group_next;
    }
}

void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *pos, *tmp;

    /* Traverse the list and remove nodes that are smaller than any node that
     * comes after them */
    list_for_each_safe (pos, tmp, head) {
        bool del = false;
        element_t *el_pos = container_of(pos, element_t, list);
        for (struct list_head *cur = pos->next; cur != head; cur = cur->next) {
            const element_t *el_cur = container_of(cur, element_t, list);
            if (strcmp(el_cur->value, el_pos->value) < 0) {
                del = true;
                break;
            }
        }
        if (del) {
            list_del(pos);
            q_release_element(el_pos);
        }
    }

    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    struct list_head *pos, *tmp;

    /* Traverse the list and remove nodes that are smaller than any node that
     * comes after them */
    list_for_each_safe (pos, tmp, head) {
        bool del = false;
        element_t *el_pos = container_of(pos, element_t, list);
        for (struct list_head *cur = pos->next; cur != head; cur = cur->next) {
            const element_t *el_cur = container_of(cur, element_t, list);
            if (strcmp(el_cur->value, el_pos->value) > 0) {
                del = true;
                break;
            }
        }
        if (del) {
            list_del(pos);
            q_release_element(el_pos);
        }
    }

    return q_size(head);
}


/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
