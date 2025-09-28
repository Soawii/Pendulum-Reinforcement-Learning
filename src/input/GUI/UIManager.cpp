#include "UIManager.hpp"
#include "WindowElement.hpp"
#include "Button.hpp"
#include "TextElement.hpp"
#include "../../engine/EngineContext.hpp"
#include <iostream>

UIManager::UIManager(EngineContext context) : m_engineContext(context) {
    m_windowElement = new WindowElement(m_engineContext.m_windowHandler);
    addElement(m_windowElement, nullptr, nullptr, "window", {});
    
    float transitionTime = 0.1f;

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

    auto class_buttonmodal = [transitionTime](UIElement* e) {
        e->m_vars.borderRadius.setAll(10.0f);

        e->m_vars.size.relative.setAll({1.0f, 0.0f});
        e->m_vars.size.absolute.setAll({0.0f, 80.0f});

        e->m_vars.origin.relative.setAll({0.0f, 1.0f});
        e->m_vars.pos.relative.setAll({0.0f, 1.0f});
    };

    auto center_text = [transitionTime](UIElement* e) {
        e->m_vars.pos.relative.setAll({0.5f, 0.5f});
        e->m_vars.origin.relative.setAll({0.5f, 0.5f});
    };

    auto class_textinbutton = [transitionTime](UIElement* e) {
        e->m_vars.color.setAll(ColorHSL(130, 130, 255));
        e->m_vars.color.setDuration(transitionTime);
        e->m_vars.color.setState(UIElementState::ACTIVE, ColorHSL(245, 245, 245));
        e->m_vars.pos.relative.setAll({0.5f, 0.5f});
    };
    
    auto class_button_color = [](UIElement* e) {
        ColorHSL baseColor = ColorHSL(200, 200, 200);
        ColorHSL ligherBaseColor = ColorHSL(baseColor.h, baseColor.s, baseColor.l - 0.1f, baseColor.a);
        ColorHSL activeColor = ColorHSL(baseColor.h, baseColor.s, baseColor.l - 0.2f, baseColor.a);

        e->m_vars.color.setAll(baseColor);
        e->m_vars.color.setState(UIElementState::HOVER, ligherBaseColor);
        e->m_vars.color.setState(UIElementState::ACTIVE, activeColor);
    };

    auto class_button_green = [](UIElement* e) {
        ColorHSL baseColor = ColorHSL(9, 121, 105);
        ColorHSL ligherBaseColor = ColorHSL(baseColor.h, baseColor.s, baseColor.l + 0.05f, baseColor.a);
        ColorHSL activeColor = ColorHSL(baseColor.h, baseColor.s, baseColor.l + 0.1f, baseColor.a);
        e->m_vars.color.setAll(baseColor);
        e->m_vars.color.setState(UIElementState::HOVER, ligherBaseColor);
        e->m_vars.color.setState(UIElementState::ACTIVE, activeColor);
    };
    auto class_button_white = [transitionTime](UIElement* e) {
        ColorHSL baseColor = ColorHSL(250,251,252);
        e->m_vars.color.setAll(ColorHSL(250,251,252));
        e->m_vars.color.setState(UIElementState::HOVER, ColorHSL(baseColor.h, baseColor.s, baseColor.l - 0.07f, baseColor.a));
        e->m_vars.color.setState(UIElementState::ACTIVE, ColorHSL(baseColor.h, baseColor.s, baseColor.l - 0.14f, baseColor.a));
        e->m_vars.color.setDuration(transitionTime);
    };
    auto class_text_white = [transitionTime](UIElement* e) {
        ColorHSL baseColor = ColorHSL(255, 255, 255);
        e->m_vars.color.setDuration(transitionTime);
        e->m_vars.color.setAll(baseColor);
    };
    auto class_text_dark = [transitionTime](UIElement* e) {
        ColorHSL baseColor = ColorHSL(36,41,46);
        e->m_vars.color.setDuration(transitionTime);
        e->m_vars.color.setAll(baseColor);
    };
    auto class_button_move = [](UIElement* e) {
        e->m_vars.borderRadius.setAll(10.0f);
        e->m_vars.size.absolute.setAll({80.0f, 80.0f});
    };

    registerClass("button-white", class_button_white);
    registerClass("text-dark", class_text_dark);
    registerClass("button-move", class_button_move);
    registerClass("button-green", class_button_green);
    registerClass("text-white", class_text_white);
    registerClass("button-grey", class_button_color);
    registerClass("button-modal", class_buttonmodal);
    registerClass("button", class_textbutton);
    registerClass("text_center", center_text);
    registerClass("text_button", class_textinbutton);

    Button* button_left = new Button();
    addElement(button_left, m_windowElement, m_windowElement, "button1", 
        {"button-move", "button-white"});
    button_left->m_context->onPressKeys.push_back(sf::Keyboard::Left);
    button_left->m_vars.origin.relative.setAll({0.0f, 1.0f});
    button_left->m_vars.pos.absolute.setAll({15.0f, -15.0f});
    button_left->m_vars.pos.relative.setAll({0.0f, 1.0f});
    button_left->m_vars.translate.absolute.setState(UIElementState::ACTIVE, {0.0f, -15.0f});
    button_left->m_vars.translate.absolute.setDuration(transitionTime);
    button_left->m_context->whileActive.push_back([this]() {
        this->m_engineContext.m_pendulum->setBaseGoalVelocity(
            this->m_engineContext.m_pendulum->m_baseGoalVelocity - this->m_engineContext.m_pendulum->m_maxBaseVelocity);
    });
    sf::String leftArrowStr;
    leftArrowStr += static_cast<sf::Uint32>(0x2190);
    TextElement* text1 = new TextElement(leftArrowStr, conf::fonts::mono, 36, 1);
    addElement(text1, button_left, button_left, "button1-text", {"text_center", "text-dark"});
    text1->m_vars.color.setContext(button_left->m_context);

    Button* button_right = new Button();
    addElement(button_right, m_windowElement, button_left, "button2", 
        {"button-move", "button-white"});
    button_right->m_context->onPressKeys.push_back(sf::Keyboard::Right);
    button_right->m_vars.pos.absolute.setAll({15.0f, 0.0f});
    button_right->m_vars.pos.relative.setAll({1.0f, 0.0f});
    button_right->m_vars.translate.absolute.setState(UIElementState::ACTIVE, {0.0f, -15.0f});
    button_right->m_vars.translate.absolute.setDuration(transitionTime);
    button_right->m_context->whileActive.push_back([this]() {
        this->m_engineContext.m_pendulum->setBaseGoalVelocity(
            this->m_engineContext.m_pendulum->m_baseGoalVelocity + this->m_engineContext.m_pendulum->m_maxBaseVelocity);
    });
    sf::String rightArrowStr;
    rightArrowStr += static_cast<sf::Uint32>(0x2192);
    TextElement* text2 = new TextElement(rightArrowStr, conf::fonts::mono, 36, 1);
    addElement(text2, button_right, button_right, "button2-text", {"text_center", "text-dark"});
    text2->m_vars.color.setContext(button_right->m_context);

    const float modal_time = 0.4f;
    auto class_modal = [modal_time](UIElement* e) {
        e->m_vars.borderRadius.setAll(20.0f);
        e->m_vars.borderWidth.setAll(5.0f);
        e->m_vars.padding.setAll(10.0f);

        ColorHSL baseColor = ColorHSL(255, 255, 255);
        ColorHSL borderColor = ColorHSL(230, 230, 230);

        e->m_vars.color.setAll(baseColor);
        e->m_vars.borderColor.setAll(borderColor);

        e->m_vars.origin.relative.setAll({0.5f, 0.5f});
        e->m_vars.pos.relative.setAll({0.5, 0.5f});
        e->m_vars.size.absolute.setAll({400, 500});

        e->m_vars.scale.setAll(1.0f);
        e->m_vars.scale.setState(UIElementState::DISABLED, 0.1f);

        e->m_vars.rotate.setAll(0.0f);
        e->m_vars.rotate.setState(UIElementState::DISABLED, -270.0f);

        e->m_vars.opacity.setAll(1.0f);
        e->m_vars.opacity.setState(UIElementState::DISABLED, 0.0f);

        e->setState(UIElementState::DISABLED);

        e->m_vars.rotate.setDuration(modal_time);
        e->m_vars.opacity.setDuration(modal_time);
        e->m_vars.scale.setDuration(modal_time);
    };  
    registerClass("class-modal", class_modal);

    UIElement* modal = new UIElement();
    addElement(modal, m_windowElement, m_windowElement, "modal", {"class-modal"});

    sf::String text_modal_info_str("Info");
    TextElement* text_modal_info = new TextElement(text_modal_info_str, conf::fonts::mono, 32, 1);
    text_modal_info->m_vars.color.setAll(ColorHSL(0,0,0,255));
    text_modal_info->m_vars.origin.relative.setAll({0.5f, 0.0f});
    text_modal_info->m_vars.pos.relative.setAll({0.5f, 0.0f});
    text_modal_info->m_vars.pos.absolute.setAll({0.0f, 10.0f});
    addElement(text_modal_info, modal, modal, "text_modal_info", {});

    sf::String text_modal_stat_str = 
        sf::String("W|A|S|D - move camera\n")
        + leftArrowStr + sf::String("|") + rightArrowStr + sf::String("     - move base\n")
        + sf::String("Ctrl+   - zoom in\nCtrl-   - zoom out\n1|2|3   - focus camera\nSPACE   - stop weights");
    TextElement* text_modal_stat = new TextElement(text_modal_stat_str, conf::fonts::mono, 24, 1);
    text_modal_stat->m_vars.color.setAll(ColorHSL(0,0,0,255));
    text_modal_stat->m_vars.pos.absolute.setAll({0.0f, 60.0f});
    addElement(text_modal_stat, modal, modal, "text_modal_stat", {});

    Button* modal_button = new Button();
    addElement(modal_button, modal, modal, "modal_button", {"button-modal", "button-green"});
    modal_button->m_context->onClick.push_back([modal]() {
        if (modal->m_context->m_current.state != UIElementState::DISABLED) {
            modal->setState(UIElementState::DISABLED);
        }
    });
    sf::String text_modal_button_str("okay...");
    TextElement* text_modal_button = new TextElement(text_modal_button_str, conf::fonts::mono, 24, 1);
    addElement(text_modal_button, modal_button, modal_button, "text_modal_button", {"text_center", "text-white"});

    Button* button_open = new Button();
    addElement(button_open, m_windowElement, m_windowElement, "modal_open", {"button-green"});
    button_open->m_vars.origin.relative.setAll({1.0f, 1.0f});
    button_open->m_vars.pos.relative.setAll({1.0f, 1.0f});
    button_open->m_vars.pos.absolute.setAll({-15.0f, -15.0f});
    button_open->m_vars.size.absolute.setAll({150.0f, 80.0f});
    button_open->m_vars.borderRadius.setAll(10.0f);
    button_open->m_context->onClick.push_back([modal]() {
        if (modal->m_context->m_current.state == UIElementState::DISABLED) {
            modal->setState(UIElementState::NORMAL);
        }
        else {
            modal->setState(UIElementState::DISABLED);
        }
    });
    sf::String text_open("info");
    TextElement* text_modal_open = new TextElement(text_open, conf::fonts::mono, 22, 1);
    text_modal_open->m_vars.color.setAll(ColorHSL(0,0,0,255));
    addElement(text_modal_open, button_open, button_open, "text_modal_button_1", {"text_center", "text-white"});
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
    elem->m_context->id = elementId;

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
    m_windowElement->computeTransforms();
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