#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "readWrite.h"
#include "type.h"

int read(char *text, int size){
	char *tmp = NULL;

	if(fgets(text,size,stdin) != NULL){
		tmp = strchr(text, '\n');
		if(tmp != NULL) {
			*tmp = '\0';
		}
		return 0;
	} else {
		return 1;
	}
}

int main(int argc, char** argv) {
	
	int nb_point = count_nb_points("metro.txt");
	int stationStart = 0, stationEnd = 0;
	char *EntryStation = malloc (sizeof (*EntryStation) * 256);
	char *ExitStation = malloc (sizeof (*ExitStation) * 256);

	SUBWAY sub = init_subway(nb_point);
	sub.g = init_station("metro.txt", sub.g);
	sub.g = init_system("metro.txt", sub.g);
	
	printf("Veuillez entré une gare de départ\n");
	//scanf("%49s",d);
	read(EntryStation,sizeof (*EntryStation) * 256);
	//Vérifie à quel station correspond la station entré par l'utilisateur
	for(int i = 0; i < nb_point; i++) {
		char j[255];
		sprintf(j,"%d", sub.g.station[i].number_station);

		if(strcasecmp(EntryStation, sub.g.station[i].nom_station) == 0) {//Si l'utilisateur entre le nom de la station
			if(stationStart == 0){
				stationStart = sub.g.station[i].number_station;
			}
		} else if (strcmp(EntryStation, j) == 0) {//Si l'utilisateur entre l'id de la station
			if(stationStart == 0){
				stationStart = sub.g.station[i].number_station;
			printf("L'id de la gare entré est %s.\n", sub.g.station[i].nom_station);
			}
		} else if (i == nb_point-1 && stationStart == 0) {
			printf("Désolé la garde de départ entré ne correspond à aucune gare connus.\n");
			return 1;
		}
	}	
	printf("Veuillez entré une gare de d'arriver\n");
	read(ExitStation,sizeof (*ExitStation) * 256);
	for(int i = 0; i < nb_point; i++) {
		char j[255];
		sprintf(j,"%d", sub.g.station[i].number_station);

		if(strcasecmp(ExitStation, sub.g.station[i].nom_station) == 0) {//Si l'utilisateur entre le nom de la station
			if(stationEnd == 0) {
				stationEnd = sub.g.station[i].number_station;
			}
		} else if (strcmp(ExitStation, j) == 0) {//Si l'utilisateur entre l'id de la station
			if(stationEnd == 0) {
				stationEnd = sub.g.station[i].number_station;
			printf("L'id de la gare entré est %s.\n", sub.g.station[i].nom_station);
			}
		} else if (i == nb_point-1 && stationEnd == 0) {
			printf("Désolé la garde d'arriver entré ne correspond à aucune gare connus.\n");
			return 1;
		}
	}

	operation_short_way(sub.g, stationStart, stationEnd);

	//libération de la mémoire
	free(EntryStation);
	free(ExitStation);
	free_graph(sub.g);
	free_way_dijkstra(sub.g.d.way);
	return 0;
}
