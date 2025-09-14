#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include "conf.hpp"
#include "util.hpp"
#include <iostream>

namespace util {
    float getSquaredLengthBetweenPoints(b2Vec2 pos1, b2Vec2 pos2) {
        b2Vec2 diff = pos2 - pos1;
        return diff.x * diff.x + diff.y * diff.y;
    }

    float getLen(sf::Vector2f vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    sf::VertexArray getRect(sf::Vector2f topLeft, float width, float height, sf::Color color) {
        sf::VertexArray arr(sf::TriangleFan, 4);
        arr[0].position = topLeft;
        arr[1].position = {topLeft.x + width, topLeft.y};
        arr[2].position = {topLeft.x + width, topLeft.y + height};
        arr[3].position = {topLeft.x, topLeft.y + height};

        arr[0].color = color;
        arr[1].color = color;
        arr[2].color = color;
        arr[3].color = color;
        return arr;
    }
    sf::VertexArray getRectFromTo(sf::Vector2f start, sf::Vector2f end, float width, sf::Color color) {
        sf::VertexArray arr(sf::TriangleFan, 4);
        sf::Vector2f startToEnd = end - start;
        sf::Vector2f normal{-startToEnd.y, startToEnd.x};
        normal = normal / getLen(normal) * (width / 2);

        arr[0].position = start + normal;
        arr[1].position = start - normal;
        arr[2].position = end - normal;
        arr[3].position = end + normal;

        arr[0].color = color;
        arr[1].color = color;
        arr[2].color = color;
        arr[3].color = color;

        return arr;
    }

    sf::VertexArray getLine(sf::Vector2f start, sf::Vector2f end, sf::Color color) {
        sf::VertexArray arr(sf::Lines, 2);

        arr[0].position = start;
        arr[1].position = end;

        arr[0].color = color;
        arr[1].color = color;

        return arr;
    }

    sf::VertexArray getHollowCircle(sf::Vector2f center, float outerRadius, float strokeWidth, int points, sf::Color color) {
        sf::VertexArray arr(sf::TriangleStrip, points * 2 + 2);
        sf::VertexArray inner(sf::Points, points + 1);
        sf::VertexArray outer(sf::Points, points + 1);

        float innerRadius = outerRadius - strokeWidth;

        float angle = 0;
        float d_angle = 2 * conf::constants::PI / points;
        for (int i = 0; i < points + 1; i++) {
            inner[i].position = {center.x + innerRadius * cos(angle), center.y + innerRadius * sin(angle)};
            outer[i].position = {center.x + outerRadius * cos(angle), center.y + outerRadius * sin(angle)};
            angle += d_angle;
        }

        for (int i = 0; i < points + 1; i++) {
            arr[i * 2].position = outer[i].position;
            arr[i * 2 + 1].position = inner[i].position;
        }
        for (int i = 0; i < points * 2 + 2; i++) {
            arr[i].color = color;
        }

        return arr;
    }

    void printVec2f(sf::Vector2f v, const char* name) {
        std::cout << name << " " << v.x << " " << v.y << '\n';
    }

    sf::VertexArray getRoundedRectPoints(sf::Vector2f topLeft, float width, float height, float radius, int points_quarterCircle, sf::Color color) {
        sf::VertexArray arr(sf::Points, points_quarterCircle * 4);
        sf::Vector2f centerTopLeft = {topLeft.x + radius, topLeft.y + radius};
        sf::Vector2f centerTopRight = {topLeft.x + width - radius, topLeft.y + radius};
        sf::Vector2f centerBottomRight = {topLeft.x + width - radius, topLeft.y + height - radius};
        sf::Vector2f centerBottomLeft = {topLeft.x + radius, topLeft.y + height - radius};

        // starting from the top left quarter circle
        int idx = 0;
        float angle = conf::constants::PI;
        float d_angle = -(conf::constants::PI / 2) / (points_quarterCircle - 1);
        for (int i = 0; i < points_quarterCircle; i++) {
            arr[idx++].position = {centerTopLeft.x + cos(angle) * radius, centerTopLeft.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = conf::constants::PI / 2;
        for (int i = 0; i < points_quarterCircle; i++) {
            arr[idx++].position = {centerTopRight.x + cos(angle) * radius, centerTopRight.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = 0.0f;
        for (int i = 0; i < points_quarterCircle; i++) {
            arr[idx++].position = {centerBottomRight.x + cos(angle) * radius, centerBottomRight.y - sin(angle) * radius};
            angle += d_angle;
        }
        angle = conf::constants::PI * 3 / 2;
        for (int i = 0; i < points_quarterCircle; i++) {
            arr[idx++].position = {centerBottomLeft.x + cos(angle) * radius, centerBottomLeft.y - sin(angle) * radius};
            angle += d_angle;
        }
        for (int i = 0; i < idx; i++) {
            arr[i].color = color;
        }
        return arr;
    }

    std::pair<sf::VertexArray, sf::VertexArray> getRoundedRect(sf::Vector2f topLeft, float width, float height, float radius, sf::Color color, float outlineWidth, sf::Color outlineColor, float outlinePadding) {
        const int points_quarterCircle = 15;
        
        sf::VertexArray outerOutline = getRoundedRectPoints(
            topLeft, width, height, radius, points_quarterCircle, outlineColor);

        sf::VertexArray innerOutline = getRoundedRectPoints(
            sf::Vector2f(topLeft.x + outlineWidth, topLeft.y + outlineWidth), 
            width - outlineWidth * 2, 
            height - outlineWidth * 2, 
            radius - outlineWidth, 
            points_quarterCircle,
            outlineColor);

        sf::VertexArray innerRect = getRoundedRectPoints(
            sf::Vector2f(topLeft.x + outlineWidth + outlinePadding, topLeft.y + outlineWidth + outlinePadding), 
            width - outlineWidth * 2 - outlinePadding * 2, 
            height - outlineWidth * 2 - outlinePadding * 2, 
            radius - outlineWidth - outlinePadding,  
            points_quarterCircle,
            color);
        innerRect.setPrimitiveType(sf::TriangleFan);
        
        sf::VertexArray outline(sf::TriangleStrip, points_quarterCircle * 4 * 2 + 2);

        for (int i = 0; i < points_quarterCircle * 4; i++) {
            outline[2 * i].position = innerOutline[i].position;
            outline[2 * i + 1].position = outerOutline[i].position;
            outline[2 * i].color = innerOutline[i].color;
            outline[2 * i + 1].color = outerOutline[i].color;
        }
        outline[points_quarterCircle * 4 * 2] = outline[0];
        outline[points_quarterCircle * 4 * 2 + 1] = outline[1];

        return {innerRect, outline};
    }
}