#pragma once
#include <functional>
#include <chrono>
#include <SFML/Graphics.hpp>

namespace TimingFunctions {
    float linear(float t);
    float smoothstep(float t);
}

sf::Color operator*(const sf::Color& left, float right);

template <typename T>
class TimingVariable {
public:
    TimingVariable(T startingVar, float duration, const std::function<float(float)>& timingFunction) 
    : m_timingFunction(timingFunction), m_start(startingVar), m_end(startingVar) {
        m_duration = duration;
        m_timepoint = std::chrono::steady_clock::now() - std::chrono::seconds(100000);
    }

    TimingVariable operator=(T other) {
        if (m_duration == 0.0f) {
            m_end = other;
            return *this;
        }
        if (m_end == other) {
            return *this;
        }
        float timeLeft = 0.0f;
        if (m_start != m_end) {
            std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
            std::chrono::duration<float> dur(now - m_timepoint);
            float secondsPassed = dur.count();
            timeLeft = m_duration - secondsPassed;
            m_start = m_end;
        }
        m_end = other;
        m_timepoint = std::chrono::steady_clock::now() - std::chrono::milliseconds(int(timeLeft * 1000.0f));
        return *this;
    }
    void setInstantly(T var) {
        m_start = var;
        m_end = var;
    }
    T get() {
        if (m_duration == 0.0f)
            return m_end;
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        std::chrono::duration<float> dur(now - m_timepoint);
        float secondsPassed = dur.count();

        if (secondsPassed > m_duration) {
            if (m_start != m_end)
                m_start = m_end;
            return m_end;
        }
        return m_start + (m_end - m_start) * m_timingFunction(secondsPassed / m_duration);
    }
    T getActual() {
        return m_end;
    }

    void setDuration(float t) {
        m_duration = t;
    }
    void setTimingFunction( std::function<float(float)> func) {
        m_timingFunction = func;
    }
    void setParameters(float t, std::function<float(float)> func) {
        setDuration(t);
        setTimingFunction(func);
    }
private:
    T m_start, m_end;
    std::function<float(float)> m_timingFunction;
    std::chrono::steady_clock::time_point m_timepoint;
    float m_duration;
};