#include "KeyboardHandler.hpp"
#include "Camera.hpp"
#include <iostream>

KeyboardHandlerContext::KeyboardHandlerContext() {
    for (int i = 0; i < sf::Keyboard::KeyCount; i++) {
        isPressed[i] = false;
        isTriggeredThisFrame[i] = false;
    }
}

KeyboardHandler::KeyboardHandler(EngineContext& context) : m_engineContext(context) {
    
}

bool KeyboardHandler::isPressed(sf::Keyboard::Key key) {
    return m_context.isPressed[key];
}
bool KeyboardHandler::isPressedThisFrame(sf::Keyboard::Key key) {
    return m_context.isPressed[key] && !m_contextPrev.isPressed[key];
}
bool KeyboardHandler::isTriggeredThisFrame(sf::Keyboard::Key key) {
    return m_context.isTriggeredThisFrame[key];
}

void KeyboardHandler::startFrame() {
    for (int i = 0; i < sf::Keyboard::KeyCount; i++)
        m_context.isTriggeredThisFrame[i] = false;
}
void KeyboardHandler::handleEvent(sf::Event& e) {
    if (e.type == sf::Event::KeyPressed) {
        sf::Keyboard::Key key = e.key.code;
        m_context.isPressed[key] = true;
        m_context.isTriggeredThisFrame[key] = true;
    }
    else if (e.type == sf::Event::KeyReleased) {
        sf::Keyboard::Key key = e.key.code;
        m_context.isPressed[key] = false;
    }
}
void KeyboardHandler::update(float dt) {
    Camera& camera = m_engineContext.m_windowHandler->m_camera;

    if (isPressedThisFrame(sf::Keyboard::Space)) {
        for (int i = 0; i < m_engineContext.m_pendulum->m_weightAmount; i++) {
            b2Body_SetLinearVelocity(m_engineContext.m_pendulum->m_weights[i], {0.0f, 0.0f});
        }
    }
    if (isPressed(sf::Keyboard::LControl)) {
        if (isTriggeredThisFrame(sf::Keyboard::Equal)) {
            camera.zoom(conf::inputs::zoomMult);
        }
        if (isTriggeredThisFrame(sf::Keyboard::Dash)) {
            camera.zoom(1 / conf::inputs::zoomMult);
        }
    }

    for (int i = int(sf::Keyboard::Num1); i < int(sf::Keyboard::Num9); i++) {
        if (isPressedThisFrame(sf::Keyboard::Key(i))) {
            int weightNumber = i - int(sf::Keyboard::Num1);
            if (weightNumber == 0) {
                camera.bindToBody(m_engineContext.m_pendulum->m_anchor);
            }
            else if (weightNumber - 1 < m_engineContext.m_pendulum->m_weightAmount) {
                camera.bindToBody(m_engineContext.m_pendulum->m_weights[weightNumber - 1]);
            }
        }
    }

    if (isPressed(sf::Keyboard::W)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(0.0f, conf::inputs::cameraKeyboardSensitivity));
    }
    if (isPressed(sf::Keyboard::A)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(-conf::inputs::cameraKeyboardSensitivity, 0.0f));
    }
    if (isPressed(sf::Keyboard::S)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(0.0f, -conf::inputs::cameraKeyboardSensitivity));
    }
    if (isPressed(sf::Keyboard::D)) {
        camera.unbindFromBody();
        camera.move(sf::Vector2f(conf::inputs::cameraKeyboardSensitivity, 0.0f));
    }

    float goalVelocity = 0.0f;
    if (isPressed(sf::Keyboard::Left)) {
        goalVelocity = -conf::inputs::baseMaxVelocity;
    }
    if (isPressed(sf::Keyboard::Right)) {
        goalVelocity = conf::inputs::baseMaxVelocity;
    }
    m_engineContext.m_pendulum->setBaseGoalVelocity(goalVelocity);

    m_contextPrev = m_context;
}
