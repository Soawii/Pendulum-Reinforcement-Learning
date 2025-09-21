#include "StateVariable.hpp"
#include <iostream>

StateVariables::StateVariables(UIElementContext* defaultContext)
:   relativePos(defaultContext, {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),
    absolutePos(defaultContext, {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),
    relativeSize(defaultContext, {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),
    absoluteSize(defaultContext, {0.0f, 0.0f}, 0.0f, TimingFunctions::smoothstep),     
    borderRadius(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    padding(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    borderWidth(defaultContext, 0.0f, 0.0f, TimingFunctions::smoothstep),
    color(defaultContext, ColorHSL(255, 0, 0), 0.0f, TimingFunctions::smoothstep),
    borderColor(defaultContext, ColorHSL(255, 255, 255), 0.0f, TimingFunctions::smoothstep) {

}

bool StateVariables::checkChangedStates() {
    bool flag = false;
    if (relativePos.checkChangedState())
        flag = true;
    if (absolutePos.checkChangedState())
        flag = true;
    if (relativeSize.checkChangedState())
        flag = true;
    if (absoluteSize.checkChangedState())
        flag = true; 
    if (borderRadius.checkChangedState())
        flag = true;
    if (padding.checkChangedState())
        flag = true;
    if (borderWidth.checkChangedState())
        flag = true;
    if (color.checkChangedState())
        flag = true;
    if (borderColor.checkChangedState())
        flag = true;
    if (flag) {
        int a = 1;
    }
    return flag;
}