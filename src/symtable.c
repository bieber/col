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
#include <string.h>

#include "symtable.h"
#include "interpreter.h"
#include "list.h"

unsigned int hash(char *s);

// Creates a new symtable
struct symtable *symtable_new()
{
    int i;
    struct symtable *retval = (struct symtable*)malloc(sizeof(struct symtable));

    retval->size = SYMTABLE_SIZE;
    for(i = 0; i < SYMTABLE_SIZE; i++)
        retval->entries[i] = list_new();
    
    return retval;
}

// Deletes a symtable
struct symtable *symtable_delete(struct symtable *table)
{
    int i;
    struct list *list = NULL;
    struct symtable_entry *entry = NULL;

    if(!table)
        return;

    for(i = 0; i < SYMTABLE_SIZE; i++)
    {
        list = table->entries[i];
        for(list_cursor_begin(list); list->cursor; list_next(list))
        {
            entry = list_at_cursor(list);
            free(entry->name);
            function_delete(entry->data);
            free(entry);
        }
        list_delete(list);
    }
    
    free(table);
}

// Adds an entry to the table
void symtable_add(struct symtable *table, char *name, struct function *data)
{
    struct symtable_entry *entry = 
        (struct symtable_entry*)malloc(sizeof(struct symtable_entry));
    struct list *list = table->entries[hash(name) % table->size];

    entry->name = strdup(name);
    entry->data = data;
    list_push(list, entry);
}

// Retrieves an entry from the table
struct function *symtable_find(struct symtable *table, char *name)
{
    struct list *list = table->entries[hash(name) % table->size];
    struct symtable_entry *entry = NULL;

    for(list_cursor_begin(list); list->cursor; list_next(list))
    {
        entry = list_at_cursor(list);
        
        if(!entry)
            break;

        if(!strcmp(entry->name, name))
            return entry->data;
    }

    return NULL;
}

// Removes an entry from the table
void symtable_remove(struct symtable *table, char *name);

unsigned int hash(char *s)
{
    int i = 0;
    char *c = s;
    unsigned int total = 0;
    char buf[sizeof(unsigned int) / sizeof(char)];

    for(i = 0; i < sizeof(unsigned int) / sizeof(char); i++)
        buf[i] = 0;

    for(c = s, i = 0; *c != '\0'; c++, i++)
    {
        buf[i % (sizeof(unsigned int) / sizeof(char))] = *c;
        if(i % (sizeof(unsigned int) / sizeof(char))
           == (sizeof(unsigned int) / sizeof(char)) - 1)
            total += *((unsigned int*)buf);
    }

    if(i % (sizeof(unsigned int) / sizeof(char)) != 0)
        total += *((unsigned int*)buf);
        
    return total;
}
