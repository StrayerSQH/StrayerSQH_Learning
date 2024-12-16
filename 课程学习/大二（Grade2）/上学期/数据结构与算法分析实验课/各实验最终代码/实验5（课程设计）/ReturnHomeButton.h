#ifndef RETURNHOMEBUTTON_H
#define RETURNHOMEBUTTON_H

#include "Button.h"

class ReturnHomeButton : public Button {
public:
    static float ReturnHomeButtonWidth, ReturnHomeButtonHeight;

    ReturnHomeButton(float x, float y);

    void DrawButton(sf::RenderWindow& window);

    int ExitProgram(sf::RenderWindow& window);

private:
    bool isClicked;  // ��ť�Ƿ񱻵��
};

#endif // !RETURNHOMEBUTTON_H
