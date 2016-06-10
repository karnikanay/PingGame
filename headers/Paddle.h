#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "WindowObject.h"
#ifndef __Paddle__
#define __Paddle__

class Paddle
: public WindowObject
{

protected:

    sf::Texture obPaddleTexture;
    sf::Sprite obPaddle;

    const float SPEED = 3.f;

    bool isMovingUp = false;
    bool isMovingDown = false;

    float wallMargin;

public:

    enum Color {Red, Blue};

    Paddle()
    {

    }

    Paddle(Color paddleColor, float newWallMargin)
    {
        if(paddleColor == Color::Blue)
            obPaddleTexture.loadFromFile("resources/paddle_blue.png");
        if(paddleColor == Color::Red)
            obPaddleTexture.loadFromFile("resources/paddle_red.png");

        obPaddle.setTexture(obPaddleTexture);
        wallMargin = newWallMargin;
    }

    void setPosition(sf::Vector2f newPosition)
    {
        obPaddle.setPosition(newPosition);
    }

    void draw()
    {
        parentWindow->draw(obPaddle);
    }

    sf::Vector2u getSize()
    {
        return obPaddleTexture.getSize();
    }

    void run()
    {
        if(isMovingUp && isSpaceUp())
            obPaddle.move(sf::Vector2f(0.f, -SPEED));

        if(isMovingDown && isSpaceDown())
            obPaddle.move(sf::Vector2f(0.f, +SPEED));
    }

    void setIsMovingUp(bool newIsMovingUp)
    {
        isMovingUp = newIsMovingUp;
    }

    void setIsMovingDown(bool newIsMovingDown)
    {
        isMovingDown = newIsMovingDown;
    }

    sf::FloatRect getBounds()
    {
        return obPaddle.getGlobalBounds();
    }

    float getVelocity()
    {
        if(isMovingUp && isMovingUp)
        {
            return 0;
        }
        else if (isMovingUp)
        {
            return SPEED * 1;
        }
        else if (isMovingDown)
        {
            return SPEED * -1;
        }
        else
        {
            return 0;
        }
    }

protected:

    bool isSpaceDown()
    {
        return obPaddle.getPosition().y <= parentWindow->getSize().y - wallMargin - obPaddleTexture.getSize().y;
    }

    bool isSpaceUp()
    {
        return obPaddle.getPosition().y >= wallMargin;
    }

};

#endif // __Paddle__
