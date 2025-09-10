#include <SFML/Graphics.hpp>
#include "DoublePendulum.hpp"
#include "conf.hpp"
#include "util.hpp"
#include "WindowHandler.hpp"
#include <iostream>

WindowHandler::WindowHandler(sf::VideoMode mode, const sf::String &title, sf::Uint32 style, const sf::ContextSettings &settings) {
    m_window.create(mode, title, style, settings);

    m_weightShape = new sf::CircleShape(conf::sim::weightRadius * conf::draw::scale);
    m_baseShape = new sf::CircleShape(conf::sim::baseSize * conf::draw::scale, 50);
    m_jointConnectionShape = new sf::RectangleShape(sf::Vector2f(conf::draw::jointConnectionWidth, conf::draw::jointConnectionWidth));

    sf::FloatRect weightBounds = m_weightShape->getLocalBounds();
    sf::FloatRect baseBounds = m_baseShape->getLocalBounds();
    sf::FloatRect jointConnectionBounds = m_jointConnectionShape->getLocalBounds();
    m_weightShape->setOrigin(weightBounds.width / 2, weightBounds.height / 2);
    m_baseShape->setOrigin(baseBounds.width / 2, baseBounds.height / 2);
    m_jointConnectionShape->setOrigin(jointConnectionBounds.width / 2, jointConnectionBounds.height / 2);

    m_backgroundColor = sf::Color(30, 30, 30);
    m_jointColor = sf::Color(255, 255, 255);
    m_jointConnectionColor = sf::Color(255, 255, 255);
    m_baseColor = sf::Color(57, 65, 100);
    m_weightColor = sf::Color(118, 86, 29);
    m_weightOutlineColor = sf::Color(255, 172, 28);
    m_sliderColor = sf::Color(0,0,0,0);
    m_sliderOutlineColor = sf::Color(230, 230, 230);

    m_weightShape->setFillColor(m_weightColor);
    m_baseShape->setFillColor(m_baseColor);
    m_jointConnectionShape->setFillColor(m_jointConnectionColor);
}

void WindowHandler::drawSlider() {
    sf::Vector2f topLeft = {
        float(util::worldToScreen({conf::sim::rangeLeft - conf::sim::baseSize - conf::sim::sliderOutlineWidth - conf::sim::sliderOutlinePadding, 0.0f}).x),
        float(util::worldToScreen({0.0f, conf::sim::baseSize + conf::sim::sliderOutlineWidth + conf::sim::sliderOutlinePadding}).y)
    };
    const float width = (conf::sim::rangeRight - conf::sim::rangeLeft + conf::sim::baseSize * 2 + conf::sim::sliderOutlineWidth * 2 + conf::sim::sliderOutlinePadding * 2) * conf::draw::scale;
    const float height = (conf::sim::baseSize * 2 + conf::sim::sliderOutlineWidth * 2 + conf::sim::sliderOutlinePadding * 2) * conf::draw::scale;

    std::pair<sf::VertexArray, sf::VertexArray> p = util::getRoundedRect(
            topLeft, width, height,
            (conf::sim::baseRadius + conf::sim::sliderOutlinePadding + conf::sim::sliderOutlineWidth) * conf::draw::scale, 
            m_sliderColor,
            conf::sim::sliderOutlineWidth * conf::draw::scale, 
            m_sliderOutlineColor,
            conf::sim::sliderOutlinePadding * conf::draw::scale);

    m_window.draw(p.first);
    m_window.draw(p.second);
}

void WindowHandler::drawBase(const b2BodyId baseId) {
    const b2Vec2 world_anchor = b2Body_GetPosition(baseId);
    const sf::Vector2i screen_anchor = util::worldToScreen(world_anchor);

    sf::VertexArray p = util::getRoundedRect(
            sf::Vector2f(screen_anchor.x - conf::sim::baseSize * conf::draw::scale, screen_anchor.y - conf::sim::baseSize * conf::draw::scale), 
            conf::sim::baseSize * conf::draw::scale * 2, conf::sim::baseSize * conf::draw::scale * 2,
            conf::sim::baseRadius * conf::draw::scale, 
            m_baseColor).first;

    m_window.draw(p);
}

void WindowHandler::drawWeight(const b2BodyId weightId) {
    const b2Vec2 world_body = b2Body_GetPosition(weightId);
    const sf::Vector2i screen_body = util::worldToScreen(world_body);
    const b2Rot body_rot = b2Body_GetRotation(weightId);
    const float angle = b2Rot_GetAngle(body_rot);
    const float screen_radius = conf::sim::weightRadius * conf::draw::scale;
    
    const sf::VertexArray hollow_circle = util::getHollowCircle(
        (sf::Vector2f)screen_body,
        screen_radius, 
        conf::draw::weightStrokeWidth,
        50,
        m_weightOutlineColor);
    
    /*
    const sf::VertexArray pointing_rect = util::getRect(
        (sf::Vector2f)screen_body, 
        {screen_body.x + screen_radius * cos(angle), screen_body.y + screen_radius * sin(angle)},
        conf::draw::weightStrokeWidth,
        m_weightOutlineColor);
    */

    m_weightShape->setPosition((sf::Vector2f)screen_body);

    m_window.draw(*m_weightShape);
    m_window.draw(hollow_circle);
    //m_window.draw(pointing_rect);
}

void WindowHandler::drawJoint(const b2JointId jointId) {
    const b2BodyId body1 = b2Joint_GetBodyA(jointId);
    const b2BodyId body2 = b2Joint_GetBodyB(jointId);

    const b2Vec2 world_body1 = b2Body_GetPosition(body1);
    const b2Vec2 world_body2 = b2Body_GetPosition(body2);
    const sf::Vector2i screen_body1 = util::worldToScreen(world_body1);
    const sf::Vector2i screen_body2 = util::worldToScreen(world_body2);
    
    sf::VertexArray line = util::getLine((sf::Vector2f)screen_body1, (sf::Vector2f)screen_body2, m_jointColor);
    m_window.draw(line);
    m_jointConnectionShape->setPosition((sf::Vector2f)screen_body1);
    m_window.draw(*m_jointConnectionShape);
    m_jointConnectionShape->setPosition((sf::Vector2f)screen_body2);
    m_window.draw(*m_jointConnectionShape);
}

void WindowHandler::drawPendulum(const DoublePendulum& pendulum) {
    drawSlider();
    drawBase(pendulum.m_anchor);

    for (int i = 0; i < pendulum.m_weightAmount; i++) 
        drawWeight(pendulum.m_weights[i]);

    for (int i = 0; i < pendulum.m_weightAmount; i++) 
        drawJoint(pendulum.m_joints[i]);

}

void WindowHandler::draw(const DoublePendulum& pendulum) {
    m_window.clear(m_backgroundColor);
    drawPendulum(pendulum);
    m_window.display();
}