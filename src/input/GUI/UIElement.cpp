#include "UIElement.hpp"

UIElement::UIElement() : m_vars(m_context) {
    m_context->opacityShader.loadFromMemory(R"(
        uniform float opacity;
        void main()
        {
            gl_FragColor = gl_Color * vec4(1.0, 1.0, 1.0, opacity);
        })", sf::Shader::Fragment);
}

sf::FloatRect UIElement::getLocalBounds() {
    const Bounds& anchorRect = (m_parent == m_anchor 
        ? m_anchor->m_context->childrenBounds
        : m_anchor->m_context->anchorBounds);

    sf::Vector2f absoluteSize = m_vars.size.absolute.get();
    sf::Vector2f relativeSize = m_vars.size.relative.get();
    sf::Vector2f size = {
        relativeSize.x * anchorRect.m_width + absoluteSize.x,
        relativeSize.y * anchorRect.m_height + absoluteSize.y
    };

    return sf::FloatRect(0.0f, 0.0f, size.x, size.y);
}

sf::FloatRect UIElement::getGlobalBounds() {
    const Bounds& anchorRect = (m_parent == m_anchor 
        ? m_anchor->m_context->childrenBounds
        : m_anchor->m_context->anchorBounds);

    sf::FloatRect localBounds = getLocalBounds();
    sf::Vector2f absolutePos = m_vars.pos.absolute.get();
    sf::Vector2f relativePos = m_vars.pos.relative.get();
    sf::Vector2f absoluteOrigin = m_vars.origin.absolute.get();
    sf::Vector2f relativeOrigin = m_vars.origin.relative.get();

    sf::Vector2f pos = {
        relativePos.x * anchorRect.m_width 
        + absolutePos.x + anchorRect.m_pos.x
        - absoluteOrigin.x - relativeOrigin.x * localBounds.width
        - localBounds.left,

        relativePos.y * anchorRect.m_height 
        + absolutePos.y + anchorRect.m_pos.y
        - absoluteOrigin.y - relativeOrigin.y * localBounds.height
        - localBounds.top
    };

    return sf::FloatRect(pos.x, pos.y, localBounds.width, localBounds.height);
}

void UIElement::computeBounds() {
    float borderWidth = m_vars.borderWidth.get();
    float padding = m_vars.padding.get();
    const sf::FloatRect bounds = getGlobalBounds();
    m_context->mouseBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
    m_context->mouseBounds.m_radius = m_vars.borderRadius.get();
    m_context->childrenBounds = Bounds(
        { bounds.left + borderWidth + padding,
        bounds.top + borderWidth + padding }, 
        bounds.width - borderWidth * 2 - padding * 2,
        bounds.height - borderWidth * 2 - padding * 2
    );
    m_context->anchorBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
}

void UIElement::computeSize() {
    if (m_context->sizeMode[0] == UISizeMode::FIXED || m_context->sizeMode[1] == UISizeMode::FIXED) {
        computeBounds();
    }
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->computeSize();
    }

    if (m_context->sizeMode[0] == UISizeMode::FIT_CONTENT || m_context->sizeMode[1] == UISizeMode::FIT_CONTENT) {
        float borderWidth = m_vars.borderWidth.get();
        float padding = m_vars.padding.get();
        sf::Vector2f size = m_vars.size.absolute.get();
        sf::Vector2f newSize = size;
        
        if (m_context->sizeMode[0] == UISizeMode::FIT_CONTENT) {
            if (m_children.size() == 0) {
                newSize.x = borderWidth * 2 + padding * 2;
            }
            else {
                float x_min = 10000000.0f;
                float x_max = -10000000.0f;

                for (int i = 0; i < m_children.size(); i++) {
                    UIElement* c = m_children[i];

                    sf::FloatRect bounds = c->getGlobalBounds();
                    x_min = std::min(x_min, bounds.left);
                    x_max = std::max(x_max, bounds.left + bounds.width);
                }
                newSize.x = (x_max - x_min) + borderWidth * 2 + padding * 2;
            }
        }
        if (m_context->sizeMode[1] == UISizeMode::FIT_CONTENT) {
            if (m_children.size() == 0) {
                newSize.y = borderWidth * 2 + padding * 2;
            }
            else {
                float y_min = 10000000.0f;
                float y_max = -10000000.0f;

                for (int i = 0; i < m_children.size(); i++) {
                    UIElement* c = m_children[i];

                    sf::FloatRect bounds = c->getGlobalBounds();
                    y_min = std::min(y_min, bounds.top);
                    y_max = std::max(y_max, bounds.top + bounds.height);
                }
                newSize.y = (y_max - y_min) + borderWidth * 2 + padding * 2;
            }
        }

        if (newSize.x != size.x || newSize.y != size.y) {
            m_vars.size.absolute.setAllSmoothly(newSize);
        }
        computeBounds();
    }
}

void UIElement::computeTransforms(sf::Transform parentTransform, float parentOpacity) {
    const Bounds bounds = m_context->mouseBounds;

    const float scale = m_vars.scale.get();
    const float rotate = m_vars.rotate.get();
    const sf::Vector2f absTranslate = m_vars.translate.absolute.get();
    const sf::Vector2f relTranslate = m_vars.translate.relative.get();
    const sf::Vector2f absTransformOrigin = m_vars.transformOrigin.absolute.get();
    const sf::Vector2f relTransformOrigin = m_vars.transformOrigin.relative.get();

    const sf::Vector2f translate = {
        absTranslate.x + relTranslate.x * bounds.m_width,
        absTranslate.y + relTranslate.y * bounds.m_height
    };
    const sf::Vector2f transformOrigin = {
        bounds.m_pos.x + absTransformOrigin.x + relTransformOrigin.x * bounds.m_width,
        bounds.m_pos.y + absTransformOrigin.y + relTransformOrigin.y * bounds.m_height
    };

    sf::Transform t;
    t.scale(sf::Vector2f(scale, scale), transformOrigin);
    t.rotate(rotate, transformOrigin);
    t.translate(translate);

    parentTransform *= t;
    m_context->calculatedTransform = parentTransform;
    m_context->mouseBounds.m_transform = parentTransform;
    m_context->childrenBounds.m_transform = parentTransform;

    float opacity = m_vars.opacity.get();
    parentOpacity *= opacity;
    m_context->calculatedOpacity = parentOpacity;
    m_context->opacityShader.setUniform("opacity", parentOpacity);

    for (UIElement* child : m_children) {
        child->computeTransforms(parentTransform, parentOpacity);
    }
}

void UIElement::propogateCall(const std::function<void(UIElement*)>& func) {
    func(this);
    for (int i = 0; i < m_children.size(); i++) {
        m_children[i]->propogateCall(func);
    }
}

void UIElement::draw(WindowHandler* window) {
    if (m_context->m_current.state == UIElementState::HIDDEN || fabs(m_context->calculatedOpacity - 0.0f) < 0.001f)
        return;

    ColorHSL color = m_vars.color.get();
    ColorHSL borderColor = m_vars.borderColor.get();

    const sf::Color sfColor = hslToRgb(color);
    const sf::Color sfBorderColor = hslToRgb(borderColor);
    const float borderRadius = m_vars.borderRadius.get();
    const float borderWidth = m_vars.borderWidth.get();
    const float padding = m_vars.padding.get();

    Bounds& bounds = m_context->mouseBounds;

    shapes::RoundedOutlinedRect shape(
        bounds.m_pos, bounds.m_width, bounds.m_height, 
        borderRadius, sfColor, borderWidth, sfBorderColor, 0.0f
    );

    sf::RenderStates states = sf::RenderStates::Default;
    states.transform = m_context->calculatedTransform;
    states.shader = &m_context->opacityShader;
    window->m_window.draw(shape, states);

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

    if (m_context->m_current.state == UIElementState::HIDDEN || m_context->m_current.state == UIElementState::DISABLED) {
        m_context->mousePressedHere = false;
        m_context->keyboardPressedHere = false;
        return;
    }

    bool isMouseIn = m_context->mouseBounds.doesPointIntersect(sf::Vector2f(mouseContext.m_current.pos));

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

void UIElement::setState(UIElementState state) {
    m_context->m_current.state = state;
    m_vars.checkChangedStates();
}