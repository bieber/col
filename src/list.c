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
void *list_get(struct list *list, int element);
// Removes an item from the list
void *list_remove(struct list *list, int element);
