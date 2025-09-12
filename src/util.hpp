#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "conf.hpp"

namespace util {
    sf::Vector2i worldPosToScreen(b2Vec2 worldPos);
    b2Vec2 screenPosToWorld(sf::Vector2i screenPos);
    b2Vec2 screenPosToWorld(sf::Vector2f screenPos);
    int worldSizeToScreen(float worldSize);
    float screenSizeToWorld(float screenSize);
    sf::Vector2i worldSizeToScreen(b2Vec2 worldSize);
    b2Vec2 screenSizeToWorld(sf::Vector2i screenSize);
    b2Vec2 screenSizeToWorld(sf::Vector2f screenSize);

    float getLen(sf::Vector2f vec);
    sf::VertexArray getRect(sf::Vector2f topLeft, float width, float height, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getRectFromTo(sf::Vector2f start, sf::Vector2f end, float width, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getLine(sf::Vector2f start, sf::Vector2f end, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getHollowCircle(sf::Vector2f center, float outerRadius, float strokeWidth, int points = 50, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getRoundedRectPoints(sf::Vector2f topLeft, float width, float height, float radius, int points_quarterCircle = 15, sf::Color color = sf::Color(255,255,255));
    std::pair<sf::VertexArray, sf::VertexArray> getRoundedRect(sf::Vector2f topLeft, float width, float height, float radius, sf::Color color = sf::Color(255,255,255), float outlineWidth = 0.0f, sf::Color outlineColor = sf::Color(230,230,230), float outlinePadding = 0.0f);
}