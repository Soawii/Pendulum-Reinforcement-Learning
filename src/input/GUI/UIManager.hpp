#pragma once
#include "UIElement.hpp"
#include "../../draw/WindowHandler.hpp"
#include <string>
#include <unordered_map>
#include "../../engine/EngineContext.hpp"
#include "WindowElement.hpp"
#include <vector>

class UIManager {
public:
    UIManager(EngineContext context);

    void addElement(UIElement* elem, UIElement* parent, UIElement* anchor, 
        std::string elementId, std::vector<std::string> elementClasses);
    void draw();
    void update(MouseContext& mouseContext, KeyboardContext& keyboardContext);

    UIElement* getElementById(std::string id);
    void registerClass(std::string newClass, std::function<void(UIElement*)> callback);

    void startFrame();
    void endFrame();

    WindowElement* m_windowElement;
private:
    EngineContext m_engineContext;
    std::unordered_map<std::string, std::function<void(UIElement*)>> m_classToCallback;
    std::unordered_map<std::string, UIElement*> m_idToElement;
};