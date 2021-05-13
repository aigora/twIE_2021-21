#include <stdio.h>
#include <stdlib.h>

typedef struct _jugador Jugador;

Jugador* inicializarJugador();

void resetearJugador(Jugador* j);

int puntuacionJugador(Jugador* j);

void pintarCartas(Jugador* crupier, Jugador* jugador);

int cogerCarta(Jugador* j);

void cogerCartasCrupier(Jugador* crupier);

void comprobarGanador(Jugador* crupier, Jugador* jugador);


int main()
{
		//inicializar variables
		int partida = 1;
		char opcion;

		Jugador *crupier = inicializarJugador();
		Jugador *jugador = inicializarJugador();

        printf("BLACK JACK\n");

        while(partida){//llegar a la mesa
        	resetearJugador(crupier);
        	resetearJugador(jugador);

        	cogerCarta(crupier);
        	cogerCarta(jugador);
        	cogerCarta(jugador);

        	while(1){//empezar una partida
        		pintarCartas(crupier, jugador);
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
