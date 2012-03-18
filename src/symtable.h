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

#ifndef SYMTABLE_H
#define SYMTABLE_H

#define SYMTABLE_SIZE 1024

struct list;

// symtables are stored as hash-tables with linked-list chaining for collisions
struct symtable
{
    int size;
    struct list *entries[SYMTABLE_SIZE];
};

struct symtable_entry
{
    char *name;
    void *data;
};

// Creates a new symtable
struct symtable *symtable_new();
// Deletes a symtable
struct symtable *symtable_delete();

// Adds an entry to the table
void symtable_add(struct symtable *table, char *name, void *data);
// Retrieves an entry from the table
void *symtable_find(struct symtable *table, char *name);
// Removes an entry from the table
void symtable_remove(struct symtable *table, char *name);

#endif // SYMTABLE_H
