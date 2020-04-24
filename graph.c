#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "readWrite.h"
#include "type.h"

struct elem *create_list() {
	return NULL;
}

int check_empty_list(struct elem *l) {
	if (l == NULL)
		return 1;
	else
		return 0;
}

// Fonction qui affiche le contenu de la liste avec le nom des stations et de sa ligne
void display_list(struct elem *l) {

	if (check_empty_list(l)) {
		printf("La liste du chemin est vide \n");
	}
	while (l) {
		l = l->next;
	}
	printf("\n");
}

// Fonction qui calcule le plus court chemin entre a et b
GRAPH operation_short_way(GRAPH g, int a, int b) {
	if ((a == INVALID) || (b == INVALID))
		return g;
	g.d = operation_dijkstra(g, a, b);
	display_list(g.d.way);
	write_way(g, g.d);
	return g;
}

// Fonction qui ajoute au début une station à la liste
struct elem *add_elem_start(struct elem *l, POINT i) {
	struct elem *new = malloc(sizeof(struct elem));
	new->s = i;
	new->next = l;
	return new;
}

DIJKSTRA init_dijkstra(int a, int b) {
	DIJKSTRA d;
	d.link_start = a;
	d.link_end = b;
	d.way = create_list();
	d.duration = 0;
	return d;
}

GRAPH init_graph(int n) {
	int i;
	GRAPH g;
	g.nb_points = n;
	g.d = init_dijkstra(INVALID, INVALID);
	g.station = malloc(n * sizeof(POINT));
	g.system = malloc(n * sizeof(PATH *));
	for (i = 0; i < n; i++) {	//on met 0 dans le temps entre les arcs
		g.system[i] = calloc(n, sizeof(PATH));
	}
	return g;
}

SUBWAY init_subway(int n) {
	SUBWAY m;
	m.g = init_graph(n);
	return m;
}

// Fonction qui met dans le tableau t de taille n les valeurs égales à val
void init_table(int *t, int n, int val) {
	int i;
	for (i = 0; i < n; i++) {
		t[i] = val;
	}
}

// Fonction booléenne pour savoir si toute les stations ont été traités
int check_all_point_fine(int *t, int n) {
	int i;
	for (i = 0; i < n; i++) {
		if (t[i] == UNTREATED)
			return 0;
	}
	return 1;
}

// Fonction qui permet d'ajouter une station parmi le chemin à parcourir lors du calcul de DIJKSTRA
DIJKSTRA add_dijkstra(DIJKSTRA d, POINT s) {
	d.way = add_elem_start(d.way, s);
	return d;
}

// Fonction qui remplit la liste des stations à parcourir
DIJKSTRA create_way(DIJKSTRA d, int *tab_peres, GRAPH g) {
	int temp;
	int s = d.link_end;
	d = add_dijkstra(d, g.station[s]);
	while (s != d.link_start) {
		temp = s;
		s = tab_peres[temp];
		d.duration += g.system[temp][tab_peres[temp]].timeSec;
		d = add_dijkstra(d, g.station[s]);
	}
	return d;
}

// Algorithme de Dijkstra qui calcule le plus court chemin
DIJKSTRA operation_dijkstra(GRAPH g, int link_start, int link_end) {
	DIJKSTRA d = init_dijkstra(link_start, link_end);
	int i, min, point_to_be_traited;
	if (link_end >= g.nb_points)
		error();
	if (link_start >= g.nb_points)
		error();
	if ((link_start == INVALID) || (link_end == INVALID))
		return d;
	int station_traited[g.nb_points]; //contient 0 ou 1 si la station a ete vu ou non
	int station_back[g.nb_points]; //contient le plus court entre i et un de ses pères
	int tdijkstra[g.nb_points];	//contient les plus petites distances entre la station de depart et d'arrivé

	init_table(station_traited, g.nb_points, UNTREATED); //aucune station n'a été traité
	init_table(station_back, g.nb_points, FALSEBACK); //aucune station n'a encore de père
	station_traited[link_start] = TREATED; //la station a été traité

	for (i = 0; i < g.nb_points; i++) {
		if (g.system[link_start][i].timeSec != NOLINK) {
			tdijkstra[i] = g.system[link_start][i].timeSec;
			station_back[i] = link_start;
		} else
			tdijkstra[i] = NOLINK;
	}
	tdijkstra[link_start] = 0; //la plus petite distance entre la station de départ et lui-même est 0

	while (!check_all_point_fine(station_traited, g.nb_points)) {
		min = INFINITED;
		//recherche de la prochaine station avec la plus petite distance
		for (i = 0; i < g.nb_points; i++) {
			if ((station_traited[i] == UNTREATED) && (tdijkstra[i] < min)) {
				point_to_be_traited = i;
				min = tdijkstra[i];
			}
		}
		station_traited[point_to_be_traited] = TREATED;
		// Station a traiter
		for (i = 0; i < g.nb_points; i++) {
			//si on améliore la plus petite distance en passant par cette station
			if (g.system[point_to_be_traited][i].timeSec != NOLINK) {
				if (tdijkstra[i] >= (tdijkstra[point_to_be_traited] + g.system[point_to_be_traited][i].timeSec)) {
					tdijkstra[i] = tdijkstra[point_to_be_traited] + g.system[point_to_be_traited][i].timeSec;
					station_back[i] = point_to_be_traited;	// alors le pere de cette station est la station traité au départ
				}
			}
		}
	}
	d = create_way(d, station_back, g);	//crée la liste des stations qu'il faut traverser
	return d;
}

// Fonction qui vide la mémoire d'une liste
struct elem *free_list(struct elem *l) {
	struct elem *tmp;
	while (l) {
		tmp = l->next;
		free(l);
		l = tmp;
	}
	return NULL;
}

// Fonction pour libérer la mémoire de toutes les allocations de GRAPHE
void free_graph(GRAPH g) {
	int i;
	free(g.station);
	for (i = 0; i < g.nb_points; i++) {
		free(g.system[i]);
	}
	free(g.system);
}

// Fonction qui libère la liste dans la structure dijkstra
void free_way_dijkstra(struct elem *l) {
	l = free_list(l);
}