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

#include "interpreter.h"
#include "list.h"
#include "symtable.h"
#include "primitives.h"
#include "forms.h"

// Global symtable is initially NULL, initialized in main.c
struct symtable *SYMTABLE = NULL;

// List of primitive functions, empty string at end marks end of list
char *PRIMITIVE_FUNCTION_NAMES[] = 
{
    #include "gen/primitive_names.h"
};

// List of functional forms, empty string at end marks end of list
char *FUNCTIONAL_FORM_NAMES[] = 
{
    #include "gen/form_names.h"
};

// List of primitive function pointers
struct value*(*PRIMITIVE_FUNCTIONS[])(struct list*, struct value*) = 
{
    #include "gen/primitive_defs.h"
};

// List of functional form definitions
struct value*(*FUNCTIONAL_FORMS[])(struct list*, struct value*) =
{
    #include "gen/form_defs.h"
};

// Creates an empty function struct
struct function *function_new()
{
    struct function *retval =
        (struct function*)malloc(sizeof(struct function));
    retval->index = 0;
    retval->name = NULL;
    retval->args = NULL;
    return retval;
}
// Deletes a function struct
void function_delete(struct function *function)
{
    struct list *l;

    if(function->name)
        free(function->name);
    
    if(function->type == PRIMITIVE && function->args)
    {
        l = function->args;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            value_delete((struct value*)list_at_cursor(l));
        list_delete(l);
    }
    
    if(function->type == FORM && function->args)
    {
        l = function->args;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            function_delete((struct function*)list_at_cursor(l));
        list_delete(l);
    }

    free(function);
}

// Creates an empty value struct
struct value *value_new()
{
    struct value *retval = (struct value*)malloc(sizeof(struct value));
    retval->type = BOTTOM_VAL;
    return retval;
}

// Deletes a value struct
void value_delete(struct value *value)
{
    struct list *l = NULL;

    if(value->type == SEQ_VAL)
    {
        l = value->data.seq_val;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            value_delete((struct value*)list_at_cursor(l));
        list_delete(value->data.seq_val);
    }
    else if(value->type == STRING_VAL)
    {
        free(value->data.str_val);
    }

    free(value);
}

// Copies a value struct, including any lists
struct value *value_copy(struct value *val)
{
    struct value *retval = value_new();
    struct list *l = NULL;
    
    retval->type = val->type;
    if(val->type == SEQ_VAL)
    {
        // Copying the sequence
        retval->data.seq_val = list_new();
        l = val->data.seq_val;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            list_push_back(retval->data.seq_val, value_copy(list_at_cursor(l)));
    }
    else if(val->type == STRING_VAL)
    {
        retval->data.str_val = strdup(val->data.str_val);
    }
    else
    {
        retval->data = val->data;
    }

    return retval;
}

// Checks a value for bottom, including lists
int value_is_bottom(struct value *val)
{
    struct list *l = NULL;

    if(val->type == BOTTOM_VAL)
        return 1;

    if(val->type == SEQ_VAL)
    {
        l = val->data.seq_val;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            if(value_is_bottom(list_at_cursor(l)))
                return 1;
    }

    return 0;
}

// Prints a text representation of a function
void function_print(struct function *function, int level)
{
    int i;
    struct list *l;

    for(i = 0; i < level; i++)
        printf(" ");

    switch(function->type)
    {
    case PRIMITIVE:
        printf("%s (Primitive)\n", function->name);
        
        if(function->args)
        {
            l = function->args;

            for(i = 0; i < level; i++)
                printf(" ");

            printf("Arguments:\n");
            for(list_cursor_begin(l); l->cursor; list_next(l))
                value_print((struct value*)list_at_cursor(l),
                            level + INDENT_STEP);
        }

        break;
        
    case FORM:
        printf("%s (Functional Form)\n", function->name);

        if(function->args)
        {
            l = function->args;

            for(i = 0; i < level; i++)
                printf(" ");

            printf("Arguments:\n");
            for(list_cursor_begin(l); l->cursor; list_next(l))
                function_print((struct function*)list_at_cursor(l),
                               level + INDENT_STEP);
        }

        break;

    case USER:
        printf("%s (User Defined)\n", function->name);
        break;
    }
}

// Prints a text representation of a value
void value_print(struct value *value, int level)
{
    int i;
    struct list *l;

    for(i = 0; i < level; i++)
        printf(" ");

    switch(value->type)
    {
    case INT_VAL:
        printf("%d (Integer)\n", value->data.int_val);
        break;

    case FLOAT_VAL:
        printf("%lf (Floating Point)\n", value->data.float_val);
        break;
        
    case CHAR_VAL:
        printf("'%c' (Character)\n", value->data.char_val);
        break;

    case STRING_VAL:
        printf("\"%s\" (String)\n", value->data.str_val);
        break;

    case BOOL_VAL:
        if(value->data.bool_val)
            printf("True (Boolean)\n");
        else
            printf("False (Boolean)\n");
        break;

    case BOTTOM_VAL:
        printf("Bottom\n");
        break;

    case SEQ_VAL:
        printf("Sequence:\n");
        
        l = value->data.seq_val;
        for(list_cursor_begin(l); l->cursor; list_next(l))
            value_print((struct value*)list_at_cursor(l), level + INDENT_STEP);
        break;

    default:
        printf("Unknown value type %d\n", value->type);
        break;
    }
}

// Prints a text representation of all the functions in a symtable
void symtable_print(struct symtable *table)
{
    int i = 0;
    struct list *l;
    struct symtable_entry *e = NULL;

    // Just iterating through the symtable and printing each entry
    // Not in any particular order, thanks to the hashing
    for(i = 0; i < SYMTABLE_SIZE; i++)
    {
        l = table->entries[i];
        for(list_cursor_begin(l); l->cursor; list_next(l))
        {
            e = list_at_cursor(l);
            
            printf("%s = \n", e->name);
            function_print(e->data, 0);
            printf("\n");
        }
    }
}

// Executes a function, always returns a new value object
struct value *function_exec(struct function *function, struct value *in)
{
    struct value *out = NULL;

    // Check for bottom, in which case there's no need to do anything
    if(value_is_bottom(in))
    {
        value_delete(in);
        return value_new();
    }

    switch(function->type)
    {
    case USER:
        // For user functions, just look up the function in the symtable and 
        // execute it, or return bottom if it's not found
        function = symtable_find(SYMTABLE, function->name);
        if(function)
        {
            out = function_exec(function,  in);
        }
        else
        {
            out = value_new();
            out->type = BOTTOM_VAL;
            value_delete(in);
            return out;
        }   
        break;

    case FORM:
        // For functional forms, get the apropriate function pointer from the 
        // table and pass it the input
        out = (*FUNCTIONAL_FORMS[function->index])(function->args, in);

        if(value_is_bottom(out))
        {
            value_delete(out);
            return value_new();
        }
        else
        {
            return out;
        }

    case PRIMITIVE:
        // For primitive functions, get the function pointer from the table, 
        // pass it the input, return result
        out = (*PRIMITIVE_FUNCTIONS[function->index])(function->args, in);
        value_delete(in);
        
        if(value_is_bottom(out))
        {
            value_delete(out);
            return value_new();
        }
        else
        {
            return out;
        }
    }
}
