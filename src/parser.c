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

#include "parser.h"
#include "interpreter.h"
#include "symtable.h"
#include "lexer.h"

// Possible errors
enum parser_error
{
    LEX_ERROR,
    EXPECTED_IDENT,
    EXPECTED_ASSIGN,
    UNEXPECTED_END
};

// Requires a token, returns 1 on success, 0 on failure, -1 on error
int require_token(struct lexer_state *lexer, enum token_type token);
// Parses a function definition
struct function *parse_function(struct lexer_state *lexer);

// Prints an error message
void print_error(struct lexer_state *lexer, enum parser_error error);

// Parses the output of a lexer, builds a symtable and returns it
struct symtable *parse(struct lexer_state *lexer)
{
    int error = 0;
    char *ident = NULL;
    struct function *definition = NULL;

    struct symtable *table = symtable_new();

    // At the top level, we expect
    // <ident> "=" <function-expression>

    while(lexer->error == OK)
    {
        // First lex the function name
        lex(lexer);

        // Check for end of file
        if(lexer->error == END_OF_INPUT)
            break;

        // Check for lex error
        if(lexer->error == UNRECOGNIZED_TOKEN)
        {
            print_error(lexer, LEX_ERROR);
            error = 1;
            break;
        }

        if(lexer->type != IDENT)
        {
            print_error(lexer, EXPECTED_IDENT);
            error = 1;
            break;
        }

        // Otherwise, grab the name
        ident = strdup(lexer->value.sval);

        printf("Defining %s\n", ident); ///REMOVE

        // Require the assign operator
        if(!require_token(lexer, ASSIGN))
        {
            print_error(lexer, EXPECTED_ASSIGN);
            error = 1;
            break;
        }

        // Parse the function definition
        definition = parse_function(lexer);

        if(!definition)
        {
            error = 1;
            break;
        }

        // ...and finally add both to symtable
        symtable_add(table, ident, definition);
        free(ident);
    }

    // If any errors occurred, delete the symtable and return NULL
    if(error)
    {
        symtable_delete(table);
        return NULL;
    }

    // Otherwise, return the symtable
    return table;
}

// Requires a token, returns 1 on success, 0 on failure, -1 on error
int require_token(struct lexer_state *lexer, enum token_type token)
{
    if(lexer->error != OK)
        return 0;

    lex(lexer);

    if(lexer->error == UNRECOGNIZED_TOKEN)
    {
        print_error(lexer, LEX_ERROR);
        return -1;
    }

    if(lexer->type != token)
        return 0;

    return 1;
}

// Parses a function definition
struct function *parse_function(struct lexer_state *lexer)
{
    char **name_check;
    int error = 0;
    struct function *function = NULL;

    // First getting the identifier
    lex(lexer);

    if(lexer->error == UNRECOGNIZED_TOKEN)
    {
        print_error(lexer, LEX_ERROR);
        return NULL;
    }
    
    if(lexer->error == END_OF_INPUT)
    {
        print_error(lexer, UNEXPECTED_END);
        return NULL;
    }

    if(lexer->type != IDENT)
    {
        print_error(lexer, EXPECTED_IDENT);
        return NULL;
    }

    // Storing the identifier
    function = function_new();
    function->name = strdup(lexer->value.sval);

    // Figuring out what type of function this is
    function->type = USER; // Until proven otherwise

    // Checking against primitive and form lists
    for(name_check = PRIMITIVE_FUNCTIONS; **name_check; name_check++)
        if(!strcmp(*name_check, function->name))
            function->type = PRIMITIVE;
    for(name_check = FUNCTIONAL_FORMS; **name_check; name_check++)
        if(!strcmp(*name_check, function->name))
            function->type = FORM;

    ///REMOVE ALL THIS
    switch(function->type)
    {
    case USER:
        printf("Found user function %s\n", function->name);
        break;
        
    case PRIMITIVE:
        printf("Found primitive function %s\n", function->name);
        break;

    case FORM:
        printf("Found functional form %s\n", function->name);
        break;
    }
    ///TO HERE


    return NULL;
}

// Prints an error message
void print_error(struct lexer_state *lexer, enum parser_error error)
{
    switch(error)
    {
    case LEX_ERROR:
        printf("Error: Unrecognized token at %d, %d\n",
               lexer->line, lexer->col);

    case EXPECTED_IDENT:
        printf("Error: Expected identifier at %d, %d\n",
               lexer->token_line, lexer->token_col);
        break;

    case EXPECTED_ASSIGN:
        printf("Error: Expected assignment operator at %d, %d\n",
               lexer->token_line, lexer->token_col);
        break;

    case UNEXPECTED_END:
        printf("Error: Unexpected end of file\n");
        break;
    }
}
