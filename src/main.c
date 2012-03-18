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

#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "lexer.h"
#include "symtable.h"

#define USAGE "Usage: col [-v] <source file> [command-line arguments]\n"

int main(int argc, char *argv[])
{
    char *input;
    struct lexer_state *lexer;
    
    if(argc < 2)
    {
        printf("Usage: col <source file> [command-line arguments]\n");
        return 1;
    }

    input = read_file(argv[1]);
    if(!input)
    {
        printf("Error reading input file\n");
        return 1;
    }

    lexer = lexer_new();
    lexer_init(lexer, input);

    // Testing the symtable code
    struct symtable *table = symtable_new();

    symtable_add(table, "test1", (void*)1);
    symtable_add(table, "test2", (void*)2);
    symtable_add(table, "collatz24", (void*)24);

    printf("%p\n%p\n%p\n%p\n",
           symtable_find(table, "test1"),
           symtable_find(table, "test2"),
           symtable_find(table, "collatz24"),
           symtable_find(table, "notthere"));

    return 0;
}
