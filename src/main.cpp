#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "draw/WindowHandler.hpp"
#include "sim/DoublePendulum.hpp"
#include "conf.hpp"
#include "engine/Engine.hpp"
#include <iostream>
#include <chrono>
#include "util/util.hpp"

int main() {
    conf::CONFSetup();

    Engine engine;

    engine.createWindowHandler(
        sf::VideoMode(conf::window::WIDTH, conf::window::HEIGHT),
        "Double Pendulum Balancing", 
        conf::draw::center, conf::draw::scale,
        conf::inputs::cameraCenterSmoothing * (1.0f / conf::window::dt), conf::inputs::cameraZoomSmoothing * (1.0f / conf::window::dt));
    engine.getWindow()->setFramerateLimit(conf::window::FPS);
    
    engine.createPendulum(
        conf::sim::gravity, conf::sim::jointLength, conf::sim::weightMass,
        conf::sim::weightRadius, conf::sim::weightAmount,
        conf::inputs::baseMaxVelocity, conf::inputs::baseAcceleration,
        conf::sim::angles);

    engine.createUIManager();

    while (engine.getWindow()->isOpen()) {
        sf::Event event;

        engine.startFrame();
        while (engine.getWindow()->pollEvent(event)) {
            engine.handleEvent(event);
        }
        engine.update(conf::window::dt);

        engine.m_context.m_windowHandler->clear(conf::colors::backgroundColor);
        engine.m_context.m_windowHandler->draw(*engine.m_context.m_pendulum);
        engine.m_UIManager->draw();

        engine.m_context.m_windowHandler->display();
        engine.endFrame();
    }
    return 0;
}