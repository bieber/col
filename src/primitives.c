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
#include <stdio.h>

#include "list.h"
#include "primitives.h"
#include "interpreter.h"

/*** +
 * Basic addition function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The sum of the input numbers, floating point if any of the input 
 * numbers was floating point, integer otherwise.
 */
struct value *add(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct value *e = NULL;
    struct list *l = NULL;
    int is_float = 0;
    int ival = 0;
    float fval = 0;

    // Return bottom if input isn't a sequence
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Iterate through arguments and add them if appropriate
    l = in->data.seq_val;
    for(list_cursor_begin(l); l->cursor; list_next(l))
    {
        e = list_at_cursor(l);
        if(e->type == INT_VAL)
        {
            if(is_float)
                fval += e->data.int_val;
            else
                ival += e->data.int_val;
        }
        else if(e->type == FLOAT_VAL)
        {
            if(is_float)
            {
                fval += e->data.float_val;
            }
            else
            {
                is_float = 1;
                fval = ival + e->data.float_val;
            }   
        }
        else
        {
            return out;
        }
    }

    // If we made it to the end, return the new value
    if(is_float)
    {
        out->type = FLOAT_VAL;
        out->data.float_val = fval;
    }
    else
    {
        out->type = INT_VAL;
        out->data.int_val = ival;
    }

    return out;
}

/*** -
 * Basic subtraction function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The difference of each of the numbers, starting with the first 
 * value and subtracting each successive value in order.  Result is floating 
 * point if any of the input numbers are floating point.
 */
struct value *subtract(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct value *e = NULL;
    int ival = 0;
    int fval = 0;
    int is_float = 0;
    int is_first = 1;

    // First making sure our input is a list
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Subtracting if possible
    l = in->data.seq_val;
    for(list_cursor_begin(l); l->cursor; list_next(l))
    {
        e = list_at_cursor(l);

        if(e->type == FLOAT_VAL)
        {
            if(is_first)
            {
                is_float = 1;
                is_first = 0;
                fval = e->data.float_val;
            }
            else
            {
                if(is_float)
                {
                    fval -= e->data.float_val;
                }
                else
                {
                    is_float = 1;
                    fval = ival - e->data.float_val;
                }
            }
        }
        else if(e->type == INT_VAL)
        {
            if(is_first)
            {
                ival = e->data.float_val;
                is_first = 0;
            }
            else
            {
                if(is_float)
                    fval -= e->data.int_val;
                else
                    ival -= e->data.int_val;
            }
        }
        else
        {
            return out;
        }
    }

    // If we made it this far, store and return value
    if(is_float)
    {
        out->type = FLOAT_VAL;
        out->data.float_val = fval;
    }
    else
    {
        out->type = INT_VAL;
        out->data.int_val = ival;
    }

    return out;
}

/*** *
 * Basic multiplication function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The product of the input numbers, floating point if any of the 
 * inputs were floating point.
 */
struct value *multiply(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct value *e = NULL;
    int ival = 1;
    int fval = 1;
    int is_float = 0;

    // Make sure input is a sequence
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Otherwise step through and multiply
    l = in->data.seq_val;
    for(list_cursor_begin(l); l->cursor; list_next(l))
    {
        e = list_at_cursor(l);

        if(e->type == INT_VAL)
        {
            if(is_float)
                fval *= e->data.int_val;
            else
                ival *= e->data.int_val;
        }
        else if(e->type == FLOAT_VAL)
        {
            if(is_float)
            {
                fval *= e->data.float_val;
            }
            else
            {
                is_float = 1;
                fval = ival * e->data.float_val;
            }
        }
        else
        {
            return out;
        }
    }

    // If loop completed, store and return value
    if(is_float)
    {
        out->type = FLOAT_VAL;
        out->data.float_val = fval;
    }
    else
    {
        out->type = INT_VAL;
        out->data.int_val = ival;
    }

    return out;
}

/*** /
 * Basic division function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The quotient of each of the numbers, starting with the first 
 * value and subtracting each successive value in order.  Result is a floating 
 * point number.
 */
struct value *divide(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct value *e = NULL;
    float val = 0;
    int is_first = 1;

    // First make sure we have a sequence input
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Otherwise divide all the values
    l = in->data.seq_val;
    for(list_cursor_begin(l); l->cursor; list_next(l))
    {
        e = list_at_cursor(l);
        
        if(e->type == INT_VAL)
        {
            if(is_first)
            {
                is_first = 0;
                val = e->data.int_val;
            }
            else
            {
                val /= e->data.int_val;
            }
        }
        else if(e->type == FLOAT_VAL)
        {
            if(is_first)
            {
                is_first = 0;
                val = e->data.float_val;
            }
            else
            {
                val /= e->data.float_val;
            }
        }
    }

    out->type = FLOAT_VAL;
    out->data.float_val = val;
    return out;
}

/*** mod
 * Modulus operation.
 * Input - A sequence of two or more integers.
 * Output - The result of applying the modulus operator (remainder of division)
 * between the first and second value, then between that result and the third
 * value, and so on.
 */
struct value *mod(struct list *args, struct value *in)
{
    struct list *l = NULL;
    struct value *out = value_new();
    struct value *e = NULL;
    int result = -1;

    // Checking for invalid input
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Performing the modulo operation
    l = in->data.seq_val;
    for(list_cursor_begin(l); l->cursor; list_next(l))
    {
        e = list_at_cursor(l);
        if(e->type != INT_VAL)
            return out;

        if(result < 0)
            result = e->data.int_val;
        else
            result %= e->data.int_val;
    }

    out->type = INT_VAL;
    out->data.int_val = result;
    return out;
}

/*** const
 * Constant function.
 * Specializers - const(n)
 * Input - Any value other than bottom.
 * Output - The value n.
 */
struct value *constant(struct list *args, struct value *in)
{
    struct value *n = list_get(args, 0);

    if(n)
        return value_copy(n);
    else
        return value_new(); // struct value is bottom by default
}

/*** id
 * Identity function.
 * Input - Any value.
 * Output - The same value as the input.
 */
struct value *id(struct list *args, struct value *in)
{
    return value_copy(in);
}

int _compare_values(struct value *a, struct value *b)
{
    struct list *la = NULL;
    struct list *lb = NULL;

    if(a->type != b->type)
        return 0;

    if(a->type == SEQ_VAL)
    {
        la = a->data.seq_val;
        lb = b->data.seq_val;

        if(la->count != lb->count)
            return 0;

        for(list_cursor_begin(la), list_cursor_begin(lb);
            la->cursor && lb->cursor;
            list_next(la), list_next(lb))
        {
            if(!_compare_values(list_at_cursor(la), list_at_cursor(lb)))
                return 0;
        }
    }
    else
    {
        switch(a->type)
        {
        case INT_VAL:
            return a->data.int_val == b->data.int_val;
        case FLOAT_VAL:
            return a->data.float_val == b->data.float_val;
        case BOOL_VAL:
            return a->data.bool_val == b->data.float_val;
        case CHAR_VAL:
            return a->data.char_val == b->data.char_val;
        case BOTTOM_VAL:
            return 1;
        case STRING_VAL:
            return !strcmp(a->data.str_val, b->data.str_val);
        }
    }

    return 1;
}

/*** eq
 * Comparison function.
 * Input - A sequence of two or more values.
 * Output - True if all values in the sequence are equivalent, False if they are
 * not, or bottom for invalid input.
 */
struct value *eq(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct value *last = NULL;
    struct list *l = NULL;
    
    // Making sure we at least have a sequence
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Now step through and compare adjacent values
    l = in->data.seq_val;
    list_cursor_begin(l);
    last = list_at_cursor(l);
    list_next(l);
    
    while(l->cursor)
    {
        if(!_compare_values(last, list_at_cursor(l)))
            return out;
        last = list_at_cursor(l);
        list_next(l);
    }

    out->data.bool_val = 1;
    return out;
}
