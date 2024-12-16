#include "WindowsShow.h"
#include <iostream>

void WindowsShow::ShowWindow()
{
    // ��ȡ����ֱ��ʲ�����һ��ȫ������
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    // ����������Ϊ���Ĵ���ѧ��������ģ������
    window.create(desktop, "�Ĵ���ѧ��������ģ����", sf::Style::Fullscreen);
    // ����Ĭ�ϱ���ͼƬ������һ�����鲢��������Ϊ����ͼƬ
    if (!DefaultBackgroundImg.loadFromFile("Picture/BGI.jpg"))
    {
        std::cout << "�������Ĭ��ͼƬʧ�ܣ�����ͼƬ�Ƿ���ڣ�����" << std::endl;
        // sleep(3);
        // return -1;
    }
    // ��ͼƬ����Ϊȫ��
    background.setTexture(DefaultBackgroundImg);
    background.setScale(
        window.getSize().x / background.getLocalBounds().width,
        window.getSize().y / background.getLocalBounds().height
    );

    // ��ȡ���ڴ�С
    sf::Vector2u windowSize = window.getSize();
    sf::Vector2f windowSizeFloat(windowSize.x, windowSize.y);
}

void WindowsShow::DrawBackground(sf::RenderWindow& window)
{
    window.draw(background);
}

sf::RenderWindow& WindowsShow::getWindow() {
    return window;
}