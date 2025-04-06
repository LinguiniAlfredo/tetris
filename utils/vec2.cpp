#include "vec2.h"

Vec2::Vec2() {
    this->x = 0;
    this->y = 0;
}

Vec2::Vec2(int x, int y) {
    this->x = x * 8;
    this->y = y * 8;
}

void Vec2::operator+=(const Vec2& rhs) {
    int tmpx = this->x + rhs.x;
    int tmpy = this->y + rhs.y;

    this->x = tmpx;
    this->y = tmpy;
}

bool Vec2::operator==(const Vec2& rhs) {
    return this->x == rhs.x && this->y == rhs.y;
}


Vec2d::Vec2d() {
    this->x = 0;
    this->y = 0;
}

Vec2d::Vec2d(float x, float y) {
    this->x = x * 8;
    this->y = y * 8;
}

void Vec2d::operator+=(const Vec2d& rhs) {
    int tmpx = this->x + rhs.x;
    int tmpy = this->y + rhs.y;

    this->x = tmpx;
    this->y = tmpy;
}

bool Vec2d::operator==(const Vec2d& rhs) {
    return this->x == rhs.x && this->y == rhs.y;
}