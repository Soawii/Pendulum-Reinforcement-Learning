#include "KeyboardHandler.hpp"
#include "../draw/Camera.hpp"
#include "GUI/Button.hpp"
#include <iostream>

KeyboardHandler::KeyboardHandler(EngineContext& context) : m_engineContext(context) {
    
}

void KeyboardHandler::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::KeyPressed) {
        m_context.press(e.key.code);
    }
    else if (e.type == sf::Event::KeyReleased) {
        m_context.release(e.key.code);
    }
}
void KeyboardHandler::beforeUpdate() {
    
}
void KeyboardHandler::update(float dt) {
    Camera& camera = m_engineContext.m_windowHandler->m_camera;

    if (m_context.isPressedThisFrame(sf::Keyboard::Space)) {
        for (int i = 0; i < m_engineContext.m_pendulum->m_weightAmount; i++) {
            b2Body_SetLinearVelocity(m_engineContext.m_pendulum->m_weights[i], {0.0f, 0.0f});
        }
    }
    if (m_context.isPressed(sf::Keyboard::LControl)) {
        if (m_context.isTriggeredThisFrame(sf::Keyboard::Equal)) {
            camera.zoom(conf::inputs::zoomMult);
        }
        if (m_context.isTriggeredThisFrame(sf::Keyboard::Dash)) {
            camera.zoom(1 / conf::inputs::zoomMult);
        }
    }

    for (int i = int(sf::Keyboard::Num1); i < int(sf::Keyboard::Num9); i++) {
        if (m_context.isPressedThisFrame(sf::Keyboard::Key(i))) {
            int weightNumber = i - int(sf::Keyboard::Num1);
            if (weightNumber == 0) {
                camera.bindToBody(m_engineContext.m_pendulum->m_anchor);
            }
            else if (weightNumber - 1 < m_engineContext.m_pendulum->m_weightAmount) {
                camera.bindToBody(m_engineContext.m_pendulum->m_weights[weightNumber - 1]);
            }
        }
    }

    if (m_context.isPressed(sf::Keyboard::W)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(0.0f, conf::inputs::cameraKeyboardSensitivity));
    }
    if (m_context.isPressed(sf::Keyboard::A)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(-conf::inputs::cameraKeyboardSensitivity, 0.0f));
    }
    if (m_context.isPressed(sf::Keyboard::S)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(0.0f, -conf::inputs::cameraKeyboardSensitivity));
    }
    if (m_context.isPressed(sf::Keyboard::D)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(conf::inputs::cameraKeyboardSensitivity, 0.0f));
    }
}

void KeyboardHandler::startFrame() {
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        m_context.m_current.isTriggeredThisFrame[i] = false;
    }
}
void KeyboardHandler::endFrame() {
    m_context.m_prev = m_context.m_current;
}