#include <SFML/Graphics.hpp>
#include "DoublePendulum.hpp"
#include "conf.hpp"
#include "util.hpp"
#include "WindowHandler.hpp"
#include "Camera.hpp"
#include <iostream>

WindowHandler::WindowHandler(
        sf::VideoMode mode, const sf::String &title, 
        b2Vec2 cameraCenter, float cameraZoom,
        float cameraCenterSmoothing, float cameraZoomSmoothing,
        sf::Uint32 style, const sf::ContextSettings &settings) 
        : m_camera(cameraCenter, cameraZoom, cameraCenterSmoothing, cameraZoomSmoothing) {
    m_window.create(mode, title, style, settings);
}

void WindowHandler::drawSlider() {
    sf::Vector2f topLeft = {
        float(m_camera.worldPosToScreen({conf::sim::rangeLeft - conf::sim::baseSize - conf::sim::sliderOutlineWidth - conf::sim::sliderOutlinePadding, 0.0f}).x),
        float(m_camera.worldPosToScreen({0.0f, conf::sim::baseSize + conf::sim::sliderOutlineWidth + conf::sim::sliderOutlinePadding}).y)
    };
    const float width = m_camera.worldSizeToScreen(conf::sim::rangeRight - conf::sim::rangeLeft + conf::sim::baseSize * 2 + conf::sim::sliderOutlineWidth * 2 + conf::sim::sliderOutlinePadding * 2);
    const float height = m_camera.worldSizeToScreen(conf::sim::baseSize * 2 + conf::sim::sliderOutlineWidth * 2 + conf::sim::sliderOutlinePadding * 2);

    std::pair<sf::VertexArray, sf::VertexArray> p = util::getRoundedRect(
            topLeft, width, height,
            m_camera.worldSizeToScreen(conf::sim::baseRadius + conf::sim::sliderOutlinePadding + conf::sim::sliderOutlineWidth), 
            conf::colors::sliderColor,
            std::max(1.0f, m_camera.worldSizeToScreen(conf::sim::sliderOutlineWidth)), 
            conf::colors::sliderOutlineColor,
            float(m_camera.worldSizeToScreen(conf::sim::sliderOutlinePadding)));

    m_window.draw(p.first);
    m_window.draw(p.second);
}

void WindowHandler::drawBase(const b2BodyId baseId) {
    const b2Vec2 world_anchor = b2Body_GetPosition(baseId);
    const sf::Vector2i screen_anchor = m_camera.worldPosToScreen(world_anchor);

    sf::VertexArray p = util::getRoundedRect(
            sf::Vector2f(screen_anchor.x - m_camera.worldSizeToScreen(conf::sim::baseSize), screen_anchor.y - m_camera.worldSizeToScreen(conf::sim::baseSize)), 
            m_camera.worldSizeToScreen(2 * conf::sim::baseSize), m_camera.worldSizeToScreen(2 * conf::sim::baseSize),
            m_camera.worldSizeToScreen(conf::sim::baseRadius), 
            conf::colors::baseColor).first;

    m_window.draw(p);
}

void WindowHandler::drawWeight(const b2BodyId weightId) {
    const b2Vec2 world_body = b2Body_GetPosition(weightId);
    const sf::Vector2i screen_body = m_camera.worldPosToScreen(world_body);
    const b2Rot body_rot = b2Body_GetRotation(weightId);
    const float angle = b2Rot_GetAngle(body_rot);
    const float screen_radius = m_camera.worldSizeToScreen(conf::sim::weightRadius);
    
    const sf::VertexArray hollow_circle = util::getHollowCircle(
        (sf::Vector2f)screen_body,
        screen_radius, 
        m_camera.worldSizeToScreen(conf::sim::weightStrokeWidth),
        50,
        conf::colors::weightOutlineColor);


    float weightRadius = m_camera.worldSizeToScreen(conf::sim::weightRadius);
    sf::CircleShape weightShape(weightRadius);
    weightShape.setOrigin({weightRadius, weightRadius});
    weightShape.setFillColor(conf::colors::weightColor);
    weightShape.setPosition((sf::Vector2f)screen_body);

    m_window.draw(weightShape);
    m_window.draw(hollow_circle);
}

void WindowHandler::drawJoint(const b2JointId jointId) {
    const b2BodyId body1 = b2Joint_GetBodyA(jointId);
    const b2BodyId body2 = b2Joint_GetBodyB(jointId);

    const b2Vec2 world_body1 = b2Body_GetPosition(body1);
    const b2Vec2 world_body2 = b2Body_GetPosition(body2);
    const sf::Vector2i screen_body1 = m_camera.worldPosToScreen(world_body1);
    const sf::Vector2i screen_body2 = m_camera.worldPosToScreen(world_body2);
    
    sf::VertexArray line = util::getLine((sf::Vector2f)screen_body1, (sf::Vector2f)screen_body2, conf::colors::jointColor);
    sf::VertexArray connection1 = util::getRect(
        (sf::Vector2f)screen_body1 - sf::Vector2f(conf::draw::jointConnectionWidth, conf::draw::jointConnectionWidth) / 2.0f,
        conf::draw::jointConnectionWidth, conf::draw::jointConnectionWidth, conf::colors::jointConnectionColor);
    sf::VertexArray connection2 = util::getRect(
        (sf::Vector2f)screen_body2 - sf::Vector2f(conf::draw::jointConnectionWidth, conf::draw::jointConnectionWidth) / 2.0f,
        conf::draw::jointConnectionWidth, conf::draw::jointConnectionWidth, conf::colors::jointConnectionColor);
    
    m_window.draw(line);
    m_window.draw(connection1);
    m_window.draw(connection2);
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
    m_window.clear(conf::colors::backgroundColor);
    drawPendulum(pendulum);
    m_window.display();
}