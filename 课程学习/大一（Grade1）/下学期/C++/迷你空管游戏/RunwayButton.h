#ifndef RUNWAYBUTTON_H
#define RUNWAYBUTTON_H

#include <SFML/Graphics.hpp>
#include "TakingOffPlane.h"

class RunwayButton
{
public:
    sf::CircleShape point;
    sf::Clock clock;
    bool isClicked = false;  

    RunwayButton()
    {
        point.setRadius(7.5);
        point.setFillColor(sf::Color::Transparent);
    }

    void handleInput(const sf::Event& event, sf::RenderWindow& window)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);

            if (point.getGlobalBounds().contains(mousePosition))
            {
                isClicked = true;
                clock.restart();
            }
        }
    }

    void update()
    {
        if (isClicked)
        {
            float elapsed = clock.getElapsedTime().asSeconds();

            if (elapsed < 1)  // 渐变出现
            {
                point.setFillColor(sf::Color(255, 255, 255, elapsed * 255));
            }
            else if (elapsed < 3)  // 保持显示
            {
                point.setFillColor(sf::Color::White);
            }
            else if (elapsed < 4)  // 渐变消失
            {
                point.setFillColor(sf::Color(255, 255, 255, (5 - elapsed) * 255));
            }
            else  // 完全消失
            {
                point.setFillColor(sf::Color::Transparent);
                isClicked = false;
            }
        }
    }
};

#endif
