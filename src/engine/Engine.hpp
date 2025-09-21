#pragma once
#include "EngineContext.hpp"
#include "../draw/WindowHandler.hpp"
#include "../sim/DoublePendulum.hpp"
#include "../input/MouseHandler.hpp"
#include "../input/KeyboardHandler.hpp"
#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>
#include <vector>
#include "../input/GUI/UIManager.hpp"

class Engine {
public:
    Engine();

    void createWindowHandler(
        sf::VideoMode mode, const sf::String &title, 
        b2Vec2 cameraCenter, float cameraZoom,
        float cameraCenterSmoothing, float cameraZoomSmoothing,
        sf::Uint32 style = 7U, const sf::ContextSettings &settings = sf::ContextSettings());
    void createPendulum(
        b2Vec2 gravity, float jointLength, float weightMass, 
        float weightRadius, int weightAmount, 
        float maxBaseVelocity, float baseAcceleration,
        std::vector<float> angles);
    void createUIManager();
    
    void startFrame();
    void endFrame();
    void handleEvent(sf::Event& e);
    void update(float dt);

    sf::RenderWindow* getWindow();

    EngineContext m_context;

    UIManager* m_UIManager;
    MouseHandler m_mouseHandler;
    KeyboardHandler m_keyboardHandler;
};