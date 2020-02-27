#include "digit.h"
#include "player.h"

extern float factor;
extern Player player;

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
    std::cout << "\033[" << (int)(pos.y * factor) << ';' << (int)pos.x << 'H'; // Posiciona el cursor en el dígito
    std::cout << num; // Muestra el dígito en la pantalla
}