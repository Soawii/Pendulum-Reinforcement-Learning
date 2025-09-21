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

class UIElement {
public:
    UIElement();

    virtual void computeSize();

    virtual sf::Vector2f getSize();

    virtual void propogateCall(const std::function<void(UIElement*)>& func);

    virtual void draw(WindowHandler* window);
    
    virtual void update(MouseContext& mouseContext, KeyboardContext& keyboardContext);

    UIElement* m_parent = nullptr;
    UIElement* m_anchor = nullptr;
    std::vector<UIElement*> m_children;
    UIElementContext* m_context = new UIElementContext();
    StateVariables m_vars;
};
