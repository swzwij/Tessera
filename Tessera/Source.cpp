#include <SFML/Graphics.hpp>
#include <cmath>
#include "Vector3.h"
#include "Matrix4x4.h"
#include <iostream>

class TesseraEngine
{
    MatrixOperations matrixUtil;
    VectorOperations vectorUtil;

    sf::RenderWindow window;

    Matrix4x4 projectionMatrix;
    Vector3 cameraPosition;
    Vector3 cameraRotation;

    float cameraYaw = 0.0f;
    float cameraPitch = 0.0f;
    float zoomFactor = 3.0f;

    float fTheta = 0.0f;

    sf::Vector2i lastMousePosition;
    bool isMousePressed = false;
    bool isWireframe = true;

public:
    TesseraEngine() : matrixUtil(), vectorUtil(), window(sf::VideoMode(800, 600), "Tessera Engine")
    {
        window.create(sf::VideoMode(800, 600), "Tessera Engine");

        window.setFramerateLimit(60);

        lastMousePosition = sf::Mouse::getPosition(window);

        float fNear = 0.1f;
        float fFar = 1000.0f;
        float fFov = 90.0f;
        float fAspectRatio = (float)window.getSize().y / (float)window.getSize().x;
        float fFovRad = 1.0f / tanf((fFov * 0.5f) * (3.14159f / 180.0f));

        projectionMatrix.elements[0][0] = fAspectRatio * fFovRad;
        projectionMatrix.elements[1][1] = fFovRad;
        projectionMatrix.elements[2][2] = fFar / (fFar - fNear);
        projectionMatrix.elements[3][2] = (-fFar * fNear) / (fFar - fNear);
        projectionMatrix.elements[2][3] = 1.0f;
        projectionMatrix.elements[3][3] = 0.0f;
    }

    void Start()
    {
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::MouseWheelScrolled)
                {
                    if (event.mouseWheelScroll.delta > 0)
                        zoomFactor -= 1.0f;
                    else
                        zoomFactor += 1.0f;
                    if (zoomFactor < 1.0f)
                        zoomFactor = 1.0f;
                }

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    isMousePressed = true;
                    lastMousePosition = sf::Mouse::getPosition(window);
                }

                if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                {
                    isMousePressed = false;
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                {
                    isWireframe = !isWireframe;
                }
            }

            Update();
            Draw();
        }
    }

    void Update()
    {
        if (isMousePressed)
        {
            sf::Vector2i currentMousePosition = sf::Mouse::getPosition(window);
            sf::Vector2i mouseDelta = currentMousePosition - lastMousePosition;

            cameraYaw -= -mouseDelta.x * 0.005f;
            cameraPitch -= mouseDelta.y * 0.005f;

            if (cameraPitch > 1.5f)
                cameraPitch = 1.5f;
            if (cameraPitch < -1.5f)
                cameraPitch = -1.5f;

            lastMousePosition = currentMousePosition;
        }
    }

    void Draw()
    {
        window.clear();

        Vector3 cubeVertices[8] =
        {
            { -1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            { -1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f }
        };

        int cubeEdges[12][2] =
        {
            { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 0 }, // Back face
            { 4, 5 }, { 5, 6 }, { 6, 7 }, { 7, 4 }, // Front face
            { 0, 4 }, { 1, 5 }, { 2, 6 }, { 3, 7 }  // Connecting edges
        };

        int cubeFaces[6][4] =
        {
            { 0, 1, 2, 3 }, // Back
            { 4, 5, 6, 7 }, // Front
            { 0, 1, 5, 4 }, // Bottom 
            { 2, 3, 7, 6 }, // Top 
            { 0, 3, 7, 4 }, // Left 
            { 1, 2, 6, 5 }  // Right 
        };

        Matrix4x4 rotationMatrix = matrixUtil.MakeRotationY(fTheta);
        Matrix4x4 worldMatrix = rotationMatrix;

        cameraPosition =
        {
            zoomFactor * cosf(cameraPitch) * sinf(cameraYaw),
            zoomFactor * sinf(cameraPitch),
            zoomFactor * cosf(cameraPitch) * cosf(cameraYaw)
        };

        Vector3 up = { 0, 1, 0 };
        Vector3 cameraTarget = { 0, 0, 0 };
        Matrix4x4 cameraMatrix = matrixUtil.PointAt(cameraPosition, cameraTarget, up);

        Matrix4x4 viewMatrix = matrixUtil.QuickInverse(cameraMatrix);

        for (int i = 0; i < 8; i++)
        {
            cubeVertices[i] = matrixUtil.MultiplyVector(worldMatrix, cubeVertices[i]);
            cubeVertices[i] = matrixUtil.MultiplyVector(viewMatrix, cubeVertices[i]);
            cubeVertices[i] = matrixUtil.MultiplyVector(projectionMatrix, cubeVertices[i]);

            if (cubeVertices[i].z != 0)
            {
                cubeVertices[i].x /= cubeVertices[i].z;
                cubeVertices[i].y /= cubeVertices[i].z;
            }

            cubeVertices[i] = vectorUtil.Multiply(cubeVertices[i], 100.0f);

            cubeVertices[i].x += 400;
            cubeVertices[i].y += 300;
        }

        if (isWireframe)
        {
            for (int i = 0; i < 12; i++)
            {
                Vector3 v1 = cubeVertices[cubeEdges[i][0]];
                Vector3 v2 = cubeVertices[cubeEdges[i][1]];

                sf::Vertex line[] =
                {
                    sf::Vertex(sf::Vector2f(v1.x, v1.y)),
                    sf::Vertex(sf::Vector2f(v2.x, v2.y))
                };

                window.draw(line, 2, sf::Lines);
            }
        }
        else
        {
            for (int i = 0; i < 6; i++)
            {
                Vector3 v1 = cubeVertices[cubeFaces[i][0]];
                Vector3 v2 = cubeVertices[cubeFaces[i][1]];
                Vector3 v3 = cubeVertices[cubeFaces[i][2]];
                Vector3 v4 = cubeVertices[cubeFaces[i][3]];

                sf::Vertex quad[] =
                {
                    sf::Vertex(sf::Vector2f(v1.x, v1.y)),
                    sf::Vertex(sf::Vector2f(v2.x, v2.y)),
                    sf::Vertex(sf::Vector2f(v3.x, v3.y)),
                    sf::Vertex(sf::Vector2f(v4.x, v4.y))
                };

                window.draw(quad, 4, sf::Quads);
            }
        }

        window.display();
    }
};

int main()
{
    TesseraEngine engine;
    engine.Start();

    return 0;
}
