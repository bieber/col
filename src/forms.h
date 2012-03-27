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

#ifndef FORMS_H
#define FORMS_H

struct value;
struct list;

/**
 * The functional forms and comments in this header file will be
 * parsed and turned into documentation and array definitions by a
 * script.  Every form definition should be preceded by /*** form-name
 * where form-name is the col-legal name of the functional form.  Each
 * line of the comment thereafter will be included in the
 * documentation.
 */

/*** compose
 * Function composition.  Feeds its input to the last function in its argument
 * list, then feeds that function's output to the second-to-last, and so on,
 * eventually returning the output of the last function in the list as the 
 * combined function's output.
 */
struct value *compose(struct list *args, struct value *in);

/*** construct
 * Sequence construction.  Feeds its input to each of its argument functions,
 * and generate a sequence where each element is the output of one of the 
 * argument functions.
 */
struct value *construct(struct list *args, struct value *in);

/*** if
 * Conditional form.  Accepts exactly three arguments.  First feeds its input 
 * to the first argument.  If the result is boolean True, it feeds the input to
 * its second argument, if False then it feeds it to its third argument, 
 * otherwise it just returns Bottom.
 *
 * if{ f, g, h } : x = if f : x then g : x else h : x
 */
struct value *iff(struct list *args, struct value *in);

#endif // FORMS_H
