#pragma once
#include <SFML/Graphics.hpp>
#include "DoublePendulum.hpp"
#include "conf.hpp"
#include "util.hpp"

class WindowHandler {
public:
    WindowHandler(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings);

    void drawSlider();
    void drawBase(const b2BodyId baseId);
    void drawWeight(const b2BodyId weightId);
    void drawJoint(const b2JointId jointId);
    void drawPendulum(const DoublePendulum& pendulum);
    void draw(const DoublePendulum& pendulum);

    sf::RenderWindow m_window;
    sf::Shape* m_weightShape;
    sf::Shape* m_baseShape;
    sf::Shape* m_jointConnectionShape;

    sf::Color m_backgroundColor,
                m_baseColor, 
                m_jointColor, 
                m_jointConnectionColor, 
                m_weightColor, 
                m_weightOutlineColor,
                m_sliderColor,
                m_sliderOutlineColor;
};