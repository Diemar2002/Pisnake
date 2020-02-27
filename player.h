#ifndef PLAYER_H
#define PLAYER_H

#include "base.h"
#include "digit.h"
#include <vector>

#define SPEED 5

class Player {
    Vec speed = Vec(0, 0);

    public:
        bool alive = true;
    int len = 1;
    std::vector<Digit*> tail = {};
    Vec pos = Vec(0, 0);
    Player(float posx, float posy);
    void update();
    void render();
    void eat(Digit* d);
    void stop();
};

#endif
