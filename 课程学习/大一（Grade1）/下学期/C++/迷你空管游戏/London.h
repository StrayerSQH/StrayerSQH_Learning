#ifndef LONDON_H
#define LONDON_H

#include <SFML/Graphics.hpp>
#include "GeneralAir.h"
#include "RunwayButton.h"
#include "WaitingPlaneShow.h"
#include "TakingOffPlane.h" // 引入TakingOffPlane类

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
    TakingOffPlane plane; // 添加飞机对象

    London() : waitingPlaneShow(widthWaitingPlaneShow, heightWaitingPlaneShow, radiusWaitingPlaneShow, (screenWidth / 2.f) - (widthWaitingPlaneShow / 2.f), screenHeight - 200, heightWaitingPlaneShow),
        plane(radiusPlane)// 初始化飞机对象
    {
        rectangle.setSize(sf::Vector2f(200.f, 15.f));//定义跑道
        rectangle.setPosition(sf::Vector2f(posX, posY));///设置跑道位置
        rectangle.setFillColor(sf::Color(128, 128, 128, 255));//定义跑道颜色

        // 初始化两个点
        point1.point.setPosition(sf::Vector2f(posX, posY));
        point2.point.setPosition(sf::Vector2f(posX + rectangle.getSize().x - 15.f, posY));

        // 设置飞机的初始位置
        plane.setPosition(radiusPlane, radiusPlane);
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(rectangle);
        window.draw(point1.point);
        window.draw(point2.point);
        waitingPlaneShow.draw(window);
        plane.draw(window, radiusPlane); // 绘制飞机
    }

    void update()
    {
        float speed = 0.2f; // 飞机的速度
        plane.move(speed); // 移动飞机
    }
};

#endif
