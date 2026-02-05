// Vectors.h
#pragma once
#include <cmath>

struct Vector2
{
    float x, y;

    Vector2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    static Vector2 zero() { return Vector2(0.0f, 0.0f); }
    static Vector2 one() { return Vector2(1.0f, 1.0f); }
    static Vector2 up() { return Vector2(0.0f, 1.0f); }
    static Vector2 down() { return Vector2(0.0f, -1.0f); }
    static Vector2 right() { return Vector2(1.0f, 0.0f); }
    static Vector2 left() { return Vector2(-1.0f, 0.0f); }

    Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }
    Vector2 operator-(const Vector2 &other) const { return Vector2(x - other.x, y - other.y); }
    Vector2 operator+(float scalar) const { return Vector2(x + scalar, y + scalar); }
    Vector2 operator-(float scalar) const { return Vector2(x - scalar, y - scalar); }
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

    Vector2 &operator+=(const Vector2 &other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }
    Vector2 &operator-=(const Vector2 &other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    Vector2 &operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Vector2 &operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool operator==(const Vector2 &other) const { return x == other.x && y == other.y; }
    bool operator!=(const Vector2 &other) const { return !(*this == other); }

    float Length() const { return std::sqrt(x * x + y * y); }
    float LengthSquared() const { return x * x + y * y; }
    Vector2 Normalized() const
    {
        float len = Length();
        return (len != 0.0f) ? Vector2(x / len, y / len) : zero();
    }
    float Dot(const Vector2 &other) const { return x * other.x + y * other.y; }
    float Distance(const Vector2 &other) const { return (*this - other).Length(); }
};