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

#include "lexer.h"

// Creates a new lexer
struct lexer_state *lexer_new()
{
    struct lexer_state *retval =
        (struct lexer_state*)malloc(sizeof(struct lexer_state));

    retval->line = 0;
    retval->col = 0;
    retval->cursor = NULL;
    retval->type = NONE;
    retval->value.sval = NULL;
    retval->error = END_OF_INPUT;
    retval->open_seq = retval->open_spec = retval->open_form = 0;

    return retval;
}

// Initializes a lexer with a new input
void lexer_init(struct lexer_state *state, char *input)
{
    state->line = state->col = 0;
    state->cursor = input;
    state->type = NONE;
    state->value.sval = NULL;
    state->error = OK;
    state->open_seq = state->open_spec = state->open_form = 0;
}

// Attempts to lex one token
void lex(struct lexer_state *state)
{
    // Here goes the really fun stuff
}
