#pragma once
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <vector>

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
        static int weightAmount;

        static std::vector<float> angles;
        static b2Vec2 gravity;
        static int substepCount;
        static float jointLength;
        static float weightMass;
        static float weightRadius;
        static float weightStrokeWidth;
        static float rangeLeft;
        static float rangeRight;

        static float baseSize;
        static float baseRadius;
        static float sliderOutlineWidth;
        static float sliderOutlinePadding;
    };

    class draw {
    public:
        static float scale;
        static float minScale;
        static float maxScale;
        static b2Vec2 center;

        static float jointWidth;
        static float jointConnectionWidth;
    };

    class inputs {
    public:
        static float baseMaxVelocity;
        static float baseAcceleration;
        static float zoomMult;
        static float cameraMouseSensitivity;
        static float cameraKeyboardSensitivity;
        static float cameraCenterSmoothing;
        static float cameraZoomSmoothing;
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
}