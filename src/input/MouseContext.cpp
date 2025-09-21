#include "MouseContext.hpp"

MouseFrameContext::MouseFrameContext() {
    for (int i = 0; i < sf::Mouse::ButtonCount; i++) {
        isPressed[i] = false;
    }
    pos = {0, 0};
    mouseWheelScrolled = false;
    scrollDelta = 0;
}

MouseContext::MouseContext() {

}
void MouseContext::press(sf::Mouse::Button button) {
    m_current.isPressed[button] = true;
}
void MouseContext::release(sf::Mouse::Button button) {
    m_current.isPressed[button] = false;
}
bool MouseContext::isPressed(sf::Mouse::Button button) {
    return m_current.isPressed[button];
}
bool MouseContext::isPressedThisFrame(sf::Mouse::Button button) {
    return m_current.isPressed[button] && !m_prev.isPressed[button];
}
bool MouseContext::isPressedForMultipleFrames(sf::Mouse::Button button) {
    return m_current.isPressed[button] && m_prev.isPressed[button];
}
bool MouseContext::isReleasedThisFrame(sf::Mouse::Button button) {
    return !m_current.isPressed[button] && m_prev.isPressed[button];
}