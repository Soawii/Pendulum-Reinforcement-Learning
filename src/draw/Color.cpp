#include "Color.hpp"
#include <iostream>

ColorHSL::ColorHSL(int r, int g, int b, int a) {
    ColorHSL color = rgbToHsl(sf::Color(r, g, b, a));
    this->h = color.h;
    this->s = color.s;
    this->l = color.l;
    this->a = color.a;
}
ColorHSL::ColorHSL(float new_h, float new_s, float new_l, float new_a) {
    this->h = new_h;
    this->s = new_s;
    this->l = new_l;
    this->a = new_a;
}

ColorHSL ColorHSL::operator+(const ColorHSL& other) const {
    return ColorHSL(
        std::fmod(h + other.h + 360.f, 360.f),
        s + other.s,
        l + other.l,
        a + other.a
    );
}

ColorHSL ColorHSL::operator-(const ColorHSL& other) const {
    float hueDiff = std::fmod((h - other.h + 540.f), 360.f) - 180.f;
    return ColorHSL(
        hueDiff,
        s - other.s,
        l - other.l,
        a - other.a
    );
}

ColorHSL ColorHSL::operator*(float scalar) const {
    return ColorHSL(h * scalar, s * scalar, l * scalar, a * scalar);
}

bool ColorHSL::operator!=(const ColorHSL& other) const {
    return h != other.h || s != other.s || l != other.l || a != other.a;
}
bool ColorHSL::operator==(const ColorHSL& other) const {
    return h == other.h && s == other.s && l == other.l && a == other.a;
}

ColorHSL rgbToHsl(const sf::Color& c) {
    float r = int(c.r) / 255.f, g = int(c.g) / 255.f, b = int(c.b) / 255.f;
    float max = std::max(r, std::max(g, b));
    float min = std::min(r, std::min(g, b));
    float h = 0.f, s = 0.f;
    float l = (max + min) / 2.f;

    if (max != min) {
        float d = max - min;
        s = l > 0.5f ? d / (2.f - max - min) : d / (max + min);

        if (max == r)
            h = fmod(((g - b) / d + (g < b ? 6.f : 0.f)), 6.f);
        else if (max == g)
            h = (b - r) / d + 2.f;
        else
            h = (r - g) / d + 4.f;

        h *= 60.f;
    }

    return ColorHSL(h, s, l, int(c.a) / 255.0f);
}

sf::Color hslToRgb(const ColorHSL& hsl) {
    float c = (1.f - std::fabs(2.f * hsl.l - 1.f)) * hsl.s;
    float x = c * (1.f - std::fabs(fmod(hsl.h / 60.f, 2.f) - 1.f));
    float m = hsl.l - c / 2.f;

    float r, g, b;
    if (hsl.h < 60)      { r = c; g = x; b = 0; }
    else if (hsl.h <120) { r = x; g = c; b = 0; }
    else if (hsl.h <180) { r = 0; g = c; b = x; }
    else if (hsl.h <240) { r = 0; g = x; b = c; }
    else if (hsl.h <300) { r = x; g = 0; b = c; }
    else                 { r = c; g = 0; b = x; }

    return sf::Color(
        static_cast<sf::Uint8>((r + m) * 255),
        static_cast<sf::Uint8>((g + m) * 255),
        static_cast<sf::Uint8>((b + m) * 255),
        static_cast<sf::Uint8>(hsl.a * 255)
    );
}