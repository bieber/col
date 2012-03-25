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
