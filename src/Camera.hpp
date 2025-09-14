#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "TimingVariable.hpp"

class Camera {
public:
	Camera(b2Vec2 center, float zoom, float centerSmoothing, float zoomSmoothing);

	void bindToBody(b2BodyId boundBody);
	void unbindFromBody();
	void update(float dt);

	void zoom(float mult);
	void zoomNoSmoothing(float mult);
	void zoomInAt(float mult, sf::Vector2i mousePos);
	void moveTo(b2Vec2 pos);
	void moveTo(sf::Vector2i pos);
	void moveTo(sf::Vector2f pos);
	void move(b2Vec2 dpos);
	void move(sf::Vector2i dpos);
	void move(sf::Vector2f dpos);
	void moveNoSmoothing(b2Vec2 dpos);

	sf::Vector2i worldPosToScreen(b2Vec2 worldPos);
    b2Vec2 screenPosToWorld(sf::Vector2i screenPos);
    b2Vec2 screenPosToWorld(sf::Vector2f screenPos);
    float worldSizeToScreen(float worldSize);
    float screenSizeToWorld(float screenSize);
    sf::Vector2i worldSizeToScreen(b2Vec2 worldSize);
    b2Vec2 screenSizeToWorld(sf::Vector2i screenSize);
    b2Vec2 screenSizeToWorld(sf::Vector2f screenSize);

	TimingVariable<b2Vec2> m_center;
	TimingVariable<float> m_zoom;
	b2BodyId m_boundBody;
	bool m_isFollowingBody;
};