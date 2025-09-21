#include "TimingVariable.hpp"
#include <SFML/Graphics.hpp>

sf::Color operator*(const sf::Color& left, float right) {
    return sf::Color(
        sf::Uint8(left.r * right), sf::Uint8(left.g * right),
        sf::Uint8(left.b * right), sf::Uint8(left.a * right));
}

namespace TimingFunctions {
    float linear(float t) {
        return t;
    }
    float smoothstep(float t) {
        return t * t * (3.0f - 2.0f * t);
    }
}