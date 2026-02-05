// IRenderer.h
#pragma once
#include <string>
#include "Vectors.h"

struct Color
{
    unsigned char r, g, b, a;
    Color(unsigned char r = 255, unsigned char g = 255, unsigned char b = 255, unsigned char a = 255)
        : r(r), g(g), b(b), a(a) {}
};

namespace Colors
{
    inline const Color kTransparent(0, 0, 0, 0);
    inline const Color kWhite(255, 255, 255, 255);
    inline const Color kBlack(0, 0, 0, 255);
    inline const Color kRed(255, 0, 0, 255);
    inline const Color kGreen(0, 255, 0, 255);
    inline const Color kBlue(0, 0, 255, 255);

    inline const Color kBaseCatp(30, 30, 46, 255);
    inline const Color kWhiteCatp(205, 214, 244, 255);
    inline const Color kRedCatp(243, 139, 168, 255);
    inline const Color kGreenCatp(166, 227, 161, 255);
    inline const Color kYellowCatp(249, 226, 175, 255);
    inline const Color kBlueCatp(137, 180, 250, 255);
    inline const Color kTealCatp(148, 226, 213, 255);
    inline const Color kLavenderCatp(180, 190, 254, 255);
    inline const Color kMauveCatp(203, 166, 247, 255);
    inline const Color kMauveDarkCatp(114, 63, 176, 255);


}

struct Shape
{
    Color fillColor;
    Color outlinecolor;
    Vector2 position;
    Vector2 scale;
    float rotation;
    float outlineThickness;

    Shape() = delete;
    Shape(Color fillColor = Colors::kGreen,
          Color outlinecolor = Colors::kWhite,
          Vector2 position = Vector2::zero(),
          Vector2 scale = Vector2::one(),
          float rotation = 0.f,
          float outlineThickness = 4.f)
        : fillColor(fillColor),
          outlinecolor(outlinecolor),
          position(position),
          scale(scale),
          rotation(rotation),
          outlineThickness(outlineThickness) {}
};

struct Convex : public Shape
{
    std::vector<Vector2> points;

    Convex() = delete;
    Convex(std::vector<Vector2> points,
           Color fillColor = Colors::kBlue,
           Color outlinecolor = Colors::kWhite,
           Vector2 position = Vector2::zero(),
           Vector2 scale = {Vector2::one()},
           float rotation = 0.f,
           float outlineThickness = 2.f)
        : points(points),
          Shape(fillColor, outlinecolor, position, scale, rotation, outlineThickness) {}
};

struct Circle : public Shape
{
    std::vector<Vector2> points;

    Circle() = delete;
    Circle(std::vector<Vector2> points,
           Color fillColor = Colors::kGreen,
           Color outlinecolor = Colors::kWhite,
           Vector2 position = Vector2::zero(),
           Vector2 scale = Vector2::one(),
           float rotation = 0.f,
           float outlineThickness = 4.f)
        : points(points),
          Shape(fillColor, outlinecolor, position, scale, rotation, outlineThickness) {}
};

struct Rectangle : public Shape
{
    std::vector<Vector2> points;

    Rectangle() = delete;
    Rectangle(std::vector<Vector2> points,
              Color fillColor = Colors::kGreen,
              Color outlinecolor = Colors::kWhite,
              Vector2 position = Vector2::zero(),
              Vector2 scale = Vector2::one(),
              float rotation = 0.f,
              float outlineThickness = 4.f)
        : points(points),
          Shape(fillColor, outlinecolor, position, scale, rotation, outlineThickness) {}
};

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    // Управление окном
    virtual void CreateWindow(int width, int height, const std::string &title) = 0;
    virtual bool IsOpen() const = 0;
    virtual void Close() = 0;
    virtual void Clear(const Color &color = Color(0, 0, 0, 255)) = 0;
    virtual void Display() = 0;
    virtual void GetWindowSize(int &width, int &height) const = 0;

    virtual void SetFramerateLimit(unsigned int fps) = 0;

    virtual void DrawConvex(const Convex &Convex) = 0;

    virtual void DrawCircle(const Vector2 &pos,
                            float radius,
                            const Color &color) = 0;
    virtual void DrawRectangle(const Vector2 &pos,
                               const Vector2 &size,
                               const Color &color,
                               float outlineThickness = 0.0f) = 0;
    virtual void DrawConvex(const Vector2 &pos,
                            const Vector2 &scale,
                            const float &rotation,
                            const std::vector<Vector2> &points,
                            const Color &fillColor,
                            const Color &outlinecolor) = 0;

    // Спрайты (например, текстуры)
    virtual void DrawSprite(const std::string &textureId, const Vector2 &pos) = 0;

    // Текст
    virtual void DrawText(const std::string &text,
                          const std::string &fontPath,
                          const Vector2 &pos,
                          unsigned int size,
                          const Color &color,
                          bool center = false) = 0;
};
