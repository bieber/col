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

#include <stdio.h>
#include <stdlib.h>

#include "file.h"
#include "lexer.h"
#include "list.h"

#define USAGE "Usage: col [-v] <source file> [command-line arguments]\n"

int main(int argc, char *argv[])
{
    char *input;
    struct lexer_state *lexer;
    
    if(argc < 2)
    {
        printf("Usage: col <source file> [command-line arguments]\n");
        return 1;
    }

    input = read_file(argv[1]);
    if(!input)
    {
        printf("Error reading input file\n");
        return 1;
    }

    lexer = lexer_new();
    lexer_init(lexer, input);

    while(lexer->error == OK)
    {
        lex(lexer);
        if(lexer->error == UNRECOGNIZED_TOKEN)
        {
            printf("Lex error: Unrecognized token at line %d, column %d\n",
                   lexer->line, lexer->col);
        }
        else if(lexer->error == OK)
        {
            switch(lexer->type)
            {
            case NONE:
                printf("Error: No token lexed\n");
                break;
            case INT:
                printf("Integer: %d\n", lexer->value.ival);
                break;
            case FLOAT:
                printf("Float: %f\n", lexer->value.fval);
                break;
            case CHAR:
                printf("Character: %c\n", lexer->value.cval);
                break;
            case STRING:
                printf("String: %s\n", lexer->value.sval);
                break;
            case BOTTOM:
                printf("Bottom\n");
                break;
            case TRUE:
                printf("True\n");
                break;
            case FALSE:
                printf("False\n");
                break;
            case IDENT:
                printf("Identifier: %s\n", lexer->value.sval);
                break;
            case OPEN_SEQ:
                printf("Open Sequence\n");
                break;
            case CLOSE_SEQ:
                printf("Close Sequence\n");
                break;
            case OPEN_SPEC:
                printf("Open Specializer\n");
                break;
            case CLOSE_SPEC:
                printf("Close Specializer\n");
                break;
            case OPEN_FORM:
                printf("Open Functional Form\n");
                break;
            case CLOSE_FORM:
                printf("Close Functional Form\n");
                break;
            case SEPARATOR:
                printf("Separator\n");
                break;
            case ASSIGN:
                printf("Assignment\n");
                break;
            }
        }
    }

    printf("End of file\n");

    return 0;
}
