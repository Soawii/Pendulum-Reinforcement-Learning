#pragma once
#include "../engine/EngineContext.hpp"
#include "MouseContext.hpp"
#include <SFML/Graphics.hpp>

class MouseHandler {
public:
    MouseHandler(EngineContext& context);

    void startFrame();
    void endFrame();
    void handleEvent(sf::Event& e);
    void beforeUpdate();
    void update(float dt);

    EngineContext& m_engineContext;
    MouseContext m_context;
};