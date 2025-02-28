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
    struct list_head *new = malloc(sizeof(struct list_head));
    new->next = new;
    new->prev = new;
    return new;
}

/* Free all storage used by queue */
void q_free(struct list_head *head)
{
    struct list_head *cur = head->next;
    while (cur != NULL && cur != head) {
        struct list_head *next = cur->next;
        element_t *el = container_of(cur, element_t, list);
        free(el->value);

        free(el);
        cur = next;
    }
    head->next = head;
    head->prev = head;
    free(head);
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new_el = malloc(sizeof(element_t));
    if (!new_el)
        return false;
    new_el->value = strdup(s);
    if (!new_el->value)
        return false;
    new_el->list.next = head->next;
    new_el->list.prev = head;
    head->next->prev = &(new_el->list);
    head->next = &(new_el->list);
    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new_el = malloc(sizeof(element_t));
    if (!new_el)
        return false;
    new_el->value = strdup(s);
    if (!new_el->value)
        return false;
    new_el->list.next = head;
    new_el->list.prev = head->prev;
    head->prev->next = &(new_el->list);
    head->prev = &(new_el->list);
    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->next == head)
        return NULL;
    element_t *container = container_of(head->next, element_t, list);
    if (!sp && !container->value)
        return NULL;
    strncpy(sp, container->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    head->next = head->next->next;
    container->list.next->prev = head;
    container->list.next = &(container->list);
    container->list.prev = &(container->list);
    return container;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (head == NULL || head->prev == head)
        return NULL;
    element_t *container = container_of(head->prev, element_t, list);
    if (!sp && !container->value)
        return NULL;
    strncpy(sp, container->value, bufsize - 1);
    sp[bufsize - 1] = '\0';
    head->prev = head->prev->prev;
    container->list.prev->next = head;
    container->list.next = &(container->list);
    container->list.prev = &(container->list);
    return container;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    int cnt = 0;
    struct list_head **addr = &head->next;
    for (; *addr != head; *addr = (*addr)->next)
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

    element_t *container = container_of(slow, element_t, list);
    container->list.prev->next = container->list.next;
    container->list.next->prev = container->list.prev;
    container->list.next = &(container->list);
    container->list.prev = &(container->list);
    free(container->value);
    free(container);
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    return true;
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
