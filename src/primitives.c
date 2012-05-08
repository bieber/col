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
#include <string.h>

#include "list.h"
#include "primitives.h"
#include "interpreter.h"

#define STRING_BUF_SIZE 1024

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
    struct cursor *c = NULL;
    int is_float = 0;
    int ival = 0;
    float fval = 0;

    // Return bottom if input isn't a sequence
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Iterate through arguments and add them if appropriate
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        e = cursor_get(c);
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
            cursor_delete(c);
            return out;
        }
    }
    cursor_delete(c);

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
    struct cursor *c = NULL;
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
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        e = cursor_get(c);

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
            cursor_delete(c);
            return out;
        }
    }
    cursor_delete(c);

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
    struct cursor *c = NULL;
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
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        e = cursor_get(c);

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
            cursor_delete(c);
            return out;
        }
    }
    cursor_delete(c);

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
    struct cursor *c = NULL;
    struct value *e = NULL;
    float fval = 0;
    int ival = 0;
    int is_float = 0;
    int is_first = 1;

    // First make sure we have a sequence input
    out->type = BOTTOM_VAL;
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Otherwise divide all the values
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        e = cursor_get(c);
        
        if(e->type == INT_VAL)
        {
            if(is_first)
            {
                is_first = 0;
                ival = e->data.int_val;
            }
            else
            {
                if(is_float)
                {
                    fval /= e->data.int_val;
                }
                else if(ival % e->data.int_val != 0)
                {
                    fval = (double)ival / e->data.int_val;
                    is_float = 1;
                }
                else
                {
                    ival /= e->data.int_val;
                }
            }
        }
        else if(e->type == FLOAT_VAL)
        {
            if(is_first)
            {
                is_first = 0;
                fval = e->data.float_val;
                is_float = 1;
            }
            else
            {
                if(is_float)
                {
                    fval /= e->data.float_val;
                }
                else
                {
                    fval = ival / e->data.float_val;
                    is_float = 1;
                }
            }
        }
        else
        {
            cursor_delete(c);
            return out;
        }
    }
    cursor_delete(c);

    out->type = is_float ? FLOAT_VAL : INT_VAL;
    if(is_float)
        out->data.float_val = fval;
    else
        out->data.int_val = ival;
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
    struct cursor *c = NULL;
    struct value *out = value_new();
    struct value *e = NULL;
    int result = -1;

    // Checking for invalid input
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    // Performing the modulo operation
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        e = cursor_get(c);
        if(e->type != INT_VAL)
        {
            cursor_delete(c);
            return out;
        }

        if(result < 0)
            result = e->data.int_val;
        else
            result %= e->data.int_val;
    }
    cursor_delete(c);

    out->type = INT_VAL;
    out->data.int_val = result;
    return out;
}


/*** 1+
 * Incrementor function.
 * Input - An integer or floating point number.
 * Output - The input + 1.
 */
struct value *one_plus(struct list *args, struct value *in)
{
    struct value *out = value_new();

    if(in->type == INT_VAL)
    {
        out->type = INT_VAL;
        out->data.int_val = in->data.int_val + 1;
    }
    else if(in->type == FLOAT_VAL)
    {
        out->type = FLOAT_VAL;
        out->data.float_val = in->data.float_val + 1.0;
    }

    return out;
}

/*** 1-
 * Decrementor function.
 * Input - An integer or floating point number.
 * Output - The input - 1.
 */
struct value *one_minus(struct list *args, struct value *in)
{
    struct value *out = value_new();

    if(in->type == INT_VAL)
    {
        out->type = INT_VAL;
        out->data.int_val = in->data.int_val - 1;
    }
    else if(in->type == FLOAT_VAL)
    {
        out->type = FLOAT_VAL;
        out->data.float_val = in->data.float_val - 1.0;
    }

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
    struct cursor *ca = NULL;
    struct cursor *cb = NULL;

    if(a->type != b->type)
        return 0;

    if(a->type == SEQ_VAL)
    {
        la = a->data.seq_val;
        lb = b->data.seq_val;

        if(la->count != lb->count)
            return 0;

        for(ca = cursor_new_front(la), cb = cursor_new_front(lb)
                ; cursor_valid(ca) && cursor_valid(cb)
                ; cursor_next(ca), cursor_next(cb))
        {
            if(!_compare_values(cursor_get(ca), cursor_get(cb)))
            {
                cursor_delete(ca);
                cursor_delete(cb);
                return 0;
            }
        }
        cursor_delete(ca);
        cursor_delete(cb);
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
    struct cursor *c = NULL;
    
    // Making sure we at least have a sequence
    if(in->type != SEQ_VAL || in->data.seq_val->count < 2)
        return out;

    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Now step through and compare adjacent values
    l = in->data.seq_val;
    c = cursor_new_front(l);
    last = cursor_get(c);
    cursor_next(c);
    
    while(cursor_valid(c))
    {
        if(!_compare_values(last, cursor_get(c)))
        {
            cursor_delete(c);
            return out;
        }
        last = cursor_get(c);;
        cursor_next(c);
    }
    cursor_delete(c);

    out->data.bool_val = 1;
    return out;
}

/* Return values:
 * -1 : a < b
 *  0 : a = b
 * +1 : a > b
 * -2 : Error
 */
int __order_values(struct value *a, struct value *b)
{
    switch(a->type)
    {
    case INT_VAL:
        if(b->type == INT_VAL)
        {
            if(a->data.int_val < b->data.int_val)
                return -1;
            else if(a->data.int_val == b->data.int_val)
                return 0;
            else 
                return 1;
        }
        else if(b->type == FLOAT_VAL)
        {
            if(a->data.int_val < b->data.float_val)
                return -1;
            else if(a->data.int_val == b->data.float_val)
                return 0;
            else
                return 1;
        }
        else
        {
            return -2;
        }

    case FLOAT_VAL:
        if(b->type == INT_VAL)
        {
            if(a->data.float_val < b->data.int_val)
                return -1;
            else if(a->data.float_val == b->data.int_val)
                return 0;
            else
                return 1;
        }
        else if(b->type == FLOAT_VAL)
        {
            if(a->data.float_val < b->data.float_val)
                return -1;
            else if(a->data.float_val == b->data.float_val)
                return 0;
            else
                return 1;
        }
        else
        {
            return -2;
        }

    case CHAR_VAL:
        if(b->type != CHAR_VAL)
            return -2;
        
        if(a->data.char_val < b->data.char_val)
            return -1;
        else if(a->data.char_val == b->data.char_val)
            return 0;
        else
            return 1;

    case STRING_VAL:
        if(b->type != STRING_VAL)
            return -2;

        return strcmp(a->data.str_val, b->data.str_val);

    default:
        return -2;
    }
}

/*** lt
 * Less-than comparator.
 * Input - A sequence consisting either of all numbers, all characters, or 
 * all strings.
 * Output - True if each successive element is ordered before the next, False
 * otherwise.
 */
struct value *lt(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct cursor *c = NULL;
    void *last = NULL;

    if(in->type != SEQ_VAL)
        return out;
    
    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Ensure each value is greater than the last
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        // First element gets a pass
        if(!last)
        {
            last = cursor_get(c);
        }
        else
        {
            if(__order_values(cursor_get(c), last) == -2)
            {
                out->type = BOTTOM_VAL;
                cursor_delete(c);
                return out;
            }

            if(__order_values(cursor_get(c), last) <= 0)
            {
                cursor_delete(c);
                return out;
            }
            last = cursor_get(c);
        }
    }
    cursor_delete(c);

    out->data.bool_val = 1;
    return out;
}

/*** lte
 * Less-than-or-equal-to comparator.
 * Input - A Sequence consisting either of all numbers, all characters, or all
 * strings.
 * Output - True if each successive element is ordered before or equal to the
 * next, False otherwise.
 */
struct value *lte(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct cursor *c = NULL;
    void *last = NULL;

    if(in->type != SEQ_VAL)
        return out;
    
    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Ensure each value is greater than the last
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        // First element gets a pass
        if(!last)
        {
            last = cursor_get(c);
        }
        else
        {
            if(__order_values(cursor_get(c), last) == -2)
            {
                out->type = BOTTOM_VAL;
                cursor_delete(c);
                return out;
            }

            if(__order_values(cursor_get(c), last) < 0)
            {
                cursor_delete(c);
                return out;
            }
            last = cursor_get(c);
        }
    }
    cursor_delete(c);

    out->data.bool_val = 1;
    return out;
}

/*** gt
 * Greater-than comparator.
 * Input - A sequence consisting either of all numbers, all characters, or all
 * strings.
 * Output - True if each successive element is ordered after the next, False 
 * otherwise.
 */
struct value *gt(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct cursor *c = NULL;
    void *last = NULL;

    if(in->type != SEQ_VAL)
        return out;
    
    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Ensure each value is greater than the last
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        // First element gets a pass
        if(!last)
        {
            last = cursor_get(c);
        }
        else
        {
            if(__order_values(cursor_get(c), last) == -2)
            {
                out->type = BOTTOM_VAL;
                cursor_delete(c);
                return out;
            }

            if(__order_values(last, cursor_get(c)) <= 0)
            {
                cursor_delete(c);
                return out;
            }
            last = cursor_get(c);
        }
    }
    cursor_delete(c);

    out->data.bool_val = 1;
    return out;
}

/*** gte
 * Greater-than-or-equal-to comparator.
 * Input - A sequence consisting either of all numbers, all characters, or all
 * strings.
 * Output - True if each successive element is ordered after or equal to the 
 * next, False otherwise.
 */
struct value *gte(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct cursor *c = NULL;
    void *last = NULL;

    if(in->type != SEQ_VAL)
        return out;
    
    out->type = BOOL_VAL;
    out->data.bool_val = 0;

    // Ensure each value is greater than the last
    l = in->data.seq_val;
    for(c = cursor_new_front(l); cursor_valid(c); cursor_next(c))
    {
        // First element gets a pass
        if(!last)
        {
            last = cursor_get(c);
        }
        else
        {
            if(__order_values(cursor_get(c), last) == -2)
            {
                out->type = BOTTOM_VAL;
                cursor_delete(c);
                return out;
            }

            if(__order_values(last, cursor_get(c)) < 0)
            {
                cursor_delete(c);
                return out;
            }
            last = cursor_get(c);
        }
    }
    cursor_delete(c);

    out->data.bool_val = 1;
    return out;
}

/*** int
 * Integer conversion function.
 * Input - Any value other than bottom.
 * Output - A conversion of the input value to an integer.  Floats are 
 * truncated, integers are passed through verbatim.  True becomes 1, False 
 * becomes 0.  Chars are converted to their ASCII values.  Strings are 
 * converted with the C atoi function.  Sequences simply return the sequence 
 * length.
 */
struct value *to_int(struct list *args, struct value *in)
{
    struct value *out = value_new();
    out->type = INT_VAL;

    switch(in->type)
    {
    case INT_VAL:
        out->data.int_val = in->data.int_val;
        break;
        
    case FLOAT_VAL:
        out->data.int_val = (int)in->data.float_val;
        break;

    case CHAR_VAL:
        out->data.int_val = (int)in->data.char_val;
        break;
        
    case STRING_VAL:
        out->data.int_val = atoi(in->data.str_val);
        break;

    case BOOL_VAL:
        out->data.int_val = in->data.bool_val ? 1 : 0;
        break;

    case SEQ_VAL:
        out->data.int_val = in->data.seq_val->count;
        break;
    }

    return out;
}

/*** float
 * Floating point conversion function.
 * Input - Any value other than bottom.
 * Output - A conversion of the input value to a floating point number.  
 * Integers are simply cast to floating point, floating point numbers are 
 * passed through verbatim.  True becomes 1.0, False becomes 0.0.  Chars are 
 * converted to their ASCII values and then cast to floating point.  Strings 
 * are converted with the C atof funciton.  Sequences simply return the 
 * sequence length.
 */
struct value *to_float(struct list *args, struct value *in)
{
    struct value *out = value_new();
    out->type = FLOAT_VAL;

    switch(in->type)
    {
    case INT_VAL:
        out->data.float_val = (double)in->data.int_val;
        break;
        
    case FLOAT_VAL:
        out->data.float_val = in->data.float_val;
        break;

    case CHAR_VAL:
        out->data.float_val = (double)((int)in->data.char_val);
        break;

    case STRING_VAL:
        out->data.float_val = atof(in->data.str_val);
        break;

    case BOOL_VAL:
        out->data.float_val = in->data.bool_val ? 1.0 : 0.0;
        break;
        
    case SEQ_VAL:
        out->data.float_val = (float)in->data.seq_val->count;
        break;
    }

    return out;
}

/*** str
 * String conversion function.
 * Input - Any value other than bottom.
 * Output - A conversion of the input value to a string.
 */
struct value *to_string(struct list *args, struct value *in)
{
    struct value *out = value_new();
    out->type = STRING_VAL;
    out->data.str_val = (char*)malloc(sizeof(char) * STRING_BUF_SIZE);

    switch(in->type)
    {
    case INT_VAL:
        snprintf(out->data.str_val, 1024, "%d", in->data.int_val);
        break;

    case FLOAT_VAL:
        snprintf(out->data.str_val, 1024, "%lf", in->data.float_val);
        break;

    case CHAR_VAL:
        snprintf(out->data.str_val, 1024, "%c", in->data.char_val);
        break;

    case STRING_VAL:
        free(out->data.str_val);
        out->data.str_val = strdup(in->data.str_val);
        break;

    case BOOL_VAL:
        snprintf(out->data.str_val, 1024, "%s",
                 in->data.bool_val ? "True" : "False");
        break;

    case SEQ_VAL:
        snprintf(out->data.str_val, 1024, "Sequence of length %d",
                 in->data.seq_val->count);
        break;
    }

    return out;
}

/*** print
 * Prints output to the screen.
 * Input - A string value.
 * Output - The same string that was passed in as input, or bottom if a 
 * non-string is passed in.  In additition to passing its input through 
 * unchanged, the print function prints its string input out to the screen.
 */
struct value *print_str(struct list *args, struct value *in)
{
    if(in->type != STRING_VAL)
        return value_new();

    printf("%s", in->data.str_val);
    
    return value_copy(in);
}

/*** println
 * Prints output to a line on the screen.
 * Input - A string value.
 * Output - The same string that was passed in as input, or bottom if a 
 * non-string is passed in.  In addition to passing its input through 
 * unchanged, the println function prints its string input to the screen with an
 * additional newline at the end.
 */
struct value *println_str(struct list *args, struct value *in)
{
    if(in->type != STRING_VAL)
        return value_new();
    
    printf("%s\n", in->data.str_val);

    return value_copy(in);
}

/*** readln
 * Reads a line of input from the terminal.
 * Input - Any non-bottom value.
 * Output - A string containing a line read from the user.
 */
struct value *readln_str(struct list *args, struct value *in)
{
    int bufsize = 100;
    int i = 0;
    struct value *out = value_new();
    out->type = STRING_VAL;
    out->data.str_val = malloc(sizeof(char) * bufsize);

    for(i = 0; 1; i++)
    {
        out->data.str_val[i] = fgetc(stdin);
        
        if(out->data.str_val[i] == '\n')
        {
            out->data.str_val[i] = '\0';
            return out;
        }
        
        if(i == bufsize - 1)
        {
            bufsize *= 2;
            out->data.str_val = realloc(out->data.str_val, bufsize);
        }
    }
}

/*** head
 * Returns the first element of a sequence.
 * Input - A sequence.
 * Output - The first item in that sequence, or <> if the sequence is empty.
 */
struct value *head(struct list *args, struct value *in)
{
    struct value *out = NULL;

    if(in->type != SEQ_VAL)
    {
        return value_new();
    }
    else if(in->data.seq_val->count > 0)
    {
        return value_copy(list_get(in->data.seq_val, 0));
    }
    else
    {
        out = value_new();
        out->type = SEQ_VAL;
        out->data.seq_val = list_new();
        return out;
    }
}

/*** tail
 * Returns the portion of a sequence after the head.
 * Input - A sequence
 * Output - A sequence containing every element in the input sequence except
 * for the first, or <> if the sequence is empty.
 */
struct value *tail(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;
    struct cursor *c = NULL;

    if(in->type != SEQ_VAL)
    {
        return out;
    }
    
    out->type = SEQ_VAL;
    out->data.seq_val = list_new();

    if(in->data.seq_val->count > 0)
    {
        l = in->data.seq_val;
        for(c = cursor_new_front(l), cursor_next(c)
                ; cursor_valid(c)
                ; cursor_next(c))
            list_push_back(out->data.seq_val, value_copy(cursor_get(c)));
        cursor_delete(c);
    }

    return out;
}

/*** length
 * Returns the length of a sequence or string.
 * Input - A sequence or string.
 * Output - The length of the input as an integer.
 */
struct value *length(struct list *args, struct value *in)
{
    struct value *out = value_new();
    
    if(in->type != SEQ_VAL && in->type != STRING_VAL)
        return out;

    out->type = INT_VAL;
    if(in->type == SEQ_VAL)
        out->data.int_val = in->data.seq_val->count;
    else
        out->data.int_val = strlen(in->data.str_val);
    
    return out;
}

/*** append
 * Appends an item to the end of a sequence.
 * Input - A sequence of length 2.  The second element must be a list.
 * Output - The first element of the input appended onto the end of the second.
 */
struct value *append(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;

    if(in->type != SEQ_VAL
       || in->data.seq_val->count != 2 
       || ((struct value*)list_get(in->data.seq_val, 1))->type != SEQ_VAL)
        return out;

    l = in->data.seq_val;
    out = value_copy(list_get(l, 1));
    list_push_back(out->data.seq_val, value_copy(list_get(l, 0)));
    return out;
}

/*** prepend
 * Prepends an item to the beginning of a sequence.
 * Input - A sequence of length 2.  The second element must be a list.
 * Output - The first element of the input prepended to the beginning of the 
 * second.
 */
struct value *prepend(struct list *args, struct value *in)
{
    struct value *out = value_new();
    struct list *l = NULL;

    if(in->type != SEQ_VAL
       || in->data.seq_val->count != 2 
       || ((struct value*)list_get(in->data.seq_val, 1))->type != SEQ_VAL)
        return out;

    l = in->data.seq_val;
    out = value_copy(list_get(l, 1));
    list_push(out->data.seq_val, value_copy(list_get(l, 0)));

    return out;
}

