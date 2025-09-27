#include "Engine.hpp"
#include "../draw/WindowHandler.hpp"
#include "../sim/DoublePendulum.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>

Engine::Engine() : m_context(), m_mouseHandler(m_context), m_keyboardHandler(m_context) {

}

void Engine::createWindowHandler(
        sf::VideoMode mode, const sf::String &title,
        b2Vec2 cameraCenter, float cameraZoom,
        float cameraCenterSmoothing, float cameraZoomSmoothing,
        sf::Uint32 style, const sf::ContextSettings &settings) {
    m_context.m_windowHandler = new WindowHandler(mode, title, cameraCenter, cameraZoom, cameraCenterSmoothing, cameraZoomSmoothing, style, settings);
}
void Engine::createPendulum(
        b2Vec2 gravity, float jointLength, float weightMass, 
        float weightRadius, int weightAmount,
        float maxBaseVelocity, float baseAcceleration,
        std::vector<float> angles) {
    m_context.m_pendulum = new DoublePendulum(gravity, jointLength, weightMass, weightRadius, weightAmount, maxBaseVelocity, baseAcceleration, angles);
}
void Engine::createUIManager() {
    m_UIManager = new UIManager(m_context);
}

void Engine::startFrame() {
    conf::time::now = std::chrono::steady_clock::now();

    m_mouseHandler.startFrame();
    m_keyboardHandler.startFrame();

    m_UIManager->startFrame();
}
void Engine::endFrame() {
    m_mouseHandler.endFrame();
    m_keyboardHandler.endFrame();

    m_UIManager->endFrame();
}
void Engine::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::Closed) {
        m_context.m_windowHandler->m_window.close();
    }
    else if (e.type == sf::Event::Resized) {
        m_context.m_windowHandler->m_window.setSize({e.size.width, e.size.height});
        const sf::View view(sf::FloatRect(0.0f, 0.0f, e.size.width, e.size.height));
        m_context.m_windowHandler->m_window.setView(view);
        conf::window::WIDTH = e.size.width;
        conf::window::HEIGHT = e.size.height;
    }
    m_mouseHandler.handleEvent(e);
    m_keyboardHandler.handleEvent(e);
}
void Engine::update(float dt) {
    m_mouseHandler.beforeUpdate();
    m_keyboardHandler.beforeUpdate();

    m_UIManager->update(m_mouseHandler.m_context, m_keyboardHandler.m_context);

    if (!m_UIManager->m_windowElement->m_context->stopPropogation) {
        m_mouseHandler.update(dt);
        m_keyboardHandler.update(dt);
    }
    m_context.m_pendulum->update(dt);
    m_context.m_windowHandler->m_camera.update(dt);
}

sf::RenderWindow* Engine::getWindow() {
    return &m_context.m_windowHandler->m_window;
}