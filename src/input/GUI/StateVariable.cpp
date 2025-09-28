#include "StateVariable.hpp"
#include <iostream>

ContextStateVariableRelAbs::ContextStateVariableRelAbs(
        UIElementContext* context,
        sf::Vector2f absVar, float absDuration, std::function<float(float)> absTimingFunc,
        sf::Vector2f relVar, float relDuration, std::function<float(float)> relTimingFunc) 
    : absolute(context, absVar, absDuration, absTimingFunc),
    relative(context, relVar, relDuration, relTimingFunc) {
}

bool ContextStateVariableRelAbs::checkChangedState() {
    bool flag = false;
    if (absolute.checkChangedState())
        flag = true;
    if (relative.checkChangedState())
        flag = true;
    return flag;
}

StateVariables::StateVariables(UIElementContext* defaultContext)
:   pos(defaultContext, 
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep,
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),     
    size(defaultContext, 
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep,
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),  
    origin(defaultContext, 
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep,
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),  
    transformOrigin(defaultContext, 
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep,
        {0.5f, 0.5f}, 0.0f, TimingFunctions::smoothstep),  

    translate(defaultContext, 
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep,
        {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),  
    rotate(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    scale(defaultContext, 1.0f, 0.0f, TimingFunctions::smoothstep),
    opacity(defaultContext, 1.0f, 0.0f, TimingFunctions::smoothstep),

    borderRadius(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    padding(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    borderWidth(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),

    color(defaultContext, ColorHSL(255, 0, 0), 0.0f, TimingFunctions::smoothstep),
    borderColor(defaultContext, ColorHSL(255, 255, 255), 0.0f, TimingFunctions::smoothstep) {

}

bool StateVariables::checkChangedStates() {
    bool flag = false;
    if (pos.checkChangedState()) {
        flag = true;
    }
    if (size.checkChangedState()) {
        flag = true;
    }
    if (origin.checkChangedState()) {
        flag = true;
    }
    if (transformOrigin.checkChangedState()) {
        flag = true;
    }
    if (opacity.checkChangedState()) {
        flag = true;
    }

    if (translate.checkChangedState()) {
        flag = true;
    }
    if (rotate.checkChangedState()) {
        flag = true;
    }
    if (scale.checkChangedState()) {
        flag = true;
    }

    if (borderRadius.checkChangedState()) {
        flag = true;
    }
    if (borderWidth.checkChangedState()) {
        flag = true;
    }
    if (padding.checkChangedState()) {
        flag = true;
    }

    if (color.checkChangedState()) {
        flag = true;
    }
    if (borderColor.checkChangedState()) {
        flag = true;
    }
    return flag;
}