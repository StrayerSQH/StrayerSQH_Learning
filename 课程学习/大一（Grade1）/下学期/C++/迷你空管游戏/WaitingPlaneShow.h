#ifndef WAITINGPLANESHOW_H
#define WAITINGPLANESHOW_H

#include <cmath>
#include <SFML/Graphics.hpp>

const float circleRadius = 50;

class WaitingPlaneShow
{
public:
    sf::ConvexShape semicircle1, semicircle2, semicircle3, semicircle4;
    sf::RectangleShape rectangle;


    double M_PI = 3.14159265358979323846f;

    WaitingPlaneShow(float width, float height, float radius, float posX, float posY, float heightWaitingPlaneShow)
    {
        const int pointsInHalfCircle = 30;
        semicircle1.setPointCount(pointsInHalfCircle);
        semicircle2.setPointCount(pointsInHalfCircle);
        semicircle3.setPointCount(pointsInHalfCircle);
        semicircle4.setPointCount(pointsInHalfCircle);

        // 左半圆
        for (int i = 0; i < pointsInHalfCircle; i++)
        {
            float angle = (i / static_cast<float>(pointsInHalfCircle - 1)) * 180.f;
            semicircle1.setPoint(i, sf::Vector2f(radius * std::sin(-angle * M_PI / 180.f), radius * std::cos(angle * M_PI / 180.f) + radius));
        }

        // 右半圆
        for (int i = 0; i < pointsInHalfCircle; i++)
        {
            float angle = (i / static_cast<float>(pointsInHalfCircle - 1)) * 180.f;
            semicircle2.setPoint(i, sf::Vector2f(radius * std::sin(-(180.f + angle) * M_PI / 180.f), radius * std::cos((180.f + angle) * M_PI / 180.f) + radius));
        }

        // 长方形
        rectangle.setSize(sf::Vector2f(width, height));


        // 设置颜色为白色半透明
        semicircle1.setFillColor(sf::Color(128, 128, 128, 128));
        semicircle2.setFillColor(sf::Color(128, 128, 128, 128));
        rectangle.setFillColor(sf::Color(128, 128, 128, 128));

        // 设置位置
        semicircle1.setPosition(posX, posY);
        semicircle2.setPosition(posX + rectangle.getSize().x, posY);
        rectangle.setPosition(posX, posY);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(semicircle1);
        window.draw(semicircle2);
        window.draw(rectangle);
    }
};

#endif
