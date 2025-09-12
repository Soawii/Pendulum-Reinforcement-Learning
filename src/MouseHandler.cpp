#include "MouseHandler.hpp"
#include "Camera.hpp"
#include "conf.hpp"
#include "util.hpp"
#include <iostream>


MouseHandlerContext::MouseHandlerContext() {
    for (int i = 0; i < sf::Mouse::ButtonCount; i++)
        isPressed[i] = false;
    pos = sf::Vector2i(0, 0);
    mouseWheelScrolled = false;
    scrollDelta = 0;
}

MouseHandler::MouseHandler(EngineContext& context) : m_engineContext(context) {
    
}

bool MouseHandler::isPressed(sf::Mouse::Button button) {
    return m_context.isPressed[button];
}
bool MouseHandler::isPressedThisFrame(sf::Mouse::Button button) {
    return m_context.isPressed[button] && !m_contextPrev.isPressed[button];
}
bool MouseHandler::isPressedForMultipleFrames(sf::Mouse::Button button) {
    return m_context.isPressed[button] && m_contextPrev.isPressed[button];
}

void MouseHandler::startFrame() {
    m_context.mouseWheelScrolled = false;
}
void MouseHandler::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::MouseButtonPressed) {
        m_context.isPressed[e.mouseButton.button] = true;
    }
    else if (e.type == sf::Event::MouseButtonReleased) {
        m_context.isPressed[e.mouseButton.button] = false;
    }
    else if (e.type == sf::Event::MouseWheelScrolled) {
        m_context.mouseWheelScrolled = true;
        m_context.scrollDelta = e.mouseWheelScroll.delta;
    }
}
void MouseHandler::update(float dt) {
    m_context.pos = sf::Mouse::getPosition(m_engineContext.m_windowHandler->m_window);

    if (m_context.mouseWheelScrolled) {
        m_engineContext.m_windowHandler->m_camera.zoomInAt(std::pow(conf::inputs::zoomMult, m_context.scrollDelta), m_context.pos);
    }

    if (isPressedForMultipleFrames(sf::Mouse::Left)) {
        sf::Vector2f mouse_delta(-(m_context.pos.x - m_contextPrev.pos.x), m_context.pos.y - m_contextPrev.pos.y);
        m_engineContext.m_windowHandler->m_camera.unbindFromBody();
        m_engineContext.m_windowHandler->m_camera.move(mouse_delta * conf::inputs::cameraMouseSensitivity);
    }

    if (isPressedThisFrame(sf::Mouse::Right)) {
        b2Vec2 mouseWorldPos = m_engineContext.m_windowHandler->m_camera.screenPosToWorld(m_context.pos);
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

    m_contextPrev = m_context;
}