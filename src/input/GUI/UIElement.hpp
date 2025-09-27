#pragma once
#include <SFML/Graphics.hpp>
#include "../../draw/WindowHandler.hpp"
#include "../MouseContext.hpp"
#include "../KeyboardContext.hpp"
#include "UIElementContext.hpp"
#include <string>
#include "../../draw/Shapes.hpp"
#include "../../draw/Color.hpp"
#include <iostream>
#include <vector>
#include "StateVariable.hpp"
#include "Bounds.hpp"

class UIElement {
public:
    UIElement();

    virtual sf::FloatRect getLocalBounds();
    virtual sf::FloatRect getGlobalBounds();
    virtual void computeBounds();

    virtual void computeSize();
    virtual void computeTransforms(sf::Transform parentTransform = sf::Transform(), float parentOpacity = 1.0f);

    virtual void propogateCall(const std::function<void(UIElement*)>& func);

    virtual void draw(WindowHandler* window);
    virtual void update(MouseContext& mouseContext, KeyboardContext& keyboardContext);
    void setState(UIElementState state);

    UIElement* m_parent = nullptr;
    UIElement* m_anchor = nullptr;
    std::vector<UIElement*> m_children;
    UIElementContext* m_context = new UIElementContext();
    StateVariables m_vars;
};
