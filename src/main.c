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
#include <string.h>

#include "list.h"
#include "file.h"
#include "lexer.h"
#include "symtable.h"
#include "parser.h"
#include "interpreter.h"

#define USAGE "Usage: col [-v] <source file> [command-line arguments]\n"

struct value *args_to_value(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    int verbose = 0;
    char *input;
    struct lexer_state *lexer = NULL;
    struct value *args = NULL;
    struct value *final = NULL;
    struct function *user_main;
    
    // Checking presence of command-line arguments
    if(argc < 2)
    {
        printf(USAGE);
        return 1;
    }
    
    argc--;
    argv++;

    // Checking for verbose flag
    if(!strcmp(argv[0], "-v") || !strcmp(argv[0], "-V"))
    {
        verbose = 1;
        argc--;
        argv++;
    }

    if(verbose)
        printf("Reading input file...\n");

    // Reading the input file
    input = read_file(argv[0]);
    if(!input)
    {
        printf("Error reading input file\n");
        return 1;
    }
    argc--;
    argv++;

    if(verbose)
        printf("Parsing function definitions...\n");

    // Initializing the lexer
    lexer = lexer_new();
    lexer_init(lexer, input);

    // Feeding the input to the parser and getting the symtable
    SYMTABLE = parse(lexer);
    
    if(!SYMTABLE)
    {
        free(input);
        lexer_delete(lexer);
        return 1;
    }

    if(verbose)
    {
        printf("Loaded function definitions:\n\n");
        symtable_print(SYMTABLE);
    }

    // Running the main function
    args = args_to_value(argc, argv);

    if(verbose)
    {
        printf("Command-line arguments:\n");
        value_print(args, 0);
        printf("\n");
    }

    user_main = symtable_find(SYMTABLE, "main");
    if(user_main)
    {
        final = function_exec(user_main, args);
    }
    else
    {
        printf("Error: No main function defined\n");
        free(input);
        lexer_delete(lexer);
        symtable_delete(SYMTABLE);
        return 1;
    }
    
    if(verbose)
    {
        printf("Return value of main:\n");
        value_print(final, 0);
    }

    // Cleaning up
    free(input);
    lexer_delete(lexer);
    symtable_delete(SYMTABLE);
    
    return 0;
}

struct value *args_to_value(int argc, char *argv[])
{
    struct value *args = value_new();
    struct value *arg = NULL;
    int i;
    
    args->type = SEQ_VAL;
    args->data.seq_val = list_new();
    
    for(i = 0; i < argc; i++)
    {
        arg = value_new();
        arg->type = STRING_VAL;
        arg->data.str_val = strdup(argv[i]);
        
        list_push_back(args->data.seq_val, arg);
    }

    return args;
}
