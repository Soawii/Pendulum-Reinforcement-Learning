#include "WindowElement.hpp"
#include <iostream>

WindowElement::WindowElement(WindowHandler* window) : UIElement(), m_window(window) {}

sf::FloatRect WindowElement::getGlobalBounds() {
    sf::Vector2u windowSize = m_window->getSize();
    return sf::FloatRect(0.0f, 0.0f, windowSize.x, windowSize.y);
}

void WindowElement::computeSize() {
    sf::Vector2u windowSize = m_window->getSize();
    m_context->mouseBounds = Bounds({0.0f, 0.0f}, windowSize.x, windowSize.y);
    m_context->anchorBounds = Bounds({0.0f, 0.0f}, windowSize.x, windowSize.y);
    m_context->childrenBounds = Bounds({0.0f, 0.0f}, windowSize.x, windowSize.y);

    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->computeSize();
    }
}

void WindowElement::draw(WindowHandler* window) {
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->draw(window);
    }
}

void WindowElement::update(MouseContext& mouseContext, KeyboardContext& keyboardContext) {
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->update(mouseContext, keyboardContext);
        if (m_children[i]->m_context->stopPropogation)
            m_context->stopPropogation = true;
    }
}
