#pragma once
#include "conf.hpp"

namespace conf {
	float constants::PI = 3.14159265359;

	int window::WIDTH = 1600;
	int window::HEIGHT = 900;
	int window::FPS = 60;
	float window::dt = 1.0f / FPS;

	int sim::weight_amount = 2; // should be <= 10
	float sim::gravity = -9.81f;
	int sim::substepCount = 8;
	float sim::jointLength = 1.0f;
	float sim::weightMass = 1.0f;
	float sim::weightRadius = 0.15f;
	float sim::angleA = constants::PI / 2;
	float sim::angleB = constants::PI / 2 + 0.05;
	float sim::rangeLeft = -2.0f;
	float sim::rangeRight = 2.0f;
	float sim::baseSize = 0.1f;
	float sim::baseRadius = 0.02f;
	float sim::sliderOutlineWidth = 0.014f;
	float sim::sliderOutlinePadding = 0.026f;
	float sim::baseGoalVelocity = 3.0f;
	float sim::baseMaxAcceleration = 3.0f;

	float draw::scale = 150.0f;
	float draw::jointWidth = 1.0f;
	float draw::jointConnectionWidth = 4.0f;
	float draw::weightStrokeWidth = 3.0f;

	sf::Color colors::backgroundColor = sf::Color(30, 30, 30);
	sf::Color colors::jointColor = sf::Color(255, 255, 255);
	sf::Color colors::jointConnectionColor = sf::Color(255, 255, 255);
	sf::Color colors::baseColor = sf::Color(57, 65, 100);
	sf::Color colors::weightColor = sf::Color(118, 86, 29);
	sf::Color colors::weightOutlineColor = sf::Color(255, 172, 28);
	sf::Color colors::sliderColor = sf::Color(0, 0, 0, 0);
	sf::Color colors::sliderOutlineColor = sf::Color(230, 230, 230);
}