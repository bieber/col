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

/*** mod
 * Modulus operation.
 * Input - A sequence of two or more integers.
 * Output - The result of applying the modulus operator (remainder of division)
 * between the first and second value, then between that result and the third
 * value, and so on.
 */
struct value *mod(struct list *args, struct value *in);

/*** 1+
 * Incrementor function.
 * Input - An integer or floating point number.
 * Output - The input + 1.
 */
struct value *one_plus(struct list *args, struct value *in);

/*** 1-
 * Decrementor function.
 * Input - An integer or floating point number.
 * Output - The input - 1.
 */
struct value *one_minus(struct list *args, struct value *in);

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

/*** int
 * Integer conversion function.
 * Input - Any value other than bottom.
 * Output - A conversion of the input value to an integer.  Floats are 
 * truncated, integers are passed through verbatim.  True becomes 1, False 
 * becomes 0.  Chars are converted to their ASCII values.  Strings are 
 * converted with the C atoi function.  Sequences simply return the sequence 
 * length.
 */
struct value *to_int(struct list *args, struct value *in);

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
struct value *to_float(struct list *args, struct value *in);

/*** str
 * String conversion function.
 * Input - Any value other than bottom.
 * Output - A conversion of the input value to a string.
 */
struct value *to_string(struct list *args, struct value *in);

/*** print
 * Prints output to the screen.
 * Input - A string value.
 * Output - The same string that was passed in as input, or bottom if a 
 * non-string is passed in.  In additition to passing its input through 
 * unchanged, the print function prints its string input out to the screen.
 */
struct value *print_str(struct list *args, struct value *in);

/*** println
 * Prints output to a line on the screen.
 * Input - A string value.
 * Output - The same string that was passed in as input, or bottom if a 
 * non-string is passed in.  In addition to passing its input through 
 * unchanged, the println function prints its string input to the screen with an
 * additional newline at the end.
 */
struct value *println_str(struct list *args, struct value *in);

/*** head
 * Returns the first element of a sequence.
 * Input - A sequence.
 * Output - The first item in that sequence, or <> if the sequence is empty.
 */
struct value *head(struct list *args, struct value *in);

/*** tail
 * Returns the portion of a sequence after the head.
 * Input - A sequence
 * Output - A sequence containing every element in the input sequence except
 * for the first, or <> if the sequence is empty.
 */
struct value *tail(struct list *args, struct value *in);

/*** length
 * Returns the length of a sequence.
 * Input - A sequence.
 * Output - The length of the sequence as an integer.
 */
struct value *length(struct list *args, struct value *in);

#endif // PRIMITIVES_H
