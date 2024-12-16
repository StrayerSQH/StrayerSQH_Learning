#ifndef WINDOWSSHOW_H
#define WINDOWSSHOW_H

#include <SFML/Graphics.hpp>

class WindowsShow {
private:
    sf::RenderWindow window;
    sf::Texture DefaultBackgroundImg;
    sf::Sprite background; // ��ӱ����������
public:
    void ShowWindow();
    void DrawBackground(sf::RenderWindow& window); // ��ӻ��Ʊ�����������
    sf::RenderWindow& getWindow();
};

#endif // WINDOWSSHOW_H