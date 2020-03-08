#include "player.h"
#include <stdio.h>

using namespace std;

extern Input* input;
extern float delta;
extern float factor;
extern bool eaten;
extern int sizex;
extern int sizey;
extern bool canRender;
extern unsigned int points;

Player::Player(float posx, float posy) {
    pos.set(posx, posy);
}

void Player::update() {
    if (!alive) return;
    // Movimiento de la cola
    static bool changed = false;
    static int prevposX = (int)pos.x;
    static int prevposY = (int)pos.y;

    static Vec prevPos(pos.x, pos.y);

    int firstPosX = -1, firstPosY = -1;
    if (len > 1) {
        firstPosX = tail[0]->pos.x;
        firstPosY = tail[0]->pos.y;
    }

    if (input->keys['w'] && (firstPosY != ((int)pos.y - 1))) {
        speed.y = -SPEED;
        speed.x = 0;
    }
    if (input->keys['a'] && (firstPosX != ((int)pos.x - 1))) {
        speed.x = -SPEED;
        speed.y = 0;
    }
    if (input->keys['s'] && (firstPosY != ((int)pos.y + 1))) {
        speed.y = SPEED;
        speed.x = 0;
    }
    if (input->keys['d'] && (firstPosX != ((int)pos.x + 1))) {
        speed.x = SPEED;
        speed.y = 0;
    }

    pos.x += speed.x*delta;
    pos.y += speed.y*delta;

    unsigned int tailSize = tail.size();
    if ((((int)prevPos.x != (int)pos.x) || ((int)(prevPos.y * factor) != (int)(pos.y * factor)))) {
        canRender = true;
        if (tailSize != 0) {
            for (int i = tailSize - 1; i > 0; i--) {
                tail[i]->pos.x = tail[i - 1]->pos.x;
                tail[i]->pos.y = tail[i - 1]->pos.y;
            }
            tail[0]->pos.x = prevPos.x;
            tail[0]->pos.y = prevPos.y;
            prevPos.set(pos.x, pos.y);
        }
    }

    // Comprobar si ha muerto
    if (pos.x <= 1 || (int)pos.x > (sizex - 1) || (int)(pos.y * factor) <= 1 || (int)(pos.y * factor) > (sizey - 1))
        alive = false;
    for (int i = 0; i < tailSize; i++) {
        if (((int)tail[i]->pos.x == (int)pos.x) && ((int)tail[i]->pos.y == (int)pos.y) && !tail[i]->alive)
            alive = false;
    }
}

void Player::render() {
    printf("\033[33m\033[%i;%iH3\033[0m", (int)(pos.y * factor), (int)pos.x);
}

void Player::eat(Digit* d) {
    tail.push_back(d);
    d->pos.set(1, 1);
    eaten = true;
    points += 100;
    len++;
}

void Player::stop() {
    speed.set(0, 0);
}