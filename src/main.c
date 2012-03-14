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

#include "lexer.h"
#include "list.h"

int main(int argc, char *argv[])
{
    int i;
    struct list *list = list_new();

    for(i = 0; i < 8; i++)
    {
        list_push_back(list, (void*)i);
    }

    for(i = 0; i < 8; i += 2)
        list_remove(list, i);

    for(i = 0; i < list->count; i++)
    {
        printf("%d\n", list_get(list, i));
    }

    return 0;
}
