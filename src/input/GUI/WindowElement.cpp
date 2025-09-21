#include "WindowElement.hpp"
#include <iostream>

WindowElement::WindowElement(WindowHandler* window) : UIElement(), m_window(window) {}

void WindowElement::draw(WindowHandler* window) {
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->draw(window);
    }
}

void WindowElement::update(MouseContext& mouseContext, KeyboardContext& keyboardContext) {
    sf::Vector2u windowSize = m_window->getSize();
    m_context->mouseBounds = sf::FloatRect(0.0f, 0.0f, windowSize.x, windowSize.y);
    m_context->anchorBounds = sf::FloatRect(0.0f, 0.0f, windowSize.x, windowSize.y);

    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->update(mouseContext, keyboardContext);
        if (m_children[i]->m_context->stopPropogation)
            m_context->stopPropogation = true;
    }
}
