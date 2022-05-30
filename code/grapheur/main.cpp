/*
l'objectif de ce programme est d'extraire les donnes utiles du filaire et de les
stocker pour que le programme principal les utilise : les intersections (sommets)
et les rues accessibles par un bus (aretes), 2 points sont en intersection si ils
ontla meme coordonee et sont au meme niveau (pot/sol/tunnel), la longueur de chaque
route (divisee par la limite de vitesse) est aussi entregistree et permet de definir
le cout des aretes.

d'apres les specifications, un sous-troncon ne traverse jamais une itersection, donc
les points au centre des troncons n'ont pas besoin d'etre enregistres comme sommets
potentiels

les troncons peuvent etre decoupes sans intersection, il faut faire un "nettoyage" du
graphe pour s'assurer que tous les sommets sont de varaise intersections (qui n'ont pas
exactement 2 aretes (plus complexe avec les sens uniques))

tous les troncons utilisent le format "MultiLineString" :
"{""coordinates"": [[ (liste) ]], ""type"":""MultiLineString""}"
ou (liste) est une liste json de coordonnees 2d, on s'interesse a la premiere, la derniere,
et la somme des longueurs

!! le troncon a la ligne 990 est en plusieurs parties par default ???, corrige pour l'algorithme
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>
#include <vector>

const size_t FILE_BUFFER_SIZE = 1000000; //1 mega per line
const size_t DATA_SIZE = 10000;


#define assert(cond, ...) if (!(cond)) {printf(__VA_ARGS__); exit(1);}


struct vec2 {
    double x;
    double y;
};
struct arete {
    std::string dest;
    double length;
};
struct sommet {
    std::string pos;
    std::vector<arete> sortant;
};


void attempt_add(std::map<std::string, sommet> &map, sommet obj)
{
    if (map.count(obj.pos) == 0)
    {
        //create an entry for this point
        map[obj.pos] = obj;

        assert(map[obj.pos].sortant.size() > 0, "wtf 1\n");
    }
    else
    {
        //merge obj with the already existing point, assuming it is impossible
        //to get a duplicate edge
        map[obj.pos].sortant.insert(
            map[obj.pos].sortant.end(), 
            obj.sortant.begin(), 
            obj.sortant.end()
        );

        assert(map[obj.pos].sortant.size() > 0, "wtf 2\n");
    }
}


int main()
{
    std::map<std::string, sommet> pregraph;

    //prepare one data line worth of buffers
    char* data[42];
    for (int i=0; i<42; i++) {
        data[i] = (char*) malloc(DATA_SIZE * sizeof(char));
    }


    //un premier passage pour determiner la longueur de buffer necessaire
    FILE* file = fopen("./rues-stras.csv", "r");

    if (file == NULL) {
        printf("file dont exist!\n");
        return 0;
    }

    char* buff = (char*) malloc(FILE_BUFFER_SIZE * sizeof(char));

    fseek(file, 0, SEEK_SET);

    //dump the first line
    fgets(buff, FILE_BUFFER_SIZE, file);

    int line_count = 0;
    int route_count = 0;
    int auto_count = 0;
    int sens_unique = 0;

    while (!feof(file))
    {
        fgets(buff, FILE_BUFFER_SIZE, file);

        int dn = 0;
        int dn_idx = 0;
        int idx = 0;

        //split the data on semicolons
        while (true)
        {
            if (buff[idx] == ';') {
                data[dn][dn_idx] = '\0';
                dn += 1;
                dn_idx = 0;
                idx += 1;
                continue;
            }
            if (buff[idx] == '\0') {
                data[dn][dn_idx] = '\0';
                break;
            }
            data[dn][dn_idx] = buff[idx];
            idx += 1;
            dn_idx += 1;
        }
        line_count += 1;


        //handle a line of data here

        if (strcmp(data[4], "route") == 0
         || strcmp(data[4], "autoroute") == 0)
        {
            route_count += 1;
            std::vector<std::string> spx;
            std::vector<std::string> spy;
            char* shape = data[1];
            char num_buffer[25];
            char num_buffer2[25];
            int num_idx;

            //iterate the Geo Shape until "[[" is found
            int shape_idx = 1;
            while (shape[shape_idx] != '[' || shape[shape_idx-1] != '[') {
                if (shape[shape_idx] == '\0') {
                    printf(
                        "geo shape read error, could not find '[[', on line %i, %s\n", 
                        idx,  shape);
                    exit(1);
                }
                shape_idx += 1;
            }
            shape_idx += 1;
            
            assert(shape[shape_idx-1] == '[' && shape[shape_idx-2] == '[',
                "geo shape error on line %i", idx
            );

            int len = strlen(shape);

            //iterate over the list
            while (true)
            {
                //make sure we have "["
                assert(shape[shape_idx] == '[', 
                "invalid geo shape:  [x1, x2] does not start with '[', on line %i, "
                "\n char %i / %i | %s\n", idx, shape_idx, len, shape);
                shape_idx += 1;

                //extract the first number
                num_idx = 0;
                while((shape[shape_idx] >= '0' && shape[shape_idx] <= '9') || shape[shape_idx] == '.')
                {
                    num_buffer[num_idx] = shape[shape_idx];
                    shape_idx += 1;
                    num_idx += 1;
                }
                num_buffer[num_idx] = '\0';
                spx.push_back(std::string(num_buffer));


                //make sure this is ", "
                assert(shape[shape_idx] == ',' && shape[shape_idx+1] == ' ', 
                    "\ninvalid geo shape : wrong separator on line %i, "
                    "\nchar %i / %i \n%s \nfound '%c%c' when ', ' was expected\n"
                    "number #1 : %s\n", 
                    line_count, shape_idx, len, shape, shape[shape_idx], shape[shape_idx+1], num_buffer
                );
                shape_idx += 2;

                //extract the scond number
                num_idx = 0;
                while((shape[shape_idx] >= '0' && shape[shape_idx] <= '9') || shape[shape_idx] == '.')
                {
                    num_buffer2[num_idx] = shape[shape_idx];
                    shape_idx += 1;
                    num_idx += 1;
                }
                num_buffer2[num_idx] = '\0';
                spy.push_back(std::string(num_buffer2));

                assert(shape_idx < len-2, 
                "last numbers : %s, %s"
                "\ninvalid shape : %i / %i | %s\n", 
                num_buffer, num_buffer2, shape_idx, len, shape);

                //safe exit condition
                if (shape[shape_idx] == ']' && shape[shape_idx+1] == ']' && shape[shape_idx+2] == ']') {
                    break;
                }

                //make sure we have "], "
                assert(shape[shape_idx] == ']' && shape[shape_idx+1] == ',' && shape[shape_idx+2] == ' ', 
                "\ninvalid geo shape %s : %s\n", data[10], shape);
                shape_idx += 3;
            }

            assert(spx.size() == spy.size(), "size mismatch??\n");

            int pt_num = spx.size()-1;

            double length = 0;
            for (int i=0; i<pt_num; i++)
            {
                double x1 = atof(spx[i].c_str());
                double x2 = atof(spx[i+1].c_str());
                double y1 = atof(spy[i].c_str());
                double y2 = atof(spy[i+1].c_str());
                assert(x1 != 0 && y1 != 0 && x2 != 0 && y2 != 0, "read error lol\n");
                length += sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
            }

            //calcule les 2 sommets potentiels
            sommet start = (sommet){
                .pos = spx[0] + "," + spy[0],
                .sortant = std::vector<arete>()
            };
            sommet end = (sommet){
                .pos = spx[pt_num] + "," + spy[pt_num],
                .sortant = std::vector<arete>()
            };

            start.sortant.push_back((arete){
                .dest = spx[pt_num] + "," + spy[pt_num],
                .length = length
            });

            end.sortant.push_back((arete){
                .dest = spx[0] + "," + spy[0],
                .length = length
            });

/*
            if (strcmp(data[18], "sens indirect") != 0)
            {
                start.sortant.push_back((arete){
                    .dest = spx[pt_num] + "," + spy[pt_num],
                    .length = length
                });
            }
            if (strcmp(data[18], "sens direct") != 0)
            {
                end.sortant.push_back((arete){
                    .dest = spx[0] + "," + spy[pt_num],
                    .length = length
                });
            }
*/
            //et les ajoute au pseudographe (ou les combine)
            attempt_add(pregraph, start);
            attempt_add(pregraph, end);
        }

        //printf("\rhandled line %i", line_count+1);
    }
    printf("\nha, done!!\n");
    printf("%i dont %i routes\n", line_count, route_count);
    printf("%i sommets enregistres\n", pregraph.size());

    free(buff);
    fclose(file);

    //////////////////////////////////////////////////////
    //write to file in a node-oriented format

    file = fopen("rues-graphe.gph", "w+");

    fprintf(file, "%llu\n", pregraph.size());

    for (auto it=pregraph.begin(); it!=pregraph.end(); it++)
    {
        sommet cur = it->second;
        fprintf(file, "%s\n", cur.pos.c_str());
    }

    printf("done writing nodes\n");

    for (auto it=pregraph.begin(); it!=pregraph.end(); it++)
    {
        sommet cur = it->second;
        int count = cur.sortant.size();

        fprintf(file, "%i\n", count);

        for (int i=0; i<count; i++) {
            fprintf(file, "%.20lf%s\n", cur.sortant[i].length, cur.sortant[i].dest.c_str());
        }
    }
    fclose(file);

    printf("done!\n");

    return 0;
}