/**
 *  Copyright 2012, Robert Bieber
 *
 *  This file is part of col.
 *
 *  col is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  col is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with col.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#include <stdlib.h>

#include "list.h"

// Returns an empty list
struct list *list_new()
{
    struct list *retval = (struct list*)malloc(sizeof(struct list));
    retval->front = NULL;
    retval->back = NULL;
    retval->count = 0;
    return retval;
}
// Deletes a list
void list_delete(struct list *list)
{
    struct list_node *current = list->front;
    struct list_node *next = NULL;

    if(!list)
        return;

    while(current)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Pops an item off the front of the list
void *list_pop(struct list *list)
{
    void *retval = NULL;
    struct list_node *old = NULL;

    // Check for empty list
    if(!list->front)
        return NULL;

    retval = list->front->data;
    old = list->front;

    if(list->front->next)
        list->front->next->prev = NULL;
    else
        list->back = NULL;

    list->front = list->front->next;
    free(old);
    list->count--;

    return retval;
}
// Pushes an item onto the front of the list
void list_push(struct list *list, void *element)
{
    struct list_node *new_node =
        (struct list_node*)malloc(sizeof(struct list_node));
    new_node->data = element;
    new_node->next = list->front;
    new_node->prev = NULL;

    list->front = new_node;
    if(new_node->next)
        new_node->next->prev = new_node;
    else
        list->back = new_node;
    list->count++;
}

// Pops an item off the back of the list
void *list_pop_back(struct list *list)
{
    void *retval = NULL;
    struct list_node *old = NULL;

    // Check for empty list
    if(!list->back)
        return NULL;

    retval = list->back->data;
    old = list->back;

    if(list->back->prev)
        list->back->prev->next = NULL;
    else
        list->front = NULL;

    list->back = list->back->prev;
    free(old);
    list->count--;

    return retval;
}

// Pushes an item onto the back of the list
void list_push_back(struct list *list, void *element)
{
    struct list_node *new_node =
        (struct list_node*)malloc(sizeof(struct list_node));
    new_node->data = element;
    new_node->next = NULL;
    new_node->prev = list->back;

    list->back = new_node;
    if(new_node->prev)
        new_node->prev->next = new_node;
    else
        list->front = new_node;
    list->count++;
}

// Fetches an item from the list
void *list_get(struct list *list, int element)
{
    int i;
    struct list_node *src = NULL;

    if(element >= list->count || element < 0)
    {
        // Out-of-bounds index
        return NULL;
    }
    else if(element < list->count / 2)
    {
        // Fetching from front half of list
        src = list->front;
        for(i = 0; i < element; i++)
            src = src->next;
    }
    else
    {
        // Back half of list
        src = list->back;
        for(i = 0; i < list->count - element - 1; i++)
            src = src->prev;
    }

    return src->data;
}
// Removes an item from the list
void list_remove(struct list *list, int element)
{
    int i;
    struct list_node *old = NULL;

    if(element >= list->count || element < 0)
    {
        // Index out of bounds
        return;
    }
    else if(element < list->count / 2)
    {
        // Searching from front of list
        old = list->front;
        for(i = 0; i < element; i++)
            old = old->next;
    }
    else
    {
        // Searching from back of list
        old = list->back;
        for(i = 0; i < list->count - element - 1; i++)
            old = old->prev;
    }

    if(old->prev)
        old->prev->next = old->next;
    else
        list->front = old->next;

    if(old->next)
        old->next->prev = old->prev;
    else
        list->back = old->prev;

    free(old);
    list->count--;
}

// Returns a new cursor starting at the beginning of a list
struct cursor *cursor_new_front(struct list *list)
{
    struct cursor *c = (struct cursor*)malloc(sizeof(struct cursor));
    c->list = list;
    c->cursor = list->front;
    return c;
}

// Returns a new cursor starting at the back of a list
struct cursor *cursor_new_back(struct list *list)
{
    struct cursor *c = (struct cursor*)malloc(sizeof(struct cursor));
    c->list = list;
    c->cursor = list->back;
    return c;
}

// Deletes a cursor
void cursor_delete(struct cursor *cursor)
{
    free(cursor);
}

// Steps a cursor forward one element
void cursor_next(struct cursor *cursor)
{
    if(cursor->cursor)
        cursor->cursor = cursor->cursor->next;
    else
        cursor->cursor = cursor->list->front;
}

// Steps a cursor back one element
void cursor_prev(struct cursor *cursor)
{
    if(cursor->cursor)
        cursor->cursor = cursor->cursor->prev;
    else
        cursor->cursor = cursor->list->back;
}

// Sets a cursor back to the front of the list
void cursor_rewind(struct cursor *cursor)
{
    cursor->cursor = cursor->list->front;
}

// Sets a cursor to the end of the list
void cursor_ffwd(struct cursor *cursor)
{
    cursor->cursor = cursor->list->back;
}

// Retrieves the current element pointed at by the cursor
void *cursor_get(struct cursor *cursor)
{
    if(cursor->cursor)
        return cursor->cursor->data;
    else
        return NULL;
}

// Returns non-zero if cursor points to a valid list element
int cursor_valid(struct cursor *cursor)
{
    return cursor->cursor ? 1 : 0;
}
