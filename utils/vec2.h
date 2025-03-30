#pragma once

class Vec2 {
public:
    Vec2();
    Vec2(int x, int y);

    int x;
    int y;

    void operator+=(const Vec2& rhs);
};
