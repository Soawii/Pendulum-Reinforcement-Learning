#include "MouseHandler.hpp"
#include "../draw/Camera.hpp"
#include "../conf.hpp"
#include <iostream>

MouseHandler::MouseHandler(EngineContext& context) : m_engineContext(context) {
    
}

void MouseHandler::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::MouseButtonPressed) {
        m_context.press(e.mouseButton.button);
    }
    else if (e.type == sf::Event::MouseButtonReleased) {
        m_context.release(e.mouseButton.button);
    }
    else if (e.type == sf::Event::MouseWheelScrolled) {
        m_context.m_current.mouseWheelScrolled = true;
        m_context.m_current.scrollDelta = e.mouseWheelScroll.delta;
    }
}
void MouseHandler::beforeUpdate() {
    m_context.m_current.pos = sf::Mouse::getPosition(m_engineContext.m_windowHandler->m_window);
}
void MouseHandler::update(float dt) {
    if (m_context.m_current.mouseWheelScrolled) {
        m_engineContext.m_windowHandler->m_camera.zoomInAt(std::pow(conf::inputs::zoomMult, m_context.m_current.scrollDelta), m_context.m_current.pos);
    }

    if (m_context.isPressedForMultipleFrames(sf::Mouse::Left)) {
        sf::Vector2f mouse_delta(-(m_context.m_current.pos.x - m_context.m_prev.pos.x), m_context.m_current.pos.y - m_context.m_prev.pos.y);
        m_engineContext.m_windowHandler->m_camera.unbindFromBody();
        m_engineContext.m_windowHandler->m_camera.move(mouse_delta * conf::inputs::cameraMouseSensitivity);
    }

    if (m_context.isPressedThisFrame(sf::Mouse::Right)) {
        b2Vec2 mouseWorldPos = m_engineContext.m_windowHandler->m_camera.screenPosToWorld(m_context.m_current.pos);
        b2BodyId intersectingBody = m_engineContext.m_pendulum->getInteresectingBody(mouseWorldPos);
        if (B2_IS_NON_NULL(intersectingBody)) {
            if (B2_ID_EQUALS(m_engineContext.m_windowHandler->m_camera.m_boundBody, intersectingBody)) {
                m_engineContext.m_windowHandler->m_camera.unbindFromBody();
            }
            else {
                m_engineContext.m_windowHandler->m_camera.bindToBody(intersectingBody);
            }
        }
    }
}

void MouseHandler::startFrame() {
    m_context.m_current.mouseWheelScrolled = false;
}
void MouseHandler::endFrame() {
    m_context.m_prev = m_context.m_current;
}