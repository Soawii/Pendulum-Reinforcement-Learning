#include "TextElement.hpp"

TextElement::TextElement(const sf::String& text, const sf::Font& font, unsigned int charSize, unsigned int thickness)
{
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
    m_context->anchorBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
    m_context->mouseBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
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

    const Bounds& bounds = m_context->mouseBounds;
    m_text.setPosition(bounds.m_pos);

    ColorHSL color = m_vars.color.get();
    sf::Color rgbColor = hslToRgb(color);
    rgbColor.a *= m_context->calculatedOpacity;

    m_text.setFillColor(rgbColor);
    m_text.setOutlineColor(rgbColor);

    window->m_window.draw(m_text, m_context->calculatedTransform);
}


void TextElement::resizeToFit(const Bounds& parentBounds) {
    if (m_text.getString().isEmpty())
        return;

    unsigned int size = m_text.getCharacterSize();

    while (true) {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        if (textBounds.width <= parentBounds.m_width &&
            textBounds.height <= parentBounds.m_height) {
            break;
        }
        if (size == 0) break;
        m_text.setCharacterSize(--size);
    }
}

void TextElement::resizeToFill(const Bounds& parentBounds) {
    if (m_text.getString().isEmpty())
        return;

    unsigned int size = m_text.getCharacterSize();

    while (true) {
        sf::FloatRect textBounds = m_text.getLocalBounds();
        if (textBounds.width > parentBounds.m_width &&
            textBounds.height > parentBounds.m_height) {
            break;
        }
        if (size >= 150) break;
        m_text.setCharacterSize(size++);
    }
}