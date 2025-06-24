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
void inicializarMazo(Carta* mazo, int& tam) {
    const char* nombres[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    int valores[] = {2,3,4,5,6,7,8,9,10,10,10,10,11};
    tam = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 13; j++) {
            strcpy(mazo[tam].nombre, nombres[j]);
            mazo[tam].valor = valores[j];
            tam++;
        }
    }
    for (int i = 0; i < tam; i++) {
        int r = rand() % tam;
        swap(mazo[i], mazo[r]);
    }
}

void mostrarMano(Jugador* jugador, bool ocultarPrimera = false) {
    cout << "Cartas: ";
    for (int i = 0; i < jugador->cantidad; i++) {
        if (ocultarPrimera && i == 0) cout << "[??] ";
        else cout << "[" << jugador->mano[i].nombre << "] ";
    }
    cout << endl;
}

int calcularPuntaje(Jugador* jugador) {
    int total = 0, ases = 0;
    for (int i = 0; i < jugador->cantidad; i++) {
        total += jugador->mano[i].valor;
        if (strcmp(jugador->mano[i].nombre, "A") == 0) ases++;
    }
    while (total > 21 && ases > 0) {
        total -= 10;
        ases--;
    }
    return total;
}
