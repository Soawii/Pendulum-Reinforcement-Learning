#pragma once
#include "WindowHandler.hpp"
#include "DoublePendulum.hpp"

class EngineContext {
public:
    WindowHandler* m_windowHandler;
    DoublePendulum* m_pendulum;
};