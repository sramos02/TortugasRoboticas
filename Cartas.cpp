#include "Cartas.h"
#include "checkML.h"
#include <algorithm>
#include <iostream>
using namespace std;

void insertaDatos(tMazo & mazo);


/* La siguiente función se encarga de llamar a una función que inserte los datos en los mazos y que posteriormente
* los mezcle siendo asi mazos distintos para cada jugador*/
void crearVacia(tMazo & mazo)
{
    mazo.inicio = 0;
    mazo.final = 0;
    insertaDatos(mazo);
    random_shuffle(begin(mazo.lista), end(mazo.lista));
}

/* La siguiente función se encarga de crear una mazo ordenado con el numero de cartas descritas, siempre es el mismo
* para todos los jugadores */
void insertaDatos(tMazo & mazo)
{
    mazo.baraja = new tCarta[4];
    mazo.baraja[0] = AVANZAR;
    mazo.baraja[1] = GIRODERECHA;
    mazo.baraja[2] = GIROIZQUIERDA;
    mazo.baraja[3] = LASER;
    int numero = 0;
    int avanza = 18, giraDerecha = 8, giraIzquierda = 8, laser = 4;
    for (int i = 0; i < avanza; i++) {
        mazo.lista[mazo.final] = &mazo.baraja[0];
        mazo.final++;
    }
    for (int i = 0; i < giraDerecha; i++) {
        mazo.lista[mazo.final] = &mazo.baraja[1];
        mazo.final++;
    }
    for (int i = 0; i < giraIzquierda; i++) {
        mazo.lista[mazo.final] = &mazo.baraja[2];
        mazo.final++;
    }
    for (int i = 0; i < laser; i++) {
        mazo.lista[mazo.final] = &mazo.baraja[3];
        mazo.final++;
    }
    //mazo.cont--; //Porque va de la posicion 0 a la 37 (38 en total)
}

/* La funcion sacar se encarga de sacar una carta del final de un mazo (Arriba) comprobando que queden cartas en el mazo
* y mostrando un error en caso negativo */
bool sacar(tMazo & mazo, tCarta & carta)
{
    bool sacado = false;
    if (mazo.lista[0] != nullptr) {
        carta = *mazo.lista[mazo.final];
        mazo.lista[mazo.final] = nullptr;
        sacado = true;
        if ((mazo.final != 0)) { mazo.final--; }
        else { mazo.final = (MAX_CARTAS - 1); }
        /*carta = *mazo.lista[mazo.cont - 1];
        mazo.lista[mazo.cont - 1] = nullptr;
        mazo.cont--;
        sacado = true;*/
    }
    else {
        cout << "Lo sentimos, no quedan cartas en su mazo" << endl;
    }
    return sacado;
}

/* Esta funcion hace el proceso inverso a la función sacar, introduce una carta al inicio del mazo (Abajo) suponiendo que
* no se pueden insertar mas cartas de las que tiene un mazo ya que hay que robar para poder utilizar una carta*/
void insertar(tMazo & mazo, tCarta carta)
{
    if (mazo.inicio == 0) { mazo.inicio = MAX_CARTAS - 1; };
    //mazo.lista[mazo.inicio] = &carta;
    switch (carta) {
        case AVANZAR: mazo.lista[mazo.inicio] = &mazo.baraja[0]; break;			//Falla porque en algun momento se machaca baraja
        case GIROIZQUIERDA: mazo.lista[mazo.inicio] = &mazo.baraja[1]; break;
        case GIRODERECHA: mazo.lista[mazo.inicio] = &mazo.baraja[2]; break;
        case LASER: mazo.lista[mazo.inicio] = &mazo.baraja[3]; break;
    }
    mazo.inicio--;
    /*if (mazo.lista[0]) {
        for (int i = mazo.final-1; i >= 0; i--) {
            switch (*mazo.lista[i]) {
            case AVANZAR: mazo.lista[i + 1] = &mazo.baraja[0]; break;
            case GIROIZQUIERDA: mazo.lista[i + 1] = &mazo.baraja[1]; break;
            case GIRODERECHA: mazo.lista[i + 1] = &mazo.baraja[2]; break;
            case LASER: mazo.lista[i + 1] = &mazo.baraja[3]; break;
            }
        }
    }
    mazo.lista[0] = &carta;
    mazo.final++;*/
}


void liberarMazo(tMazo mazo) {
    delete[] mazo.baraja;
}


/*DECIDIMOS DEJAR MOSTRARMAZO EN EL CODIGO POR SI ES NECESARIO PARA LABORES DE MANTENIMIENTO O COMPRENSION DEL CODIGO
void mostrarMazo(tMazo mazo)
{
	tCarta carta;
	system("cls");
	for (int pos = 0; pos < MAX_CARTAS; pos++) {
		carta = mazo.tArrayCartas[pos];
		switch (carta) {
		case AVANZAR: cout << "AVANZAR" << endl;
			break;
		case GIROIZQUIERDA: cout << "GIROIZQUIERDA" << endl;
			break;
		case GIRODERECHA: cout << "GIRODERECHA" << endl;
			break;
		case LASER: cout << "LASER" << endl;
			break;
		default: cout << "VACIO" << endl;
			break;
		}
	}

	cout << endl;
	cout << "Inicio: " << mazo.inicio << endl;
	cout << "Fin: " << mazo.fin << endl;
	cout << "Cartas: " << mazo.numCartas << endl;
}
*/