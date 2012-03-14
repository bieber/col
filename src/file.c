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
#include <string.h>

#include "file.h"

#define BUFFER_STARTSIZE (1024 * 100)

// Reads a file into a string buffer
char *read_file(const char *file)
{
    int bufsize = BUFFER_STARTSIZE;
    int chars_read = 0;
    char *buffer = (char*)calloc(bufsize, sizeof(char));
    FILE *fin = fopen(file, "r");
    int cin = 0;

    if(!fin)
        return NULL;
    
    while(!feof(fin))
    {
        cin = fgetc(fin);

        if(cin == EOF)
            break;
        
        buffer[chars_read] = cin;
        chars_read++;
        
        if(chars_read >= bufsize - 1)
        {
            // Doubling the buffer size if it's filled up
            bufsize *= 2;
            buffer = realloc(buffer, bufsize);
            memset(buffer + chars_read + 1, 0, bufsize / 2);
        }
    }

    return buffer;
}
