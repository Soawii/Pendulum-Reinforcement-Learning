#include "Camera.hpp"
#include "conf.hpp"
#include "util.hpp"
#include <iostream>

namespace camera {
	void zoom(float mult) {
        float newScale = conf::draw::scale * mult;
        newScale = std::max(newScale, conf::draw::minScale);
        newScale = std::min(newScale, conf::draw::maxScale);
        conf::draw::scale = newScale;
    }
	void zoomInAt(float mult, sf::Vector2i mousePos) {
        b2Vec2 currentWorldPos = util::screenPosToWorld(mousePos);
        zoom(mult);
        b2Vec2 newWorldPos = util::screenPosToWorld(mousePos);
        b2Vec2 toMove = currentWorldPos - newWorldPos;
        move(toMove);
    }
    
	void moveTo(b2Vec2 pos) {
        conf::draw::center = pos;
    }
    void moveTo(sf::Vector2i pos) {
        moveTo(util::screenPosToWorld(pos));
    }
    void moveTo(sf::Vector2f pos) {
        moveTo(util::screenPosToWorld(pos));
    }

	void move(b2Vec2 dpos) {
        conf::draw::center += dpos;
    }
	void move(sf::Vector2i dpos) {
        move(util::screenSizeToWorld(dpos));
    }
    void move(sf::Vector2f dpos) {
        move(util::screenSizeToWorld(dpos));
    }
};