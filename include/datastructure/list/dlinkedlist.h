/**************************************************************************
 * MIT LICENSE
 *
 * Copyright (c) 2012-2014, David Andreoletti <http://davidandreoletti.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 **************************************************************************/

/**
 *  Double LinkedList (instrusive list) inspired from Linux Kernel's list.h
 *  
 *  All functions/macros not starting with __ or _ are Public API.
 */

#ifndef INCLUDE_DATASTRUCTURE_LIST_DLINKEDLIST_H_
#define INCLUDE_DATASTRUCTURE_LIST_DLINKEDLIST_H_

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include "datastructure/macros.h"
#include "datastructure/iterator/iterator.h"

#define _INT_LEAST_32_T int_least32_t

/**
 *  A double linked list node
 */
struct dlinkedlist_node {
    struct dlinkedlist_node*   next;   /** Next node */
    struct dlinkedlist_node*   prev;   /** Previous node */
};

EXTERN_C_BEGIN

/**
 * Get the address of the structure containing the ptr
 * Inspired from "Linux Kernel linked list" excepted this version
 * is pedantic C99 (i.e no usage of C language extensions outside C99)
 *
 * \param ptr Pointer to member of type "struct dlinkedlist_node"
 * \param containertype Type of the struc ptr is embedded in
 * \param member Name of the struct dlinkedlist_node within containertype
 *
 */
#define __dlinkedlist_container_of(ptr, containertype, member)                 \
    ((containertype *) ((char *)ptr - offsetof(containertype, member)))

/**
 * Get the struc for this entry
 *
 * Time Complexity: O(1)
 * Space Complexity: O(0)
 */
#define dlinkedlist_entry(ptr, containertype, member)                          \
    __dlinkedlist_container_of(ptr, containertype, member)

/**
 * Get next entry in the list from the current node
 * and the next entry to node
 *
 * Time Complexity: O(1)
 * Space Complexity: O(0)
 */
#define dlinkedlist_prev_entry(ptr, containertype, member)                     \
    __dlinkedlist_container_of((ptr)->prev, containertype, member)

/**
 * Get next entry in the list from the current node
 * and the next entry to node
 *
 * Time Complexity: O(1)
 * Space Complexity: O(0)
 *
 */
#define dlinkedlist_next_entry(ptr, containertype, member)                     \
    __dlinkedlist_container_of((ptr)->next, containertype, member)

/**
 * Iterates over a list forward
 *
 * \param head	Lists head.
 * \param node	the &struct dlinkedlist_node to use as a loop cursor.
 */
#define dlinkedlist_for_each(head, node)                                      \
    for (node = (head)->next; node != (head) ; node = (node)->next)

/**
 * Iterates over a list backward (eg: from tail to head)
 *
 * \param head	Lists head.
 * \param node	the &struct dlinkedlist_node to use as a loop cursor.
 */
#define dlinkedlist_for_each_prev(head, node)                                 \
    for (node = (head)->prev; node != (head) ; node = (node)->prev)

/**
 *  Initializes the list head as depicted below and 
 *  optionally set list's size to 0.
 *
 *  |-- prev.head.next --|
 *  |         ^^         |
 *  |--------/  \--------|
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param head List head
 *  \param size Set list's size to 0 iff NOT NULL
 */
static inline void dlinkedlist_init_head(struct dlinkedlist_node* head,
                                  _INT_LEAST_32_T*  size) {
    ASSERT(head != NULL)
    head->next = head;
    head->prev = head;
    if (size != NULL) {*size=0;}
}

/**
 * Frees a node in the list
 *
 * \param n Node to be freed
 *
 * \return Implementation specific value
 */
typedef void* (*dlinkedlist_free_node)(struct dlinkedlist_node* n);

/**
 *  Frees list's memory loopping through all nodes in the list  
 *  (list head included) and for each node 'n' calling:
 *   - fn(n)
 *   - n = NULL
 *
 *  Time Complexity:    O(n)
 *  Space Complexity:   O(1)
 *
 *  \param head List head
 *  \param size Set list size to 0 iff NOT NULL and after all nodes have freed
 *  \param fn Function called for freeing the node
 */
static inline void dlinkedlist_free(struct dlinkedlist_node* head,
                             _INT_LEAST_32_T*  size,
                             dlinkedlist_free_node fn) {
    ASSERT(head != NULL)
    ASSERT(fn != NULL)
    struct dlinkedlist_node* n = head->prev;
    while (n != head) {
        n = n->prev;
        fn(n->next);
        n->next = NULL;
    }
    fn(head);
    head = NULL;
    if (size != NULL) {*size=0;}
}

/**
 * Indicates if the list is empty
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 * \param head List head
 * \return 0 if not empty. 
 */
static inline int dlinkedlist_empty(const struct dlinkedlist_node *head) {
    ASSERT(head != NULL)
    return head->next == head && head->prev == head;
}

/**
 * Computes list size
 *
 *  Time Complexity:    O(n)
 *  Space Complexity:   O(1)
 *
 * \param head List head
 * \return size. 
 */
static inline _INT_LEAST_32_T dlinkedlist_size(
                                        const struct dlinkedlist_node *head) {
    ASSERT(head != NULL)
    struct dlinkedlist_node* node;
    _INT_LEAST_32_T size = 0;
    dlinkedlist_for_each(head, node) {size++;}
    return size;
}

/**
 *  Adds a new node in between two consecutive nodes
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param newnode Node to add
 *  \param prev Previous node
 *  \param next Next node
 *  \param size Increments list size iff NOT NULL
 */
static inline void __dlinkedlist_add(struct dlinkedlist_node* node,
                              struct dlinkedlist_node* prev,  // NOLINT(*)
                              struct dlinkedlist_node* next,
                              _INT_LEAST_32_T*  size) {
    ASSERT(node != NULL)
    ASSERT(prev != NULL)
    ASSERT(next != NULL)
    node->next = next;
    node->prev = prev;
    prev->next = node;
    next->prev = node;
    if (size != NULL) {(*size)++;}
}

/**
 *  Adds a new node after list's head.
 * 
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param head List head
 *  \param node Node to append after head
 *  \param size Increments list size iff NOT NULL
 */
static inline void dlinkedlist_add_head(struct dlinkedlist_node* head,
                                 struct dlinkedlist_node* node,
                                 _INT_LEAST_32_T*  size) {
    ASSERT(head != NULL)
    ASSERT(node != NULL)
    __dlinkedlist_add(node, head, head->next, size);
}

/**
 *  Adds a new node after list's tail.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param head List head
 *  \param node Node to append after tail
 *  \param size Increments list size iff NOT NULL
 */
static inline void dlinkedlist_add_tail(struct dlinkedlist_node* head,
                                 struct dlinkedlist_node* node,
                                 _INT_LEAST_32_T*  size) {
    ASSERT(head != NULL)
    ASSERT(node != NULL)
    __dlinkedlist_add(node, head->prev, head, size);
}

/**
 *  Adds a new node after another node.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param node Node/head of the list to append newnode to
 *  \param newnode Node to add
 *  \param size Increments list size iff NOT NULL
 */
static inline void dlinkedlist_add_after(struct dlinkedlist_node* node,
                                  struct dlinkedlist_node* newnode,
                                  _INT_LEAST_32_T*  size) {
    ASSERT(node != NULL);
    ASSERT(newnode != NULL);
    __dlinkedlist_add(newnode, node, node->next, size);
}

/**
 *  Adds a new node before another node.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param node Node/head of the list to prepend newnode to
 *  \param newnode Node to add
 *  \param size Increments list size iff NOT NULL
 */
static inline void dlinkedlist_add_before(struct dlinkedlist_node* node,
                                   struct dlinkedlist_node* newnode,
                                   _INT_LEAST_32_T*  size) {
    ASSERT(node != NULL)
    ASSERT(newnode != NULL)
    __dlinkedlist_add(newnode, node->prev, node, size);
}

/*
 * Deletes a list node making the prev/next nodes
 * point to each other.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param prev Previous node
 *  \param next Next node
 *  \param size Decrements list size iff NOT NULL
 */
static inline void __dlinkedlist_remove(struct dlinkedlist_node * prev,
                                 struct dlinkedlist_node * next,
                                 _INT_LEAST_32_T*  size) {
    ASSERT(prev != NULL)
    ASSERT(next != NULL)
    if (dlinkedlist_empty(next->prev)) {
        if (size != NULL) {
            *size = 0;
        }
    } else {
        next->prev = prev;
        prev->next = next;
        if (size != NULL) {
            (*size)--;
        }
    }
}

/**
 *  Removes a node from the list.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param node Node to delete
 *  \param size Decrements list size iff NOT NULL
 */
static inline void dlinkedlist_remove(struct dlinkedlist_node* node,
                               _INT_LEAST_32_T*  size) {
    ASSERT(node != NULL)
    __dlinkedlist_remove(node->prev, node->next, size);
}

/**
 *  Joins two nodes together.
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(2)
 *
 *  \param list List to add
 *  \param prev Node to prepend to list's first node
 *  \param next prev's next node
 *  \param listSize list parameter's size (NULL permitted)
 *  \param headSize head parameter's size - updated only iff listSize and 
 *                  headSize are NOT NULL
 */
static inline void __dlinkedlist_splice(struct dlinkedlist_node* list,
                                 struct dlinkedlist_node* prev,
                                 struct dlinkedlist_node* next,
                                 _INT_LEAST_32_T* listSize,
                                 _INT_LEAST_32_T* headSize) {
    ASSERT(list != NULL)
    ASSERT(prev != NULL)
    ASSERT(next != NULL)
    struct dlinkedlist_node* first = list->next;
    struct dlinkedlist_node* last = list->prev;
    prev->next = first;
    first->prev = prev;
    last->next = next;
    next->prev = last;
    if (headSize != NULL && listSize != NULL) {*headSize+=*listSize;}
}

/**
 *  Joins two lists together. Fist list is appended to head of the second list
 *
 *  Eg: 
 *  list--next-->a--next-->b joinded to head--next-->c--next-->d
 *
 *  result in
 *  head--next-->a--next-->b--next-->c--next-->d
 *  list--next/prev--|
 *    ^              |
 *     \-------------|
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(2)
 *
 *  \param list Head of list to add
 *  \param head List head to append to
 *  \param listSize list parameter's size. NULL permitted
 *  \param headSize head parameter's size updated only iff listSize and
 *                  headSize are NOT NULL. NULL permitted
 */
static inline void dlinkedlist_splice(struct dlinkedlist_node* list,
                               struct dlinkedlist_node* head,
                               _INT_LEAST_32_T* listSize,
                               _INT_LEAST_32_T* headSize) {
    ASSERT(list != NULL)
    ASSERT(head != NULL)
    if (dlinkedlist_empty(list)) {return;}
    __dlinkedlist_splice(list, head, head->next, listSize, headSize);
    //  list is now empty. Make sure head->next/head->prev points to head
    //  to prevent Seg fault when freeing the list for instance.
    dlinkedlist_init_head(list, listSize);
}

/**
 *  Indicates whether a list has one node only.
 *
 *  Eg:
 *  - head--next-->a--next  returns TRUE
 *
 *  - head--next-|>         retruns FALSE
 *      ^        |
 *       \-------|
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(0)
 *
 *  \param head List head
 *  \return 0 iff list does not contain a single node.
 */
static inline int dlinkedlist_singular(struct dlinkedlist_node* head) {
    ASSERT(head != NULL)
    return !dlinkedlist_empty(head) && (head->next == head->prev);
}

/**
 *  Splits a list into two lists.
 *
 *  Time Complexity:    O(1) iff listSize and headSize are NULL
 *                      O(n) Otherwise
 *  Space Complexity:   O(1)
 *
 *  \param head List head to split
 *  \param list New list starting from node
 *  \param node Node separator
 *  \param listSize list parameter's size - updated only iff listSize and
 *                  headSize. NULL permitted.
 *  \param headSize head parameter's size - updated only iff listSize and
 *                  headSize are NOT NULL. NULL permitted.
 */
static inline void __dlinkedlist_split(struct dlinkedlist_node* head,
                                struct dlinkedlist_node* list,  // NOLINT(*)
                                struct dlinkedlist_node* node,
                                _INT_LEAST_32_T* headSize,
                                _INT_LEAST_32_T* listSize) {
    ASSERT(head != NULL)
    ASSERT(list != NULL)
    ASSERT(node != NULL)

    struct dlinkedlist_node* tailh;
    tailh = head->prev;
    list->next = node;
    head->prev = node->prev;
    node->prev->next = head;
    node->prev = list;
    list->prev = tailh;
    tailh->next = list;
    if (headSize != NULL && listSize != NULL) {
        *headSize = dlinkedlist_size(head);
        *listSize = dlinkedlist_size(list);
    }
}

/**
 *  Splits a list into two lists.
 *
 *  Eg: 
 *  head--next-->a--next-->b--next-->c--next-->d splits from c
 *  results in:
 *  - head--next-->a--next-->b 
 *  - list--next-->c--next-->d
 *
 *  Time Complexity:    O(1) iff listSize and headSize are NULL
 *                      O(n) Otherwise
 *  Space Complexity:   O(1)
 *
 *  \param head List head to split
 *  \param list An empty list
 *  \param node Node separator. Must NOT be head
 *  \param listSize list parameter's size. NULL permitted.
 *  \param headSize head parameter's size - updated only iff listSize and
 *                  headSize are NOT NULL. NULL permitted.
 */
static inline void dlinkedlist_split(struct dlinkedlist_node* head,
                              struct dlinkedlist_node* list,
                              struct dlinkedlist_node* node,
                              _INT_LEAST_32_T* headSize,
                              _INT_LEAST_32_T* listSize) {
    ASSERT(head != NULL)
    ASSERT(list != NULL)
    if (!dlinkedlist_empty(list)) {return;}
    if (dlinkedlist_empty(head)) {return;}
    if (head == node) {return;}
    __dlinkedlist_split(head, list, node, headSize, listSize);
}

/**
 *
 * Iterator Support
 *
 */

struct iterator_dlinkedlist {
    struct iterator _base;
    struct dlinkedlist_node* _current;
    struct dlinkedlist_node* _head;
    struct dlinkedlist_node* _tail;
    struct dlinkedlist_node _sentineltail;
};

static inline void* __dlinkedlist_iterator_next (struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    if (iterator2->_current != iterator2->_tail) {
        iterator2->_current = iterator2->_current->next;
    }
    return iterator2->_current;
}

static inline void* __dlinkedlist_iterator_prev (struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    if (iterator2->_current != iterator2->_head) {
        iterator2->_current = iterator2->_current->prev;
    }
    return iterator2->_current;
}

void* __dlinkedlist_iterator_current (struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    return iterator2->_current;
}

void* __dlinkedlist_iterator_begin (struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    return iterator2->_head;
}

void* __dlinkedlist_iterator_end (struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    return iterator2->_tail;
}

/**
 *  Get an iterator on a list
 *
 *  ALL iterator methods returns "struct dlinkedlist_node*" type
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(1)
 *
 *  \param head The HEAD of the list
 *  \param headSize head parameter's size - updated only iff headSize is NOT NULL. NULL permitted.
 */
static inline struct iterator* dlinkedlist_iterator_get(struct dlinkedlist_node* head,
                                     _INT_LEAST_32_T* headSize) {
    ASSERT(head != NULL)
    
    struct iterator_dlinkedlist* iterator = malloc(sizeof(struct iterator_dlinkedlist));
    iterator->_base._mode = ITERATOR_ACCESS_MODE_FORWARD | ITERATOR_ACCESS_MODE_BACKWARD;
    iterator->_base.begin = __dlinkedlist_iterator_begin;
    iterator->_base.end = __dlinkedlist_iterator_end;
    iterator->_base.next = __dlinkedlist_iterator_next;
    iterator->_base.prev = __dlinkedlist_iterator_prev;
    iterator->_base.current = __dlinkedlist_iterator_current;
    iterator->_head = head;
    iterator->_current = head;
    dlinkedlist_init_head(&(iterator->_sentineltail), NULL);
    iterator->_sentineltail.next = head;
    iterator->_sentineltail.prev = head->prev;
    iterator->_tail = &(iterator->_sentineltail);
    return &(iterator->_base);
}

/**
 *  Free iterator
 *
 *  Time Complexity:    O(1)
 *  Space Complexity:   O(1)
 *
 *  \param iterator The iterator
 */
static inline void dlinkedlist_iterator_free(struct iterator* iterator) {
    if (iterator == NULL) {
        return;
    }
    struct iterator_dlinkedlist* iterator2 = (struct iterator_dlinkedlist*) iterator;
    free(iterator2);
}

EXTERN_C_END

#endif  // INCLUDE_DATASTRUCTURE_LIST_DLINKEDLIST_H_
