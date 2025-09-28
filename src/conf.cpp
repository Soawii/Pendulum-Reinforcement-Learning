#pragma once
#include "conf.hpp"

namespace conf {
	float constants::PI = 3.14159265359f;

	int window::WIDTH = 1600;
	int window::HEIGHT = 900;
	int window::FPS = 120;
	float window::dt = 1.0f / FPS;

	int sim::weightAmount = 2; 
	std::vector<float> sim::angles = { 	// angles for the weights, needs to contain at least the same amount of angles 
		constants::PI / 2 - 0.1f,		// as there weights (indicated by sim::weightAmount)
		constants::PI / 2 - 0.1f};
	b2Vec2 sim::gravity = {0.0f, -9.81f};
	int sim::substepCount = 8;
	float sim::jointLength = 1.0f;
	float sim::weightMass = 1.0f;
	float sim::weightRadius = 0.15f;
	float sim::weightStrokeWidth = 0.01f;
	float sim::rangeLeft = -2.0f;
	float sim::rangeRight = 2.0f;
	float sim::baseSize = 0.1f;
	float sim::baseRadius = 0.02f;
	float sim::sliderOutlineWidth = 0.014f;
	float sim::sliderOutlinePadding = 0.026f;

	float draw::scale = 200.0f;
	float draw::minScale = 10.0f;
	float draw::maxScale = 5000.0f;
	b2Vec2 draw::center = { 0.0f, 0.0f };

	float inputs::baseMaxVelocity = 3.0f;
	float inputs::baseAcceleration = 3.0f;
	float inputs::zoomMult = 1.2f;
	float inputs::cameraMouseSensitivity = 1.0f;
	float inputs::cameraKeyboardSensitivity = 20.0f;
	float inputs::cameraCenterSmoothing = 0.5f;
    float inputs::cameraZoomSmoothing = 0.3f;

	float draw::jointWidth = 1.0f;
	float draw::jointConnectionWidth = 4.0f;

	sf::Color colors::backgroundColor = sf::Color(30, 30, 30);
	sf::Color colors::jointColor = sf::Color(255, 255, 255);
	sf::Color colors::jointConnectionColor = sf::Color(255, 255, 255);
	sf::Color colors::baseColor = sf::Color(57, 65, 100);
	sf::Color colors::weightColor = sf::Color(118, 86, 29);
	sf::Color colors::weightOutlineColor = sf::Color(255, 172, 28);
	sf::Color colors::sliderColor = sf::Color(0, 0, 0, 0);
	sf::Color colors::sliderOutlineColor = sf::Color(230, 230, 230);

	sf::Font fonts::mono;
	sf::Font fonts::arrows;

	std::chrono::steady_clock::time_point time::now;

	void CONFSetup() {
		fonts::mono.loadFromFile("../resources/mono.ttf");
		fonts::arrows.loadFromFile("../resources/arrows.ttf");
		time::now = std::chrono::steady_clock::now();
	}
}