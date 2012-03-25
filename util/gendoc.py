#!/usr/bin/env python

# This script is responsible for generating a list of all available primitive
# functions and functional forms, writing the appropriate include files to fill
# in the corresponding arrays in interpreter.c, and writing the documentation 
# that corresponds to them.  Run it from the project's root directory

import re

# Parses a header file, gets function names and documentation
def readFile(filename, regex):
    retval = []
    name = ''
    doc = ''
    readingDoc = False

    fin = open(filename, 'r')
    for line in fin:
        r = regex.search(line)
        if r:
            retval.append((name, doc, r.group(1)))
            doc = ''
        elif readingDoc:
            if line.lstrip()[0:2] == "*/":
                readingDoc = False
            else:
                doc = doc + line.lstrip()
        elif line.lstrip()[0:4] == "/***":
            name = line.lstrip()[4:].strip()
            readingDoc = True
    fin.close()
    return retval

# Writes a documentation file
def writeDoc(filename, data):
    fout = open(filename, 'w')
    
    for entry in data:
        fout.write(entry[0] + ':' + "\n")
        fout.write(entry[1] + "\n")

    fout.close()

# Writes a header for a name array
def writeNameHeader(filename, data):
    fout = open(filename, 'w')
    first = True

    for entry in data:
        if not first:
            fout.write(",\n")
        else:
            first = False
        fout.write('"' + entry[0] + '"')
    
    fout.write("\n");
    fout.close()

# Writes a header file for a function array
def writeFunctionHeader(filename, data):
    fout = open(filename, 'w')
    first = True

    for entry in data:
        if not first:
            fout.write(",\n")
        else:
            first = False
        fout.write(entry[2])

    fout.write("\n")
    fout.close()

regex = re.compile(r"struct\s+value\s*\*(.*)\(")

# Reading the input files and sorting the function lists
primitives = readFile('src/primitives.h', regex)
primitives.sort(key = lambda e: e[0])
forms = readFile('src/forms.h', regex);
forms.sort(key = lambda e:e[0])

# Writing the documentation files
writeDoc('PRIMITIVE_FUNCTIONS', primitives)
writeDoc('FUNCTIONAL_FORMS', forms)

# Writing the header files to fill in array definitions
writeNameHeader('src/gen/primitive_names.h', primitives)
writeFunctionHeader('src/gen/primitive_defs.h', primitives)
writeNameHeader('src/gen/form_names.h', forms)
writeFunctionHeader('src/gen/form_defs.h', forms)

