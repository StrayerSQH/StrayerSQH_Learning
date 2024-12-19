#ifndef LONDON_H
#define LONDON_H

#include <SFML/Graphics.hpp>
#include "GeneralAir.h"
#include "RunwayButton.h"
#include "WaitingPlaneShow.h"
#include "TakingOffPlane.h" // ����TakingOffPlane��

sf::RectangleShape rectangle;
const float screenWidth = static_cast<float>(sf::VideoMode::getDesktopMode().width);
const float screenHeight = static_cast<float>(sf::VideoMode::getDesktopMode().height);
const float posX = (screenWidth - rectangle.getSize().x / 2.f) / 2.f;
const float posY = (screenHeight - rectangle.getSize().y / 2.f) / 2.f;
const float widthWaitingPlaneShow = 500.f;
const float heightWaitingPlaneShow = 50.f;
const float radiusWaitingPlaneShow = heightWaitingPlaneShow / 2.f;
const float radiusPlane = 50.f;

class London
{
public:

    CoordinateSystem cs;
    RunwayButton point1, point2;
    WaitingPlaneShow waitingPlaneShow;
    TakingOffPlane plane; // ��ӷɻ�����

    London() : waitingPlaneShow(widthWaitingPlaneShow, heightWaitingPlaneShow, radiusWaitingPlaneShow, (screenWidth / 2.f) - (widthWaitingPlaneShow / 2.f), screenHeight - 200, heightWaitingPlaneShow),
        plane(radiusPlane)// ��ʼ���ɻ�����
    {
        rectangle.setSize(sf::Vector2f(200.f, 15.f));//�����ܵ�
        rectangle.setPosition(sf::Vector2f(posX, posY));///�����ܵ�λ��
        rectangle.setFillColor(sf::Color(128, 128, 128, 255));//�����ܵ���ɫ

        // ��ʼ��������
        point1.point.setPosition(sf::Vector2f(posX, posY));
        point2.point.setPosition(sf::Vector2f(posX + rectangle.getSize().x - 15.f, posY));

        // ���÷ɻ��ĳ�ʼλ��
        plane.setPosition(radiusPlane, radiusPlane);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(rectangle);
        window.draw(point1.point);
        window.draw(point2.point);
        waitingPlaneShow.draw(window);
        plane.draw(window, radiusPlane); // ���Ʒɻ�
    }

    void update()
    {
        float speed = 0.2f; // �ɻ����ٶ�
        plane.move(speed); // �ƶ��ɻ�
    }
};

#endif
