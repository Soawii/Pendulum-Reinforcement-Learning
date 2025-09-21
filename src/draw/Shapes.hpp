#pragma once
#include <SFML/Graphics.hpp>

namespace shapes {
    class Shape {
        virtual void draw(sf::RenderWindow& window) = 0;
    };

    class Line : public Shape {
    public:
        Line(sf::Vector2f start, sf::Vector2f end, sf::Color color = sf::Color(255,255,255));
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_arr;
    };

    class Circle : public Shape {
    public:
        Circle(sf::Vector2f center, float radius, int points = 50, sf::Color color = sf::Color(255,255,255));
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_arr;
    };

    class HollowCircle : public Shape {
    public:
        HollowCircle(sf::Vector2f center, float outerRadius, float strokeWidth, int points = 50, sf::Color color = sf::Color(255,255,255));
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_innerCircle, m_outerCircle;
        sf::VertexArray m_arr;
    };

    class Rect : public Shape {
    public:
        Rect(sf::Vector2f topLeft, float width, float height, sf::Color color = sf::Color(255,255,255));
        Rect(sf::Vector2f start, sf::Vector2f end, float width, sf::Color color = sf::Color(255,255,255));
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_arr;
    };

    class RoundedRect : public Shape {
    public:
        RoundedRect(sf::Vector2f topLeft, float width, float height, float radius, int points_quarterCircle = 15, sf::Color color = sf::Color(255,255,255));
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_arr;
    };

    class RoundedOutlinedRect : public Shape {
    public:
        RoundedOutlinedRect(sf::Vector2f topLeft, float width, float height, float radius, sf::Color color = sf::Color(255,255,255), float outlineWidth = 0.0f, sf::Color outlineColor = sf::Color(230,230,230), float outlinePadding = 0.0f);
        void draw(sf::RenderWindow& window) override;
        sf::VertexArray m_innerOutline, m_outerOutline, m_innerRect;
        sf::VertexArray m_outline;
    };
}