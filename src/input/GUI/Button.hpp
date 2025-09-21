#pragma once
#include "UIElement.hpp"
#include <functional>
#include "UIElementContext.hpp"

class Button : public UIElement {
public:
    Button();
    
    void draw(WindowHandler* window) override;
    void update(MouseContext& mouseContext, KeyboardContext& keyboardContext) override;
};