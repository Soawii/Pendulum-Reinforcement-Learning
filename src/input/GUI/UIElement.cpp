#include "UIElement.hpp"

UIElement::UIElement() : m_vars(m_context) {

}

void UIElement::computeSize() {
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->computeSize();
    }

    if (m_context->sizeMode != UISizeMode::FIT_CONTENT)
        return;

    float borderWidth = m_vars.borderWidth.get();
    float padding = m_vars.padding.get();
    sf::Vector2f newSize = {0.0f, 0.0f};

    if (m_children.size() == 0) {
        newSize = {borderWidth * 2 + padding * 2, borderWidth * 2 + padding * 2};
    }
    else {
        float x_max = 0.0f, y_max = 0.0f;

        for (int i = 0; i < m_children.size(); i++) {
            UIElement* c = m_children[i];

            sf::Vector2f c_size = c->getSize();
            x_max = std::max(x_max, c_size.x);
            y_max = std::max(y_max, c_size.y);
        }
        newSize = {x_max + borderWidth * 2 + padding * 2, y_max + borderWidth * 2 + padding * 2};
    }

    m_vars.absoluteSize.setAllSmoothly(newSize);
}

sf::Vector2f UIElement::getSize() {
    return m_vars.absoluteSize.get();
}

void UIElement::propogateCall(const std::function<void(UIElement*)>& func) {
    func(this);
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->propogateCall(func);
    }
}

void UIElement::draw(WindowHandler* window) {
    sf::FloatRect& anchorRect = (m_parent == m_anchor 
        ? m_anchor->m_context->anchorBounds
        : m_anchor->m_context->mouseBounds);

    sf::Vector2f absolutePos = m_vars.absolutePos.get();
    sf::Vector2f relativePos = m_vars.relativePos.get();
    sf::Vector2f absoluteSize = m_vars.absoluteSize.get();
    sf::Vector2f relativeSize = m_vars.relativeSize.get();

    sf::Vector2f pos = {
        relativePos.x * anchorRect.width + absolutePos.x + anchorRect.left,
        relativePos.y * anchorRect.height + absolutePos.y + anchorRect.top
    };

    sf::Vector2f size = {
        relativeSize.x * anchorRect.width + absoluteSize.x,
        relativeSize.y * anchorRect.height + absoluteSize.y
    };

    ColorHSL color = m_vars.color.get();
    ColorHSL borderColor = m_vars.borderColor.get();

    const sf::Color sfColor = hslToRgb(color);
    const sf::Color sfBorderColor = hslToRgb(borderColor);
    const float borderRadius = m_vars.borderRadius.get();
    const float borderWidth = m_vars.borderWidth.get();
    const float padding = m_vars.padding.get();

    shapes::RoundedOutlinedRect shape(
        pos, size.x, size.y, 
        borderRadius,
        sfColor,
        borderWidth,
        sfBorderColor,
        0.0f
    );

    shape.draw(window->m_window);
    m_context->mouseBounds = {pos.x, pos.y, size.x, size.y};
    m_context->anchorBounds = {
        pos.x + borderWidth + padding, 
        pos.y + borderWidth + padding, 
        size.x - borderWidth * 2 - padding * 2, 
        size.y - borderWidth * 2 - padding * 2};

    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->draw(window);
    }
}

void UIElement::update(MouseContext& mouseContext, KeyboardContext& keyboardContext) {
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->update(mouseContext, keyboardContext);
        if (m_children[i]->m_context->stopPropogation)
            m_context->stopPropogation = true;
    }

    bool isMouseIn = m_context->mouseBounds.contains(sf::Vector2f(mouseContext.m_current.pos));

    if (isMouseIn && mouseContext.isPressedThisFrame(sf::Mouse::Left)) {
        m_context->mousePressedHere = true;
        m_context->stopPropogation = true;
    }
    if (mouseContext.isReleasedThisFrame(sf::Mouse::Left)) {
        if (m_context->mousePressedHere) {
            if (isMouseIn)
                m_context->triggerEvents(m_context->onClick);
            m_context->stopPropogation = true;
        }
        m_context->mousePressedHere = false;
    }

    for (int i = 0; i < m_context->onPressKeys.size(); i++) {
        const sf::Keyboard::Key key = m_context->onPressKeys[i];
        if (keyboardContext.isPressedThisFrame(key)) {
            m_context->keyboardPressedHere = true;
        }
        if (keyboardContext.isReleasedThisFrame(key)) {
            if (m_context->keyboardPressedHere) {
                m_context->triggerEvents(m_context->onClick);
            }
            m_context->keyboardPressedHere = false;
        }
    }

    if (m_context->mousePressedHere || m_context->keyboardPressedHere) {
        m_context->m_current.state = UIElementState::ACTIVE;
    } else if (isMouseIn) {
        m_context->m_current.state = UIElementState::HOVER;
    } else {
        m_context->m_current.state = UIElementState::NORMAL;
    }

    if (m_context->m_current.state == UIElementState::ACTIVE) {
        m_context->triggerEvents(m_context->whileActive);
        m_context->stopPropogation = true;
    }

    if (m_context->wasStateChanged()) {
        m_context->triggerEvents(m_context->onStateChange[size_t(m_context->m_current.state)]);
    }
}