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

#ifndef INTERPRETER_H
#define INTERPRETER_H

struct list;

enum value_type
{
    INT_VAL,    // Integer
    FLOAT_VAL,  // Floating-point
    CHAR_VAL,   // Character
    STRING_VAL, // String
    BOOL_VAL,   // Boolean
    BOTTOM_VAL, // Bottom
    SEQ_VAL     // Sequence
};

enum function_type
{
    PRIMITIVE, // Primitive function
    USER,      // User-defined function
    FORM       // Functional form
};

struct value
{
    // Data type
    enum value_type type;
    
    // Actual value
    union value_data
    {
        int int_val;
        float float_val;
        int bool_val;
        char char_val;
        char *str_val;
        struct list *seq_val;
    } value;
};

struct function
{
    // Function or form identifier
    char *name;
    // Function type
    enum function_type type;
    // List of arguments, either form arguments or primitive specializers
    struct list *args;

    // Location in source file
    int line;
    int col;
};

#endif // INTERPRETER_H
