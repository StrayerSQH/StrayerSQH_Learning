#include <SFML/Graphics.hpp>
#include <iostream>
#include "GeneralAir.h"
#include "London.h"
#include "RunwayButton.h"

std::string currentGameType;
sf::Vector2f windowSize;
float widthExitExeButtonX = 500.f;
float heightExitExeButtonY = 200.f;
float widthCitySelectionButtonX = 500.f;
float heightCitySelectionButtonY = 200.f;
float exitExeButtonPositionX;
float exitExeButtonPositionY;
float CitySelectionButtonPositionX;
float CitySelectionButtonPositionY;
float widthSelectCityButtonX = 500.0f;
float heightSelectCityButtonY = 700.0f;
float SelectCityButtonSpacing;
int waitTime = 200; //milliseconds
float RunwayButtonRadius = 7.5f;

class Button
{
public:
    Button(float x, float y, float width, float height, const std::string& text, sf::Uint8 alpha = 255)
        : shape(sf::Vector2f(width, height))
    {
        shape.setPosition(x, y);
        shape.setFillColor(sf::Color(128, 128, 128, alpha));  // ������ɫ��͸����

        if (!font.loadFromFile("arial.ttf"))  // ��ȷ������һ����Ϊ"arial.ttf"�������ļ�
        {
            // �������...
        }

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(24);  // ���������С
        label.setFillColor(sf::Color::White);  // ����������ɫ
        label.setPosition(x, y);  // �����ı���λ��
    }

    virtual void draw(sf::RenderWindow& window)
    {
        window.draw(shape);
        window.draw(label);  // �����ı�
    }

    virtual bool isClicked(sf::RenderWindow& window)
    {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (shape.getGlobalBounds().contains(mousePos.x, mousePos.y))
            {
                return true;
            }
        }
        return false;
    }

protected:
    sf::RectangleShape shape;
    sf::Font font;
    sf::Text label;
};

class ArrowButton : public Button
{
public:
    ArrowButton(float x, float y, float width, float height, const std::string& text, const std::string& imagePath, sf::Uint8 alpha = 255)
        : Button(x, y, width, height, text, alpha)
    {
        if (!arrowTexture.loadFromFile("1694597194197.jpg"))  // ���ؼ�ͷͼƬ
        {
            // �������...
        }

        arrowSprite.setTexture(arrowTexture);  // ����������Ϊ��ͷͼƬ
        arrowSprite.setPosition(x, y);  // ���ü�ͷ��λ��

        // ���ü�ͷͼƬ�Ĵ�С
        arrowSprite.setScale(
            width / arrowSprite.getLocalBounds().width,
            height / arrowSprite.getLocalBounds().height);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(arrowSprite);
        window.draw(label);  // �����ı�
    }

private:
    sf::Texture arrowTexture;
    sf::Sprite arrowSprite;
};

class EnterGameButton : public Button
{
public:
    EnterGameButton(float x, float y, float width, float height, const std::string& text, const std::string& imagePath, sf::Uint8 alpha = 255)
        : Button(x, y, width, height, text, alpha)
    {
        if (!cityTexture.loadFromFile("1694597194197.jpg"))  // ʹ�� imagePath ��������ͼƬ
        {
            // �������...
        }

        citySprite.setTexture(cityTexture);
        citySprite.setPosition(x, y);

        // ����ͼƬ�Ĵ�С
        citySprite.setScale(
            width / citySprite.getLocalBounds().width,
            height / citySprite.getLocalBounds().height);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(citySprite);
        window.draw(label);  // �����ı�
    }

private:
    sf::Texture cityTexture;
    sf::Sprite citySprite;
};

enum class AppState {
    MainMenu,
    CitySelection,
    Game
};

void handleLondonGame(sf::RenderWindow& window, sf::Sprite& cityBackground, ArrowButton& arrowReturn, AppState& state, std::string& currentGameType)
{
    London london;
    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;

            london.point1.handleInput(event, window);
            london.point2.handleInput(event, window);
            london.update();
            london.plane.handleEvent(event, london.point1.point.getPosition(), london.point2.point.getPosition());
        }

        // ���µ����ɫ
        london.point1.update();
        london.point2.update();
        // ���·ɻ���λ��
        if (!london.plane.isDragging)
        {
            london.update(); // ���·ɻ���λ��
        }

        window.clear();
        window.draw(cityBackground);
        arrowReturn.draw(window);

        london.draw(window);

        if (arrowReturn.isClicked(window))
        {
            currentGameType = "";
            state = AppState::CitySelection;
            running = false;
        }

        window.display();
    }
}

void handleChengduGame(sf::RenderWindow& window, sf::Sprite& cityBackground, ArrowButton& arrowReturn, AppState& state, std::string& currentGameType)
{
    London london;
    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;

            london.point1.handleInput(event, window);
            london.point2.handleInput(event, window);
            london.update();
            london.plane.handleEvent(event, london.point1.point.getPosition(), london.point2.point.getPosition());
        }

        // ���µ����ɫ
        london.point1.update();
        london.point2.update();
        // ���·ɻ���λ��
        if (!london.plane.isDragging)
        {
            london.update(); // ���·ɻ���λ��
        }

        window.clear();
        window.draw(cityBackground);
        arrowReturn.draw(window);

        london.draw(window);

        if (arrowReturn.isClicked(window))
        {
            currentGameType = "";
            state = AppState::CitySelection;
            running = false;
        }

        window.display();
    }
}

void handleCityWindow(const sf::VideoMode& desktop, const std::string& cityName, const std::string& imagePath, sf::RenderWindow& window, AppState& state, const std::string& gameType)
{
    sf::Texture texture;
    if (!texture.loadFromFile("1694597194197.jpg"))
    {

    }

    sf::Sprite cityBackground(texture);
    cityBackground.setScale(
        window.getSize().x / cityBackground.getLocalBounds().width,
        window.getSize().y / cityBackground.getLocalBounds().height);

    //�������ذ�ť
    ArrowButton arrowReturn(2500.0f, 50.0f, 200.0f, 200.0f, "", "1694597194197.jpg", 256);
    London london;
    RunwayButton point1, point2;

    bool running = true;
    while (running)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                running = false;
        }

        window.clear();
        window.draw(cityBackground);
        arrowReturn.draw(window);
        
        if (gameType == "gameLondon")
        {
            handleLondonGame(window, cityBackground, arrowReturn, state, currentGameType);
        }
        else if (gameType == "gameChengdu")
        {
            handleChengduGame(window, cityBackground, arrowReturn, state, currentGameType);
        }


        if (arrowReturn.isClicked(window))
        {
            currentGameType = "";
            state = AppState::CitySelection;
            running = false; 
        }

        window.display();
    }
}

int main()
{
    // ��ȡ����ķֱ��ʲ�����һ��ȫ������
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    sf::RenderWindow window(desktop, "Mini_Air_Traffic_Control", sf::Style::Fullscreen);

    // ���ر���ͼƬ������һ�����鲢��������Ϊ����ͼƬ
    sf::Texture texture;
    if (!texture.loadFromFile("1694597194197.jpg"))
    {
        //return -1;  // �������ʧ�ܣ���������
    }
    sf::Sprite background(texture);

    // ������ͼƬ�Ĵ�С����Ϊ���ڵĴ�С
    background.setScale(
        window.getSize().x / background.getLocalBounds().width,
        window.getSize().y / background.getLocalBounds().height);

    // ��ȡ���ڵĴ�С�����㰴ť��λ��
    windowSize = static_cast<sf::Vector2f>(window.getSize());
    exitExeButtonPositionX = windowSize.x / 2.0f - widthExitExeButtonX / 2.0f; 
    exitExeButtonPositionY = windowSize.y - 400.0f;  // �봰�ڵײ�400����
    CitySelectionButtonPositionX = windowSize.x / 2.0f - widthCitySelectionButtonX / 2.0f;
    CitySelectionButtonPositionY = windowSize.y - 700.0f;  // �봰�ڵײ�700����
    SelectCityButtonSpacing = windowSize.x / 5.0f;  // �����ڵĿ��ƽ���ֳ�5��

    // ������ť
    Button exitExeButton(exitExeButtonPositionX, exitExeButtonPositionY, widthExitExeButtonX, heightExitExeButtonY, "EXIT", 128);
    Button CitySelectionButton(CitySelectionButtonPositionX, CitySelectionButtonPositionY, widthCitySelectionButtonX, heightCitySelectionButtonY, "NEW WINDOW", 128);
    Button londonButton(SelectCityButtonSpacing - widthSelectCityButtonX / 2.0f, windowSize.y / 2.0f - heightSelectCityButtonY / 2.0f, widthSelectCityButtonX, heightSelectCityButtonY, "Chengdu", 128);
    Button chengduButton(2 * SelectCityButtonSpacing - widthSelectCityButtonX / 2.0f, windowSize.y / 2.0f - heightSelectCityButtonY / 2.0f, widthSelectCityButtonX, heightSelectCityButtonY, "Mianyang", 128);
    Button chongqingButton(3 * SelectCityButtonSpacing - widthSelectCityButtonX / 2.0f, windowSize.y / 2.0f - heightSelectCityButtonY / 2.0f, widthSelectCityButtonX, heightSelectCityButtonY, "Chongqing", 128);
    Button beijingButton(4 * SelectCityButtonSpacing - widthSelectCityButtonX / 2.0f, windowSize.y / 2.0f - heightSelectCityButtonY / 2.0f, widthSelectCityButtonX, heightSelectCityButtonY, "Beijing", 128);
    ArrowButton arrowReturn(50.0f, 50.0f, 200.0f, 200.0f, "", "arrow_return.jpg", 256);

    //
    London london;

    //
    RunwayButton point1, point2;

    AppState state = AppState::MainMenu;
    // ��ѭ���������ڴ�ʱ��������
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            point1.handleInput(event, window);
            point2.handleInput(event, window);
        }

        // ���µ����ɫ
        point1.update();
        point2.update();

        window.clear();
        window.draw(background);

        switch (state)
        {
        case AppState::MainMenu:
            exitExeButton.draw(window);
            CitySelectionButton.draw(window);

            if (exitExeButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime));
                window.close();
            }
            else if (CitySelectionButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime)); 
                state = AppState::CitySelection;
            }
            break;

        case AppState::CitySelection:
            londonButton.draw(window);
            chengduButton.draw(window);
            chongqingButton.draw(window);
            beijingButton.draw(window);
            arrowReturn.draw(window);
            if (londonButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime));
                handleCityWindow(desktop, "London", "London.jpg", window, state, "gameLondon");
                state = AppState::Game;
            }
            else if (chengduButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime)); 
                handleCityWindow(desktop, "Mianyang", "chengdu.jpg", window, state, "gameChengdu");
                state = AppState::Game;
            }
            else if (chongqingButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime));
                handleCityWindow(desktop, "Chongqing", "chongqing.jpg", window, state, "gameChongqing");
                state = AppState::Game;
            }
            else if (beijingButton.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime)); 
                handleCityWindow(desktop, "Beijing", "beijing.jpg", window, state, "gameBeijing");
                state = AppState::Game;
            }
            else if (arrowReturn.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime)); 
                state = AppState::MainMenu;
            }
            break;

        case AppState::Game:
            arrowReturn.draw(window);
            if (currentGameType == "gameLondon")
            {
                london.draw(window);
            }
            else if (currentGameType == "gameMianyang")
            {
                
            }
            else if (currentGameType == "gameChongqing")
            {
                
            }
            else if (currentGameType == "gameBeijing")
            {
               
            }
            else if (arrowReturn.isClicked(window))
            {
                sf::sleep(sf::milliseconds(waitTime)); 
                state = AppState::CitySelection;
            }
            else if (currentGameType.empty())
            {
                state = AppState::CitySelection; 
            }
            break;

        }

        window.display();
    }

    return 0;
}