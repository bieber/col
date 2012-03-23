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
#include "parser.h"
#include "interpreter.h"

#define USAGE "Usage: col [-v] <source file> [command-line arguments]\n"

int main(int argc, char *argv[])
{
    int verbose = 0;
    char *input;
    struct lexer_state *lexer = NULL;
    struct symtable *symtable = NULL;
    
    // Checking presence of command-line arguments
    if(argc < 2)
    {
        printf(USAGE);
        return 1;
    }
    
    // Checking for verbose flag
    if(!strcmp(argv[1], "-v") || !strcmp(argv[1], "-V"))
    {
        verbose = 1;
        argc--;
        argv++;
    }

    if(verbose)
        printf("Reading input file...\n");

    // Reading the input file
    input = read_file(argv[1]);
    if(!input)
    {
        printf("Error reading input file\n");
        return 1;
    }

    if(verbose)
        printf("Parsing function definitions...\n");

    // Initializing the lexer
    lexer = lexer_new();
    lexer_init(lexer, input);

    // Feeding the input to the parser and getting the symtable
    symtable = parse(lexer);
    
    if(!symtable)
    {
        free(input);
        lexer_delete(lexer);
        return 1;
    }

    if(verbose)
    {
        printf("Loaded function definitions:\n\n");
        symtable_print(symtable);
    }

    // Cleaning up
    free(input);
    lexer_delete(lexer);
    symtable_delete(symtable);
    
    return 0;
}
