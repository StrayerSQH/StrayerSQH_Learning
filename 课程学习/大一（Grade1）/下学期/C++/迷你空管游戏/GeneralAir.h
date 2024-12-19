#ifndef GENERALAIR_H
#define GENERALAIR_H

#include <cmath>
#include <SFML/Graphics.hpp>

struct Point
{
	double x;
	double y;
};

class CoordinateSystem
{
public:
	Point origin;
    double screenHeight;

    CoordinateSystem()
        : screenHeight(sf::VideoMode::getDesktopMode().height)
    {
        origin.x = 0.0;
        origin.y = screenHeight;
    }
};

#endif