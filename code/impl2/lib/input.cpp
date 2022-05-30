#include "input.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void input::destroy()
{
    for (size_t i=0; i<input::data.size; i++)
    {
        free(input::data.names[i]);
        free(input::data.sommets[i].aretes);
    }
    input::data.size = 0;
    free(input::data.names);
    free(input::data.sommets);
    return;
}

adj_data input::extern_constr(const size_t i)
{
    if (input::data.size == 0) {
        TRACE();
        FAIL(1, "graph data is uninitialized\n");
    }

    adj_data res;
    char* ybegin = input::data.names[i];

    res.n = input::data.sommets[i].arite;
    res.meta.val = i;
    res.meta.mapx = strtod(input::data.names[i], &ybegin);
    res.meta.mapy = strtod(ybegin+1, NULL);
    res.liste = (::arete*) malloc(res.n * sizeof(::arete)); // ::arete = from global namespace

    for (int j=0; j<res.n; j++)
    {
        res.liste[j].length = input::data.sommets[i].aretes[j].len;
        res.liste[j].dest = input::data.sommets[i].aretes[j].dest;
    }

    return res;    
}

size_t input::read_graph(const char* fname)
{
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        TRACE();
        FAIL(1, "file <%s> dont exist!! (in read_graph)\n", fname);
    }

    char linebuff[100];
    if (fgets(linebuff, 100, file) == NULL) {
        TRACE();
        FAIL(1, "error reading the file (fgets)...");
    }

    //dont bother removing the newline -> converting to an int

    input::data.size = strtoull(linebuff, NULL, 10);
    if (input::data.size == 0) {
        TRACE();
        FAIL(1, "failed to read an int from data or read 0 (invalid value) | %s", linebuff);
    }

    input::data.names = (char**) malloc(input::data.size * sizeof(char*));
    input::data.sommets = (sommet*) malloc(input::data.size * sizeof(sommet));

    if (input::data.names == NULL || input::data.sommets == NULL) {
        TRACE();
        FAIL(1, "malloc failed somehow, cant continue\n");
    }

    size_t line = 2;

    for (size_t i=0; i<input::data.size; i++)
    {
        //   \/  also tests for EOF 
        if (fgets(linebuff, 100, file) == NULL) {
            TRACE();
            FAIL(1, "failed to read node on line %llu (node #%llu) | %s\n", line, i, linebuff);
        }

        int len = strlen(linebuff);
        if (linebuff[len-1] == '\n') {
            linebuff[len-1] = '\0';
            len -= 1;
        }
        input::data.names[i] = (char*) malloc(len * sizeof(char));

        if (input::data.names[i] == NULL) {
            TRACE();
            FAIL(1, "malloc failed somehow... on line %llu | %s\n", line, linebuff);
        }

        strcpy(input::data.names[i], linebuff);

        line += 1;
    }

    for (size_t i=0; i<input::data.size; i++)
    {
        if (fgets(linebuff, 100, file) == NULL) {
            TRACE();
            FAIL(1, "failed to read node data on line %llu (node #%llu) | %s\n", line, i, linebuff);
        }
        //dont bother with the newline
        int count = atoi(linebuff);

        if (count == 0) {
            if (linebuff[0] == '0' && linebuff[1] == '\0') {
                input::data.sommets[i].arite = 0;
                input::data.sommets[i].aretes = NULL;
                line += 1;
                continue;
            }
            TRACE();
            FAIL(1, "invalide edge count on line %llu, (node #%llu) | %s\n", line, i, linebuff);
        }

        input::data.sommets[i].arite = count;
        input::data.sommets[i].aretes = (arete*) malloc(count * sizeof(arete));

        if (input::data.sommets[i].aretes == NULL) {
            TRACE();
            FAIL(1, "malloc failed somehow... on line %llu \n", line);
        }
        line += 1;

        for (int j=0; j<count; j++)
        {
            if (fgets(linebuff, 100, file) == NULL) {
                TRACE();
                FAIL(1, "failed to read edge on line %llu, (node #%llu) | %s\n", line, i, linebuff);
            }
            int len = strlen(linebuff);
            if (linebuff[len-1] == '\n') {
                linebuff[len-1] = '\0';
                len -= 1;
            }


            char distbuff[23];
            memcpy(distbuff, linebuff, 22);
            distbuff[22] = '\0';
            double dist = strtod(distbuff, NULL);

            if (dist == 0.0) {
                TRACE();
                FAIL(1, "failed to read a valid distance (non zero) from edge data"
                    "on line %llu (node #%llu) | %s \n", line, i, distbuff
                );
            }

            const char* target = linebuff + 22;
            size_t match = 0;
            bool failed = true;
            for (size_t k = 0; k<input::data.size; k++)
            {
                if (strcmp(target, input::data.names[k]) == 0) {
                    match = k;
                    failed = false;
                    break;
                }
            }

            if (failed) {
                TRACE();
                FAIL(1, "could not find matching node for edge on line %llu (node #%llu) | "
                    "string '%s' did not match any known nodes | %s\n"
                    "example node: '%s'\n", line, i, target, linebuff, input::data.names[7]
                );
            }

            input::data.sommets[i].aretes[j].len = dist;
            input::data.sommets[i].aretes[j].dest = match;

            line += 1;
        }
    }

    fclose(file);

    return input::data.size;
}