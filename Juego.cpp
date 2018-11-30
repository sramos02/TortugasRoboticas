#include "Juego.h"
#include "Cartas.h"
#include "checkML.h"
#include "Dibujo.h"
#include <iostream>
#include <fstream>
#include <conio.h>
using namespace std;

//Funciones auxiliares del modulo Juego
bool transformarTablero(tJuego & juego, char caracter, int numJugador, int x, int y);
bool movimientoAvanza(tJuego & juego, tCarta carta, int x, int y);
void giroDerecha(tJuego & juego, int x, int y);
void giroIzquierda(tJuego & juego, int x, int y);
bool disparoLaser(tJuego & juego, int & x, int & y);
bool accionRobar(tJuego & juego);
bool accionSecuencia(tJuego & juego, tMazo & cartas);
void utilizarCarta(tMano & mano, tCarta tipoCarta);
bool esFinDePartida(tJuego & juego);
void incluirCarta(tMano & mano, tCarta carta);
void busquedaNoVacio(tDir direccion, tJuego juego, int x, int y, int & i, int & j);
tTecla leerTecla();

//************************************************LEER Y SOLICITAR TABLERO*************************************************//

/* Solicita el numero de jugadores al usuario, una vez introducidos pedirá el nombre de los jugadores y les asignará
* un color predefinido */
void solicitaJugadores(tJuego & juego)
{
    int pos;
    string nombre;
    cout << "Intrduzca el numero de jugadores que desean jugar: ";
    cin >> juego.numJugadores;

    while ((juego.numJugadores <= 0) || (juego.numJugadores > MAX_JUGADORES)) {
        cout << "Error inesperado, introduzca un dato valido [1-" << MAX_JUGADORES << "]: ";
        cin >> juego.numJugadores;
    }

    for (pos = 0; pos < juego.numJugadores; pos++) {
        cout << "Introduzca el nombre del jugador " << pos + 1 << ": ";
        cin >> juego.jugadores[pos].nombreJugador;
        juego.jugadores[pos].colorJugador = PALETA[pos + TIPOS_CASILLAS - 1]; //Restamos el color TORTUGA que se encuentra al final
    }
}

/* La siguiente función se encarga de pedir al usuario el nombre del fichero que desea abrir, si lo puede abrir
* busca el tablero que corresponda con el numero de jugadores deseado y lo carga, por ultimo inicializa las
* manos y los mazos de los jugadores */
bool cargarJuego(tJuego & juego)
{
    bool cargado = false;
    bool esJugador; //Indica la posicion de los jugadores en el tablero
    string nombreFichero, basura;
    ifstream fichero;
    char caracter;
    int x, y, numero; //Numero utilizado para buscar coincidencia con numJugadores
    int numJugadores = 0;

    solicitaJugadores(juego);
    cout << "Introduzca el nombre del fichero que desea abrir: ";
    cin >> nombreFichero;

    fichero.open(nombreFichero);
    if (fichero.is_open()) {
        juego.numJoyas = 0;
        //Busca el tablero con el numero de jugadores deseado
        fichero >> numero;
        while (!(numero == juego.numJugadores)) {
            for (int i = 0; i < NUM_FILAS + 1; i++) {
                getline(fichero, basura);
            }
            fichero >> numero;
        }
        fichero.get(caracter);

        //Una vez encuentra el tablero deseado lo leemos y lo convertimos
        for (x = 0; x < NUM_FILAS; x++) {
            for (y = 0; y < NUM_FILAS; y++) {
                fichero.get(caracter);
                esJugador = transformarTablero(juego, caracter, numJugadores, x, y);
                if (esJugador) {
                    juego.jugadores[numJugadores].coordenada[0] = x;
                    juego.jugadores[numJugadores].coordenada[1] = y;
                    juego.jugadores[numJugadores].posicionOriginal[0] = x;
                    juego.jugadores[numJugadores].posicionOriginal[1] = y;
                    numJugadores++;
                    juego.numJoyas++;
                }
            }
            fichero.get(caracter);
        }


        //Inicializamos las manos a 0 y cargamos los mazos una vez carga el fichero
        juego.turnoActual = 0;
        for (int x = 0; x < numJugadores; x++) {
            juego.turnoActual = x;
            crearVacia(juego.jugadores[x].mazo);
            juego.jugadores[x].mano.Avanza = 0;
            juego.jugadores[x].mano.Derecha = 0;
            juego.jugadores[x].mano.Izquierda = 0;
            juego.jugadores[x].mano.Laser = 0;
            for (int i = 0; i < 3; i++) { accionRobar(juego); }
            fichero.close();
            cargado = true;
        }
        juego.turnoActual = 0;

    }

    else { cout << "Lo sentimos, el fichero no ha podido abrirse"; }
    return cargado;
}

/* La siguiente función es la encargada de convertir caracteres leidos desde el fichero a estados de las casillas del
* tablero, en caso de ser una TORTUGA, también asignará el numero de la tortuga correspondiente con los nombres
* introducidos anteriormente y la dirección a la que mira */
bool transformarTablero(tJuego & juego, char caracter, int numJugador, int x, int y)
{
    bool esJugador = false;
    switch (caracter) {
        case 'C': juego.tablero[x][y].estado = CAJA;
            break;
        case '@': juego.tablero[x][y].estado = HIELO;
            break;
        case '#': juego.tablero[x][y].estado = MURO;
            break;
        case '$': juego.tablero[x][y].estado = JOYA;
            break;
        case 'U': juego.tablero[x][y].estado = TORTUGA;
            juego.tablero[x][y].tortuga.numero = numJugador;
            juego.tablero[x][y].tortuga.direccion = NORTE;
            esJugador = true;
            break;
        case 'D': juego.tablero[x][y].estado = TORTUGA;
            juego.tablero[x][y].tortuga.numero = numJugador;
            juego.tablero[x][y].tortuga.direccion = SUR;
            esJugador = true;
            break;
        case 'R': juego.tablero[x][y].estado = TORTUGA;
            juego.tablero[x][y].tortuga.numero = numJugador;
            juego.tablero[x][y].tortuga.direccion = ESTE;
            esJugador = true;
            break;
        case 'L': juego.tablero[x][y].estado = TORTUGA;
            juego.tablero[x][y].tortuga.numero = numJugador;
            juego.tablero[x][y].tortuga.direccion = OESTE;
            esJugador = true;
            break;
        default: juego.tablero[x][y].estado = VACIA;
            break;
    }
    return esJugador;
}

//*********************************************EJECUTAR TURNO**************************************************//

/* AccionRobar es la encargada de robar una carta del mazo cuando el jugador lo indique, su funcionamiento se basa en
* SACAR una carta del mazo y añadirla a la mano del jugador, se actualiza el tablero despues de dicha acción para que
* se pueda reflejar en pantalla */
bool accionRobar(tJuego & juego)
{
    tCarta cartaRobada;
    bool sacada = false, robada = false;
    sacada = sacar(juego.jugadores[juego.turnoActual].mazo, cartaRobada);
    if (sacada) {
        incluirCarta(juego.jugadores[juego.turnoActual].mano, cartaRobada);
        mostrarJuego(juego);
        robada = true;
    }
    return robada;
}

/* Se encarga de dirigir el proceso de crear una secuencia de cartas, es decir, una serie de movimientos que realiza el
* jugador. Para ello pide el movimiento, lo transforma tipo carta y comprueba si quedan cartas de ese tipo en la mano
* del jugador, si es asi, se ejecuta el movimiento y se resta una carta a dicha mano */
bool accionSecuencia(tJuego & juego, tMazo & cartas)
{
    bool ganaPartida, movimientoEjecutado;
    int numCartas, x, y;
    tTecla movimiento;
    tCarta tipoCarta;

    ganaPartida = esFinDePartida(juego);
    movimiento = leerTecla();
    while ((movimiento != SALIR) && (!ganaPartida)) {
        switch (movimiento) {
            case AVANZA:  tipoCarta = AVANZAR;
                numCartas = juego.jugadores[juego.turnoActual].mano.Avanza;
                break;
            case DERECHA: tipoCarta = GIRODERECHA;
                numCartas = juego.jugadores[juego.turnoActual].mano.Derecha;
                break;
            case IZQUIERDA: tipoCarta = GIROIZQUIERDA;
                numCartas = juego.jugadores[juego.turnoActual].mano.Izquierda;
                break;
            case DISPARO: tipoCarta = LASER;
                numCartas = juego.jugadores[juego.turnoActual].mano.Laser;
                break;
        }

        if (numCartas > 0) {
            x = juego.jugadores[juego.turnoActual].coordenada[0];
            y = juego.jugadores[juego.turnoActual].coordenada[1];
            movimientoEjecutado = movimientoTortuga(juego, tipoCarta, x, y);
            if (movimientoEjecutado) {
                utilizarCarta(juego.jugadores[juego.turnoActual].mano, tipoCarta);
                mostrarJuego(juego); //Tenemos que mostrar el juego aqui al haber tomado ladecision de ejecutar las cartas una a una en vez de en bloque, si no deberiamos mostrarlo en ejecutarJuego(main)
                insertar(cartas, tipoCarta);
            }
        }
        else if (numCartas <= 0) { cout << "No quedan cartas de ese tipo en su mano" << endl; }
        movimiento = leerTecla();
        ganaPartida = esFinDePartida(juego);
    }

    if (ganaPartida) {
        for (int i = 0; i < juego.numJugadores; i++) { liberarMazo(juego.jugadores[i].mazo); }
    }
    return ganaPartida;
}

/* Esta función se ejecuta cuando es posible utilizar una carta de una mano concreta, su función es quitar dicha carta de la mano*/
void utilizarCarta(tMano & mano, tCarta tipoCarta){
    switch (tipoCarta) {
        case AVANZAR: mano.Avanza--;
            break;
        case GIRODERECHA: mano.Derecha--;
            break;
        case GIROIZQUIERDA: mano.Izquierda--;
            break;
        case LASER: mano.Laser--;
            break;
    }
}


/* leerTecla es una función que lee teclas especiales del teclado, espera hasta introducir una y
* se asegura de ser una de las esperadas, (up, down, right, left) y en caso contrario
* muestra un mensaje de error*/
tTecla leerTecla()
{
    tTecla movimiento;
    int tecla;
    cout << "Esperando instrucciones para ejecutar la secuencia..." << endl;

    cin.sync();
    tecla = _getch();
    while ((tecla != 0xe0) && (tecla != 32) && (tecla != 13)) {
        cout << "Tecla incorrecta, pruebe con [UP,LEFT, RIGHT, SPACE, ENTER]" << endl;
        tecla = _getch();
    }

    if (tecla == 0xe0) {
        tecla = _getch();
        if (tecla == 72) { movimiento = AVANZA; }
        else if (tecla == 77) { movimiento = DERECHA; }
        else if (tecla == 75) { movimiento = IZQUIERDA; }
        else { cout << "Tecla no valida" << endl; movimiento = SALIR; }
    }
    else if (tecla == 32) { movimiento = DISPARO; }
    else { movimiento = SALIR; }
    return movimiento;
}

/* Ejecuta el turno de un jugador, pidiendo la opcion que desea ejecutar en su turno tantas veces como haga falta,
* una vez introducida ejecuta la acción correspondiente a dicha elección*/
bool ejecutarTurno(tJuego & juego)
{
    tMazo cartas = juego.jugadores[juego.turnoActual].mazo;
    char accion;
    bool ganaPartida = false;
    bool cartaRobada;

    cout << "Turno de: " << juego.jugadores[juego.turnoActual].nombreJugador << endl;
    cout << "E: Jugar Secuencia, R: Robar" << endl;
    cout << "Esperando instruccion..." << endl;
    accion = _getch();
    accion = toupper(accion);
    while ((accion != 'E') && (accion != 'R')) {
        cout << "Error, debe introducir una opcion valida [E/R]: ";
        accion = _getch();
        accion = toupper(accion);
    }

    if (accion == 'R') { cartaRobada = accionRobar(juego); }
    else {
        ganaPartida = accionSecuencia(juego, cartas);
        juego.jugadores[juego.turnoActual].mazo = cartas;
    }
    return ganaPartida;
}

/* Comprueba si la partida ha finalizado comparando la posicion del jugador y la dirección a la que mira con la
* posicion de las joyas */
bool esFinDePartida(tJuego & juego)
{
    bool ganaPartida = false;
    if (juego.numJoyas < juego.numJugadores) { ganaPartida = true; }
    return ganaPartida;
}

/* Cambia el turno al siguiente jugador asegurandose de que todos los jugadores jueguen */
void cambiarTurno(tJuego & juego)
{
    juego.turnoActual++;
    juego.turnoActual = juego.turnoActual % juego.numJugadores;
}

/* Incluye una carta a la mano del jugador, se utiliza cuando una carta es jugada, */
void incluirCarta(tMano & mano, tCarta carta)
{
    switch (carta) {
        case AVANZAR: mano.Avanza++;
            break;
        case GIROIZQUIERDA: mano.Izquierda++;
            break;
        case GIRODERECHA: mano.Derecha++;
            break;
        case LASER: mano.Laser++;
            break;
    }
}

//************************************************ACTUALIZACION DE TABLERO**************************************************//

/* La siguiente función se encarga de manejar el movimiento de las Tortugas, lo hace llamando a subfunciones que
* controlan cada tipo de movimiento de una manera muy simple y finalmente devolviendo la nueva posicion del jugador */
bool movimientoTortuga(tJuego & juego, tCarta carta, int x, int y)
{
    bool generado = false;
    tDir direccion;
    direccion = juego.tablero[x][y].tortuga.direccion;
    if (carta == AVANZA) { generado = movimientoAvanza(juego, carta, x, y); }
    else if (carta == GIRODERECHA) { giroDerecha(juego, x, y); generado = true; }
    else if (carta == GIROIZQUIERDA) { giroIzquierda(juego, x, y); generado = true; }
    else if (carta == LASER) { generado = disparoLaser(juego, x, y); }


    return generado;
}

/* La siguiente funcion se encarga del movimiento avanzar de una tortuga, comprobando la posicion a la que se va a mover y, si es posible, ejecutando
* el movimiento.*/
bool movimientoAvanza(tJuego & juego, tCarta carta, int x, int y)
{
    bool generado = false, permiteMovimiento = false;
    int xVacia, xCaja, yVacia, yCaja;
    tDir movimiento = juego.tablero[x][y].tortuga.direccion;
    switch (movimiento)
    {
        case NORTE:  xVacia = x; xCaja = x - 2; x = x - 1; yVacia = y; yCaja = y; permiteMovimiento = true;
            break;
        case SUR: xVacia = x; xCaja = x + 2; x = x + 1; yVacia = y; yCaja = y; permiteMovimiento = true;
            break;
        case ESTE: if ((y + 1) < NUM_FILAS){ //Usamos permiteMovimiento aqui para hacer un filtro y que no pueda saltar de linea en tablero teniendo limite
                yVacia = y; yCaja = y + 2; y = y + 1; xVacia = x; xCaja = x; permiteMovimiento = true; }
            break;
        case OESTE:if ((y - 1) >= 0) {
                yVacia = y; yCaja = y - 2; y = y - 1; xVacia = x; xCaja = x; permiteMovimiento = true;}
            break;
    }

    if (permiteMovimiento) {
        switch (juego.tablero[x][y].estado) {
            case VACIA:	juego.tablero[x][y].estado = TORTUGA;
                juego.tablero[x][y].tortuga.direccion = movimiento;
                juego.tablero[x][y].tortuga.numero = juego.tablero[xVacia][yVacia].tortuga.numero;
                juego.tablero[xVacia][yVacia].estado = VACIA;
                generado = true;
                break;
            case CAJA: if (juego.tablero[xCaja][yCaja].estado == VACIA) {
                    juego.tablero[xCaja][yCaja].estado = CAJA;
                    juego.tablero[x][y].estado = TORTUGA;
                    juego.tablero[x][y].tortuga.direccion = movimiento;
                    juego.tablero[x][y].tortuga.numero = juego.tablero[xVacia][yVacia].tortuga.numero;
                    juego.tablero[xVacia][yVacia].estado = VACIA;
                    generado = true;
                } break;

            case JOYA: juego.tablero[x][y].estado = TORTUGA;
                juego.tablero[x][y].tortuga.direccion = movimiento;
                juego.tablero[x][y].tortuga.numero = juego.tablero[xVacia][yVacia].tortuga.numero;
                juego.tablero[xVacia][yVacia].estado = VACIA;
                juego.numJoyas--;
                generado = true;
                break;
        }
    }

    if (generado)
    {
        if (movimiento == NORTE) { juego.jugadores[juego.turnoActual].coordenada[0]--; }
        else if (movimiento == SUR) { juego.jugadores[juego.turnoActual].coordenada[0]++; }
        else if (movimiento == ESTE) { juego.jugadores[juego.turnoActual].coordenada[1]++; }
        else { juego.jugadores[juego.turnoActual].coordenada[1]--; }
    }

    return generado;
}

/*Encargado de generar el giro a la derecha, se comprueban todas las posibilidades y no se contemplan
* las que derivan en error o no se pueden contemplar */
void giroDerecha(tJuego & juego, int x, int y)
{
    tDir direccion = juego.tablero[x][y].tortuga.direccion;
    switch (direccion) {
        case NORTE: direccion = ESTE; break;
        case SUR: direccion = OESTE; break;
        case ESTE: direccion = SUR; break;
        case OESTE: direccion = NORTE; break;
    }
    juego.tablero[x][y].tortuga.direccion = direccion;
}

/*Encargado de generar el giro a la izquierda, se comprueban todas las posibilidades y no se contemplan
* las que derivan en error o no se pueden contemplar */
void giroIzquierda(tJuego & juego, int x, int y)
{
    tDir direccion = juego.tablero[x][y].tortuga.direccion;
    switch (direccion) {
        case NORTE: direccion = OESTE; break;
        case SUR: direccion = ESTE; break;
        case ESTE: direccion = NORTE; break;
        case OESTE: direccion = SUR; break;
    }
    juego.tablero[x][y].tortuga.direccion = direccion;
}

/*Encargado de generar el movimiento laser, se comprueban todas las posibilidades y no se contemplan las que
* derivan en error o no se pueden contemplar. En caso de que el laser impacte contra otro jugador se comrprueba
* la posicion del nuevo y se mueve hasta que choque con algo, si es una joya, gana.*/
bool disparoLaser(tJuego & juego, int & x, int & y)
{
    bool disparo = false;
    int i = x, j = y;  //i, j  jugador2(IMPACTA) || x, y jugador1(DISPARA)
    int Jugador = 0;
    tDir direccion, dir2;

    direccion = juego.tablero[x][y].tortuga.direccion;
    int xOriginal = juego.jugadores[juego.turnoActual].posicionOriginal[0];
    int yOriginal = juego.jugadores[juego.turnoActual].posicionOriginal[1];
    busquedaNoVacio(direccion, juego, x, y, i, j);
    if (juego.tablero[i][j].estado == HIELO) {
        juego.tablero[i][j].estado = VACIA;
        cout << "Bloque derretido" << endl;
        disparo = true;
    }
    else if ((juego.tablero[i][j].estado == JOYA) && (juego.tablero[xOriginal][yOriginal].estado == VACIA)) {
        juego.tablero[xOriginal][yOriginal].estado = TORTUGA;
        juego.tablero[xOriginal][yOriginal].tortuga.direccion = direccion;
        juego.tablero[xOriginal][yOriginal].tortuga.numero = juego.tablero[x][y].tortuga.numero;
        juego.jugadores[juego.turnoActual].coordenada[0] = xOriginal;
        juego.jugadores[juego.turnoActual].coordenada[1] = yOriginal;
        juego.tablero[x][y].estado = VACIA;
        disparo = true;
    }
    else if (juego.tablero[i][j].estado == TORTUGA) {
        int n = 1;
        int turno = juego.tablero[i][j].tortuga.numero;
        dir2 = juego.tablero[i][j].tortuga.direccion;
        juego.tablero[i][j].estado = VACIA;
        busquedaNoVacio(direccion, juego, i, j, i, j);
        switch (direccion) {
            case NORTE: if (juego.tablero[i][j].estado == JOYA) {juego.numJoyas--;} else { i--; }
                break;
            case SUR:   if (juego.tablero[i][j].estado == JOYA) {juego.numJoyas--;} else { i++; }
                break;
            case ESTE:  if (juego.tablero[i][j].estado == JOYA) {juego.numJoyas--;} else { j++; }
                break;
            case OESTE: if (juego.tablero[i][j].estado == JOYA) {juego.numJoyas--;} else { j--; }
                break;
        }
        juego.tablero[i][j].estado = TORTUGA;
        juego.tablero[i][j].tortuga.direccion = dir2;
        juego.tablero[i][j].tortuga.numero = turno;
        juego.jugadores[turno].coordenada[0] = i;
        juego.jugadores[turno].coordenada[1] = j;
        disparo = true;
    }
    return disparo;
}

/* Busca la primera casilla no vacia en una direccion y devuelve su posicion en i, j*/
void busquedaNoVacio(tDir direccion, tJuego juego, int x, int y, int & i, int & j)
{
    int n = 1;
    i = x; j = y;

    switch (direccion) {
        case NORTE: while ((juego.tablero[x - n][y].estado == VACIA) && ((x - n) > 0)) { n++; }
            i = x - n;
            break;
        case SUR:   while ((juego.tablero[x + n][y].estado == VACIA) && ((x + n) < NUM_FILAS)) { n++; }
            i = x + n;
            break;
        case ESTE:  while ((juego.tablero[x][y + n].estado == VACIA) && ((y + n) < NUM_FILAS)) { if (n < NUM_FILAS) { n--; } }
            j = y + n;
            break;
        case OESTE: while ((juego.tablero[x][y - n].estado == VACIA) && ((y - n) > 0)) { if (n > 0) { n++; } }
            j = y - n;
            break;
    }
}