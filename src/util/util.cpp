#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "util.hpp"

namespace util {
    float getSquaredLengthBetweenPoints(b2Vec2 pos1, b2Vec2 pos2) {
        b2Vec2 diff = pos2 - pos1;
        return diff.x * diff.x + diff.y * diff.y;
    }

    float getLen(sf::Vector2f vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }
}