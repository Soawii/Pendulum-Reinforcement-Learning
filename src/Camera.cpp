#include "Camera.hpp"
#include "conf.hpp"
#include "util.hpp"
#include <iostream>

Camera::Camera(b2Vec2 center, float zoom) 
        :   m_center(center, 20.0f),
            m_zoom(zoom, 20.0f) {
    m_boundBody = {};
}
void Camera::bindToBody(b2BodyId boundBody) {
    m_boundBody = boundBody;
}
void Camera::unbindFromBody() {
    m_boundBody = {};
}
void Camera::update(float dt) {
    m_center.update(dt);
    m_zoom.update(dt);
    if (B2_IS_NULL(m_boundBody))
        return;
    m_center.setInstantly(b2Body_GetPosition(m_boundBody));
}

void Camera::zoom(float mult) {
    float newZoom = m_zoom.getActual() * mult;
    newZoom = std::max(newZoom, conf::draw::minScale);
    newZoom = std::min(newZoom, conf::draw::maxScale);
    m_zoom = newZoom;
}
void Camera::zoomNoSmoothing(float mult) {
    float newZoom = m_zoom.getActual() * mult;
    newZoom = std::max(newZoom, conf::draw::minScale);
    newZoom = std::min(newZoom, conf::draw::maxScale);
    m_zoom.setInstantly(newZoom);
}
void Camera::zoomInAt(float mult, sf::Vector2i mousePos) {
    b2Vec2 currentWorldPos = screenPosToWorld(mousePos);
    zoomNoSmoothing(mult);
    b2Vec2 newWorldPos = screenPosToWorld(mousePos);
    b2Vec2 toMove = currentWorldPos - newWorldPos;
    zoomNoSmoothing(1.0f / mult);

    zoom(mult);
    move(toMove);
}
void Camera::moveTo(b2Vec2 pos) {
    m_center = pos;
}
void Camera::moveTo(sf::Vector2i pos) {
    moveTo(screenPosToWorld(pos));
}
void Camera::moveTo(sf::Vector2f pos) {
    moveTo(screenPosToWorld(pos));
}
void Camera::move(b2Vec2 dpos) {
    m_center = m_center.getActual() + dpos;
}
void Camera::move(sf::Vector2i dpos) {
    move(screenSizeToWorld(dpos));
}
void Camera::move(sf::Vector2f dpos) {
    move(screenSizeToWorld(dpos));
}
void Camera::moveNoSmoothing(b2Vec2 dpos) {
    m_center.setInstantly(m_center.getActual() + dpos);
}

sf::Vector2i Camera::worldPosToScreen(b2Vec2 worldPos) {
    sf::Vector2i retVal = {};
    float zoom = m_zoom.get();
    b2Vec2 center = m_center.get();
    retVal.x = (worldPos.x - center.x) * zoom + conf::window::WIDTH / 2;
    retVal.y = -(worldPos.y - center.y) * zoom + conf::window::HEIGHT / 2;
    return retVal;
}
b2Vec2 Camera::screenPosToWorld(sf::Vector2i screenPos) {
    b2Vec2 retVal = {};
    float zoom = m_zoom.get();
    b2Vec2 center = m_center.get();
    retVal.x = (screenPos.x - conf::window::WIDTH / 2.0f) / zoom + center.x;
    retVal.y = -(screenPos.y - conf::window::HEIGHT / 2.0f) / zoom + center.y;
    return retVal;
}
b2Vec2 Camera::screenPosToWorld(sf::Vector2f screenPos) {
    b2Vec2 retVal = {};
    float zoom = m_zoom.get();
    b2Vec2 center = m_center.get();
    retVal.x = (screenPos.x - conf::window::WIDTH / 2.0f) / zoom + center.x;
    retVal.y = -(screenPos.y - conf::window::HEIGHT / 2.0f) / zoom + center.y;
    return retVal;
}
float Camera::worldSizeToScreen(float worldSize) {
    return worldSize * m_zoom.get();
}
float Camera::screenSizeToWorld(float screenSize) {
    return screenSize / m_zoom.get();
}
sf::Vector2i Camera::worldSizeToScreen(b2Vec2 worldSize) {
    return sf::Vector2i(worldSizeToScreen(worldSize.x), worldSizeToScreen(worldSize.y));
}
b2Vec2 Camera::screenSizeToWorld(sf::Vector2i screenSize) {
    return {screenSizeToWorld(screenSize.x), screenSizeToWorld(screenSize.y)};
}
b2Vec2 Camera::screenSizeToWorld(sf::Vector2f screenSize) {
    return {screenSizeToWorld(screenSize.x), screenSizeToWorld(screenSize.y)};
}