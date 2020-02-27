#include <iostream>
#include <sys/ioctl.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>

#include "base.h"
#include "digit.h"
using namespace std;

// TODO: Evitar que los cositos aparezcan dentro del jugador

/*
 * Control del tiempo para la frecuencia de actualización de la pantalla
 * Tamaño de la pantalla 
 * Jugador (update, render, tamaño dínamico)
 * Bordes (colisiones, coordenadas)
 * Digitos de pi, manzanas, (posiciones, colision, update, render, que sigan al Jugador)
 * Puntuacion (numero)
 * Mejores puntuaciones (txt)
 * 
*/

// Variables globales
Input* input;
struct winsize w;
int sizex = 0, sizey = 0;
float frameRate = 0;
float delta = 0; // Tiempo entre 2 frames
char digits[100] = {};  // Array donde se guardan los 100 primeros dígitos de pi
float factor = 1;
bool canRender = true;
unsigned int points = 0;

bool eaten = true;

#include "player.h"
Player player(10, 10);


#define MAXFRAMES 30.0f

#define WIDTH 20
#define HEIGHT 20

int main() {
    // Setup
    fstream pi;
    pi.open("digits.txt");
    pi.read(digits, 100);
    pi.close();

    input = new Input();
    ioctl(0, TIOCGWINSZ, &w);
    sizex = w.ws_col; sizey = w.ws_row; // Tamaño de la pantalla
    if (WIDTH <= sizex) sizex = WIDTH;
    if (HEIGHT <= sizey) sizey = HEIGHT;

    // Inicialización de variables

    Digit* dig;

    // Bienvenida del juego.
    std::cout << "\033[1;1H\033[2JBienvenido a pisnake.";
    std::getchar();
    std::cout << "Para mostrar los controles pulse la tecla \"c\"";
    char sel = std::getchar();
    if (sel == 'c') {
        std::cout << "Controles:\nArriba: w\nAbajo: s\nIzquierda: a\nDerecha d\n";
        std::cin.ignore();
        std::getchar();
    }

    start: // Inicio del juego. Se va a esta etiqueta cuando termina la secuencia de muerte
    points = 0;
    canRender = true;
    bool open = true;
    while (open) { // Bucle principal
        auto startframe = chrono::system_clock::now();
        std::cout.flush();
        // Bucle principal
        input->getUserInput(); // Conseguir la entrada del usuario

        // Secuencia de muerte del jugador.
        if (!player.alive) {
            player.alive = true; // Restablece la vida del jugador
            player.pos.set(sizex / 2, sizey / 2); // Restablece la posición del jugador
            player.tail.clear();
            player.stop();
            std::cout << "\033[107m\033[30m\033[1;2HGame Over\033[0m"; // Muestra Game Over en la parte superior
            std::getchar();

            player.len = 1; // Restablece el tamaño del jugador
            eaten = true;
            delete dig;

            std::cout << "\033[1;1H\033[2JGame Over";
            std::cout << "\nPuntuación: " << points << '\n';

            std::getchar();

            goto start;
        }


        // Retablecimiento de la variable que controla el renderizado de la pantalla
        static bool startFrame = true;
        if (!startFrame) canRender = false;
        startFrame = false;

        // Jugador
        player.update();

        if (canRender) {
            cout << "\033[1;1H";
            cout << "\033[2J"; // Limpia la pantalla
        }

        if (canRender) player.render();

        // Dígitos

        if (dig != nullptr) {
            dig->update();
            if (canRender) {
                std::cout << "\033[35m";
                dig->render();
                std::cout << "\033[0m";
            }
        }

        for (Digit* d : player.tail) {
            d->update();
            if (canRender) d->render();
        }

        // Control del spawn de puntos
        if (eaten) {
            eaten = false;
            // Hace aparecer un nuevo punto en una posición aleatoria
            dig = new Digit(rand() % (sizex - 2) + 2, rand() % (sizey - 2) + 2, digits[player.len]);
        }

        // Dibujado de los bordes
        if (canRender) {
            std::cout << "\033[107m\033[1;1H"; // Superior
            for (unsigned int i = 0; i < WIDTH; i++)
                std::cout << ' ';
            std::cout << "\033[" << HEIGHT << ";1H"; // Inferior
            for (unsigned int i = 0; i < WIDTH; i++)
                std::cout << ' ';
            std::cout << "\033[1;1H";
            for (unsigned int i = 0; i < HEIGHT; i++) // Izquierda
                std::cout << " \033[D\033[B";
            std::cout << "\033[1;" << WIDTH << 'H';
            for (unsigned int i = 0; i < HEIGHT; i++) // Derecha
                std::cout << " \033[D\033[B";
            std::cout << "\033[0m";
        }


        // Renderizado de los puntos
        if (canRender) {
            std::cout << "\033[" << (int)(sizey + 1) << ";1HPuntos: " << points;
        }

        // Pone el cursor al inicio de la pantalla
        std::cout << "\033[1;1H";

        // Control de los fps
        double elapsed = 0; // tiempo entre dos frames, variable
        do {
            auto endframe = chrono::system_clock::now();  
            chrono::duration<double> time = endframe-startframe;  //calcular el tiempo entre frames
            elapsed = time.count(); // Castear el tiempo a double
        } while (elapsed <= (float)(1/(float)MAXFRAMES)); // Limita la frecuencia a la que se actualiza el bucle principal
        delta = elapsed;
        frameRate = 1/delta; // Guarda los fps
    }

    return 0;
}



