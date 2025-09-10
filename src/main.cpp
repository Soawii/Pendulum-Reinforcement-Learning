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

    float angles[2] = {conf::sim::angleA, conf::sim::angleB};
    engine.createPendulum(b2Vec2{0.0f, conf::sim::gravity}, conf::sim::jointLength, conf::sim::weightMass, conf::sim::weightRadius, angles, conf::sim::weight_amount);

    while (engine.getWindow()->isOpen()) {
        sf::Event event;
        while (engine.getWindow()->pollEvent(event)) {
            engine.handleEvent(event);
        }
        engine.update(conf::window::dt);
        engine.m_windowHandler->draw(*engine.m_pendulum);
    }
    return 0;
}