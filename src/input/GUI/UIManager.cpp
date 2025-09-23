#include "UIManager.hpp"
#include "WindowElement.hpp"
#include "Button.hpp"
#include "TextElement.hpp"
#include "../../engine/EngineContext.hpp"
#include <iostream>

UIManager::UIManager(EngineContext context) : m_engineContext(context) {
    m_windowElement = new WindowElement(m_engineContext.m_windowHandler);
    addElement(m_windowElement, nullptr, nullptr, "window", {});
    
    float transitionTime = 0.075f;

    auto class_textbutton = [transitionTime](UIElement* e) {
        e->m_vars.borderRadius.setAll(20.0f);
        e->m_vars.borderWidth.setAll(3.0f);

        ColorHSL baseColor = ColorHSL(245, 245, 245);
        ColorHSL ligherBaseColor = ColorHSL(baseColor.h, baseColor.s, baseColor.l - 0.2f, baseColor.a);
        ColorHSL borderColor = ColorHSL(130, 130, 255);
        ColorHSL ligherBorderColor = ColorHSL(borderColor.h, borderColor.s, borderColor.l - 0.2f, borderColor.a);

        e->m_vars.color.setAll(baseColor);
        e->m_vars.color.setDuration(transitionTime);
        e->m_vars.color.setState(UIElementState::HOVER, ligherBaseColor);
        e->m_vars.color.setState(UIElementState::ACTIVE, borderColor);

        e->m_vars.borderColor.setAll(borderColor);
        e->m_vars.borderColor.setDuration(transitionTime);
        e->m_vars.borderColor.setState(UIElementState::ACTIVE, baseColor);
    };

    auto class_textinbutton = [transitionTime](UIElement* e) {
        e->m_vars.color.setAll(ColorHSL(130, 130, 255));
        e->m_vars.color.setDuration(transitionTime);
        e->m_vars.color.setState(UIElementState::ACTIVE, ColorHSL(245, 245, 245));
        e->m_vars.pos.relative.setAll({0.5f, 0.5f});
    };

    registerClass("button", class_textbutton);
    registerClass("text_button", class_textinbutton);

    Button* button_left = new Button();
    addElement(button_left, m_windowElement, m_windowElement, "button1", {"button"});
    button_left->m_context->onPressKeys.push_back(sf::Keyboard::Left);
    button_left->m_vars.pos.absolute.setAll({15.0f, -105.0f});
    button_left->m_vars.pos.relative.setAll({0.0f, 1.0f});
    button_left->m_vars.size.absolute.setAll({90.0f, 90.0f});

    button_left->m_vars.translate.absolute.setDuration(transitionTime);
    button_left->m_vars.translate.absolute.setState(UIElementState::ACTIVE, {0.0f, -15.0f});

    button_left->m_context->whileActive.push_back([this]() {
        this->m_engineContext.m_pendulum->setBaseGoalVelocity(
            this->m_engineContext.m_pendulum->m_baseGoalVelocity - this->m_engineContext.m_pendulum->m_maxBaseVelocity);
    });

    sf::String leftArrowStr;
    leftArrowStr += static_cast<sf::Uint32>(0x2190);
    TextElement* text1 = new TextElement(leftArrowStr, conf::fonts::mono, 36, 1);
    text1->m_vars.origin.relative.setAll({0.5f, 0.5f});
    addElement(text1, button_left, button_left, "button1-text", {"text_button"});
    text1->m_vars.color.setContext(button_left->m_context);

    Button* button_right = new Button();
    addElement(button_right, m_windowElement, button_left, "button2", {"button"});
    button_right->m_context->onPressKeys.push_back(sf::Keyboard::Right);
    button_right->m_vars.pos.absolute.setAll({15.0f, 0.0f});
    button_right->m_vars.pos.relative.setAll({1.0f, 0.0f});
    button_right->m_vars.size.absolute.setAll({90.0f, 90.0f});

    button_right->m_vars.translate.absolute.setDuration(transitionTime);
    button_right->m_vars.translate.absolute.setState(UIElementState::ACTIVE, {0.0f, -15.0f});

    button_right->m_context->whileActive.push_back([this]() {
        this->m_engineContext.m_pendulum->setBaseGoalVelocity(
            this->m_engineContext.m_pendulum->m_baseGoalVelocity + this->m_engineContext.m_pendulum->m_maxBaseVelocity);
    });
    
    sf::String rightArrowStr;
    rightArrowStr += static_cast<sf::Uint32>(0x2192);
    TextElement* text2 = new TextElement(rightArrowStr, conf::fonts::mono, 36, 1);
    text2->m_vars.origin.relative.setAll({0.5f, 0.5f});
    addElement(text2, button_right, button_right, "button2-text", {"text_button"});
    text2->m_vars.color.setContext(button_right->m_context);
}

void UIManager::addElement(
        UIElement* elem, UIElement* parent, UIElement* anchor, 
        std::string elementId, std::vector<std::string> elementClasses) {
    if (m_idToElement.find(elementId) != m_idToElement.end()) {
        std::cerr << "Duplicate ID: " << elementId << std::endl;
        return;
    }

    m_idToElement[elementId] = elem;
    elem->m_anchor = anchor;
    elem->m_parent = parent;
    if (parent)
        parent->m_children.push_back(elem);

    for (int i = 0; i < elementClasses.size(); i++) {
        if (m_classToCallback.find(elementClasses[i]) != m_classToCallback.end()) {
            m_classToCallback[elementClasses[i]](elem);
        }
    }
}

void UIManager::draw() {
    m_windowElement->draw(m_engineContext.m_windowHandler);
}

void UIManager::update(MouseContext& mouseContext, KeyboardContext& keyboardContext) {
    m_windowElement->computeSize();
    m_windowElement->update(mouseContext, keyboardContext);
    m_windowElement->propogateCall([](UIElement* e){
        e->m_vars.checkChangedStates();
    });
}

UIElement* UIManager::getElementById(std::string id) {
    if (m_idToElement.find(id) != m_idToElement.end())
        return m_idToElement[id];
    return nullptr;
}

void UIManager::registerClass(std::string newClass, std::function<void(UIElement*)> callback) {
    m_classToCallback[newClass] = callback;
}

void UIManager::startFrame() {
    m_engineContext.m_pendulum->setBaseGoalVelocity(0.0f);
    m_windowElement->propogateCall([](UIElement* e) {
        e->m_context->startFrame();
    });
}
void UIManager::endFrame() {
    m_windowElement->propogateCall([](UIElement* e) {
        e->m_context->endFrame();
    });
}