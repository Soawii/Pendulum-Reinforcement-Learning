#include <box2d/box2d.h>

class Camera {
public:
	Camera(b2Vec2 center, float scale) {
		m_center = center;
		m_scale = scale;
	}

	void zoom(float mult) {
		m_scale *= mult;
	}
	
	void zoomIn(float mult, b2Vec2 mousePos) {
		
	}

	void moveTo(b2Vec pos) {
		m_center = pos;
	}

	b2Vec2 m_center;
	float m_scale;
};