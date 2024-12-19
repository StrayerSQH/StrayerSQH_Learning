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

        // ���Բ
        for (int i = 0; i < pointsInHalfCircle; i++)
        {
            float angle = (i / static_cast<float>(pointsInHalfCircle - 1)) * 180.f;
            semicircle1.setPoint(i, sf::Vector2f(radius * std::sin(-angle * M_PI / 180.f), radius * std::cos(angle * M_PI / 180.f) + radius));
        }

        // �Ұ�Բ
        for (int i = 0; i < pointsInHalfCircle; i++)
        {
            float angle = (i / static_cast<float>(pointsInHalfCircle - 1)) * 180.f;
            semicircle2.setPoint(i, sf::Vector2f(radius * std::sin(-(180.f + angle) * M_PI / 180.f), radius * std::cos((180.f + angle) * M_PI / 180.f) + radius));
        }

        // ������
        rectangle.setSize(sf::Vector2f(width, height));


        // ������ɫΪ��ɫ��͸��
        semicircle1.setFillColor(sf::Color(128, 128, 128, 128));
        semicircle2.setFillColor(sf::Color(128, 128, 128, 128));
        rectangle.setFillColor(sf::Color(128, 128, 128, 128));

        // ����λ��
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
