#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace conf {
    namespace constants {
        inline const float PI = 3.14159265359;
    };

    namespace window {
        inline const int WIDTH = 1600;
        inline const int HEIGHT = 900;
        inline const int FPS = 60;
        inline const float dt = 1.0f / FPS;
    };

    namespace sim {
        inline const uint8_t weight_amount = 2; // should be <= 10

        inline const float gravity = -9.81f;
        inline const int substepCount = 8;
        inline const float jointLength = 1.0f;
        inline const float weightMass = 1.0f;
        inline const float weightRadius = 0.15f;
        inline const float angleA = constants::PI / 2;
        inline const float angleB = constants::PI / 2 + 0.05;
        inline const float rangeLeft = -2.0f;
        inline const float rangeRight = 2.0f;

        inline const float baseSize = 0.1f;
        inline const float baseRadius = 0.02f;
        inline const float sliderOutlineWidth = 0.014f;
        inline const float sliderOutlinePadding = 0.026f;

        inline const float baseGoalVelocity = 3.0f;
        inline const float baseMaxAcceleration = 3.0f;
    };

    namespace draw {
        inline const float scale = 150.0f;
        inline const float jointWidth = 1.0f;
        inline const float jointConnectionWidth = 4.0f;
        inline const float weightStrokeWidth = 3.0f;
    };
};