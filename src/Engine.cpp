#include "Engine.hpp"
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Engine::Engine() {
    
}
void Engine::createWindowHandler(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings) {
    m_windowHandler = new WindowHandler(mode, title, style, settings);
}
void Engine::createPendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, float angles[2], int weightAmount) {
    m_pendulum = new DoublePendulum(gravity, jointLength, weightMass, weightRadius, angles, weightAmount);
}
void Engine::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::Closed) {
        m_windowHandler->m_window.close();
    }
    else if (e.type == sf::Event::KeyPressed) {
        const auto code = e.key.code;
        if (code == sf::Keyboard::Space) {
            for (int i = 0; i < m_pendulum->m_weightAmount; i++) {
                b2Body_SetLinearVelocity(m_pendulum->m_weights[i], {0.0f, 0.0f});
            }
        }
    }
}
void Engine::update(float dt) {
    float goalVelocity = 0.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        goalVelocity = -conf::sim::baseGoalVelocity;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        goalVelocity = conf::sim::baseGoalVelocity;
    }
    m_pendulum->setBaseGoalVelocity(goalVelocity);

    m_pendulum->update(dt);
}

sf::RenderWindow* Engine::getWindow() {
    return &m_windowHandler->m_window;
}