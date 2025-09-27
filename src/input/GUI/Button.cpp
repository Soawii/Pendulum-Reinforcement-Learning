#include "Button.hpp"
#include "../../draw/Shapes.hpp"
#include <iostream>

Button::Button()
: UIElement() {}

void Button::draw(WindowHandler* window) {
    UIElement::draw(window);
}

void Button::update(MouseContext& mouseContext, KeyboardContext& keyboardContext) {
    UIElement::update(mouseContext, keyboardContext);

    if (m_context->didMouseMoveIn()) {
        m_context->triggerEvents(m_context->onMouseEnter);
    }
    if (m_context->didMouseMoveOut()) {
        m_context->triggerEvents(m_context->onMouseLeave);
    }
}
