#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "conf.hpp"

namespace util {
    sf::Vector2i worldToScreen(b2Vec2 worldPos);
    b2Vec2 screenToWorld(sf::Vector2i screenPos);
    float getLen(sf::Vector2f vec);
    sf::VertexArray getRect(sf::Vector2f start, sf::Vector2f end, float width, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getLine(sf::Vector2f start, sf::Vector2f end, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getHollowCircle(sf::Vector2f center, float outerRadius, float strokeWidth, int points = 50, sf::Color color = sf::Color(255,255,255));
    sf::VertexArray getRoundedRectPoints(sf::Vector2f topLeft, float width, float height, float radius, int points_quarterCircle = 15, sf::Color color = sf::Color(255,255,255));
    std::pair<sf::VertexArray, sf::VertexArray> getRoundedRect(sf::Vector2f topLeft, float width, float height, float radius, sf::Color color = sf::Color(255,255,255), float outlineWidth = 0.0f, sf::Color outlineColor = sf::Color(230,230,230), float outlinePadding = 0.0f);
}