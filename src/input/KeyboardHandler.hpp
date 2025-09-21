#pragma once
#include "../engine/EngineContext.hpp"
#include <SFML/Graphics.hpp>
#include "KeyboardContext.hpp"

class KeyboardHandler {
public:
    KeyboardHandler(EngineContext& context);

    void startFrame();
    void endFrame();
    void handleEvent(sf::Event& e);
    void beforeUpdate();
    void update(float dt);

    EngineContext& m_engineContext;
    KeyboardContext m_context;
};