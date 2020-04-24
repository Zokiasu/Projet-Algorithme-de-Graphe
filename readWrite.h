#include "type.h"
#include "graph.h"

#ifndef __READWRITE_H
#define __READWRITE_H

void error();

int count_nb_points(char *File);
GRAPH init_station(char *File, GRAPH g);
GRAPH init_system(char *File, GRAPH g);
void write_way(GRAPH g, DIJKSTRA d);

#endif
