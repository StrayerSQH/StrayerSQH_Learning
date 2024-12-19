#ifndef TAKINGOFFPLANE_H
#define TAKINGOFFPLANE_H

#include <SFML/Graphics.hpp>
#include "TakingOffPlane.h"

//bugs:1.解决飞机“水滴转弯”问题，通过圆+切线的方式解决
//     2.飞机未完成起飞即可转弯
//     3.飞机在空中时，鼠标点击飞机，再点击另外一个点，飞机即可转弯，而不是向游戏那样

class TakingOffPlane
{
public:
    sf::CircleShape plane;
    sf::CircleShape choosePlane;
    sf::Texture planePicture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f direction; // 飞机的移动方向
    sf::Clock clock;
    bool isDragging; // 是否正在拖动飞机
    bool isOnRunway; // 飞机是否在跑道上
    bool isFlying;//飞机是否在空中
    bool isControlled;//飞机是否被控制
    bool isMousePressed;//鼠标是否被按下
    bool isChosen;//飞机是否被选中
    float planeTravelDistance = 0.0f;
    sf::Vector2f dragOffset; // 鼠标位置和飞机位置的偏移量

    TakingOffPlane(float radius)
        : isDragging(false), isOnRunway(false), isFlying(false), isControlled(false), isMousePressed(false)
    {
        // 加载飞机的图片
        if (!planePicture.loadFromFile("1694597194197.jpg"))
        {
            // 处理错误
        }
        setPosition(radius, radius);
        plane.setRadius(radius);
        plane.setOrigin(radius, radius);
        plane.setPosition(radius, radius);
        plane.setFillColor(sf::Color::Transparent); // 设置为透明色
        plane.setOutlineThickness(-5); // 设置轮廓宽度为5像素
        plane.setOutlineColor(sf::Color::White); // 设置轮廓颜色为白色
        choosePlane.setRadius(radius + 5);
        choosePlane.setOrigin(radius, radius);
        choosePlane.setFillColor(sf::Color::Transparent);
        choosePlane.setOutlineThickness(-5);
        choosePlane.setOutlineColor(sf::Color::Red);
        sprite.setTexture(planePicture);
        sprite.setOrigin(planePicture.getSize().x / 2, planePicture.getSize().y / 2);
        sprite.setPosition(radius, radius);
        sprite.setScale(radius * 2 / sprite.getLocalBounds().width, radius * 2 / sprite.getLocalBounds().height);
    }

    void handleEvent(const sf::Event& event, const sf::Vector2f& point1, const sf::Vector2f& point2)
    {
        sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
        
        if (event.type == sf::Event::MouseButtonPressed)
        {
           isMousePressed = true;
           if (!isOnRunway && plane.getGlobalBounds().contains(mousePos))
            {
                isDragging = true;
                dragOffset = position - mousePos;
            }
            else if (isOnRunway && isFlying && plane.getGlobalBounds().contains(mousePos))
            {
                isControlled = true; 
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            isDragging = false;
            isMousePressed = false;

            sf::Time elapsed = clock.getElapsedTime();

            if ((plane.getGlobalBounds().contains(point1) || plane.getGlobalBounds().contains(point2)) && !isOnRunway)
            {
                float length = 0.0f;

                isOnRunway = true;
                
                if (plane.getGlobalBounds().contains(point1))
                {
                    isControlled = false;
                    sf::Vector2f diff = point2 - point1;
                    length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                    direction = sf::Vector2f(diff.x / length, diff.y / length);
                    setPosition(point1.x + 7.5, point1.y + 7.5);
                }
                else if (plane.getGlobalBounds().contains(point2))
                {
                    isControlled = false;
                    sf::Vector2f diff = point1 - point2;
                    length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                    direction = sf::Vector2f(diff.x / length, diff.y / length);
                    setPosition(point2.x + 7.5, point2.y + 7.5);
                }

                
                std::cout << planeTravelDistance << std::endl;
                if (planeTravelDistance >= length)
                {
                    isFlying = true;
                }
            }
            else
            {
                if (!isOnRunway) // 飞机不在跑道上并且从未进行起飞
                {
                    setPosition(plane.getRadius(), plane.getRadius());
                }
                else if ((!isMousePressed && !plane.getGlobalBounds().contains(mousePos)) && isFlying) // 飞机正在飞行中
				{
                    sf::Vector2f diff = mousePos - position; 

                    if (diff.y >= 0)//向下
                    {
                        if (direction.x > 0)
                        {
                            float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                            direction = sf::Vector2f(diff.x / length, diff.y / length);
                        }
                        else
                        {
                            float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                            direction = sf::Vector2f(diff.x / length, diff.y / length);
                        }
                    }
                    else if (diff.y < 0)//向上
                    {
                        if (direction.x > 0)
                        {
                            float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                            direction = sf::Vector2f(diff.x / length, diff.y / length);
                        }
                        else
                        {
                            float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
                            direction = sf::Vector2f(diff.x / length, diff.y / length);
                        }
                    }
                    isControlled = false;
				}
            }
        }
        else if (event.type == sf::Event::MouseMoved && !isOnRunway)
        {
            sf::Vector2f mousePos(static_cast<float>(event.mouseMove.x), static_cast<float>(event.mouseMove.y));
            if (isDragging)
            {
                setPosition(mousePos.x + dragOffset.x, mousePos.y + dragOffset.y);
            }
        }
    }

    void setPosition(float x, float y)
    {
        position.x = x;
        position.y = y;
        plane.setPosition(x, y);
        sprite.setPosition(x, y);
    }

    float move(float speed)
    {
        if (isOnRunway)
        {
            position += direction * speed;
            planeTravelDistance += sqrt(pow(direction.x, 2) + pow(direction.y, 2)) * speed;
            plane.setPosition(position);
            sprite.setPosition(position);

            float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;
            // 将这个角度设置为 sprite 的旋转角度
            sprite.setRotation(angle - 90); // 减去90度是因为我们希望飞机的头部指向运动方向
        }

        return planeTravelDistance;
    }


    void draw(sf::RenderWindow& window, float radius)
    {
        window.draw(sprite);
        window.draw(plane);

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if ((!isFlying && isMousePressed && plane.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y)))
            ||(isFlying && isMousePressed && isControlled && !plane.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y))
            || (isFlying && isMousePressed && isControlled && plane.getGlobalBounds().contains(static_cast<float>(sf::Mouse::getPosition(window).x), static_cast<float>(sf::Mouse::getPosition(window).y))))
            )//不能动了！！！
        {
            choosePlane.setPosition(position.x - 5, position.y - 5);
            window.draw(choosePlane);
        }
    }
};

#endif