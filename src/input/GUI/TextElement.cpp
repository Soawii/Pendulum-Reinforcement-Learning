#include "TextElement.hpp"

TextElement::TextElement(const sf::String& text, const sf::Font& font, unsigned int charSize, unsigned int thickness) {
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(charSize);
    m_text.setOutlineThickness(thickness);
}

sf::Vector2f TextElement::getSize() {
    sf::FloatRect textBounds = m_text.getLocalBounds();
    return {textBounds.width, textBounds.height};
}

void TextElement::setString(const sf::String& str) {
    m_text.setString(str);
}
void TextElement::setAutoFit(bool enable) {
    m_autoFit = enable;
}
void TextElement::setAutoFill(bool enable) {
    m_autoFill = enable;
}
void TextElement::setOrigin(sf::Vector2f origin) {
    m_origin = origin;
}

void TextElement::draw(WindowHandler* window) {
    if (clock.getElapsedTime().asSeconds() > 3) {
        int a = 1;
        this;
    }

    sf::FloatRect& anchorRect = m_anchor->m_context->anchorBounds;

    sf::Vector2f absolutePos = m_vars.absolutePos.get();
    sf::Vector2f relativePos = m_vars.relativePos.get();

    sf::Vector2f pos = {
        relativePos.x * anchorRect.width + absolutePos.x + anchorRect.left,
        relativePos.y * anchorRect.height + absolutePos.y + anchorRect.top
    };

    if (m_autoFit || m_autoFill) {
        auto parentBounds = m_anchor->m_context->anchorBounds;
        if (m_autoFit)
            resizeToFit(parentBounds);
        if (m_autoFill)
            resizeToFill(parentBounds);
    }

    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.left + textBounds.width * m_origin.x,
                        textBounds.top + textBounds.height * m_origin.y);
    m_text.setPosition(pos);
    ColorHSL color = m_vars.color.get();
    m_text.setFillColor(hslToRgb(color));
    m_text.setOutlineColor(hslToRgb(color));

    m_context->mouseBounds = m_text.getGlobalBounds();
    m_context->anchorBounds = m_text.getGlobalBounds();
    window->m_window.draw(m_text);
}


void TextElement::resizeToFit(const sf::FloatRect& parentBounds) {
    if (m_text.getString().isEmpty())
        return;

    unsigned int size = m_text.getCharacterSize();

    while (true) {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        if (textBounds.width <= parentBounds.width &&
            textBounds.height <= parentBounds.height) {
            break;
        }
        if (size == 0) break;
        m_text.setCharacterSize(--size);
    }
}

void TextElement::resizeToFill(const sf::FloatRect& parentBounds) {
    if (m_text.getString().isEmpty())
        return;

    unsigned int size = m_text.getCharacterSize();

    while (true) {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        if (textBounds.width > parentBounds.width &&
            textBounds.height > parentBounds.height) {
            break;
        }
        if (size >= 150) break;
        m_text.setCharacterSize(size++);
    }
}