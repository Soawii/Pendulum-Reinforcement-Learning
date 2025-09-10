#pragma once
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

class Engine {
public:
    Engine();

    void createWindowHandler(sf::VideoMode mode, const sf::String &title, sf::Uint32 style = 7U, const sf::ContextSettings &settings = sf::ContextSettings());
    void createPendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, float anles[2], int weightAmount);
    void handleEvent(sf::Event& e);
    void update(float dt);

    sf::RenderWindow* getWindow();

    WindowHandler* m_windowHandler;
    DoublePendulum* m_pendulum;
};