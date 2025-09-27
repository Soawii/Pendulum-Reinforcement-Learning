#pragma once
#include "UIElement.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class TextElement : public UIElement {
public:
    TextElement(const sf::String& text, const sf::Font& font, unsigned int charSize = 24, unsigned int thickness = 0);

    sf::FloatRect getLocalBounds() override;
    void computeSize() override;

    void setString(const sf::String& str);
    void setFillColor(const sf::Color& color);
    void setAutoFit(bool enable);
    void setAutoFill(bool enable);
    void draw(WindowHandler* window) override;
private:
    void resizeToFit(const Bounds& parentBounds);
    void resizeToFill(const Bounds& parentBounds);

    sf::Text m_text;
    bool m_autoFit = false;
    bool m_autoFill = false;
};