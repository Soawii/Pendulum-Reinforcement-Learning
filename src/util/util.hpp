#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <functional>

namespace util {
    float getSquaredLengthBetweenPoints(b2Vec2 pos1, b2Vec2 pos2);
    float getLen(sf::Vector2f vec);
    float timeFunction(const std::function<void(void)>& func);
}