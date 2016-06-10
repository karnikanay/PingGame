#ifndef __WindowObject__
#define __WindowObject__

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

class WindowObject
{

protected:

    sf::RenderWindow* parentWindow;

public:

    void setParentWindow(sf::RenderWindow* newParentWindow)
    {
        parentWindow = newParentWindow;
    }

};

#endif // __WindowObject__
