#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <Windows.h>

using namespace std;

class Button
{
public:
    // ���캯��
    Button(float window_x, float window_y, float width, float height, const std::string& text, sf::Uint8 alpha = 128);

    // ���ư�ť
    void DrawButton(sf::RenderWindow& window);

    // �������Ƿ��ڰ�ť��Χ��
    bool CheckHover(const sf::Vector2f& mousePos);

    // ���ð�ť��״̬
    void SetHovered(bool hovered);

    sf::RectangleShape shape;   // ��ť����״
    sf::Text label;             // ��ť�ϵ��ı�
    sf::Font font;              // ��ť���ı�������
    bool isHovered;             // ��ť�Ƿ������ͣ
    float characterSize = 80;
};

#endif