#include "vec2.h"

Vec2::Vec2() {
    this->x = 0;
    this->y = 0;
}

Vec2::Vec2(int x, int y) {
    this->x = x * 8;
    this->y = y * 8;
}
