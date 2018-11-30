#ifndef CARTAS_H
#define CARTAS_H

//****************************************DECLARACION DE CONSTANTES Y TIPOS*******************************************//
const int MAX_CARTAS = 38;
const int COORDENADAS = 2;
const int NUM_FILAS = 8;

enum tCarta { AVANZAR, GIROIZQUIERDA, GIRODERECHA, LASER, VACIO };

typedef tCarta * tArrayPtrCartas[MAX_CARTAS];
typedef int tCoordenada[COORDENADAS];

typedef struct {
    tArrayPtrCartas lista;
    tCarta * baraja;
    int inicio;
    int final;
} tMazo;

typedef struct {
    int Avanza;
    int Derecha;
    int Izquierda;
    int Laser;
} tMano;

//**********************************************PROTOTIPOS DE FUNCIONES*************************************************//
void crearVacia(tMazo & mazo);
bool sacar(tMazo & mazo, tCarta & carta);
void insertar(tMazo & mazo, tCarta carta);
void liberarMazo(tMazo mazo);
//void mostrarMazo(tMazo mazo);

#endif