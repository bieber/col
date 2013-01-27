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

#ifndef LIST_H
#define LIST_H

struct list_node
{
    void *data;
    struct list_node *prev;
    struct list_node *next;
};

struct list
{
    int count;
    struct list_node *front;
    struct list_node *back;
};

struct cursor
{
    struct list *list;
    struct list_node *cursor;
};

// Returns an empty list
struct list *list_new();
// Deletes a list
void list_delete(struct list *list);

// Pops an item off the front of the list
void *list_pop(struct list *list);
// Pushes an item onto the front of the list
void list_push(struct list *list, void *element);

// Pops an item off the back of the list
void *list_pop_back(struct list *list);
// Pushes an item onto the back of the list
void list_push_back(struct list *list, void *element);

// Fetches an item from the list
void *list_get(struct list *list, int element);
// Removes an item from the list
void list_remove(struct list *list, int element);

// Returns a new cursor starting at the beginning of a list
struct cursor *cursor_new_front(struct list *list);
// Returns a new cursor starting at the back of a list
struct cursor *cursor_new_back(struct list *list);
// Deletes a cursor
void cursor_delete(struct cursor *cursor);

// Steps a cursor forward one element
void cursor_next(struct cursor *cursor);
// Steps a cursor back one element
void cursor_prev(struct cursor *cursor);
// Sets a cursor back to the front of the list
void cursor_rewind(struct cursor *cursor);
// Sets a cursor to the end of the list
void cursor_ffwd(struct cursor *cursor);

// Retrieves the current element pointed at by the cursor
void *cursor_get(struct cursor *cursor);
// Returns non-zero if cursor points to a valid list element
int cursor_valid(struct cursor *cursor);

#endif // LIST_H
