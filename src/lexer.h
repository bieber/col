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

#ifndef LEXER_H
#define LEXER_H

enum token_type
{
    NONE,        // Used on error or beginning of input
    INT,         // 5
    FLOAT,       // 5.3
    CHAR,        // 'x'
    STRING,      // "string"
    BOTTOM,      // bottom
    TRUE,        // true
    FALSE,       // false
    IDENT,       // identifier
    OPEN_SEQ,    // <
    CLOSE_SEQ,   // >
    OPEN_SPEC,   // (
    CLOSE_SPEC,  // )
    OPEN_FORM,   // {
    CLOSE_FORM,  // }
    SEPARATOR,   // ,
    ASSIGN       // =
};

union token_val
{
    int ival;
    float fval;
    char cval;
    char *sval;
};

enum lex_error
{
    OK,
    END_OF_INPUT,
    UNRECOGNIZED_TOKEN
};

struct lexer_state
{
    // Cursor's current position in the file
    int line;
    int col;

    // Current location in string
    char *cursor;

    // Details of the last lexed token
    enum token_type type;
    union token_val value;
    
    // Current error code
    enum lex_error error;

    // Number of open sequences, specializers, and forms
    int open_seq;
    int open_spec;
    int open_form;
};

// Creates a new lexer
struct lexer_state *lexer_new();
// Initializes a lexer with a new input
void lexer_init(struct lexer_state *state, char *input);
// Deletes a lexer
void lexer_delete(struct lexer_state *state);

// Attempts to lex one token
void lex(struct lexer_state *state);

#endif // LEXER_H
