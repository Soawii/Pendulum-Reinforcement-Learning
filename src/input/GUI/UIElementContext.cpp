#include "UIElementContext.hpp"

bool UIElementContext::didMouseMoveIn() {
    return m_prev.state == UIElementState::NORMAL && m_current.state == UIElementState::HOVER;
}
bool UIElementContext::didMouseMoveOut() {
    return m_prev.state == UIElementState::HOVER && m_current.state == UIElementState::NORMAL;
}
void UIElementContext::startFrame() {
    stopPropogation = false;
}
void UIElementContext::endFrame() {
    m_prev.state = m_current.state;
}
bool UIElementContext::wasStateChanged() {
    return m_prev.state != m_current.state;
}
void UIElementContext::triggerEvents(const std::vector<std::function<void()>>& events) {
    for (int i = 0; i < events.size(); i++) {
        events[i]();
    }
}