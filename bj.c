#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CARTAS 9

typedef struct  {
	int numCartas;
	int cartas[MAX_CARTAS];
} Jugador;

Jugador* inicializarJugador();

void resetearJugador(Jugador* j);

int puntuacionJugador(Jugador* j);

void pintarCartas(Jugador* crupier, Jugador* jugador);

int cogerCarta(Jugador* j);

void cogerCartasCrupier(Jugador* crupier);

void comprobarGanador(Jugador* crupier, Jugador* jugador);



void cargarTablero(char *tablero, int *manocrupier, int *pcrupier, int *manojugador, int *pjugador){
	FILE *fp;
	int i = 0, j = 0;
	char c;
	char auxTablero[999];

	//abrimos fichero
	fp = fopen("tablero.txt", "r");

	//comprobamos q se abre bien
	if (fp == NULL){
		printf("Error while opening the file.\n");
		return;
	}

	//bucle q lee caracter por caracter y va metiendoselo a auxTablero
	//termina cuando el caracter c vale EOF (caracter especial de end of file)
	c = fgetc(fp);
	while(c != EOF){
		//detecta una b y guarda el indice para luego meter ahi las cartas y las puntuaciones
		if (c == 'b'){
			if (j == 0){
				*manocrupier = i;
			}
			else if (j == 1){
				*pcrupier = i;
			}
			else if (j == 2){
				*manojugador = i;
			}
			else if (j == 3){
				*pjugador = i;
			}
			j++;
		}
		auxTablero[i] = c;//mete el caracter en la posicion correspondiente de auxTablero

		i++;
		c = fgetc(fp);//lee el siguiente caracter
	}
	//termina el bucle y mete en tablero (la variable del main) lo q vale auxTablero
	strcpy(tablero,auxTablero);

	//cierra el fp (se hace siempre al terminar de leer un fichero)
	fclose(fp);
}

void pintarTablero(Jugador* crupier, Jugador* jugador,
					char* tablero, int mcrupier, int pcrupier, int mjugador, int pjugador){
	int i = 0, j = 0;

	printf("\n");
	for(i = 0; i < mcrupier; i++){
		printf("%c", tablero[i]);
	}
	for (j = 0; j < crupier->numCartas; j++) printf(" %d", crupier->cartas[j]);
	for (; tablero[i] != '\n'; i++);

	for(; i < pcrupier; i++){
		printf("%c", tablero[i]);
	}
	printf("%d", puntuacionJugador(crupier));

	for (; tablero[i] != '\n'; i++);

	for(; i < mjugador; i++){
		printf("%c", tablero[i]);
	}
	for (j = 0; j < jugador->numCartas; j++) printf(" %d", jugador->cartas[j]);
	for (; tablero[i] != '\n'; i++);

	for(; i < pjugador; i++){
		printf("%c", tablero[i]);
	}
	printf("%d", puntuacionJugador(jugador));
	for (; tablero[i] != '\n'; i++);

	for(; tablero[i] != '.'; i++){
		printf("%c", tablero[i]);
	}


	printf("\n");
}

int main()
{
        srand(time(NULL));

		//inicializar variables
		int partida = 1;
		char opcion;

		Jugador *crupier = inicializarJugador();
		Jugador *jugador = inicializarJugador();

        //carga el tablero
		char tablero[999];
		int manocrupier;
		int pcrupier;
		int manojugador;
		int pjugador;
		cargarTablero(tablero, &manocrupier, &pcrupier, &manojugador, &pjugador);

        printf("BLACK JACK\n");

        while(partida){//llegar a la mesa
        	resetearJugador(crupier);
        	resetearJugador(jugador);

        	cogerCarta(crupier);
        	cogerCarta(jugador);
        	cogerCarta(jugador);

        	while(1){//empezar una partida
        		pintarTablero(crupier, jugador, tablero, manocrupier, pcrupier, manojugador, pjugador);
        		printf("¿Que quieres hacer?\n");
        		printf("Opciones: Pedir carta (c), Plantarte (p), Doblar (d)\n");
        		scanf(" %c", &opcion);

        		if (opcion == 'c'){
		        	printf("Te sale la carta %d\n", cogerCarta(jugador));
		        	if(puntuacionJugador(jugador) > 21){
		        		printf("Te has pasado\n\n");
		        		break;//termina partida
		        	}
        		} else if (opcion == 'p'){
		        	cogerCartasCrupier(crupier);
		        	comprobarGanador(crupier, jugador);
		        	break;
        		} else if (opcion == 'd'){
        			printf("Has doblado. Te sale la carta %d\n", cogerCarta(jugador));
		        	cogerCartasCrupier(crupier);
		        	comprobarGanador(crupier, jugador);
		        	break;
        		}
        	}//terminar una partida

        	printf("¿Quieres volver a jugar?\n");
        	printf("Opciones: Si (s), No (n)\n");
 			scanf(" %c", &opcion);
 			if(opcion == 'n'){//irte de la mesa
 				partida = 0;
 			}

        }
        free(jugador);
        free(crupier);
        printf("Nos vemos\n");

        return 0;
}

Jugador* inicializarJugador(){
	int i = 0;

	Jugador *j = (Jugador*)malloc(sizeof(Jugador));
	j->numCartas = 0;
	for(i = 0; i < MAX_CARTAS; i++) j->cartas[i] = 0;

	return j;
}

void resetearJugador(Jugador* j){
	int i = 0;

	j->numCartas = 0;
	for(i = 0; i < MAX_CARTAS; i++) j->cartas[i] = 0;

	return;
}

int puntuacionJugador(Jugador* j){
	int i = 0, puntuacion = 0, carta = 0, numAses = 0;

	for(i = 0; i < j->numCartas; i++) {
		carta = j->cartas[i];
		if (carta > 10)
			puntuacion += 10;
		else if (carta == 1){//si llega un as
			puntuacion += 1;
			numAses++;
		} else
			puntuacion += carta;
	}

	for(i = 0; i < numAses; i++){
		if (puntuacion < 11){
			puntuacion += 10;
		}
	}

	return puntuacion;
}

void pintarCartas(Jugador* crupier, Jugador* jugador){
	int i = 0;
	printf("\n-----------------------\n");
	printf("CARTAS DEL CRUPIER:");
	for (i = 0; i < crupier->numCartas; i++) printf(" %d", crupier->cartas[i]);
	printf("\nPuntuacion Total Crupier: %d\n\n", puntuacionJugador(crupier));
	printf("CARTAS DEL JUGADOR:");
	for (i = 0; i < jugador->numCartas; i++) printf(" %d", jugador->cartas[i]);
	printf("\nPuntuacion Total Jugador: %d\n\n", puntuacionJugador(jugador));
}

int cogerCarta(Jugador* j){
	int carta = (rand() % 12) + 1;
	j->cartas[j->numCartas] = carta;
	j->numCartas++;
	return carta;
}

void cogerCartasCrupier(Jugador* crupier){
	for (; puntuacionJugador(crupier) < 17;){
		cogerCarta(crupier);
	}

	return;
}

void comprobarGanador(Jugador* crupier, Jugador* jugador){
	int pCrupier = puntuacionJugador(crupier);
	int pJugador = puntuacionJugador(jugador);

    pintarCartas(crupier, jugador);

	if (pCrupier > 21){
		printf("El crupier se ha pasado. ¡HAS GANADO!");
	} else if (pCrupier > pJugador){
		printf("Tienes menos puntuación que el crupier. PIERDES");
	} else {
		printf("Tienes mas puntuación que el crupier. ¡HAS GANADO!");
	}

	printf("\n");

	return;
}
