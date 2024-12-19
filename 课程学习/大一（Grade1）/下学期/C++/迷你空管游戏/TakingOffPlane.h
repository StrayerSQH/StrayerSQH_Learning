#ifndef TAKINGOFFPLANE_H
#define TAKINGOFFPLANE_H

#include <SFML/Graphics.hpp>
#include "TakingOffPlane.h"

//bugs:1.����ɻ���ˮ��ת�䡱���⣬ͨ��Բ+���ߵķ�ʽ���
//     2.�ɻ�δ�����ɼ���ת��
//     3.�ɻ��ڿ���ʱ��������ɻ����ٵ������һ���㣬�ɻ�����ת�䣬����������Ϸ����

class TakingOffPlane
{
public:
    sf::CircleShape plane;
    sf::CircleShape choosePlane;
    sf::Texture planePicture;
    sf::Sprite sprite;
    sf::Vector2f position;
    sf::Vector2f direction; // �ɻ����ƶ�����
    sf::Clock clock;
    bool isDragging; // �Ƿ������϶��ɻ�
    bool isOnRunway; // �ɻ��Ƿ����ܵ���
    bool isFlying;//�ɻ��Ƿ��ڿ���
    bool isControlled;//�ɻ��Ƿ񱻿���
    bool isMousePressed;//����Ƿ񱻰���
    bool isChosen;//�ɻ��Ƿ�ѡ��
    float planeTravelDistance = 0.0f;
    sf::Vector2f dragOffset; // ���λ�úͷɻ�λ�õ�ƫ����

    TakingOffPlane(float radius)
        : isDragging(false), isOnRunway(false), isFlying(false), isControlled(false), isMousePressed(false)
    {
        // ���طɻ���ͼƬ
        if (!planePicture.loadFromFile("1694597194197.jpg"))
        {
            // �������
        }
        setPosition(radius, radius);
        plane.setRadius(radius);
        plane.setOrigin(radius, radius);
        plane.setPosition(radius, radius);
        plane.setFillColor(sf::Color::Transparent); // ����Ϊ͸��ɫ
        plane.setOutlineThickness(-5); // �����������Ϊ5����
        plane.setOutlineColor(sf::Color::White); // ����������ɫΪ��ɫ
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
                if (!isOnRunway) // �ɻ������ܵ��ϲ��Ҵ�δ�������
                {
                    setPosition(plane.getRadius(), plane.getRadius());
                }
                else if ((!isMousePressed && !plane.getGlobalBounds().contains(mousePos)) && isFlying) // �ɻ����ڷ�����
				{
                    sf::Vector2f diff = mousePos - position; 

                    if (diff.y >= 0)//����
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
                    else if (diff.y < 0)//����
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
            // ������Ƕ�����Ϊ sprite ����ת�Ƕ�
            sprite.setRotation(angle - 90); // ��ȥ90������Ϊ����ϣ���ɻ���ͷ��ָ���˶�����
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
            )//���ܶ��ˣ�����
        {
            choosePlane.setPosition(position.x - 5, position.y - 5);
            window.draw(choosePlane);
        }
    }
};

#endif