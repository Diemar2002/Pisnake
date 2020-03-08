#include "digit.h"
#include "player.h"
#include <stdio.h>

extern float factor;
extern Player player;
extern float delta;
extern int sizex, sizey;
extern int totalSizeY;

Digit::Digit(const float x, const float y, const unsigned char num) {
    this->pos = Vec(x, y);
    this->num = num;
}

void Digit::update() {
    if (!alive) return;
    if (((int)player.pos.x == (int)pos.x) && ((int)(player.pos.y * factor) == (int)(pos.y * factor))) {
        alive = false;
        player.eat(this);
    }
}

void Digit::render() {
    printf("\033[%i;%iH%c", (int)(pos.y * factor), (int)pos.x, num); // Muestra el número en pantalla
}

void Digit::drop() {
    // Físicas
    speed += GRAVITY * delta;

    // if((pos.y + speed) >= sizey - 1) {
    //     dropped = true; 
    // }

    pos.y += speed;
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