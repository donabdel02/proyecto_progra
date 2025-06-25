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

Carta sacarCarta(Carta* mazo, int& tam) {
    tam--;
    return mazo[tam];
}

void agregarCarta(Jugador* jugador, Carta carta) {
    if (jugador->cantidad < MAX_MANO) {
        jugador->mano[jugador->cantidad++] = carta;
    }
}

void turnoJugador(Jugador* jugador, Carta* mazo, int& tam) {
    string opcion;
    while (true) {
        jugador->puntaje = calcularPuntaje(jugador);
        cout << "Tu puntaje: " << jugador->puntaje << endl;
        if (jugador->puntaje >= 21) break;
        cout << "Quieres pedir carta (Hit) o plantarte (Stand)? ";
        cin >> opcion;
        if (opcion == "Hit" || opcion == "hit") {
            agregarCarta(jugador, sacarCarta(mazo, tam));
            mostrarMano(jugador);
        } else {
            break;
        }
    }
}

void turnoBanca(Jugador* banca, Carta* mazo, int& tam) {
    banca->puntaje = calcularPuntaje(banca);
    while (banca->puntaje < 17) {
        agregarCarta(banca, sacarCarta(mazo, tam));
        banca->puntaje = calcularPuntaje(banca);
    }
}

void guardarHistorial(const char* nombre, const char* resultado) {
    ofstream archivo("historial.txt", ios::app);
    if (!archivo) {
        cerr << "Error al abrir el archivo de historial.\n";
        return;
    }

    time_t ahora = time(0);
    tm* t = localtime(&ahora);

    archivo << left << setw(20) << nombre
            << setw(10) << resultado
            << setfill('0')
            << setw(4) << (1900 + t->tm_year) << "/"
            << setw(1) << (1 + t->tm_mon) <<  "/"
            << setw(2) << t->tm_mday - 1 << " "
            << setw(2) << t->tm_hour << ":"
            << setw(2) << t->tm_min << ":"
            << setw(2) << t->tm_sec
            << setfill(' ') << "\n"; 

    archivo.close();
}

void jugarPartida() {
    Carta* mazo = new Carta[MAX_CARTAS];
    int tam;
    inicializarMazo(mazo, tam);

    Jugador* jugador = new Jugador;
    Jugador* banca = new Jugador;

    jugador->mano = new Carta[MAX_MANO];
    banca->mano = new Carta[MAX_MANO];
    jugador->cantidad = 0;
    banca->cantidad = 0;

    cout << "Ingresa tu nombre: ";
    cin.ignore();
    cin.getline(jugador->nombre, 50);

    agregarCarta(jugador, sacarCarta(mazo, tam));
    agregarCarta(jugador, sacarCarta(mazo, tam));

    agregarCarta(banca, sacarCarta(mazo, tam));
    agregarCarta(banca, sacarCarta(mazo, tam));

    cout << "\nTus cartas: ";
    mostrarMano(jugador);
    cout << "Cartas de la banca: ";
    mostrarMano(banca, true);

    turnoJugador(jugador, mazo, tam);

    if (jugador->puntaje > 21) {
        cout << "Te pasaste de 21. Pierdes.\n";
        guardarHistorial(jugador->nombre, "Perdio");
    } else {
        turnoBanca(banca, mazo, tam);

        cout << "Cartas de la banca: ";
        mostrarMano(banca);
        cout << "Puntaje de la banca: " << banca->puntaje << endl;

        if (banca->puntaje > 21 || jugador->puntaje > banca->puntaje) {
            cout << "Ganaste!\n";
            guardarHistorial(jugador->nombre, "Gano");
        } else if (jugador->puntaje < banca->puntaje) {
            cout << "Perdiste.\n";
            guardarHistorial(jugador->nombre, "Perdio");
        } else {
            cout << "Empate. Gana la banca.\n";
            guardarHistorial(jugador->nombre, "Empate");
        }
    }

    delete[] mazo;
    delete[] jugador->mano;
    delete[] banca->mano;
    delete jugador;
    delete banca;
}

void mostrarHistorial() {
    ifstream archivo("historial.txt");
    if (!archivo) {
        cout << "No hay historial guardado.\n";
        return;
    }

    cout << "\n--- HISTORIAL DE PARTIDAS ---\n";
    string linea;
    while (getline(archivo, linea)) {
        cout << linea << endl;
    }
    archivo.close();
}

int main() {
    srand(time(0));
    int opcion;
    do {
        cout << "\n--- MENU PRINCIPAL ---\n";
        cout << "1. Jugar partida\n";
        cout << "2. Ver historial\n";
        cout << "3. Salir\n";
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                jugarPartida();
                break;
            case 2:
                mostrarHistorial();
                break;
            case 3:
                cout << "Gracias por jugar, hermanito!\n";
                break;
            default:
                cout << "Opción inválida.\n";
        }

    } while (opcion != 3);

    return 0;
}