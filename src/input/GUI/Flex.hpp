#include "UIElement.hpp"

enum class FlexDirection {
    horizontal = 0,
    vertical = 1
};

enum class JustifyContent {
    start = 0,
    end = 1,
    space_between = 2,
    space_around = 3,
    center = 4
};

enum class AlignItems {
    start = 0,
    end = 1,
    center = 2
};

class Flex : public UIElement {
public:
    Flex();

    virtual void computeSize() override {
        float borderWidth = m_vars.borderWidth.get();
        float padding = m_vars.padding.get();

        if (m_context->sizeMode == UISizeMode::FIXED) {
            const sf::FloatRect bounds = getGlobalBounds();

            m_context->mouseBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
            m_context->mouseBounds.m_radius = m_vars.borderRadius.get();
            m_context->childrenBounds = Bounds(
                { bounds.left + borderWidth + padding,
                bounds.top + borderWidth + padding }, 
                bounds.width - borderWidth * 2 - padding * 2,
                bounds.height - borderWidth * 2 - padding * 2
            );
            m_context->anchorBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
        }

        for (int i = 0; i < m_children.size(); i++) {
            m_children[i]->computeSize();
        }

        if (m_context->sizeMode == UISizeMode::FIT_CONTENT) {
            sf::Vector2f size = m_vars.size.absolute.get();
            sf::Vector2f newSize = {0.0f, 0.0f};

            if (m_children.size() == 0) {
                newSize = {borderWidth * 2 + padding * 2, borderWidth * 2 + padding * 2};
            }
            else {
                float x_min = 10000000.0f, y_min = 10000000.0f;
                float x_max = -10000000.0f, y_max = -10000000.0f;

                for (int i = 0; i < m_children.size(); i++) {
                    UIElement* c = m_children[i];

                    sf::FloatRect bounds = c->getGlobalBounds();
                    x_min = std::min(x_min, bounds.left);
                    y_min = std::min(y_min, bounds.top);
                    x_max = std::max(x_max, bounds.left + bounds.width);
                    y_max = std::max(y_max, bounds.top + bounds.height);
                }

                newSize = {(x_max - x_min) + borderWidth * 2 + padding * 2, (y_max - y_min) + borderWidth * 2 + padding * 2};
            }
            if (size != newSize)
                m_vars.size.absolute.setAllSmoothly(newSize);

            const sf::FloatRect bounds = getGlobalBounds();
            m_context->mouseBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
            m_context->mouseBounds.m_radius = m_vars.borderRadius.get();

            m_context->childrenBounds = Bounds(
                { bounds.left + borderWidth + padding,
                bounds.top + borderWidth + padding }, 
                bounds.width - borderWidth * 2 - padding * 2,
                bounds.height - borderWidth * 2 - padding * 2
            );
            m_context->anchorBounds = Bounds({bounds.left, bounds.top}, bounds.width, bounds.height);
        }
    }
    virtual void computeTransforms(sf::Transform parentTransform = sf::Transform(), float parentOpacity = 1.0f) {
        const Bounds bounds = m_context->mouseBounds;

        const float scale = m_vars.scale.get();
        const float rotate = m_vars.rotate.get();
        const sf::Vector2f absTranslate = m_vars.translate.absolute.get();
        const sf::Vector2f relTranslate = m_vars.translate.relative.get();
        const sf::Vector2f absTransformOrigin = m_vars.transformOrigin.absolute.get();
        const sf::Vector2f relTransformOrigin = m_vars.transformOrigin.relative.get();

        const sf::Vector2f translate = {
            absTranslate.x + relTranslate.x * bounds.m_width,
            absTranslate.y + relTranslate.y * bounds.m_height
        };
        const sf::Vector2f transformOrigin = {
            bounds.m_pos.x + absTransformOrigin.x + relTransformOrigin.x * bounds.m_width,
            bounds.m_pos.y + absTransformOrigin.y + relTransformOrigin.y * bounds.m_height
        };

        sf::Transform t;
        t.scale(sf::Vector2f(scale, scale), transformOrigin);
        t.rotate(rotate, transformOrigin);
        t.translate(translate);

        parentTransform *= t;
        m_context->calculatedTransform = parentTransform;
        m_context->mouseBounds.m_transform = parentTransform;
        m_context->childrenBounds.m_transform = parentTransform;

        float opacity = m_vars.opacity.get();
        parentOpacity *= opacity;
        m_context->calculatedOpacity = parentOpacity;
        m_context->opacityShader.setUniform("opacity", parentOpacity);

        for (UIElement* child : m_children) {
            child->computeTransforms(parentTransform, parentOpacity);
        }
    }

    virtual void draw(WindowHandler* window);
    virtual void update(MouseContext& mouseContext, KeyboardContext& keyboardContext);

    FlexDirection m_direction;
    float gap;
};