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
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "lexer.h"

// Lexing functions
void skip_whitespace(struct lexer_state *state);
void skip_comment(struct lexer_state *state);
void lex_possible_number(struct lexer_state *state);
void lex_ident(struct lexer_state *state);
void lex_char(struct lexer_state *state);
void lex_string(struct lexer_state *state);

// Character identification functions
int is_single_char_token(char c);
int is_valid_ident_char(char c);

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
    state->line = state->col = 1;
    state->cursor = input;
    state->type = NONE;
    state->value.sval = NULL;
    state->error = OK;
    state->open_seq = state->open_spec = state->open_form = 0;
}

// Deletes a lexer
void lexer_delete(struct lexer_state *state)
{
    if(state->type == IDENT || state->type == STRING)
        free(state->value.sval);
    free(state);
}

// Attempts to lex one token
void lex(struct lexer_state *state)
{
    if(state->error != OK)
        return;
    
    // Clearing out any string data that may be left over
    if(state->type == IDENT || state->type == STRING)
        free(state->value.sval);

    // First making sure we have something to lex
    skip_whitespace(state);

    // Checking for possible comments
    while(state->cursor[0] == '#')
    {
        skip_comment(state);
        skip_whitespace(state);
    }

    // Checking for end of input
    if(*(state->cursor) == '\0')
    {
        state->error = END_OF_INPUT;
        state->type = NONE;
        return;
    }

    // Checking for single-character tokens
    if(is_single_char_token(*(state->cursor)))
    {
        switch(*(state->cursor))
        {
        case '<':
            state->type = OPEN_SEQ;
            state->open_seq++;
            break;
        case '>':
            state->type = CLOSE_SEQ;
            state->open_seq--;
            break;
            
        case '(':
            state->type = OPEN_SPEC;
            state->open_spec++;
            break;
        case ')':
            state->type = CLOSE_SPEC;
            state->open_spec--;
            break;

        case '{':
            state->type = OPEN_FORM;
            state->open_form++;
            break;
        case '}':
            state->type = CLOSE_FORM;
            state->open_form--;
            break;

        case ',':
            state->type = SEPARATOR;
            break;

        case '=':
            state->type = ASSIGN;
            break;

        default:
            printf("Something has gone horribly wrong in the lex() function\n");
            break;
        }
        
        state->cursor++;
        state->col++;
        return;
    }

    // Checking for possible numbers
    if(isdigit(*(state->cursor))
       || *(state->cursor) == '+'
       || *(state->cursor) == '-'
       || *(state->cursor) == '.')
    {
        lex_possible_number(state);
        return;
    }
    
    // Checking for identifiers
    if(is_valid_ident_char(*(state->cursor)))
    {
        lex_ident(state);
        return;
    }

    // Checking for characters
    if(*(state->cursor) == '\'')
    {
        lex_char(state);
        return;
    }

    // Checking for strings
    if(*(state->cursor) == '"')
    {
        lex_string(state);
        return;
    }

    // If nothing at all matches, error out...
    state->type = NONE;
    state->error = UNRECOGNIZED_TOKEN;
}

void skip_whitespace(struct lexer_state *state)
{
    char *c = state->cursor;

    // Only operate on valid state
    if(state->error != OK)
        return;

    while(isspace(*c))
    {
        if(*c == '\n')
        {
            state->line++;
            state->col = 1;
        }
        else
        {
            state->col++;
        }
        
        c++;
    }

    state->cursor = c;
}

void skip_comment(struct lexer_state *state)
{
    char *c = state->cursor;
    while(*c != '\0' && *c != '\n')
    {
        c++;
        state->col++;
    }

    if(*c == '\n')
    {
        c++;
        state->col = 1;
        state->line++;
    }

    state->cursor = c;
}

void lex_possible_number(struct lexer_state *state)
{
    char *c = state->cursor;
    char *buf = NULL;
    int i;
    int floating_point = 0;
    int has_digits = 0;

    if(*c == '+' || *c == '-')
        c++;

    // There must be at least one digit or a period to open with
    if(!isdigit(*c) && *c != '.')
    {
        lex_ident(state);
        return;
    }

    if(isdigit(*c))
        has_digits = 1;

    // Now scan any digits before the (optional) decimal point
    while(isdigit(*c))
        c++;

    // Checking for decimal point or non-numeric characters
    if(*c == '.')
    {
        floating_point = 1;
        c++;
        if(isdigit(*c))
            has_digits = 1;
    }
    else if(is_valid_ident_char(*c))
    {
        lex_ident(state);
        return;
    }
    
    // Reading any digits after the decimal point
    while(isdigit(*c))
        c++;

    // Checking for e, E, or non-numeric characters
    if(*c == 'e' || *c == 'E')
    {
        c++;
        
        // If we've already had a . and there aren't any digits left, error out
        if(floating_point && !isdigit(*c))
        {
            state->error = UNRECOGNIZED_TOKEN;
            state->type = NONE;
            return;
        }

        floating_point = 1;

        // Making sure there are digits
        if(!isdigit(*c))
        {
            lex_ident(state);
            return;
        }

        // Now scanning past the post-E digits
        while(isdigit(*c))
            c++;

    }

    // Erroring out if necessary
    if(!has_digits)
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }

    // Now grabbing the actual number
    buf = (char*)calloc(c - state->cursor + 1, sizeof(char));
    for(i = 0; i < c - state->cursor; i++)
        buf[i] = state->cursor[i];
    
    // And reading it
    if(floating_point)
    {
        state->type = FLOAT;
        state->value.fval = atof(buf);
    }
    else
    {
        state->type = INT;
        state->value.ival = atoi(buf);
    }

    free(buf);
    state->col += c - state->cursor;
    state->cursor = c;

}

void lex_ident(struct lexer_state *state)
{
    char *c = state->cursor;
    char *buf = NULL;
    int i;

    // Error out on ineligible characters
    if(!is_valid_ident_char(*c))
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }

    // Catching all the eligible characters
    while(is_valid_ident_char(*c))
        c++;

    // Storing the ident
    buf = (char*)calloc(c - state->cursor + 1, sizeof(char));
    for(i = 0; i < c - state->cursor; i++)
        buf[i] = state->cursor[i];

    // Setting state
    state->col += c - state->cursor;
    state->cursor = c;
    state->type = IDENT;
    state->value.sval = buf;

    // Now checking for reserved words
    if(!strcmp(buf, "true"))
    {
        state->type = TRUE;
        free(buf);
    }

    if(!strcmp(buf, "false"))
    {
        state->type = FALSE;
        free(buf);
    }

    if(!strcmp(buf, "bottom"))
    {
        state->type = BOTTOM;
        free(buf);
    }

}

void lex_char(struct lexer_state *state)
{
    char *c = state->cursor;
    
    if(*c != '\'')
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }
    else
    {
        state->type = CHAR;
        c++;
    }

    if(*c != '\\')
    {
        state->value.cval = *c;
    }
    else
    {
        c++;
        switch(*c)
        {
        case '\'':
            state->value.cval = '\'';
            break;
            
        case '\\':
            state->value.cval = '\\';
            break;

        case 'n':
            state->value.cval = '\n';
            break;

        case 't':
            state->value.cval = '\t';
            break;

        default:
            state->error = UNRECOGNIZED_TOKEN;
            state->type = NONE;
            return;
        }
    }
    
    c++;
    if(*c != '\'')
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }
    else
    {
        c++;
    }

    state->col += c - state->cursor;
    state->cursor = c;
}

void lex_string(struct lexer_state *state)
{
    char *c = state->cursor;
    char *buf = NULL;
    int count = 0;
    int i, j;

    if(*c != '"')
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }
    else
    {
        c++;
    }

    // Counting and verifying the characters in the string
    while(*c != '"' && *c != '\0' && *c != '\n')
    {
        count++;
        if(*c == '\\')
        {
            c++;

            if(*c != '\\'
               && *c != '"'
               && *c != 't'
               && *c != 'n')
            {
                state->error = UNRECOGNIZED_TOKEN;
                state->type = NONE;
                return;
            }
        }

        c++;
    }

    if(*c != '"')
    {
        state->error = UNRECOGNIZED_TOKEN;
        state->type = NONE;
        return;
    }
    else
    {
        c++;
    }

    // Copying and setting final state
    buf = (char*)calloc(count + 1, sizeof(char));
    for(i = 1, j = 0; j < count; j++, i++)
    {
        if(state->cursor[i] == '\\')
        {
            i++;
            switch(state->cursor[i])
            {
            case '\\':
                buf[j] = '\\';
                break;
            case '"':
                buf[j] = '"';
                break;
            case 't':
                buf[j] = '\t';
                break;
            case 'n':
                buf[j] = '\n';
                break;
            }
        }
        else
        {
            buf[j] = state->cursor[i];
        }
    }

    state->col += c - state->cursor;
    state->cursor = c;
    state->type = STRING;
    state->value.sval = buf;
}

int is_single_char_token(char c)
{
    return c == '<'
        || c == '>'
        || c == '('
        || c == ')'
        || c == '{'
        || c == '}'
        || c == ','
        || c == '=';
}

int is_valid_ident_char(char c)
{
    return isalnum(c)
        || c == '+'
        || c == '-'
        || c == '*'
        || c == '/'
        || c == '!'
        || c == '_'
        || c == '-';
}
