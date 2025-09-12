#pragma once

template <typename T>
class TimingVariable {
public:
    TimingVariable(T startingVar, float smoothing) {
        m_smoothing = smoothing;
        m_var = startingVar;
        m_smoothedVar = startingVar;
    }
    TimingVariable operator=(T other) {
        m_var = other;
        return *this;
    }
    void setInstantly(T var) {
        m_var = var;
        m_smoothedVar = var;
    }
    void setSmoothing(float smoothing) {
        m_smoothing = smoothing;
    }
    T get() {
        return m_smoothedVar;
    }
    T getActual() {
        return m_var;
    }
    void update(float dt) {
        m_smoothedVar += (m_var - m_smoothedVar) * m_smoothing * dt;
    }
private:
    T m_var, m_smoothedVar;
    float m_smoothing;
};