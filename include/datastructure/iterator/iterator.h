/**************************************************************************
 * MIT LICENSE
 *
 * Copyright (c) 2014, David Andreoletti <http://davidandreoletti.com>
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
 *  Iterator 
 *  
 *  All functions/macros not starting with __ or _ are Public API.
 */

#ifndef INCLUDE_DATASTRUCTURE_ITERATOR_ITERATOR_H_
#define INCLUDE_DATASTRUCTURE_ITERATOR_ITERATOR_H_

#include "datastructure/macros.h"

enum iterator_mode {
    ITERATOR_ACCESS_MODE_FORWARD    = 1 << 0,                // Can go to next
    ITERATOR_ACCESS_MODE_BACKWARD   = 1 << 1,                // Can go to prev
    ITERATOR_ACCESS_MODE_WRITABLE   = 1 << 2,                // Can change value of current item in iterator
    ITERATOR_ACCESS_MODE_MUTABLE    = 1 << 3                 // Can sequence of item change while iterating over it
};

struct iterator;

/**
 *  An iterator
 *
 *  Implementations returning iterators MUST specify all values in this struct
 */
struct iterator {
    char _mode;                                                     // Access/Writable mode
    void* (*next) (struct iterator* iterator);                      // Move iterator to the next item
    void* (*prev) (struct iterator* iterator);                      // Move iterator to the next item
    void* (*current) (struct iterator* iterator);                   // Get item at current iterator position
    void* (*begin) (struct iterator* iterator);                     // Get first item
    void* (*end) (struct iterator* iterator);                       // Get last item
    void* _first;                                                   // First item
    void* _last;                                                    // Last item
};


EXTERN_C_BEGIN

/**
 * Iterates to the next item
 * \param iterator The iterator to iterate onto
 * \return If ITERATOR_ACCESS_MODE_FORWARD enabled, then the item (NULL still possible). Otherwise NULL.
 */
static inline void* iterator_item_next(struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    else if (iterator->_mode & ITERATOR_ACCESS_MODE_FORWARD) {
        return iterator->next(iterator);
    }
    return NULL;
}

/**
 * Iterates to the previous item
 * \param iterator The iterator to iterate onto
 * \return If ITERATOR_ACCESS_MODE_BACKWARD enabled, then the item (NULL still possible). Otherwise NULL.
 */
static inline void* iterator_item_prev(struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    else if (iterator->_mode & ITERATOR_ACCESS_MODE_BACKWARD) {
        return iterator->prev(iterator);
    }
    return NULL;
}

/**
 * Iterates to current item
 * \param iterator The iterator to iterate onto
 * \return the item 
 */
void* iterator_item_current(struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    return iterator->current(iterator);
}

/**
 * Return first item
 * \param iterator The iterator to iterate onto
 * \return the item
 */
static inline void* iterator_item_begin(struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    else if (iterator->_first != NULL) {
        return iterator->_first;
    }
    return iterator->begin(iterator);
}

/**
 * Return past-the-end item
 * \param iterator The iterator to iterate onto
 * \return the item
 */
static inline void* iterator_item_end(struct iterator* iterator) {
    if (iterator == NULL) {
        return NULL;
    }
    else if (iterator->_last != NULL) {
        return iterator->_last;
    }
    return iterator->end(iterator);
}

/**
 *  Sequences providing iterators SHOULD name methods to get an iterator and 
 *  free the iterator as follow:
 *
 *  - struct iterator* xxxx_iterator_get(...) where xxxx is anything you like
 *  - void xxxx_iterator_free(struct iterator* iterator, ...) where xxxx is anything you like
 */

EXTERN_C_END

#endif  // INCLUDE_DATASTRUCTURE_ITERATOR_ITERATOR_H_
