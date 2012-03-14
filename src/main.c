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

#include "file.h"
#include "lexer.h"
#include "list.h"

int main(int argc, char *argv[])
{
    char *input;
    
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

    printf("%s\n", input);

    return 0;
}
