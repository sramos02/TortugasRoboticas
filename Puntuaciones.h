#ifndef PUNTUACIONES_H
#define PUNTUACIONES_H
#include <string>
#include <stdlib.h>
using namespace std;

//*************************************DECLARACION DE CONSTANTES Y TIPOS****************************************//

const int MAX_PUNTUACIONES = 10;

typedef struct {
    string nombreJugador;
    int puntos;
} tDatos;

typedef struct {
    tDatos * arrayPuntos;
    int contador;
    int capacidadActual;
} tPuntuaciones;

//*********************************************PROTOTIPOS DE FUNCIONES*************************************************//
void redimensionar(tPuntuaciones & puntos);
void liberar(tPuntuaciones & puntos);
bool cargarPuntuaciones(tPuntuaciones & puntos);
void guardarPuntuaciones(const tPuntuaciones & puntos);
void mostrarPuntaciones(const tPuntuaciones & puntos);
void ordenarPuntuaciones(tPuntuaciones & puntos);
bool actualizarPuntuacion(tPuntuaciones & puntos, const string & nombre, int nuevos);
#endif