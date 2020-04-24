#include "type.h"

#ifndef __GRAPH_H
#define __GRAPH_H

struct point {
	char nom_station[MAX_STRING];
	int number_station; //id de la station dans metro.txt
	int number_link; //numéro de la ligne de la station
};
typedef struct point POINT;

struct path {
	int terminal; //PAS DE TERMINUS si c'est un changement de ligne
	int timeSec; //temps en seconde d'un arc
};
typedef struct path PATH;

struct dijkstra {
	int link_start; //numéro de la station de départ
	int link_end; //numéro de la station d'arrivée
	int duration; //durée qu'il faut pour aller de link_start à link_end
	struct elem *way; //liste des stations qu'il faut traverser
};
typedef struct dijkstra DIJKSTRA;

struct graph {
	DIJKSTRA d; //contient le plus court chemin
	int nb_points; //nombre de station au total dans le GRAPH
	POINT *station;	//tableau des station du GRAPH
	PATH **system; //tableau des arcs du GRAPH
};
typedef struct graph GRAPH;

struct subway {
	GRAPH g; //GRAPH du projet
};
typedef struct subway SUBWAY;

struct elem { //liste des stations
	POINT s;
	struct elem *next;
};

int check_empty_list(struct elem *l);
void display_list(struct elem *l);

struct elem *free_list(struct elem *l);
void free_way_dijkstra(struct elem *l);

GRAPH init_graph(int n);
SUBWAY init_subway(int n);
DIJKSTRA operation_dijkstra(GRAPH g, int link_start, int link_end);
GRAPH operation_short_way(GRAPH g, int a, int b);
void free_graph(GRAPH g);

#endif
