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
 * script.  Every form definition should be preceded by *** form-name
 * where form-name is the col-legal name of the functional form.  Each
 * line of the comment thereafter will be included in the
 * documentation.
 */

/*** compose
 * Function composition.  Feeds its input to the last function in its argument
 * list, then feeds that function's output to the second-to-last, and so on,
 * eventually returning the output of the last function in the list as the
 * combined function's output.
 *
 * compose{ f, g } : x = f : (g : x)
 */
struct value *compose(struct list *args, struct value *in);

/*** construct
 * Sequence construction.  Feeds its input to each of its argument functions,
 * and generate a sequence where each element is the output of one of the
 * argument functions.
 *
 * construct{ f, g } : x  = < f : x, g : x >
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

/*** map
 * Mapping functional form.  Accepts a single function argument.  Input to the
 * form should always be in the form of a list, and the return value will be
 * the result of applying the argument function to each element in the list.
 *
 * map{ f } : < x, y, z > = < f : x, f : y, f : z >
 */
struct value *map(struct list *args, struct value *in);

/*** reduce
 * Reducing functional form.  Accepts a single function argument.  Expects
 * input in the form of a list, return value is the result of first applying
 * the argument function to a pair formed from the first two elements of the
 * input list, then forming a new pair from that result and the next
 * right-most element, and so on until the list is exhausted.
 *
 * reduce{ f } : < x, y, z > = f : < f : < x, y>, z >
 */
struct value *reduce(struct list *args, struct value *in);

#endif // FORMS_H
