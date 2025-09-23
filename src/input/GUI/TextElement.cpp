#include "TextElement.hpp"

TextElement::TextElement(const sf::String& text, const sf::Font& font, unsigned int charSize, unsigned int thickness) {
    m_text.setFont(font);
    m_text.setString(text);
    m_text.setCharacterSize(charSize);
    m_text.setOutlineThickness(thickness);
}

sf::FloatRect TextElement::getLocalBounds() {
    return m_text.getLocalBounds();
}

void TextElement::computeSize() {
    sf::FloatRect bounds = getGlobalBounds();
    m_context->anchorBounds = bounds;
    m_context->mouseBounds = bounds;
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

void TextElement::draw(WindowHandler* window) {
    if (m_autoFit || m_autoFill) {
        auto parentBounds = m_anchor->m_context->childrenBounds;
        if (m_autoFit)
            resizeToFit(parentBounds);
        if (m_autoFill)
            resizeToFill(parentBounds);
    }

    sf::FloatRect bounds = getGlobalBounds();

    m_text.setPosition({bounds.left, bounds.top});
    ColorHSL color = m_vars.color.get();
    m_text.setFillColor(hslToRgb(color));
    m_text.setOutlineColor(hslToRgb(color));

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