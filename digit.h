#ifndef DIGIT_H
#define DIGIT_H

#include "base.h"
#include <iostream>

class Digit {
public:
    Vec pos = Vec(0.0f, 0.0f);
    unsigned char num;
    bool alive = true;
    Digit(const float x, const float y, const unsigned char num);
    void update();
    void render();
};

#endif