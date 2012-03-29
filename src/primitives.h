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

#ifndef PRIMITIVES_H
#define PRIMITIVES_H

struct value;

/**
 * The functions and comments in this header file will be parsed and
 * turned into documentation and array definitions by a script.  Every
 * function definition should be preceded by /*** function-name where
 * function-name is the col-legal name of the primitive function.
 * Each line of the comment thereafter will be included in the
 * documentation.
 */

/*** +
 * Basic addition function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The sum of the input numbers, floating point if any of the input 
 * numbers was floating point, integer otherwise.
 */
struct value *add(struct list *args, struct value *in);

/*** -
 * Basic subtraction function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The difference of each of the numbers, starting with the first 
 * value and subtracting each successive value in order.  Result is floating 
 * point if any of the input numbers are floating point.
 */
struct value *subtract(struct list *args, struct value *in);

/*** *
 * Basic multiplication function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The product of the input numbers, floating point if any of the 
 * inputs were floating point.
 */
struct value *multiply(struct list *args, struct value *in);

/*** /
 * Basic division function.
 * Input - A sequence of numbers, integer or floating point.
 * Output - The quotient of each of the numbers, starting with the first 
 * value and subtracting each successive value in order.  Result is a floating 
 * point number.
 */
struct value *divide(struct list *args, struct value *in);

/*** const
 * Constant function.
 * Specializers - const(n)
 * Input - Any value other than bottom.
 * Output - The value n.
 */
struct value *constant(struct list *args, struct value *in);

/*** id
 * Identity function.
 * Input - Any value.
 * Output - The same value as the input.
 */
struct value *id(struct list *args, struct value *in);

/*** eq
 * Comparison function.
 * Input - A sequence of two or more values.
 * Output - True if all values in the sequence are equivalent, False if they are
 * not, or bottom for invalid input.
 */
struct value *eq(struct list *args, struct value *in);

#endif // PRIMITIVES_H
