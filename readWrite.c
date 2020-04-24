#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readWrite.h"
#include "graph.h"
#include "type.h"

void error(){
	exit(1);
}

// Compte le nombre de station dans le fichier (nom en argument)
int count_nb_points(char *File) {
	FILE *f = fopen(File, "r");
	int i = 0;
	char c;
	char d = 'a';
	while (i < 3) {
		c = fgetc(f);
		if (c == '\n') {
		    i++;
		}
	}
	i = 1;
	do {
		c = fgetc(f);
		if (c == '\n') {
			d = fgetc(f);
			if (d == 'V') {
				i++;
			}
		}
	} while (d != 'E');
	fclose(f);
	return i;
}

// Fonction qui lit le fichier et remplit la structure GRAPH
GRAPH init_station(char *File, GRAPH g) {
	FILE *f = fopen(File, "r");
	char c;
	int num_s;
	int num_l;
	int i = 0;
	int j = 0;
	while (i < 3) {
		c = fgetc(f);
		if (c == '\n') {
			i++;
		}
	}
	//Pour chaque station on lit ses différentes coordonnées
	for (i = 0; i < g.nb_points; i++) {
		c = fgetc(f);
		fscanf(f, "%d %d ", &num_s, &num_l);
		j = 0;
		while (c != '\n') { //lecture du nom de la station
			c = fgetc(f);
			g.station[i].nom_station[j] = c;
			j++;
		}
		g.station[i].nom_station[j - 1] = '\0';
		g.station[i].number_station = num_s;
		g.station[i].number_link = num_l;
	}
	fclose(f);
	return g;
}

// Fonction qui lit le fichier et initialise les arcs entre les stations
GRAPH init_system(char *File, GRAPH g) {
	FILE *f = fopen(File, "r");
	char c, reader = 'a';
	int test = 0;
	int sommet1, sommet2, tmpSec, terminal1, terminal2;
	int i, j;

	while (reader != 'E') { //on lit jusqu'à les lignes commençant par 'E'
		c = fgetc(f);
		if (c == '\n')
			reader = fgetc(f);
	}

	reader = ungetc(reader, f);

	//on initialise tout à null
	for (i = 0; i < g.nb_points; i++) {
		for (j = 0; j < g.nb_points; j++) {
			g.system[i][j].terminal = NO_TERMINAL;
			g.system[i][j].timeSec = NOLINK;
		}
	}

	while (test != 1) {	//on lit le fichier jusqu'à la fin
		fscanf(f, "%c %d %d %d %d %d ", &c, &sommet1, &sommet2, &tmpSec, &terminal1, &terminal2);
		reader = fgetc(f);
		if (reader == EOF){
			test = 1;
		}
		reader = ungetc(reader, f);
		//tmpSec correspond au temps en seconde d'une station à une autre d'un arc
		//terminal1 terminus entre la premiere et la deuxieme station
		//terminal2 terminus entre la deuxieme et la premiere station
		g.system[sommet1][sommet2].timeSec = tmpSec;
		g.system[sommet1][sommet2].terminal = terminal1;
		//le symétrique est aussi initialisé car les lignes de metro vont dans les 2 sens
		g.system[sommet2][sommet1].timeSec = tmpSec;
		g.system[sommet2][sommet1].terminal = terminal2;
	}
	fclose(f);
	return g;
}

// Fonction qui calcule le nombre d'heures en fonction d'une durée
int operation_hour(int duration) {
	return duration / 3600;
}

// Fonction qui calcule le nombre de minutes en fonction d'une durée
int operation_minute(int duration) {
	return duration / 60;
}

// Fonction qui écrit le plus court chemin entre les 2 stations
void write_way(GRAPH g, DIJKSTRA d) {
	int h, m, s;
	h = operation_hour(d.duration);
	m = operation_minute(d.duration - h);
	s = d.duration - (h * 3600 + m * 60);
	struct elem *p1 = d.way;
	struct elem *p2 = d.way->next;
	struct elem *temp;
	printf("- Vous êtes à %s.\n", g.station[d.link_start].nom_station);

	while (!check_empty_list(p2)) {
		if ((g.system[p1->s.number_station][p2->s.number_station].terminal != NO_TERMINAL) && ((p1->s.number_station) == d.link_start)) {
			temp = p1->next;
			if (p1->s.number_link == 30)
				printf("- Prenez la ligne 3bis");
			else if (p1->s.number_link == 70)
				printf("- Prenez la ligne 7bis");
			else
				printf("- Prenez la ligne %d", p1->s.number_link);
			printf(" direction %s. \n",
			       g.station[g.
					 system[p1->s.number_station][temp->s.
								  number_station].
					 terminal].nom_station);
		}
		//Changement de ligne
		if (g.system[p1->s.number_station][p2->s.number_station].terminal == NO_TERMINAL) {
			printf("- A %s, prenez la ligne ", p1->s.nom_station);
			if (p1->s.number_link == 30)
				printf("3bis ");
			else if (p1->s.number_link == 70)
				printf("7bis ");
			else
				printf("%d ", p1->s.number_link);
			if (p2->s.number_station != d.link_end) {
				temp = p2->next;
				printf("direction %s.\n",
				g.station[g.system[p2->s.number_station][temp-> s.number_station].terminal].nom_station);
			} else
				printf("\n");
		}
		p1 = p1->next;
		p2 = p2->next;
	}

	//écrit le temps qu'il faut pour aller entre 2 stations
	printf("- Vous devriez arriver à %s dans ", g.station[d.link_end].nom_station);
	if (h > 1)
		printf("%d heures ", h);
	else if (h == 1)
		printf("%d heure ", h);
	if (m > 1)
		printf("%d minutes ", m);
	else if (m == 1)
		printf("%d minute ", m);
	if (s > 1)
		printf("%d secondes.\n", s);
	else if (s == 1)
		printf("%d seconde.\n", s);
	printf("\n\n");
}
