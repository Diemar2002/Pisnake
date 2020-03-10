#ifndef DIGIT_H
#define DIGIT_H

#include "base.h"
#include <iostream>

#define GRAVITY 1

class Digit {
public:
    Vec pos = Vec(0.0f, 0.0f);
    unsigned char num;
    bool alive = true;
    bool dropped = false;
    Vec speed = Vec(0.0f, 0.0f);
    Digit(const float x, const float y, const unsigned char num);
    void update();
    void render();
    void drop();
};

#endif
