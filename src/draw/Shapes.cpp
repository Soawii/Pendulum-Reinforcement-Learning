#include "Shapes.hpp"
#include "../conf.hpp"
#include "../util/util.hpp"

namespace shapes {
    Line::Line(sf::Vector2f start, sf::Vector2f end, sf::Color color) 
    : m_arr(sf::Lines, 2) {
        m_arr[0].position = start;
        m_arr[1].position = end;
        m_arr[0].color = color;
        m_arr[1].color = color;
    }
    void Line::draw(sf::RenderWindow& window) {
        window.draw(m_arr);
    }

    Circle::Circle(sf::Vector2f center, float radius, int points, sf::Color color)
    : m_arr(sf::TriangleFan, points) {
        float angle = 0;
        float d_angle = 2 * conf::constants::PI / points;
        for (int i = 0; i < points; i++) {
            m_arr[i].position = {center.x + radius * cos(angle), center.y + radius * sin(angle)};
            angle += d_angle;
        }
        for (int i = 0; i < points; i++) {
            m_arr[i].color = color;
        }
    }
    void Circle::draw(sf::RenderWindow& window) {
        window.draw(m_arr);
    }

    HollowCircle::HollowCircle(
        sf::Vector2f center, float outerRadius, 
        float strokeWidth, int points, sf::Color color) 
    : m_innerCircle(sf::Points, points), m_outerCircle(sf::Points, points), m_arr(sf::TriangleStrip, points * 2 + 2) {
        float innerRadius = outerRadius - strokeWidth;

        float angle = 0;
        float d_angle = 2 * conf::constants::PI / points;
        for (int i = 0; i < points; i++) {
            m_innerCircle[i].position = {center.x + innerRadius * cos(angle), center.y + innerRadius * sin(angle)};
            m_outerCircle[i].position = {center.x + outerRadius * cos(angle), center.y + outerRadius * sin(angle)};
            m_innerCircle[i].color = color;
            m_outerCircle[i].color = color;
            angle += d_angle;
        }

        for (int i = 0; i < points; i++) {
            m_arr[2 * i] = m_innerCircle[i];
            m_arr[2 * i + 1] = m_outerCircle[i];
        }
        m_arr[points * 2] = m_arr[0];
        m_arr[points * 2 + 1] = m_arr[1];
    }
    void HollowCircle::draw(sf::RenderWindow& window) {
        window.draw(m_arr);
    }

    Rect::Rect(sf::Vector2f topLeft, float width, float height, sf::Color color)
    : m_arr(sf::TriangleFan, 4) {
        m_arr[0].position = topLeft;
        m_arr[1].position = {topLeft.x + width, topLeft.y};
        m_arr[2].position = {topLeft.x + width, topLeft.y + height};
        m_arr[3].position = {topLeft.x, topLeft.y + height};

        m_arr[0].color = color;
        m_arr[1].color = color;
        m_arr[2].color = color;
        m_arr[3].color = color;
    }
    Rect::Rect(sf::Vector2f start, sf::Vector2f end, float width, sf::Color color)
    : m_arr(sf::TriangleFan, 4) {
        sf::Vector2f startToEnd = end - start;
        sf::Vector2f normal{-startToEnd.y, startToEnd.x};
        normal = normal / util::getLen(normal) * (width / 2);

        m_arr[0].position = start + normal;
        m_arr[1].position = start - normal;
        m_arr[2].position = end - normal;
        m_arr[3].position = end + normal;

        m_arr[0].color = color;
        m_arr[1].color = color;
        m_arr[2].color = color;
        m_arr[3].color = color;
    }
    void Rect::draw(sf::RenderWindow& window) {
        window.draw(m_arr);
    }

    RoundedRect::RoundedRect(
        sf::Vector2f topLeft, float width, float height, float radius, 
        int points_quarterCircle, sf::Color color) 
    : m_arr(sf::TriangleFan, points_quarterCircle * 4) {
        sf::Vector2f centerTopLeft = {topLeft.x + radius, topLeft.y + radius};
        sf::Vector2f centerTopRight = {topLeft.x + width - radius, topLeft.y + radius};
        sf::Vector2f centerBottomRight = {topLeft.x + width - radius, topLeft.y + height - radius};
        sf::Vector2f centerBottomLeft = {topLeft.x + radius, topLeft.y + height - radius};

        int idx = 0;
        float angle = conf::constants::PI;
        float d_angle = -(conf::constants::PI / 2) / (points_quarterCircle - 1);
        for (int i = 0; i < points_quarterCircle; i++) {
            m_arr[idx++].position = {centerTopLeft.x + cos(angle) * radius, centerTopLeft.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = conf::constants::PI / 2;
        for (int i = 0; i < points_quarterCircle; i++) {
            m_arr[idx++].position = {centerTopRight.x + cos(angle) * radius, centerTopRight.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = 0.0f;
        for (int i = 0; i < points_quarterCircle; i++) {
            m_arr[idx++].position = {centerBottomRight.x + cos(angle) * radius, centerBottomRight.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = conf::constants::PI * 3 / 2;
        for (int i = 0; i < points_quarterCircle; i++) {
            m_arr[idx++].position = {centerBottomLeft.x + cos(angle) * radius, centerBottomLeft.y - sin(angle) * radius};
            angle += d_angle;
        }
        for (int i = 0; i < idx; i++) {
            m_arr[i].color = color;
        }
    }
    void RoundedRect::draw(sf::RenderWindow& window) {
        window.draw(m_arr);
    }

    RoundedOutlinedRect::RoundedOutlinedRect(
        sf::Vector2f topLeft, float width, float height, float radius, 
        sf::Color color, float outlineWidth, sf::Color outlineColor, float outlinePadding) 
    {
        const int points_quarterCircle = 15;
        
        m_outerOutline = RoundedRect(
            topLeft, width, height, radius, 
            points_quarterCircle, outlineColor).m_arr;

        m_innerOutline = RoundedRect(
            sf::Vector2f(topLeft.x + outlineWidth, topLeft.y + outlineWidth), 
            std::max(0.0f, width - outlineWidth * 2), 
            std::max(0.0f, height - outlineWidth * 2), 
            std::max(0.0f, radius - outlineWidth), 
            points_quarterCircle,
            outlineColor).m_arr;

        m_innerRect = RoundedRect(
            sf::Vector2f(topLeft.x + outlineWidth + outlinePadding, topLeft.y + outlineWidth + outlinePadding), 
            std::max(0.0f, width - outlineWidth * 2 - outlinePadding * 2), 
            std::max(0.0f, height - outlineWidth * 2 - outlinePadding * 2), 
            std::max(0.0f, radius - outlineWidth - outlinePadding),  
            points_quarterCircle,
            color).m_arr;
            
        m_innerRect.setPrimitiveType(sf::TriangleFan);
        
        m_outline.setPrimitiveType(sf::TriangleStrip);
        m_outline.resize(points_quarterCircle * 4 * 2 + 2);

        for (int i = 0; i < points_quarterCircle * 4; i++) {
            m_outline[2 * i].position = m_innerOutline[i].position;
            m_outline[2 * i + 1].position = m_outerOutline[i].position;
            m_outline[2 * i].color = m_innerOutline[i].color;
            m_outline[2 * i + 1].color = m_outerOutline[i].color;
        }
        m_outline[points_quarterCircle * 4 * 2] = m_outline[0];
        m_outline[points_quarterCircle * 4 * 2 + 1] = m_outline[1];
    }
    void RoundedOutlinedRect::draw(sf::RenderWindow& window) {
        window.draw(m_innerRect);
        window.draw(m_outline);
    }
}