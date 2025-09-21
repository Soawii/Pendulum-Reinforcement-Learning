#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include "../../util/TimingVariable.hpp"
#include <string>
#include <vector>
#include "../../draw/Color.hpp"
#include "UIElementContext.hpp"


template <typename T>
class StateVariable {
public:
    StateVariable(T startingVar, float duration, std::function<float(float)> timingFunc) 
    : m_var(startingVar, duration, timingFunc) {
        setAll(startingVar);
    }

    void setDuration(float duration) {
        m_var.setDuration(duration);
    }

    void setTimingFunction(std::function<float(float)>& func) {
        m_var.setTimingFunction(func);
    }

    void setAll(T var) {
        m_var.setInstantly(var);
        for (int i = 0; i < size_t(UIElementState::STATE_AMOUNT); i++) {
            m_onStateChange[i] = [this, var]() { 
                this->m_var = var;
            };
        }
    }

    void setAllSmoothly(T var) {
        m_var = var;
        for (int i = 0; i < size_t(UIElementState::STATE_AMOUNT); i++) {
            m_onStateChange[i] = [this, var]() { 
                this->m_var = var;
            };
        }
    }

    void setState(UIElementState state, T var) {
        m_onStateChange[size_t(state)] = [this, var]() { 
            this->m_var = var; 
        };
    }

    void stateChanged(UIElementState newState) {
        m_onStateChange[size_t(newState)]();
    }

    T get() {
        return m_var.get();
    }

    TimingVariable<T> m_var;
    std::function<void()> m_onStateChange[size_t(UIElementState::STATE_AMOUNT)];
};

template<typename T>
class ContextStateVariable {
public:
    ContextStateVariable(
        UIElementContext* context, T startingVar,
        float duration, std::function<float(float)> timingFunc)
    : m_context(context), m_var(startingVar, duration, timingFunc) {}

    void setContext(UIElementContext* context) {
        m_context = context;
    }

    bool checkChangedState() {
        if (m_context->wasStateChanged()) {
            stateChanged(m_context->m_current.state);
            return true;
        }
        return false;
    }

    void setDuration(float duration) {
        m_var.setDuration(duration);
    }

    void setTimingFunction(std::function<float(float)>& func) {
        m_var.setTimingFunction(func);
    }

    void setAll(T var) {
        m_var.setAll(var);
    }

    void setAllSmoothly(T var) {
        m_var.setAllSmoothly(var);
    }

    void setState(UIElementState state, T var) {
        m_var.setState(state, var);
    }

    void stateChanged(UIElementState newState) {
        m_var.stateChanged(newState);
    }

    T get() {
        return m_var.get();
    }

    UIElementContext* m_context;
    StateVariable<T> m_var;
};

class StateVariables {
public:
    StateVariables(UIElementContext* defaultContext);

    bool checkChangedStates();

    ContextStateVariable<sf::Vector2f> relativePos;
    ContextStateVariable<sf::Vector2f> absolutePos;
    ContextStateVariable<sf::Vector2f> relativeSize;
    ContextStateVariable<sf::Vector2f> absoluteSize; 

    ContextStateVariable<float> borderRadius;
    ContextStateVariable<float> padding;
    ContextStateVariable<float> borderWidth;

    ContextStateVariable<ColorHSL> color;
    ContextStateVariable<ColorHSL> borderColor;
};