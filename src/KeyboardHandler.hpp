#pragma once
#include "EngineContext.hpp"
#include <SFML/Graphics.hpp>

class KeyboardHandlerContext {
public:
    KeyboardHandlerContext();

    bool isPressed[sf::Keyboard::KeyCount];
    bool isTriggeredThisFrame[sf::Keyboard::KeyCount];
};

class KeyboardHandler {
public:
    KeyboardHandler(EngineContext& context);

    bool isPressed(sf::Keyboard::Key key);
    bool isPressedThisFrame(sf::Keyboard::Key key);
    bool isTriggeredThisFrame(sf::Keyboard::Key key);

    void startFrame();
    void handleEvent(sf::Event& e);
    void update(float dt);

    EngineContext& m_engineContext;
    KeyboardHandlerContext m_context, m_contextPrev;
};