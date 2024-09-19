#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

void drawCircle(float radius, int segments)
{
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= segments; i++)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = radius * cosf(theta);
        float y = radius * sinf(theta);
        glVertex2f(x, y);
    }
    glEnd();
}

void setProjection(int width, int height)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    if (width > height) {
        glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
    }
    else {
        glOrtho(-1.0, 1.0, -1.0 / aspectRatio, 1.0 / aspectRatio, -1.0, 1.0);
    }
}

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::Window window(sf::VideoMode(800, 600), "SFML window with OpenGL", sf::Style::Default, settings);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glViewport(0, 0, window.getSize().x, window.getSize().y);
    setProjection(window.getSize().x, window.getSize().y);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, event.size.width, event.size.height);
                setProjection(event.size.width, event.size.height);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        drawCircle(0.5f, 100);
        window.display();
    }

    return 0;
}