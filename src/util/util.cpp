#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "util.hpp"
#include <chrono>

namespace util {
    float getSquaredLengthBetweenPoints(b2Vec2 pos1, b2Vec2 pos2) {
        b2Vec2 diff = pos2 - pos1;
        return diff.x * diff.x + diff.y * diff.y;
    }

    float getLen(sf::Vector2f vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    float timeFunction(const std::function<void(void)>& func) {
        auto start = std::chrono::steady_clock::now();
        func();
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration<float, std::micro>(end - start).count();
    }
}