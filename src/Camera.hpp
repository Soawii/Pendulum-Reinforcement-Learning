#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

namespace camera {
	void zoom(float mult);
	void zoomInAt(float mult, sf::Vector2i mousePos);
    
	void moveTo(b2Vec2 pos);
	void moveTo(sf::Vector2i pos);
	void moveTo(sf::Vector2f pos);
	void move(b2Vec2 dpos);
	void move(sf::Vector2i dpos);
	void move(sf::Vector2f dpos);
};