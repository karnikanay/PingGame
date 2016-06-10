#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "WindowObject.h"
#include <iostream>

class Ball
: public WindowObject
{

private:

    sf::Texture obBallTexture;
    sf::Sprite obBall;

    sf::Clock lastPaddleCollision;
    sf::Clock lastWallCollision;

    sf::SoundBuffer ballBounceBuffer;
    sf::Sound ballBounce;

    float hVelocity = 0;
    float vVelocity = 0;

    const float MAX_VELOCITY = 2.5f;
    const float MIN_VELOCITY = 1.5f;

    short int vNegator = 1;
    short int hNegator = 1;

    float wallMargin;

    sf::Clock randomClock1;
    sf::Clock randomClock2;
    sf::Clock randomClock3;
    sf::Clock randomClock4;
    sf::Clock randomClock5;

    bool hasCollidedPaddle = false;

public:

    Ball()
    {

    }

    Ball(float newWallMargin)
    {
        obBallTexture.loadFromFile("resources/ball.png");
        obBall.setTexture(obBallTexture);

        ballBounceBuffer.loadFromFile("resources/bounce.ogg");
        ballBounce.setBuffer(ballBounceBuffer);

        hVelocity = randomVelocity(randomClock1.restart().asMicroseconds(), randomClock3.restart().asMicroseconds());
        vVelocity = randomVelocity(randomClock2.restart().asMicroseconds(), randomClock4.restart().asMicroseconds());
        wallMargin = newWallMargin;

        vNegator = (randomClock5.restart().asMicroseconds() % 2) == 0 ? -1 : +1;

        std::cout << hVelocity << " " << vVelocity << std::endl;
    }

    void setPosition(sf::Vector2f newPosition)
    {
        obBall.setPosition(newPosition);
    }

    void draw()
    {
        parentWindow->draw(obBall);
    }

    sf::Vector2u getSize()
    {
        return obBallTexture.getSize();
    }

    void run()
    {
        if(hasCollidedWall() && lastWallCollision.getElapsedTime().asSeconds() >= 0.2)
        {
            vVelocity = -vVelocity;
            hVelocity *= 2.4;
            vVelocity /= 1.4;
            ballBounce.play();
            lastWallCollision.restart();
        }
        if(hasCollidedPaddle)
        {
            hVelocity = -hVelocity;
            ballBounce.play();
        }

        hasCollidedPaddle = false;

        limitVelocity();

        obBall.move(sf::Vector2f(hVelocity * 2 * hNegator, vNegator * vVelocity));
    }

    void setHasCollidedPaddle(bool newHasCollidedPaddle, float paddleYVelocity)
    {
        if(lastPaddleCollision.getElapsedTime().asSeconds() >= 0.2)
        {
            hasCollidedPaddle = newHasCollidedPaddle;
            vVelocity += paddleYVelocity / 5;
            hVelocity /= 1.4;
            vVelocity *= 2.4;
            lastPaddleCollision.restart();
        }
    }

    sf::FloatRect getBounds()
    {
        return obBall.getGlobalBounds();
    }

    float getX()
    {
        return obBall.getPosition().x;
    }

    float getY()
    {
        return obBall.getPosition().y;
    }

    void setHNegator(int newNegator)
    {
        std::cout << "New Negator: " << newNegator << std::endl;
        hNegator = newNegator;
    }

private:

    bool hasCollidedWall()
    {
        return (obBall.getPosition().y <= wallMargin) || (obBall.getPosition().y >= parentWindow->getSize().y - wallMargin - obBallTexture.getSize().y);
    }

    float randomVelocity(int seed1, int seed2)
    {
        float finalVelocity;
        std::cout << randomClock3.getElapsedTime().asMicroseconds() << std::endl;
        int reducedSize = ((seed1 * seed2) % 50) + 15;
        finalVelocity = (double)reducedSize / 60;
        return finalVelocity;
    }

    void limitVelocity()
    {
        if (vVelocity < 0)
        {
            if (-vVelocity > MAX_VELOCITY)
            {
                vVelocity = -MAX_VELOCITY;
            }
            if (-vVelocity < MIN_VELOCITY)
            {
                vVelocity = -MIN_VELOCITY;
            }
        }

        if (vVelocity > 0)
        {
            if (vVelocity > MAX_VELOCITY)
            {
                vVelocity = MAX_VELOCITY;
            }
            if (vVelocity < MIN_VELOCITY)
            {
                vVelocity = MIN_VELOCITY;
            }
        }

        if (hVelocity < 0)
        {
            if (-hVelocity > MAX_VELOCITY)
            {
                hVelocity = -MAX_VELOCITY;
            }
            if (-hVelocity < MIN_VELOCITY)
            {
                hVelocity = -MIN_VELOCITY;
            }
        }

        if (hVelocity > 0)
        {
            if (hVelocity > MAX_VELOCITY)
            {
                hVelocity = MAX_VELOCITY;
            }
            if (hVelocity < MIN_VELOCITY)
            {
                hVelocity = MIN_VELOCITY;
            }
        }

        std::cout << "H Velocity: " << hVelocity << std::endl;
        std::cout << "V Velocity: " << vVelocity << std::endl;
    }

};

