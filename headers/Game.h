#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Ball.h"
#include "Paddle.h"
#include "PaddleController.h"
#include <sstream>

class Game
{

private:

    std::stringstream intToString;

    enum GameState {Paused, Running};
    GameState pingState = GameState::Paused;

    int numPingGames = 0;

    sf::RenderWindow pingWindow;
    const float PING_WINDOW_WIDTH = 1400.f;
    const float PING_WINDOW_HEIGHT = 1400.f * 2.f / 3.f;

    const sf::Vector2f PING_WALL_SIZE = sf::Vector2f(PING_WINDOW_WIDTH, 40);
    const sf::Color PING_WALL_COLOR = sf::Color(100, 100, 100);

    sf::Font scoreFont;
    sf::Text leftScoreText;
    sf::Text rightScoreText;

    sf::RectangleShape pingWallUp;
    sf::RectangleShape pingWallDown;

    Ball* pingBall;
    Paddle* pingPaddleLeft;
    PaddleController* pingPaddleRight;

    sf::Clock randomClock1;
    sf::Clock randomClock2;

    const float PING_PADDLE_MARGIN = 20.f;

    const unsigned int PING_SCORE_FONT_SIZE = 200;

    int leftScore = 0;
    int rightScore = 0;

    const sf::Color PING_SCORE_COLOR = sf::Color(255, 255, 255, 120);

public:

    Game()
    {
        pingWindow.create(sf::VideoMode(PING_WINDOW_WIDTH, PING_WINDOW_HEIGHT), "Ping");

        pingWallUp.setFillColor(PING_WALL_COLOR);
        pingWallUp.setSize(PING_WALL_SIZE);
        pingWallUp.setPosition(sf::Vector2f(0, 0));

        pingWallDown.setFillColor(PING_WALL_COLOR);
        pingWallDown.setSize(PING_WALL_SIZE);
        pingWallDown.setPosition(sf::Vector2f(0, PING_WINDOW_HEIGHT - PING_WALL_SIZE.y));

        pingBall = new Ball(PING_WALL_SIZE.y);
        pingBall->setPosition(sf::Vector2f(PING_WINDOW_WIDTH/2 - pingBall->getSize().x/2, PING_WINDOW_HEIGHT/2 - pingBall->getSize().y/2));
        pingBall->setParentWindow(&pingWindow);

        pingPaddleLeft = new Paddle(Paddle::Color::Red, PING_WALL_SIZE.y);
        pingPaddleLeft->setPosition(sf::Vector2f(PING_PADDLE_MARGIN, PING_WINDOW_HEIGHT/2 - pingPaddleLeft->getSize().y/2));
        pingPaddleLeft->setParentWindow(&pingWindow);

        pingPaddleRight = new PaddleController(Paddle::Color::Blue, PING_WALL_SIZE.y);
        pingPaddleRight->setPosition(sf::Vector2f(PING_WINDOW_WIDTH - pingPaddleRight->getSize().x - PING_PADDLE_MARGIN, PING_WINDOW_HEIGHT/2 - pingPaddleRight->getSize().y/2));
        pingPaddleRight->setParentWindow(&pingWindow);

        scoreFont.loadFromFile("resources/Arctik 5.ttf");

        leftScoreText.setFont(scoreFont);
        rightScoreText.setFont(scoreFont);

        leftScoreText.setCharacterSize(PING_SCORE_FONT_SIZE);
        rightScoreText.setCharacterSize(PING_SCORE_FONT_SIZE);

        leftScoreText.setString("0");
        rightScoreText.setString("0");

        leftScoreText.setPosition(PING_WINDOW_WIDTH/2 - 200, 0);
        rightScoreText.setPosition(PING_WINDOW_WIDTH/2 + 100, 0);

        leftScoreText.setColor(PING_SCORE_COLOR);
        rightScoreText.setColor(PING_SCORE_COLOR);
    }

    void start()
    {
        while(pingWindow.isOpen())
        {
            processEvents();
            update();
            render();
        }
    }

private:

    void processEvents()
    {
        sf::Event event;
        while(pingWindow.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed:
                pingWindow.close();
                break;
            case sf::Event::KeyPressed:
                handleKeyboard(event.key.code, true);
                break;
            case sf::Event::KeyReleased:
                handleKeyboard(event.key.code, false);
                break;
            default:
                break;
            }
        }
    }

    void update()
    {
        if(checkCollision(pingPaddleLeft->getBounds(), pingBall->getBounds()))
            pingBall->setHasCollidedPaddle(true, pingPaddleLeft->getVelocity());
        if(checkCollision(pingPaddleRight->getBounds(), pingBall->getBounds()))
            pingBall->setHasCollidedPaddle(true, pingPaddleRight->getVelocity());

        if(pingState == GameState::Running)
            pingPaddleRight->autoMove(pingBall->getY(), randomClock1.restart().asMicroseconds() + randomClock2.restart().asMicroseconds());

        if(pingBall->getX() <= 0 || (pingBall->getX() >= PING_WINDOW_WIDTH + pingBall->getSize().x))
        {
            numPingGames++;
            pingState = GameState::Paused;
            pingBall->getX() <= 0 ? rightScore++ : leftScore++;

            delete pingBall;
            pingBall = nullptr;
            pingBall = new Ball(PING_WALL_SIZE.y);
            pingBall->setPosition(sf::Vector2f(PING_WINDOW_WIDTH/2 - pingBall->getSize().x/2, PING_WINDOW_HEIGHT/2 - pingBall->getSize().y/2));
            pingBall->setParentWindow(&pingWindow);

            if(leftScore < 10 || rightScore < 10)
                pingBall->setHNegator(((numPingGames % 4) == 0 || (numPingGames % 4) == 1) ? 1 : -1);
            else
                pingBall->setHNegator(((numPingGames % 2) == 0) ? 1 : -1);

            intToString << leftScore;
            leftScoreText.setString(intToString.str());
            intToString.str("");

            intToString << rightScore;
            rightScoreText.setString(intToString.str());
            intToString.str("");
        }
        if(pingState == GameState::Running)
        {
            pingBall->run();
        }
        pingPaddleLeft->run();
        pingPaddleRight->run();
    }

    void render()
    {
        pingWindow.clear(sf::Color(128, 128,128));

        pingWindow.draw(pingWallUp);
        pingWindow.draw(pingWallDown);

        pingBall->draw();
        pingPaddleLeft->draw();
        pingPaddleRight->draw();

        pingWindow.draw(leftScoreText);
        pingWindow.draw(rightScoreText);

        pingWindow.display();
    }

    void handleKeyboard(sf::Keyboard::Key key, bool isPressed)
    {
        switch(key)
        {
        case sf::Keyboard::W:
            pingPaddleLeft->setIsMovingUp(isPressed);
            break;
        case sf::Keyboard::S:
            pingPaddleLeft->setIsMovingDown(isPressed);
            break;
        //case sf::Keyboard::Up:
        //    pingPaddleRight->setIsMovingUp(isPressed);
        //    break;
        //case sf::Keyboard::Down:
        //    pingPaddleRight->setIsMovingDown(isPressed);
        //    break;
        case sf::Keyboard::Space:
            if((pingState == GameState::Paused))
            {
                pingState = GameState::Running;
            }
            break;
        default:
            break;
        }
    }

    bool checkCollision(sf::FloatRect rect1, sf::FloatRect rect2)
    {
        return rect1.intersects(rect2);
    }
};
