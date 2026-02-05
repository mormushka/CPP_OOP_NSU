// SFMLRenderer.h
#pragma once
#include "IRenderer.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>

class SFMLRenderer : public IRenderer
{
private:
    sf::RenderWindow window;
    std::unordered_map<std::string, sf::Font> fonts;

public:
    void CreateWindow(int width, int height, const std::string &title) override
    {
        window.create(sf::VideoMode(width, height), title);
    }

    bool IsOpen() const override { return window.isOpen(); }
    void Close() override { window.close(); }
    void Clear(const Color &color = Color(0, 0, 0, 255)) override
    {
        window.clear(sf::Color(color.r, color.g, color.b, color.a));
    }
    void Display() override { window.display(); }

    void GetWindowSize(int &width, int &height) const override
    {
        sf::Vector2u size = window.getSize();
        width = size.x;
        height = size.y;
    }

    void SetFramerateLimit(unsigned int fps) override { window.setFramerateLimit(fps); }

    void DrawCircle(const Vector2 &pos, float radius, const Color &color) override
    {
        sf::CircleShape circle(radius);
        circle.setPosition(pos.x, pos.y);
        circle.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
        window.draw(circle);
    }

    void DrawSprite(const std::string &textureId, const Vector2 &pos) override
    {
        static std::unordered_map<std::string, sf::Texture> textures;

        if (textures.find(textureId) == textures.end())
        {
            sf::Texture tex;
            if (tex.loadFromFile(textureId))
            {
                textures[textureId] = std::move(tex);
            }
        }

        sf::Sprite sprite;
        sprite.setTexture(textures[textureId]);
        sprite.setPosition(pos.x, pos.y);
        window.draw(sprite);
    }

    void DrawRectangle(const Vector2 &pos, const Vector2 &size, const Color &color, float outlineThickness) override
    {
        sf::RectangleShape rect(sf::Vector2f(size.x, size.y));
        rect.setPosition(pos.x, pos.y);
        rect.setFillColor(sf::Color(color.r, color.g, color.b, color.a));
        if (outlineThickness > 0.0f)
        {
            rect.setOutlineColor(sf::Color::White);
            rect.setOutlineThickness(outlineThickness);
        }
        window.draw(rect);
    }

    virtual void DrawConvex(const Convex &c) override
    {
        sf::ConvexShape convex;
        convex.setPointCount(c.points.size());
        for (size_t i = 0; i < c.points.size(); ++i)
        {
            convex.setPoint(i, sf::Vector2f(c.points[i].x, c.points[i].y));
        }
        convex.setFillColor(sf::Color(c.fillColor.r, c.fillColor.g, c.fillColor.b, c.fillColor.a));
        convex.setOutlineColor(sf::Color(c.outlinecolor.r, c.outlinecolor.g, c.outlinecolor.b, c.outlinecolor.a));
        convex.setOutlineThickness(c.outlineThickness);
        convex.setPosition(c.position.x, c.position.y);
        convex.setScale(c.scale.x, c.scale.y);
        convex.setRotation(c.rotation);
        window.draw(convex);
    }

    // const std::vector<sf::Vector2f> &points, const sf::Color &col = sf::Color::White
    void DrawConvex(const Vector2 &pos,
                    const Vector2 &scale,
                    const float &rotation,
                    const std::vector<Vector2> &points,
                    const Color &fillColor,
                    const Color &outlinecolor) override
    {
        sf::ConvexShape convex;
        convex.setPointCount(points.size());
        for (size_t i = 0; i < points.size(); ++i)
        {
            convex.setPoint(i, sf::Vector2f(points[i].x, points[i].y));
        }
        convex.setFillColor(sf::Color(fillColor.r, fillColor.g, fillColor.b, fillColor.a));
        convex.setOutlineColor(sf::Color(outlinecolor.r, outlinecolor.g, outlinecolor.b, outlinecolor.a));
        convex.setOutlineThickness(1.5f);
        convex.setPosition(pos.x, pos.y);
        convex.setScale(scale.x, scale.y);
        convex.setRotation(rotation);
        window.draw(convex);
    }

    void DrawText(const std::string &text,
                  const std::string &fontPath,
                  const Vector2 &pos,
                  unsigned int size,
                  const Color &color,
                  bool center = false) override
    {
        if (fonts.find(fontPath) == fonts.end())
        {
            sf::Font font;
            if (font.loadFromFile(fontPath))
            {
                fonts[fontPath] = std::move(font);
            }
        }

        sf::Text sfText;
        sfText.setFont(fonts[fontPath]);
        sfText.setString(text);
        sfText.setCharacterSize(size);
        sfText.setFillColor(sf::Color(color.r, color.g, color.b, color.a));

        if (center)
        {
            sf::FloatRect bounds = sfText.getLocalBounds();
            sfText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        }

        sfText.setPosition(pos.x, pos.y);
        window.draw(sfText);
    }

    sf::RenderWindow &GetWindow() { return window; }
};
