#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"
#include "conf.hpp"
#include "Engine.hpp"
#include <iostream>

int main() {
    Engine engine;

    engine.createWindowHandler(sf::VideoMode(conf::window::WIDTH, conf::window::HEIGHT), "SFML + Box2D with CMake");
    engine.getWindow()->setFramerateLimit(conf::window::FPS);

    engine.createPendulum(conf::sim::gravity, conf::sim::jointLength, conf::sim::weightMass, conf::sim::weightRadius, conf::sim::weightAmount, conf::sim::angles);

    while (engine.getWindow()->isOpen()) {
        sf::Event event;

        engine.startFrame();
        while (engine.getWindow()->pollEvent(event)) {
            engine.handleEvent(event);
        }
        engine.update(conf::window::dt);
        engine.m_context.m_windowHandler->draw(*engine.m_context.m_pendulum);
    }
    return 0;
}