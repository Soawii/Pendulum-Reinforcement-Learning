#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../../util/TimingVariable.hpp"
#include <string>
#include <vector>
#include "../../draw/Color.hpp"
#include "UIElementState.hpp"
#include "Bounds.hpp"


class UIElementFrameContext {
public:
    UIElementState state = UIElementState::NORMAL;
};

class UIElementContext {
public:
    UIElementFrameContext m_prev, m_current;
    std::string id;

    bool mousePressedHere = false, keyboardPressedHere = false, stopPropogation = false;

    Bounds childrenBounds, anchorBounds, mouseBounds;

    sf::Transform calculatedTransform;
    float calculatedOpacity;

    sf::Shader opacityShader;

    UISizeMode sizeMode[2] = {UISizeMode::FIXED, UISizeMode::FIXED};

    std::vector<sf::Keyboard::Key> onPressKeys;
    std::vector<std::function<void()>> onClick;
    std::vector<std::function<void()>> onMouseEnter;
    std::vector<std::function<void()>> onMouseLeave;
    std::vector<std::function<void()>> whileActive;
    
    std::vector<std::function<void()>> onStateChange[size_t(UIElementState::STATE_AMOUNT)];

    bool didMouseMoveIn();
    bool didMouseMoveOut();

    bool wasStateChanged();
    void triggerEvents(const std::vector<std::function<void()>>& events);
    void changedState();

    void startFrame();
    void endFrame();
};