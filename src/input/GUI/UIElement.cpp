#include "UIElement.hpp"

UIElement::UIElement() : m_vars(m_context) {

}

sf::FloatRect UIElement::getLocalBounds() {
    sf::FloatRect& anchorRect = (m_parent == m_anchor 
        ? m_anchor->m_context->childrenBounds
        : m_anchor->m_context->anchorBounds);

    sf::Vector2f absoluteSize = m_vars.size.absolute.get();
    sf::Vector2f relativeSize = m_vars.size.relative.get();
    sf::Vector2f size = {
        relativeSize.x * anchorRect.width + absoluteSize.x,
        relativeSize.y * anchorRect.height + absoluteSize.y
    };

    return sf::FloatRect(0.0f, 0.0f, size.x, size.y);
}

sf::FloatRect UIElement::getGlobalBounds() {
    sf::FloatRect& anchorRect = (m_parent == m_anchor 
        ? m_anchor->m_context->childrenBounds
        : m_anchor->m_context->anchorBounds);

    sf::FloatRect localBounds = getLocalBounds();
    sf::Vector2f absolutePos = m_vars.pos.absolute.get();
    sf::Vector2f relativePos = m_vars.pos.relative.get();
    sf::Vector2f absoluteOrigin = m_vars.origin.absolute.get();
    sf::Vector2f relativeOrigin = m_vars.origin.relative.get();

    sf::Vector2f pos = {
        relativePos.x * anchorRect.width 
        + absolutePos.x + anchorRect.left
        - absoluteOrigin.x - relativeOrigin.x * localBounds.width
        - localBounds.left,

        relativePos.y * anchorRect.height 
        + absolutePos.y + anchorRect.top
        - absoluteOrigin.y - relativeOrigin.y * localBounds.height
        - localBounds.top
    };

    return sf::FloatRect(pos.x, pos.y, localBounds.width, localBounds.height);
}

sf::FloatRect UIElement::applyTransform(const sf::FloatRect& globalBounds) {
    sf::Vector2f absoluteTranslate = m_vars.translate.absolute.get();
    sf::Vector2f relativeTranslate = m_vars.translate.relative.get();

    return sf::FloatRect(
        globalBounds.left + absoluteTranslate.x + relativeTranslate.x * globalBounds.width,
        globalBounds.top + absoluteTranslate.y + relativeTranslate.y * globalBounds.height,
        globalBounds.width, globalBounds.height);
}

void UIElement::computeSize() {
    float borderWidth = m_vars.borderWidth.get();
    float padding = m_vars.padding.get();

    if (m_context->sizeMode == UISizeMode::FIXED) {
        sf::FloatRect bounds = getGlobalBounds();
        sf::FloatRect transformed = applyTransform(bounds);

        m_context->mouseBounds = {transformed.left, transformed.top, transformed.width, transformed.height};
        m_context->childrenBounds = {
            transformed.left + borderWidth + padding, 
            transformed.top + borderWidth + padding, 
            transformed.width - borderWidth * 2 - padding * 2, 
            transformed.height - borderWidth * 2 - padding * 2};
        m_context->anchorBounds = {
            bounds.left, 
            bounds.top, 
            bounds.width, 
            bounds.height};
    }

    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->computeSize();
    }

    if (m_context->sizeMode == UISizeMode::FIT_CONTENT) {
        sf::Vector2f newSize = {0.0f, 0.0f};

        if (m_children.size() == 0) {
            newSize = {borderWidth * 2 + padding * 2, borderWidth * 2 + padding * 2};
        }
        else {
            float x_min = 10000000.0f, y_min = 10000000.0f;
            float x_max = -10000000.0f, y_max = -10000000.0f;

            for (int i = 0; i < m_children.size(); i++) {
                UIElement* c = m_children[i];

                sf::FloatRect bounds = c->getGlobalBounds();
                x_min = std::min(x_min, bounds.left);
                y_min = std::min(y_min, bounds.top);
                x_max = std::max(x_max, bounds.left + bounds.width);
                y_max = std::max(y_max, bounds.top + bounds.height);
            }

            newSize = {(x_max - x_min) + borderWidth * 2 + padding * 2, (y_max - y_min) + borderWidth * 2 + padding * 2};
        }
        m_vars.size.absolute.setAllSmoothly(newSize);

        sf::FloatRect bounds = getGlobalBounds();
        sf::FloatRect transformed = applyTransform(bounds);
        m_context->mouseBounds = {transformed.left, transformed.top, transformed.width, transformed.height};
        m_context->childrenBounds = {
            transformed.left + borderWidth + padding, 
            transformed.top + borderWidth + padding, 
            transformed.width - borderWidth * 2 - padding * 2, 
            transformed.height - borderWidth * 2 - padding * 2};
        m_context->anchorBounds = {
            bounds.left, 
            bounds.top, 
            bounds.width, 
            bounds.height};
    }
}

void UIElement::propogateCall(const std::function<void(UIElement*)>& func) {
    func(this);
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->propogateCall(func);
    }
}

void UIElement::draw(WindowHandler* window) {
    ColorHSL color = m_vars.color.get();
    ColorHSL borderColor = m_vars.borderColor.get();

    const sf::Color sfColor = hslToRgb(color);
    const sf::Color sfBorderColor = hslToRgb(borderColor);
    const float borderRadius = m_vars.borderRadius.get();
    const float borderWidth = m_vars.borderWidth.get();
    const float padding = m_vars.padding.get();

    sf::FloatRect& bounds = m_context->mouseBounds;

    shapes::RoundedOutlinedRect shape(
        {bounds.left, bounds.top}, bounds.width, bounds.height, 
        borderRadius, sfColor, borderWidth, sfBorderColor, 0.0f
    );

    shape.draw(window->m_window);

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