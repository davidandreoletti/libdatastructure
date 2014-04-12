//
//  dlinkedlistTest.c
//
//  Created by Andreoletti David on 3/31/12.
//  Copyright 2012 IO Stark. All rights reserved.
//

#include "datastructureapi/list/dlinkedlistTest.h"
#include "datastructure/list/dlinkedlist.h"
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#define STRUCT_NODE  struct dlinkedlist_node
#define MALLOC_NODE()  (malloc(sizeof(STRUCT_NODE)))
#define FREE_NODE(n)    if (n != NULL) { \
                            free(n);     \
                            n = NULL;    \
                        }

#define ADD_NODES(head, nodeCount, size)                    \
    for (int xyz = 0; xyz < nodeCount; xyz++) {             \
        struct dlinkedlist_node* n = MALLOC_NODE();         \
        dlinkedlist_add_tail(head, n, size);                \
    }

#define INITIAL_SIZE_VALUE  -10

#define REQUIRE_EQUAL(value0, value1) assert(value0 == value1)
#define REQUIRE(condition) assert(condition)

/** Testing data structure */
struct foo {
    int bar;
    struct dlinkedlist_node list;
};

void* freeNode(struct dlinkedlist_node* n) {
    FREE_NODE(n)
    return NULL;
}

struct fixture {
    struct dlinkedlist_node*   h;
    int_least32_t       size;
};

void fixture_setup(struct fixture* f) {
        f->h = MALLOC_NODE();
        f->size = INITIAL_SIZE_VALUE;
        dlinkedlist_init_head(f->h, &(f->size));
}

void fixture_teardown(struct fixture* f) {
        dlinkedlist_free(f->h, &(f->size), &freeNode);
}

void dlinkedlist_macro_entry0(struct fixture* f) {
    struct foo baz = {
        .bar = 0,
        .list = MALLOC_NODE()
    };
    struct foo* ptr = &baz;
    struct dlinkedlist_node* listptr = &(ptr->list);
    dlinkedlist_init_head(listptr, NULL);
    struct foo* ptr2 = dlinkedlist_entry(listptr, struct foo, list);
    REQUIRE(ptr == ptr2);
}

void dlinkedlist_macro_prev_entry0(struct fixture* f) {
    struct foo baz = {
        .bar = 10,
        .list = MALLOC_NODE()
    };
    struct foo baz2 = {
        .bar = 20,
        .list = MALLOC_NODE()
    };
    dlinkedlist_init_head(&(baz.list), NULL);
    dlinkedlist_init_head(&(baz2.list), NULL);
    dlinkedlist_add_tail(&(baz.list), &(baz2.list), NULL);
    struct foo* ptr = dlinkedlist_prev_entry(&(baz2.list), struct foo, list);
    REQUIRE(ptr == &baz);
}

void dlinkedlist_macro_next_entry0(struct fixture* f) {
    struct foo baz = {
        .bar = 10,
        .list = MALLOC_NODE()
    };
    struct foo baz2 = {
        .bar = 20,
        .list = MALLOC_NODE()
    };
    dlinkedlist_init_head(&(baz.list), NULL);
    dlinkedlist_init_head(&(baz2.list), NULL);
    dlinkedlist_add_tail(&(baz.list), &(baz2.list), NULL);
    struct foo* ptr = dlinkedlist_prev_entry(&(baz2.list), struct foo, list);
    REQUIRE(ptr == &baz);
}

void dlinkedlist_init_head0(struct fixture* f) {
    f->h->next = (struct dlinkedlist_node*) 0x01;
    f->h->prev = (struct dlinkedlist_node*) 0x02;
    f->size = 10;
    dlinkedlist_init_head(f->h, &(f->size));
    REQUIRE_EQUAL(f->h, f->h->prev);
    REQUIRE_EQUAL(f->h->prev, f->h->prev);
    REQUIRE_EQUAL(f->size, 0);
}

static int_least32_t dlinkedlist_free0_freeNode_num;
void* dlinkedlist_free0_freeNode(struct dlinkedlist_node* n) {
    free(n);
    dlinkedlist_free0_freeNode_num++;
    return NULL;
}

void dlinkedlist_free0(struct fixture* f) {
    for (int i = 0; i < 3; i++) {
        // Create list and optionaaly add nodes
        struct dlinkedlist_node* list = MALLOC_NODE();
        int_least32_t s;
        dlinkedlist_init_head(list, &s);
        REQUIRE(list != NULL);
        int numNodes = i;
        ADD_NODES(list, numNodes, &s);
        struct dlinkedlist_node* n = NULL;
        dlinkedlist_for_each(list, n) {
            REQUIRE(n != NULL);
        }

        // Free list
        dlinkedlist_free0_freeNode_num = 0;
        dlinkedlist_free(list, &s, dlinkedlist_free0_freeNode);

        // Check list is fully freed
        REQUIRE_EQUAL(s, 0);
        REQUIRE_EQUAL(dlinkedlist_free0_freeNode_num, (numNodes+1));
    }
}

void dlinkedlist_empty_0(struct fixture* f) {
    for (int i = 0; i < 3; i++) {
        struct dlinkedlist_node* list = MALLOC_NODE();
        int_least32_t s;
        dlinkedlist_init_head(list, &s);
        REQUIRE(dlinkedlist_empty(list));
        REQUIRE_EQUAL(s, 0);

        ADD_NODES(f->h, i, &s);
        REQUIRE_EQUAL(s, i);
        if (i == 0) {
            REQUIRE(dlinkedlist_empty(f->h));
        } else {
            REQUIRE(!dlinkedlist_empty(f->h));
        }

        dlinkedlist_free(list, &s, freeNode);
    }
}

void dlinkedlist_size_0(struct fixture* f) {
    REQUIRE_EQUAL(f->size, 0);
    REQUIRE_EQUAL(dlinkedlist_size(f->h), 0);
    for (int i = 0; i < 10; i++) {
        struct dlinkedlist_node* n = MALLOC_NODE();
        dlinkedlist_add_head(f->h, n, &(f->size));
        REQUIRE_EQUAL(dlinkedlist_size(f->h), i+1);
        REQUIRE_EQUAL(f->size, i+1);
    }
}

void dlinkedlist_add_head_0(struct fixture* f) {
    struct dlinkedlist_node* n0;
    for (int i = 0; i < 10 ; i++) {
        struct dlinkedlist_node* n1 = MALLOC_NODE();
        dlinkedlist_add_head(f->h, n1, &(f->size));
        REQUIRE_EQUAL(f->size, i+1);

        REQUIRE_EQUAL(f->h->next, n1);
        REQUIRE_EQUAL(f->h, n1->prev);

        if (i != 0) {
           REQUIRE_EQUAL(n1->next, n0);
           REQUIRE_EQUAL(n1, n0->prev);
        }
        n0 = n1;
    }
}

void dlinkedlist_add_tail_0(struct fixture* f) {
    struct dlinkedlist_node* n0;
    for (int i = 0; i < 10; i++) {
        struct dlinkedlist_node* n1 = MALLOC_NODE();
        dlinkedlist_add_tail(f->h, n1, &(f->size));
        REQUIRE_EQUAL(f->size, i+1);

        REQUIRE_EQUAL(f->h->prev, n1);
        REQUIRE_EQUAL(f->h, n1->next);

        if (i != 0) {
           REQUIRE_EQUAL(n0->next, n1);
           REQUIRE_EQUAL(n0, n1->prev);
        }
        n0 = n1;
    }
}

// Skipped testing dlinkedlist_add_after/dlinkedlist_add_before
// since there are respectively the same as dlinkedlist_add_head
// and dlinkedlist_add_tail

void dlinkedlist_remove_0(struct fixture* f) {
    // Add nodes
    REQUIRE_EQUAL(f->size, 0);
    int max = 5;
    ADD_NODES(f->h, max, &(f->size))
    REQUIRE_EQUAL(f->size, max);

    // Remove all nodes
    struct dlinkedlist_node* n0;
    for (int i = 0; i < max; i++) {
        n0 = f->h->next;
        dlinkedlist_remove(n0, &(f->size));

        // Check node is removed
        REQUIRE_EQUAL(f->size, max-i-1);
        REQUIRE_EQUAL(f->h->next, n0->next);
        REQUIRE_EQUAL(f->h, n0->next->prev);

        struct dlinkedlist_node* n1;
        dlinkedlist_for_each(f->h, n1) {
            REQUIRE(n1 != n0);
        }
    }

    REQUIRE_EQUAL(f->size, 0);
    REQUIRE(dlinkedlist_empty(f->h));
}

void dlinkedlist_splice0(struct fixture* f) {
    int v[32] = {   0, 0,    1, 0,    1, 1,    2, 0,    2, 1,    2, 2,
                    3, 0,    3, 1,    3, 2,    3, 3,    2, 3,    1, 3,
                    0, 3,    1, 2,    0, 2,    0, 1};
    for (int y = 0; y < 16; y++) {
        // Add nodes in lists
        struct dlinkedlist_node* list = MALLOC_NODE();
        struct dlinkedlist_node* list2 = MALLOC_NODE();
        int_least32_t listSize = -100;
        int_least32_t listSize2 = -100;
        dlinkedlist_init_head(list, &listSize);
        dlinkedlist_init_head(list2, &listSize2);
        int lMax = v[y*2];
        int l2Max = v[(y*2)+1];
        ADD_NODES(list, lMax, &listSize)
        ADD_NODES(list2, l2Max, &listSize2)
        REQUIRE_EQUAL(listSize, lMax);
        REQUIRE_EQUAL(listSize2, l2Max);

        struct dlinkedlist_node** arr = (struct dlinkedlist_node**)
                                        malloc((lMax + l2Max)
                                        * sizeof(struct dlinkedlist_node*));
        struct dlinkedlist_node* n = list2;
        for (int i = 0; i< (lMax + l2Max); i++) {
            if (i < l2Max) {
                n = n->next;
            } else if (i == l2Max) {
                n = list->next;
            } else {
                n = n->next;
            }
            arr[i] = n;
        }

        dlinkedlist_splice(list2, list, &listSize2, &listSize);

        // Check joined list has all elements
        REQUIRE_EQUAL(listSize, (lMax+l2Max));
        n = NULL;
        int i = 0;
        dlinkedlist_for_each(list, n) {
            REQUIRE_EQUAL(n, arr[i]);
            i++;
        }
        uint32_t s = dlinkedlist_size(list);
        REQUIRE_EQUAL(s, listSize);
        REQUIRE_EQUAL(s, i);
        dlinkedlist_free(list, &listSize, freeNode);
        dlinkedlist_free(list2, &listSize2, freeNode);
    }
}

void dlinkedlist_singular0(struct fixture* f) {
    REQUIRE(!dlinkedlist_singular(f->h));
    ADD_NODES(f->h, 1, NULL)
    REQUIRE(dlinkedlist_singular(f->h));
    ADD_NODES(f->h, 1, NULL)
    REQUIRE(!dlinkedlist_singular(f->h));
}

void dlinkedlist_split0(struct fixture* f) {
    int v[30] = {   0, 0,   1, 0,   1, 1,   2, 0,   2, 1,   2, 2,
                    3, 0,   3, 1,   3, 2,   3, 3,   4, 0,   4, 1,
                    4, 2,   4, 3,   4, 4};
    // Test with 0,1,X nodes in the list
    for (int y = 0; y < 15; y++) {
        // Add nodes in lists
        int numTotalNodes = v[y*2];
        struct dlinkedlist_node* list = MALLOC_NODE();
        struct dlinkedlist_node* list2 = MALLOC_NODE();
        int_least32_t listSize;
        int_least32_t listSize2;
        dlinkedlist_init_head(list, &listSize);
        dlinkedlist_init_head(list2, &listSize2);
        ADD_NODES(list, numTotalNodes, &listSize)
        struct dlinkedlist_node** arr = (struct dlinkedlist_node**) malloc(numTotalNodes * sizeof(struct dlinkedlist_node *));  // NOLINT(whitespace/line_length)
        struct dlinkedlist_node* n = NULL;
        REQUIRE_EQUAL(listSize, numTotalNodes);
        REQUIRE_EQUAL(listSize2, 0);
        int z = 0;
        dlinkedlist_for_each(list, n) {
            arr[z] = n;
            z++;
        }
        REQUIRE_EQUAL(listSize, z);

        // Pick node to use as list separator
        int lNumNodes = v[y*2+1];
        int l2NumNodes = (numTotalNodes == 0 || lNumNodes == 0) ?
                            0 : (numTotalNodes - lNumNodes+1);
        n = list;
        for (int i = 0; i < lNumNodes; i++) {
            n = n->next;
        }
        int lNumNodes2 = lNumNodes;
        lNumNodes = (lNumNodes == 0) ? 0 : numTotalNodes - l2NumNodes;

        dlinkedlist_split(list, list2, n, &listSize, &listSize2);

        // Check both lists
        if (list == n) {
            REQUIRE_EQUAL(listSize, listSize);
        } else {
            REQUIRE_EQUAL(listSize, lNumNodes);
            REQUIRE_EQUAL(listSize2, l2NumNodes);
        }

        n = list;
        int lNumCountedNodes = 0;
        int l2NumCountedNodes = 0;
        for (int i = 0; i < numTotalNodes; i++) {
            if (i < lNumNodes || lNumNodes2 == 0) {
                n = n->next;
                lNumCountedNodes++;
            } else {
                if (i == lNumNodes) {
                    n = list2->next;
                } else {
                    n = n->next;
                }
                l2NumCountedNodes++;
            }
            REQUIRE_EQUAL(arr[i], n);
        }

        uint32_t s = dlinkedlist_size(list);
        REQUIRE_EQUAL(s, listSize);
        REQUIRE_EQUAL(s, lNumCountedNodes);
        s = dlinkedlist_size(list2);
        REQUIRE_EQUAL(s, listSize2);
        REQUIRE_EQUAL(s, l2NumCountedNodes);

        dlinkedlist_free(list, &listSize, freeNode);
        dlinkedlist_free(list2, &listSize2, freeNode);
        free(arr);
    }
}

int run_unit_tests_dlinkedlist() {
    struct fixture f = {0};
    fixture_setup(&f);
    dlinkedlist_macro_entry0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_macro_prev_entry0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_macro_next_entry0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_init_head0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_free0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_empty_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_size_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_add_head_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_add_tail_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_remove_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_remove_0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_splice0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_singular0(&f);
    fixture_teardown(&f);
    fixture_setup(&f);
    dlinkedlist_split0(&f);
    fixture_teardown(&f);
    return 1;
}
