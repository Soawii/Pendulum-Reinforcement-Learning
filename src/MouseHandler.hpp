#pragma once
#include "EngineContext.hpp"
#include <SFML/Graphics.hpp>

class MouseHandlerContext {
public:
    MouseHandlerContext();

    bool isPressed[sf::Mouse::ButtonCount];
    sf::Vector2i pos;
    bool mouseWheelScrolled;
    int scrollDelta;
};

class MouseHandler {
public:
    MouseHandler(EngineContext& context);

    bool isPressed(sf::Mouse::Button button);
    bool isPressedThisFrame(sf::Mouse::Button button);
    bool isPressedForMultipleFrames(sf::Mouse::Button button);

    void startFrame();
    void handleEvent(sf::Event& e);
    void update(float dt);

    EngineContext& m_engineContext;
    MouseHandlerContext m_context, m_contextPrev;
};