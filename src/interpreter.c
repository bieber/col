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

#include "interpreter.h"
#include "list.h"

// List of primitive functions, empty string at end marks end of list
// Currently far from complete, just sufficient for parsing example
char *PRIMITIVE_FUNCTIONS[] = 
{
    "const",
    "id",
    "eq",
    "mod",
    "/",
    "1+",
    "*",
    ""
};

// List of primitive functions, empty string at end marks end of list
// Currently far from complete, just sufficient for parsing example
char *FUNCTIONAL_FORMS[] = 
{
    "compose",
    "construct",
    "if",
    ""
};

// Creates an empty function struct
struct function *function_new()
{
    struct function *retval =
        (struct function*)malloc(sizeof(struct function));
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
    
    if(function->type == FORM)
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

    free(value);
}
