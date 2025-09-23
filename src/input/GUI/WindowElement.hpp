#pragma once
#include "UIElement.hpp"
#include "../../draw/WindowHandler.hpp"

class WindowElement : public UIElement {
public:
    WindowElement(WindowHandler* window);

    sf::FloatRect getGlobalBounds() override;
    void computeSize() override;
    void draw(WindowHandler* window) override;
    void update(MouseContext& mouseContext, KeyboardContext& keyboardContext) override;

    WindowHandler* m_window;
};
