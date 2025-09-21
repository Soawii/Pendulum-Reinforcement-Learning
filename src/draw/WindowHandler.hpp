#pragma once
#include <SFML/Graphics.hpp>
#include "../sim/DoublePendulum.hpp"
#include "../conf.hpp"
#include "Camera.hpp"

class WindowHandler {
public:
    WindowHandler(
        sf::VideoMode mode, const sf::String &title, 
        b2Vec2 cameraCenter, float cameraZoom,
        float cameraCenterSmoothing, float cameraZoomSmoothing,
        sf::Uint32 style, const sf::ContextSettings &settings);

    sf::Vector2u getSize();

    void clear(sf::Color color);
    void drawSlider();
    void drawBase(const b2BodyId baseId);
    void drawWeight(const b2BodyId weightId);
    void drawJoint(const b2JointId jointId);
    void drawPendulum(const DoublePendulum& pendulum);
    void draw(const DoublePendulum& pendulum);
    void draw(const sf::Drawable& drawable);
    void display();

    sf::RenderWindow m_window;
    Camera m_camera;
};