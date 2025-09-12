#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

namespace conf {
    class constants {
    public:
        static float PI;
    };

    class window {
    public:
        static int WIDTH;
        static int HEIGHT;
        static int FPS;
        static float dt;
    };

    class sim {
    public:
        static int weight_amount;

        static float gravity;
        static int substepCount;
        static float jointLength;
        static float weightMass;
        static float weightRadius;
        static float angleA;
        static float angleB;
        static float rangeLeft;
        static float rangeRight;

        static float baseSize;
        static float baseRadius;
        static float sliderOutlineWidth;
        static float sliderOutlinePadding;

        static float baseGoalVelocity;
        static float baseMaxAcceleration;
    };

    class draw {
    public:
        static float scale;
        static b2Vec2 center;

        static float jointWidth;
        static float jointConnectionWidth;
        static float weightStrokeWidth;
    };

    class colors {
    public:
        static sf::Color backgroundColor,
            baseColor,
            jointColor,
            jointConnectionColor,
            weightColor,
            weightOutlineColor,
            sliderColor,
            sliderOutlineColor;
    };
};