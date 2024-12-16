#ifndef SEXCHOOSESYSTEM_H
#define SEXCHOOSESYSTEM_H

#include "Button.h"
#include "SFML/Graphics.hpp"
#include <string.h>

class SexChooseButton : public Button {
private:
	sf::Texture sexTexture;
	sf::Sprite sexSprite;

public:
	static float SexChooseButtonHeight, SexChooseButtonWidth;
	string SexChooseFilePath;

	SexChooseButton(float x, float y, std::string path, std::string sex);

	int SexChooseButtonPitureSet(float x, float y);

	void DrawButton(sf::RenderWindow& window);

};

class SexChooseSystem {
private:
	sf::Texture SexBackgroundImg;
	sf::Sprite sexBackground; // ��ӱ����������

public:
	int SexChooseResult = 1;//Ĭ������Ϊ1��Ů��Ϊ0��

	int SexChooseProgram(sf::RenderWindow& window);
};

#endif // !SEXCHOOSESYSTEM_H
