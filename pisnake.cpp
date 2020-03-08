#include <iostream>
#include <stdio.h>
#include <sys/ioctl.h>
#include <chrono>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>

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
int totalSizeX, totalSizeY;
float frameRate = 0;
float delta = 0; // Tiempo entre 2 frames
char digits[100] = {};  // Array donde se guardan los 100 primeros dígitos de pi
float factor = 1;
bool canRender = true;
unsigned int points = 0;

bool* buffCells;
vector<int> freeCells;

bool eaten = true;

#include "player.h"
Player player(10, 10);


#define MAXFRAMES 30.0f

#define WIDTH 20
#define HEIGHT 20

int insertionSort(Digit* arr[], int n);

int main() {
    // Setup
    fstream pi;
    pi.open("digits.txt");
    pi.read(digits, 100);
    pi.close();

    input = new Input();
    ioctl(0, TIOCGWINSZ, &w);
    sizex = w.ws_col; sizey = w.ws_row; // Tamaño de la pantalla
    totalSizeX = sizex;
    totalSizeY = sizey;
    if (WIDTH <= sizex) sizex = WIDTH;
    if (HEIGHT <= sizey) sizey = HEIGHT;

    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Inicialización de variables

    Digit* dig;
    buffCells = new bool[(sizex-2) * (sizey-2)];

    // Bienvenida del juego.
    printf("\033[1;1H\033[2JBienvenido a pisnake.");
    std::getchar();
    printf("Para mostrar los controles pulse la tecla \"c\"");
    char sel = std::getchar();
    if (sel == 'c') {
        printf("Controles:\nArriba: w\nAbajo: s\nIzquierda: a\nDerecha d\n");
        std::cin.ignore();
        std::getchar();
    }

    start: // Inicio del juego. Se va a esta etiqueta cuando termina la secuencia de muerte
    points = 0;
    canRender = true;
    bool open = true;
    while (open) { // Bucle principal
        auto startframe = chrono::system_clock::now();
        // Bucle principal
        input->getUserInput(); // Conseguir la entrada del usuario

        // Secuencia de muerte del jugador.
        if (!player.alive) {
            static bool first = true;

            if (first) {
                insertionSort(&player.tail[0], player.len - 1);
            }

            // Animación
            printf("\033[2J");
            static bool canps = true;
            bool all = true;
            for (int i = player.tail.size() - 1; i >= 0; i--) {
                if (!player.tail[i]->dropped) player.tail[i]->drop();
                if (!player.tail[i]->dropped) all = false;
                player.tail[i]->render();
            }
            canps = !all;
            printf("\033[%i;%iH\033[93m3\033[0m", (int)player.pos.y, (int)player.pos.x);
            first = false;
            if (canps)
                goto mainLoopEnd;

            canps = true;
            first = true;

            fflush(stdin);
            player.alive = true; // Restablece la vida del jugador
            player.pos.set(sizex / 2, sizey / 2); // Restablece la posición del jugador
            player.tail.clear();
            player.stop();
            printf("\033[2J\033[1;1H");

            player.len = 1; // Restablece el tamaño del jugador
            eaten = true;
            delete dig;

            printf("\033[1;1H\033[2JGame Over");
            printf("\nPuntuación: %i\n", points);

            std::getchar();

            goto start;
        }


        // Retablecimiento de la variable que controla el renderizado de la pantalla
        static bool startFrame = true;
        if (!startFrame) canRender = false;
        startFrame = false;

        // Jugador
        player.update();

        // Actualización del array que contiene las posiciones disponibles
        if (canRender) {
            unsigned int total = (sizex-2) * (sizey-2);
            freeCells.clear();
            memset(buffCells, false, total);

            buffCells[(int)player.pos.x - 2 + (((int)player.pos.y - 2) * (sizex - 2))] = true;
            for (int i = 0; i < player.len - 1; i++) {
                buffCells[(int)player.tail[i]->pos.x - 2 + ((int)player.tail[i]->pos.y - 2) * (sizex - 2)] = true;
            }

            for (int i = 0; i < total; i++) {
                if (!buffCells[i]) {
                    //Guardamos la posición en el array unidimensional de posición. Para recuperar x e y: x = i%sizex, y = (i-i%sizex) / sizex
                    freeCells.push_back(i);
                }
            }
        }
        
        if (canRender) {
            printf("\033[1;1H\033[2J");
        }

        if (canRender) player.render();


        // Control del spawn de puntos
        if (eaten) {
            eaten = false;
            // Hace aparecer un nuevo punto en una posición aleatoria
            int index = freeCells[rand() % freeCells.size()];
            dig = new Digit((index % (sizex - 2)) + 2, (int)(index/(sizex - 2)) + 2, digits[player.len]);
            
        }

        // Dígitos

        if (dig != nullptr) {
            dig->update();
            if (canRender) {
                printf("\033[92m");
                dig->render();
                printf("\033[0m");
            }
        }

        for (Digit* d : player.tail) {
            d->update();
            if (canRender) d->render();
        }

        

        // Dibujado de los bordes
        if (canRender) {
            printf("\033[107m\033[1;1H");
            for (unsigned int i = 0; i < WIDTH; i++) // Arriba
                printf(" ");
            printf("\033[%i;1H", HEIGHT);
            for (unsigned int i = 0; i < WIDTH; i++) // Abajo
                printf(" ");
            printf("\033[1;1H");
            for (unsigned int i = 0; i < HEIGHT; i++) // Izquierda
                printf(" \033[D\033[B");
            printf("\033[1;%iH", WIDTH);
            for (unsigned int i = 0; i < HEIGHT; i++) // Derecha
                printf(" \033[D\033[B");
            printf("\033[0m"); 
        }


        // Renderizado de los puntos
        if (canRender) {
            printf("\033[%i;1HPuntos: %i", (int)(sizey + 1), points);
        }

        mainLoopEnd: 
        // Pone el cursor al inicio de la pantalla
        printf("\033[%i;1H", HEIGHT + 1);
        fflush(stdout);

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


int insertionSort(Digit* arr[], int n) // Para nada copiado de geeksforgeeks porque nos queríamos morir en este momento. Joder que eran las 1:37 de la mañana. No nos juzgues.
{
    int i, j;
    Digit* key;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j]->pos.y > key->pos.y)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
    return 0;
}
