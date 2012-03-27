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

#include "forms.h"
#include "list.h"
#include "interpreter.h"

/*** compose
 * Function composition.  Feeds its input to the last function in its argument
 * list, then feeds that function's output to the second-to-last, and so on,
 * eventually returning the output of the last function in the list as the 
 * combined function's output.
 *
 * compose{ f, g } : x = f : (g : x)
 */
struct value *compose(struct list *args, struct value *in)
{
    struct value *current = in;
    struct value *last = in;
    struct function *f = NULL;

    // Stepping backwards through the list of arguments and feeding
    // input to successive functions, deleting intermediate values
    for(list_cursor_end(args); args->cursor; list_prev(args))
    {
        f = list_at_cursor(args);
        last = current;
        current = function_exec(f, current);
    }
    
    return current;
}

/*** construct
 * Sequence construction.  Feeds its input to each of its argument functions,
 * and generate a sequence where each element is the output of one of the 
 * argument functions.
 *
 * construct{ f, g } : x  = < f : x, g : x >
 */
struct value *construct(struct list *args, struct value *in)
{
    struct value *out = value_new();

    out->type = SEQ_VAL;
    out->data.seq_val = list_new();

    for(list_cursor_begin(args); args->cursor; list_next(args))
    {
        list_push_back(out->data.seq_val, 
                       function_exec(list_at_cursor(args), value_copy(in)));
    }
    value_delete(in);
    return out;
}

/*** if
 * Conditional form.  Accepts exactly three arguments.  First feeds its input 
 * to the first argument.  If the result is boolean True, it feeds the input to
 * its second argument, if False then it feeds it to its third argument, 
 * otherwise it just returns Bottom.
 *
 * if{ f, g, h } : x = if f : x then g : x else h : x
 */
struct value *iff(struct list *args, struct value *in)
{
    struct value *test = value_copy(in);
    struct value *out = NULL;

    // Checking for correct number of arguments
    if(args->count != 3)
    {
        value_delete(test);
        value_delete(in);
        return value_new();
    }

    // Testing input with first argument
    test = function_exec(list_get(args, 0), test);

    if(test->type == BOOL_VAL)
    {
        if(test->data.bool_val)
        {
            value_delete(test);
            return function_exec(list_get(args, 1), in);
        }
        else
        {
            value_delete(test);
            return function_exec(list_get(args, 2), in);
        }
    }
    else
    {
        value_delete(in);
        return value_new();
    }
}
