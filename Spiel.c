#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "random.h"

struct _statisch
{
	int breite;
	int hoehe;
	int aufloesung;
	char* spieler1;
	char* spieler2;
};

typedef struct _statisch statisch;

struct _beweglich
{
	int ball_x;
	int ball_y;
	int schlaeg1;
	int schlaeg2;
	int bewegung_x;
	int bewegung_y;
	int vorzeichen_x;
	int vorzeichen_y;
	unsigned int geschwindigkeit;
	int punkte1;
	int punkte2;
	int exit;
};

typedef struct _beweglich beweglich;

void print_Spielfeld(char** spielfeld, statisch daten){

	for (int i = 0; i < daten.hoehe; i++){
		for (int l = 0; l < daten.breite; l++){
			printf("%c", spielfeld[l][i]);
		}
		printf("\n");
	}
}

void delay(unsigned int mseconds){

    clock_t goal = mseconds + clock();
    while (goal > clock());
}

statisch get_spieler(statisch daten){

	//mit input schreiben 
	daten.spieler1 = "Carl";
	daten.spieler2 = "BOT";
	return daten;
}

void abprallen_oben_unten(beweglich* objekte, statisch daten){

	objekte->ball_y = objekte->ball_y + objekte->vorzeichen_y;

	objekte->ball_x = objekte->ball_x + objekte->vorzeichen_x;//??
	
	objekte->vorzeichen_y = objekte->vorzeichen_y * (-1);
	objekte->ball_y = objekte->ball_y + objekte->vorzeichen_y;

}

void abprallen_schlaeger(beweglich* objekte, statisch daten){

	objekte->vorzeichen_x = objekte->vorzeichen_x * (-1);
}

void setze_Spielfeld(beweglich* objekte, statisch daten, char** spielfeld){

	//Position des Ball und der Schläger bestimmen
	objekte->ball_x = daten.breite / 2;
	objekte->ball_y = daten.hoehe / 2;
	objekte->schlaeg1 = ((daten.hoehe / 2) - 2); 
	objekte->schlaeg2 = ((daten.hoehe / 2) - 2); 

	for (int i = 0; i < daten.hoehe; i++){
		for (int l = 0; l < daten.breite; l++){
			spielfeld[l][i] = ' ';
		}
	}

	spielfeld[objekte->ball_x][objekte->ball_y] = 'o';

	for (int i = 0; i < 5; i++){
		spielfeld[2][objekte->schlaeg1 + i] = 'I';
		spielfeld[daten.breite - 3][objekte->schlaeg2 + i] = 'I';
	}
	for (int i = 1; i < daten.hoehe; i++){
		spielfeld[0][i] = '|';
		spielfeld[daten.breite - 1][i] = '|';
	}
	for (int i = 1; i < daten.breite - 1; i++){
		spielfeld[i][0] = '_';
		spielfeld[i][daten.hoehe - 1] = '_';
	}
}

void BOT_schlaeger(beweglich* objekte, statisch daten, char** spielfeld){

	if (objekte->ball_y - 1 > 1 && objekte->ball_y + 2 < daten.hoehe - 2){

	 	for (int i = 0; i < 5; i++){
			spielfeld[daten.breite - 3][objekte->schlaeg2 + i] = ' ';
		}

		objekte->schlaeg2 = objekte->ball_y - 1;

		for (int i = 0; i < 5; i++){
			spielfeld[daten.breite - 3][objekte->schlaeg2 + i] = 'I';
		}
	}

	if (objekte->ball_y - 1 > 1 && objekte->ball_y + 2 < daten.hoehe - 2){

	 	for (int i = 0; i < 5; i++){
			spielfeld[2][objekte->schlaeg1 + i] = ' ';
		}

		objekte->schlaeg1 = objekte->ball_y - 1;

		for (int i = 0; i < 5; i++){
			spielfeld[2][objekte->schlaeg1 + i] = 'I';
		}
	}
}

void punkt(beweglich* objekte, statisch daten, char** spielfeld, int a){

	if (a == 1){
		objekte->punkte1 = objekte->punkte1 + 1;
	}
	if (a == 2){
		objekte->punkte2 = objekte->punkte2 + 1;
	}
	setze_Spielfeld(objekte, daten, spielfeld);
	printf("\n");
}

void bewege_Ball(beweglich* objekte, statisch daten, char** spielfeld){

	//Alte Position des Balls im Spielfeld löschen
	spielfeld[objekte->ball_x][objekte->ball_y] = ' ';

	//Bewegung des Balls auf der y-Achse
	for (int i = 0; i < objekte->bewegung_y; i++){

		//printf("X = %d \n  Y = %d\n X_v = %d\n Y-v = %d\n", objekte->bewegung_x, objekte->bewegung_y, objekte->vorzeichen_x, objekte->vorzeichen_y);
		BOT_schlaeger(objekte, daten, spielfeld);

		//Wenn der Ball oben oder oben ist
		if (objekte->ball_y + objekte->vorzeichen_y < 1 || 
			objekte->ball_y + objekte->vorzeichen_y > (daten.hoehe - 2)){
			abprallen_oben_unten(objekte, daten);
			break;
		}

		objekte->ball_y = objekte->ball_y + objekte->vorzeichen_y;
	}

	//Bewegung des Balls auf der x-Achse
	for (int i = 0; i < objekte->bewegung_x; i++){
		
		//Wenn der Ball auf den Schläger 1 prallt
		for (int j = 0; j < 5; j++){
			if (objekte->ball_x  == 2 && objekte->ball_y == objekte->schlaeg1 + j){
				abprallen_schlaeger(objekte, daten);
			}
		}

		//Wenn der Ball auf den Schläger 2 prallt
		for (int j = 0; j < 5; j++){
			if (objekte->ball_x  == daten.breite - 3 && objekte->ball_y == objekte->schlaeg2 + j){
				abprallen_schlaeger(objekte, daten);
			}
		}

		//Wenn der Ball auf der linken Seite ist
		if (objekte->ball_x  == 1){
			punkt(objekte, daten, spielfeld, 2);
			objekte->exit = 0;
			return;
		}

		//Wenn der Ball auf der rechten Seite ist
		if (objekte->ball_x == daten.breite - 2){
			punkt(objekte, daten, spielfeld, 1);
			objekte->exit = 0;
			return;
		}

		objekte->ball_x = objekte->ball_x + objekte->vorzeichen_x;
	}

	//Neue Position im Spielfeld des Balls erzeugen
	spielfeld[objekte->ball_x][objekte->ball_y] = 'o';
	
	//printf("\n"); 
}

void starte_spiel(beweglich* objekte, statisch daten, char** spielfeld){
	objekte->geschwindigkeit = 1;
	objekte->punkte1 = 0;
	objekte->punkte2 = 0;

	objekte->bewegung_x = 1;
	objekte->bewegung_y = 1;
	objekte->vorzeichen_x = 1;
	objekte->vorzeichen_y = -1;

	objekte->exit = 1;
	while (objekte->exit == 1){
		bewege_Ball(objekte, daten, spielfeld);
		print_Spielfeld(spielfeld, daten);
		delay(20000 * objekte->geschwindigkeit);
	}
}

statisch get_aufloesung(){

	//Speichert Eigenschaften der Konsole in struct ab
	struct winsize w;
    ioctl(0, TIOCGWINSZ, &w);

    //Wenn Breite und Höhe nicht ungerade sind
    if (w.ws_col % 2 == 0){
    	w.ws_col = w.ws_col - 1;
    }
    if (w.ws_row % 2 == 0){
    	w.ws_row = w.ws_row - 1;
    }

    //Struct für statische Daten initialisieren
	statisch daten;
	daten.breite = w.ws_col;
	daten.hoehe = w.ws_row - 1; 
	daten.aufloesung = daten.breite * daten.hoehe;
	return daten;
}

int main(void){

	//Auflösung
	statisch daten = get_aufloesung();

	//Array für Spielfeld initialisieren
    char** spielfeld = (char**) malloc(daten.breite * sizeof(char *));
    for (int i = 0; i < daten.breite; i++){
         spielfeld[i] = (char*) malloc(daten.hoehe * sizeof(char));
	}

	//Struct für bewegliche Objekte initialisieren
	beweglich* objekte = (beweglich*) malloc(sizeof(beweglich));
	
	//Spielernamen eingeben
	daten = get_spieler(daten);
	
	//Setzt das Spielfeld
	setze_Spielfeld(objekte, daten, spielfeld);

	//Starte Spiel
	starte_spiel(objekte, daten, spielfeld);
	
	print_Spielfeld(spielfeld, daten);

	printf("PUNKTE\n%s: %d\n%s: %d\n", daten.spieler1, objekte->punkte1, daten.spieler2, objekte->punkte2);

	
    for (int i = 0; i < daten.hoehe; i++){
         free(spielfeld[i]);
 	}

	free(spielfeld);

	free(objekte);
}

