#pragma once

class Vec2 {
public:
    Vec2();
    Vec2(int x, int y);

    int x;
    int y;

    void operator+=(const Vec2& rhs);
    bool operator==(const Vec2& rhs);
};

class Vec2d {
public:
    Vec2d();
    Vec2d(float x, float y);

    float x;
    float y;

    void operator+=(const Vec2d& rhs);
    bool operator==(const Vec2d& rhs);
};
