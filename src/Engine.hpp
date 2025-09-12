#pragma once
#include "EngineContext.hpp"
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"
#include "MouseHandler.hpp"
#include "KeyboardHandler.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <vector>

class Engine {
public:
    Engine();

    void createWindowHandler(sf::VideoMode mode, const sf::String &title, sf::Uint32 style = 7U, const sf::ContextSettings &settings = sf::ContextSettings());
    void createPendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, int weightAmount, std::vector<float> angles);
    
    void startFrame();
    void handleEvent(sf::Event& e);
    void update(float dt);

    sf::RenderWindow* getWindow();


    EngineContext m_context;
    MouseHandler m_mouseHandler;
    KeyboardHandler m_keyboardHandler;
};