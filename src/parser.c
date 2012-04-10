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
#include "list.h"

// Possible errors
enum parser_error
{
    LEX_ERROR,
    EXPECTED_IDENT,
    EXPECTED_ASSIGN,
    EXPECTED_ARGS,
    EXPECTED_CLOSE,
    EXPECTED_CONST,
    UNEXPECTED_END,
    INVALID_ELEMENT
};

// Requires a token
int require_token(struct lexer_state *lexer, enum token_type token);
// Parses a function definition
struct function *parse_function(struct lexer_state *lexer);
// Parses a set of constant arguments
struct list *parse_constant_args(struct lexer_state *lexer,
                                 enum token_type close);
// Parses a set of function arguments
struct list *parse_function_args(struct lexer_state *lexer);
// Parses a constant
struct value *parse_constant(struct lexer_state *lexer);

// Deletes every value in a list
void clear_value_list(struct list *args);
// Deletes every function in a list
void clear_function_list(struct list *args);

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

// Requires a token
int require_token(struct lexer_state *lexer, enum token_type token)
{
    if(lexer->error != OK)
        return 0;

    lex(lexer);

    if(lexer->error == UNRECOGNIZED_TOKEN)
    {
        print_error(lexer, LEX_ERROR);
        return 0;
    }

    if(lexer->type != token)
        return 0;

    return 1;
}

// Parses a function definition
struct function *parse_function(struct lexer_state *lexer)
{
    int error = 0;
    int i = 0;
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
    for(i = 0; *(PRIMITIVE_FUNCTION_NAMES[i]); i++)
    {
        if(!strcmp(PRIMITIVE_FUNCTION_NAMES[i], function->name))
        {
            function->type = PRIMITIVE;
            function->index = i;
        }
    }
    
    for(i = 0; *(FUNCTIONAL_FORM_NAMES[i]); i++)
    {
        if(!strcmp(FUNCTIONAL_FORM_NAMES[i], function->name))
        {
            function->type = FORM;
            function->index = i;
        }
    }

    // Now checking for possible arguments
    if(function->type == PRIMITIVE)
    {
        lex(lexer);
        
        if(lexer->error == UNRECOGNIZED_TOKEN)
        {
            print_error(lexer, LEX_ERROR);
            function_delete(function);
            return NULL;
        }

        // Now checking for opening paren
        if(lexer->error == OK && lexer->type == OPEN_SPEC)
        {
            function->args = parse_constant_args(lexer, CLOSE_SPEC);
            if(!function->args)
            {
                function_delete(function);
                return NULL;
            }
        }
        else
        {
            lexer_rewind(lexer);
        }
    }
    else if(function->type == FORM)
    {
        // A functional form requires arguments
        if(!require_token(lexer, OPEN_FORM))
        {
            print_error(lexer, EXPECTED_ARGS);
            function_delete(function);
            return NULL;
        }

        function->args = parse_function_args(lexer);
        if(!function->args)
        {
            function_delete(function);
            return NULL;
        }
    }
    
    return function;
}

// Parses a set of constant arguments
struct list *parse_constant_args(struct lexer_state *lexer, 
                                 enum token_type close)
{
    struct value *arg = NULL;
    struct list *args = list_new();
    
    // First check for empty list
    lex(lexer);
    
    if(lexer->error == UNRECOGNIZED_TOKEN)
    {
        print_error(lexer, LEX_ERROR);
        clear_value_list(args);
        return NULL;
    }
    
    if(lexer->error == END_OF_INPUT)
    {
        print_error(lexer, UNEXPECTED_END);
        clear_value_list(args);
        return NULL;
    }

    // If we already have close token, just return empty list
    if(lexer->type == close)
        return args;
    else
        lexer_rewind(lexer);

    while(lexer->error == OK)
    {        
        // First parse a value and add it to the list
        arg = parse_constant(lexer);

        if(!arg)
        {
            clear_function_list(args);
            return NULL;
        }

        list_push_back(args, arg);

        // Now checking for a separator or end of the list
        lex(lexer);

        if(lexer->error == UNRECOGNIZED_TOKEN)
        {
            print_error(lexer, LEX_ERROR);
            clear_value_list(args);
            return NULL;
        }

        if(lexer->error == END_OF_INPUT)
        {
            print_error(lexer, UNEXPECTED_END);
            clear_value_list(args);
            return NULL;
        }

        if(lexer->type == close)
            break;

        // If the list is still open, we expect a separator
        if(lexer->type != SEPARATOR)
        {
            print_error(lexer, INVALID_ELEMENT);
            clear_value_list(args);
            return NULL;
        }
    }

    return args;
}
// Parses a set of function arguments
struct list *parse_function_args(struct lexer_state *lexer)
{
    struct function *arg = NULL;
    struct list *args = list_new();

    while(lexer->error == OK)
    {
        // First parse a function and add it to the list
        arg = parse_function(lexer);

        if(!arg)
        {
            clear_function_list(args);
            return NULL;
        }

        list_push_back(args, arg);

        // Then look for either a separator or a close 
        lex(lexer);

        if(lexer->error == UNRECOGNIZED_TOKEN)
        {
            print_error(lexer, LEX_ERROR);
            clear_function_list(args);
            return NULL;
        }

        if(lexer->error == END_OF_INPUT)
        {
            print_error(lexer, UNEXPECTED_END);
            clear_function_list(args);
            return NULL;
        }

        if(lexer->type == CLOSE_FORM)
            break;

        // If there hasn't been a close, then it better be a separator
        if(lexer->type != SEPARATOR)
        {
            print_error(lexer, INVALID_ELEMENT);
            clear_function_list(args);
            return NULL;
        }
    }

    return args;
}

// Parses a constant
struct value *parse_constant(struct lexer_state *lexer)
{
    struct value *arg = value_new();
    
    // First grab the next token
    lex(lexer);

    if(lexer->error == END_OF_INPUT)
    {
        print_error(lexer, UNEXPECTED_END);
        value_delete(arg);
        return NULL;
    }

    if(lexer->error == UNRECOGNIZED_TOKEN)
    {
        print_error(lexer, LEX_ERROR);
        value_delete(arg);
        return NULL;
    }

    switch(lexer->type)
    {
        
    case BOTTOM:
        arg->type = BOTTOM_VAL;
        break;

    case INT:
        arg->type = INT_VAL;
        arg->data.int_val = lexer->value.ival;
        break;

    case FLOAT:
        arg->type = FLOAT_VAL;
        arg->data.float_val = lexer->value.fval;
        break;

    case TRUE:
        arg->type = BOOL_VAL;
        arg->data.bool_val = 1;
        break;

    case FALSE:
        arg->type = BOOL_VAL;
        arg->data.bool_val = 0;
        break;

    case CHAR:
        arg->type = CHAR_VAL;
        arg->data.char_val = lexer->value.cval;
        break;

    case STRING:
        arg->type = STRING_VAL;
        arg->data.str_val = strdup(lexer->value.sval);
        break;
        
    case OPEN_SEQ:
        arg->type = SEQ_VAL;
        arg->data.seq_val = parse_constant_args(lexer, CLOSE_SEQ);
        if(!arg->data.seq_val)
        {
            value_delete(arg);
            return NULL;
        }
        break;

    default:
        print_error(lexer, EXPECTED_CONST);
        value_delete(arg);
        return NULL;
    }

    return arg;
}

// Deletes every value in a list
void clear_value_list(struct list *args)
{
    struct cursor *c = NULL;
    for(c = cursor_new_front(args); cursor_valid(c); cursor_next(c))
        if(cursor_get(c))
            value_delete(cursor_get(c));
    list_delete(args);
    cursor_delete(c);
}

// Deletes every function in a list
void clear_function_list(struct list *args)
{
    struct cursor *c = NULL;
    for(c = cursor_new_front(args); cursor_valid(c); cursor_next(c))
        if(cursor_get(c))
            function_delete(cursor_get(c));
    list_delete(args);
    cursor_delete(c);
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

    case EXPECTED_ARGS:
        printf("Error: Expected function arguments at %d, %d\n",
               lexer->token_line, lexer->token_col);
        break;

    case EXPECTED_CONST:
        printf("Error: Expected constant at %d, %d\n",
               lexer->token_line, lexer->token_col);
        break;

    case UNEXPECTED_END:
        printf("Error: Unexpected end of file\n");
        break;

    case INVALID_ELEMENT:
        printf("Error: Invalid list element at %d, %d\n",
               lexer->token_line, lexer->token_col);
        break;
    }
}
