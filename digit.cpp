#include "digit.h"
#include "player.h"
#include <stdio.h>

extern float factor;
extern Player player;
extern float delta;
extern int sizex, sizey;
extern int totalSizeY, totalSizeX;

Digit::Digit(const float x, const float y, const unsigned char num) {
    this->pos = Vec(x, y);
    this->num = num;
    this->speed.set(((rand() % 1000) / 100.0f) - 5.0f, ((rand() % 1000) / 100.0f) - 5.0f);
}

void Digit::update() {
    if (!alive) return;
    if (((int)player.pos.x == (int)pos.x) && ((int)(player.pos.y * factor) == (int)(pos.y * factor))) {
        alive = false;
        player.eat(this);
    }
}

void Digit::render() {
    if ((int)pos.x >= 0 && (int)pos.x < totalSizeX && (int)pos.y >= 0 && (int)pos.y < totalSizeY)
        printf("\033[%i;%iH%c", (int)(pos.y * factor), (int)pos.x, num); // Muestra el número en pantalla
}

void Digit::drop() {
    // Físicas
    speed.y += GRAVITY * delta;

    // if((pos.y + speed) >= sizey - 1) {
    //     dropped = true; 
    // }

    pos.set(pos.x + speed.x * delta, pos.y + speed.y * delta);
    if (pos.y > totalSizeY + 1) dropped = true;

    int playSize = player.len - 1;
    /*for (int i = 0; i < playSize; i++) {
        if (player.tail[i] == this) continue;
        if ((int)(player.tail[i]->pos.y <= (int)(pos.y+1)) && ((int)player.tail[i]->pos.x == (int)pos.x)) {
            if (player.tail[i]->dropped) {
                dropped = true;
                return;            
            }
        }
    }*/
    
}
