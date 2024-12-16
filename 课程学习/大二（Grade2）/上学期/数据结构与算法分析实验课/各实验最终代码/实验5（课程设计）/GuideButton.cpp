#include "GuideButton.h"
#include "SFML/Graphics.hpp"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

float GuideButton::GuideButtonHeight = 170;

GuideButton::GuideButton(std::string text) : isHovered(false), characterIndex(0), guideButtonText(text)
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	window_x = static_cast<float>(desktopMode.width);
	window_y = static_cast<float>(desktopMode.height);

	GuideButtonWidth = window_x;

	shape.setPosition(0, 4 * window_y / 5);
	shape.setSize(sf::Vector2f(GuideButtonWidth, GuideButtonHeight));
	shape.setFillColor(sf::Color(128, 128, 128, 128));

	if (!font.loadFromFile("��ͼĦ��С����(�������).ttf"))
	{
		cout << "�������Ĭ������ʧ�ܣ����������Ƿ���ڣ�����" << endl;
	}
	
	label.setFont(font);
	label.setCharacterSize(characterSize);
	label.setFillColor(sf::Color::White);

};

bool GuideButton::DrawButton(sf::RenderWindow& window)
{
	bool isPrint = false;

	window.draw(shape);
	sf::FloatRect labelBounds = label.getLocalBounds();
	sf::FloatRect shapeBounds = shape.getGlobalBounds();
	label.setOrigin(labelBounds.left + labelBounds.width / 2, labelBounds.top + labelBounds.height / 2);

	// �����ı�����λ��
	float centerX = shape.getPosition().x + 300;
	float centerY = shape.getPosition().y +50;
	label.setPosition(centerX, centerY);

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		sf::Time elapsed = clock.getElapsedTime();

		// �����������
		if (elapsed.asSeconds() > labelSpeed && currentLabel.length() < guideButtonText.length())
		{
			currentLabel += guideButtonText[currentLabel.length()];
			label.setString(currentLabel);

			// ����ʱ��
			clock.restart();
		}
		else if (currentLabel.length() == guideButtonText.length())
		{
			// ��������ȫ���أ����ñ�־Ϊfalse������ѭ��
			isPrint = true;
			return isPrint;
		}
		//window.clear();
		window.draw(label);
		window.display();
	}

	label.setString("");
}
