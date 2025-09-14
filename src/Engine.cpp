#include "Engine.hpp"
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>

Engine::Engine() : m_context(), m_mouseHandler(m_context), m_keyboardHandler(m_context) {

}

void Engine::createWindowHandler(
        sf::VideoMode mode, const sf::String &title,
        b2Vec2 cameraCenter, float cameraZoom,
        float cameraCenterSmoothing, float cameraZoomSmoothing,
        sf::Uint32 style, const sf::ContextSettings &settings) {
    m_context.m_windowHandler = new WindowHandler(mode, title, cameraCenter, cameraZoom, cameraCenterSmoothing, cameraZoomSmoothing, style, settings);
}
void Engine::createPendulum(b2Vec2 gravity, float jointLength, float weightMass, float weightRadius, int weightAmount, std::vector<float> angles) {
    m_context.m_pendulum = new DoublePendulum(gravity, jointLength, weightMass, weightRadius, weightAmount, angles);
}

void Engine::startFrame() {
    m_mouseHandler.startFrame();
    m_keyboardHandler.startFrame();
}
void Engine::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::Closed) {
        m_context.m_windowHandler->m_window.close();
    }
    m_mouseHandler.handleEvent(e);
    m_keyboardHandler.handleEvent(e);
}
void Engine::update(float dt) {
    m_mouseHandler.update(dt);
    m_keyboardHandler.update(dt);
    m_context.m_pendulum->update(dt);
    m_context.m_windowHandler->m_camera.update(dt);
}

sf::RenderWindow* Engine::getWindow() {
    return &m_context.m_windowHandler->m_window;
}