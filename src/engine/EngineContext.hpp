#pragma once
#include "../draw/WindowHandler.hpp"
#include "../sim/DoublePendulum.hpp"

class EngineContext {
public:
    WindowHandler* m_windowHandler;
    DoublePendulum* m_pendulum;
};