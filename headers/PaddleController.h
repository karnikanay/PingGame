#include "Paddle.h"

class PaddleController
: public Paddle
{
public:

    PaddleController()
    {

    }

    PaddleController(Paddle::Color paddleColor, float newWallMargin)
    : Paddle(paddleColor, newWallMargin)
    {

    }

    void autoMove(float ballY, int randomInt)
    {
        std::cout << "randomInt mod 3 = " << randomInt % 3 << std::endl;
        if (ballY > obPaddle.getPosition().y)
        {
            setIsMovingDown(true && (randomInt % 3 != 0));
            setIsMovingUp(false);
        }
        else
        {
            setIsMovingUp(true && (randomInt % 3 != 0));
            setIsMovingDown(false);
        }

    }
};
