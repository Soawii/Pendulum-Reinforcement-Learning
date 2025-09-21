#pragma once
#include "UIElement.hpp"
#include <SFML/Graphics.hpp>
#include <string>

class TextElement : public UIElement {
public:
    TextElement(const sf::String& text, const sf::Font& font, unsigned int charSize = 24, unsigned int thickness = 0);

    sf::Vector2f getSize() override;
    void setString(const sf::String& str);
    void setFillColor(const sf::Color& color);
    void setAutoFit(bool enable);
    void setAutoFill(bool enable);
    void setOrigin(sf::Vector2f origin);
    void draw(WindowHandler* window) override;
private:
    void resizeToFit(const sf::FloatRect& parentBounds);
    void resizeToFill(const sf::FloatRect& parentBounds);

    sf::Text m_text;
    bool m_autoFit = false;
    bool m_autoFill = false;
    sf::Vector2f m_origin = {0.5f, 0.5f};
    sf::Clock clock;
};