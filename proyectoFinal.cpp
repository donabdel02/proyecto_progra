#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX_CARTAS = 52;
const int MAX_MANO = 12;

struct Carta {
    char nombre[4];
    int valor;
};

struct Jugador {
    Carta* mano;
    int cantidad;
    int puntaje;
    char nombre[50];
};
