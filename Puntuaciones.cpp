#include "puntuaciones.h"
#include "checkML.h"
#include <fstream>
#include <stdlib.h>
#include <iostream>
using namespace std;

//Funciones auxiliares modulo Puntuaciones
void mostrarPuntuacionesReordenadas(tPuntuaciones puntos);
void mostrarPuntuacionesDefault(const tPuntuaciones & puntos);

/* La funcion redimensionar crea un espacio en memoria dinamica donde guardar las puntuaciones guardando posiciones de 4 en 4
*  Asi se asegura de consumir demasiada memoria. Para redimensionar copia la información en un array auxiliar y luego lo escribe en un nuevo ya
*  redimensionado, por ultimo, borra el primer array y el auxiliar*/
void redimensionar(tPuntuaciones & puntos)
{
    puntos.capacidadActual += 4;
    tDatos *aux = new tDatos[puntos.capacidadActual];
    for (int i = 0; i < puntos.contador; i++) { aux[i] = puntos.arrayPuntos[i]; }
    delete[] puntos.arrayPuntos;
    puntos.arrayPuntos = new tDatos[puntos.capacidadActual];
    for (int i = 0; i < puntos.contador; i++) { puntos.arrayPuntos[i] = aux[i]; }
    delete[] aux;
}

/* Esta funcion borra toda la información que hay en memoria dinamica con respecto a puntuaciones sin qeu queden restos tras el programa */
void liberar(tPuntuaciones & puntos) {
    delete[] puntos.arrayPuntos;
}

/* El fichero que lee la funcion cargarPuntuaciones contiene las puntuaciones de los jugadores, haciendo como una especie
* de historial el cual se actualiza despues de jugar una partida */
bool cargarPuntuaciones(tPuntuaciones & puntos)
{
    bool abierto = false;
    ifstream fichero;
    fichero.open("Puntuaciones.txt");
    if (fichero.is_open()) {
        puntos.arrayPuntos = new tDatos[4]; //Reservo memoria dinamica y creo un array de tipo tDatos apuntado desde puntos.arraypuntos
        puntos.contador = 0;
        puntos.capacidadActual = 4;
        abierto = true;

        fichero >> puntos.arrayPuntos[puntos.contador].nombreJugador;
        fichero >> puntos.arrayPuntos[puntos.contador].puntos;
        if (puntos.arrayPuntos[puntos.contador].puntos > 0) { puntos.contador++; }

        while ((!fichero.eof()) && (puntos.contador > 0)) {
            fichero >> puntos.arrayPuntos[puntos.contador].nombreJugador;
            fichero >> puntos.arrayPuntos[puntos.contador].puntos;
            puntos.contador++;
            if (puntos.capacidadActual == puntos.contador) { redimensionar(puntos); }
        }
    }
    else { cout << "Lo sentimos, no ha podido abrirse el archivo de puntuaciones" << endl; }
    fichero.close();
    return abierto;
}

/* La funcion guardarPuntuaciones se encarga de guardar la informacion actualizada en el fichero para poder
* leerse de nuevo cuando se desee */
void guardarPuntuaciones(const tPuntuaciones & puntos)
{
    ofstream fichero;
    int pos = 0;
    fichero.open("Puntuaciones.txt");
    if (fichero.is_open()) { //Realizamos este algoritmo para que no escriba un salto de linea al final del fichero
        fichero << puntos.arrayPuntos[pos].nombreJugador << " ";
        fichero << puntos.arrayPuntos[pos].puntos;
        pos++;

        for (pos; pos < puntos.contador; pos++) {
            fichero << endl << puntos.arrayPuntos[pos].nombreJugador << " ";
            fichero << puntos.arrayPuntos[pos].puntos;
        }
    }
    else { cout << "El fichero no pudo abrise" << endl; }
    fichero.close();
}

/* Muestra por pantalla un menu para elegir la forma de mostrar las puntuaciones
* para ello llamamos a las funciones mostrarDefault o mostrarReordenado. No mostrarmos directamente porque modificaría
* el array y no queremos eso, solo que lo muestre reordenado*/
void mostrarPuntaciones(const tPuntuaciones & puntos)
{
    int opc;
    cout << endl;
    cout << "Como desea ordenar las puntuaciones: " << endl;
    cout << "1- Puntuacion Decreciente (Y nombre decreciencite si procede)" << endl;
    cout << "2- Nombre Decreciente" << endl;
    cin >> opc;
    while ((opc != 1) && (opc != 2)) { cout << "Error, introduzca una opcion valida: "; cin >> opc; }
    system("cls");
    if (opc == 1){ mostrarPuntuacionesDefault(puntos); }
    else { mostrarPuntuacionesReordenadas(puntos);}
}

/* La siguiente función se encarga de actualizar las puntuaciones de los jugadores, si el jugador ya está en el
* fichero se le suma la puntuacion correspondiente, en caso contrario se crea una nueva "ficha de jugador" y se
* añade tambien al fichero comprobando que quepa, si no cabe reemplaza a la ultima porsicion*/
bool actualizarPuntuacion(tPuntuaciones & puntos, const string & nombre, int nuevos)
{
    bool actualizado = false;
    int posicion = 0;
    while ((!actualizado) && (posicion < puntos.contador)) {
        if (puntos.arrayPuntos[posicion].nombreJugador == nombre) {
            actualizado = true;
            puntos.arrayPuntos[posicion].puntos += nuevos;
        }
        posicion++;
    }
    if ((!actualizado)) {
        if (puntos.contador < MAX_PUNTUACIONES) {
            puntos.arrayPuntos[puntos.contador].nombreJugador = nombre;
            puntos.arrayPuntos[puntos.contador].puntos = nuevos;
            puntos.contador++;
        }
        else {
            puntos.arrayPuntos[MAX_PUNTUACIONES - 1].nombreJugador = nombre;
            puntos.arrayPuntos[MAX_PUNTUACIONES - 1].puntos = nuevos;
        }
        actualizado = true;
    }
    ordenarPuntuaciones(puntos);
    return actualizado;
}

/* La funcion ordenar, como su nombre indica, ordena las puntuaciones. Se ha optado por utilizar el metodo
 * de insercion ya que se acompaña una sola palabra (nombre simple) de una puntuacion y no se pueden repetir nombres */
void ordenarPuntuaciones(tPuntuaciones & puntos)
{
    tDatos aux;
    int i, j;
    for (i = 1; i < puntos.contador; i++) {
        aux = puntos.arrayPuntos[i];
        j = i;
        while ((j > 0) && (aux.puntos > puntos.arrayPuntos[j - 1].puntos)) {
            puntos.arrayPuntos[j] = puntos.arrayPuntos[j - 1];
            j--;
        }
        puntos.arrayPuntos[j] = aux;
    }
}

/* Muestra por pantalla el array de puntuaciones tal y como se encuentra ordenado*/
void mostrarPuntuacionesDefault(const tPuntuaciones & puntos)
{
    for (int pos = 0; pos < puntos.contador; pos++) {
        cout << puntos.arrayPuntos[pos].nombreJugador << "\t";
        cout << puntos.arrayPuntos[pos].puntos << endl;
    }
}

/* Reordena el array de puntuaciones PERO NO LO MODIICA, SOLO REORDENA DE FORMA LOCAL, y lo muestra por pantalla*/
void mostrarPuntuacionesReordenadas(tPuntuaciones puntos)
{
    tDatos aux;
    int i, j;
    for (i = 1; i < puntos.contador; i++) {
        j = i;
        aux = puntos.arrayPuntos[i];
        while ((j > 0) && (aux.nombreJugador > puntos.arrayPuntos[j - 1].nombreJugador)) {
            puntos.arrayPuntos[j] = puntos.arrayPuntos[j - 1];
            j--;
        }
        puntos.arrayPuntos[j] = aux;
    }
    mostrarPuntuacionesDefault(puntos);
    ordenarPuntuaciones(puntos);
}