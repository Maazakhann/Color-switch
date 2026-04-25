#include <SFML/Graphics.hpp>
#include <optional>
#include <cstdlib>
#include <ctime>
using namespace std;
class Shape
{
public:
    virtual void draw(sf::RenderWindow& window) = 0; // pure virtual
    virtual ~Shape() {}
};
class CircleShapeObj : public Shape
{
private:
    sf::CircleShape shape;

public:
    CircleShapeObj(float x, float y, sf::Color color)
    {
       
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(30.f);
        shape.setRadius(150.f);
        shape.setOrigin({ 150.f, 150.f });
        shape.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};
class RectangleShapeObj : public Shape
{
private:
    sf::RectangleShape shape;

public:
    RectangleShapeObj(float x, float y, sf::Color color)
    {
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(30.f);
        shape.setSize({ 250.f, 250.f });
        shape.setOrigin({ 125.f, 125.f });
        shape.setPosition({ x, y });
    }
    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};
class TriangleShapeObj : public Shape
{
private:
    sf::CircleShape shape;

public:
    TriangleShapeObj(float x, float y, sf::Color color)
    {
   
        shape = sf::CircleShape(200.f, 3);
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(color);
        shape.setOutlineThickness(30.f);
        shape.setOrigin({ 200.f, 200.f });
        shape.setPosition({ x, y });
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(shape);
    }
};
class Ball
{
private:
    sf::CircleShape circle;
    float velocityY;

public:
    Ball(float radius, float startX, float startY)
    {
        circle.setRadius(radius);
        circle.setFillColor(sf::Color(128, 213, 111));
        circle.setOrigin({ radius, radius });
        circle.setPosition({ startX, startY });
        velocityY = 0.f;
    }
    // functions for ball positioning 

    void draw(sf::RenderWindow& window)
    {
        window.draw(circle);
    }


    void setPosition(float x, float y)
    {
        circle.setPosition({ x, y });
    }

    sf::Vector2f getPosition() const
    {
        return circle.getPosition();
    }

    // functions for ball movement

    void move(float dx, float dy)
    {
        circle.move({ dx, dy });
    }

    void setVelocityY(float v)
    {
        velocityY = v;
    }

    float getVelocityY() const
    {
        return velocityY;
    }

    void addVelocityY(float value)
    {
        velocityY += value;
    }
};

class Camera
{
private:
    sf::View view;
    float cameraVelocity;
    float cameraFollowSpeed;

public:
    Camera(float width, float height)
        : view(sf::FloatRect({ 0.f, 0.f }, { width, height }))
    {
        cameraVelocity = 0.f;
        cameraFollowSpeed = 0.05f;
    }

    sf::View& getView()
    {
        return view;
    }

    // functions for camera setup and movement
    float getCameraVelocity() const
    {
        return cameraVelocity;
    }

    void setCameraVelocity(float v)
    {
        cameraVelocity = v;
    }

    float getCameraFollowSpeed() const
    {
        return cameraFollowSpeed;
    }

    void move(float dx, float dy)
    {
        view.move({ dx, dy });
    }

    void setCenter(float x, float y)
    {
        view.setCenter({ x, y });
    }

    sf::Vector2f getCenter() const
    {
        return view.getCenter();
    }
};

void handleEvents(sf::RenderWindow& window)
{
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

void handleInput(Ball& ball, float jumpStrength)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space))
    {
        ball.setVelocityY(jumpStrength);
    }
}

void applyGravity(Ball& ball, float gravity)
{
    ball.addVelocityY(gravity);   // adding gravity to ball speed each frame
}

void moveBall(Ball& ball)
{
    ball.move(0.f, ball.getVelocityY());
}

void resetBallOnGround(Ball& ball, float groundY)
{
    // rest the ball if it reached the ground
    if (ball.getPosition().y >= groundY)
    {
        ball.setPosition(ball.getPosition().x, groundY);
        ball.setVelocityY(0.f);
    }
}

void updateCamera(Camera& camera, Ball& ball, int width, int height)
{
    float triggerLine = camera.getCenter().y;   // where the view started to follow the ball

    if (ball.getPosition().y < triggerLine)
    {
        float diff = ball.getPosition().y - triggerLine;   // distance between center of window and the ball
        camera.setCameraVelocity(diff * camera.getCameraFollowSpeed());   // converts the distance into a movement speed
    }
    else
    {
        camera.setCameraVelocity(0.f);   // if ball is below the center the camera doesnt follow it
    }

    camera.move(0.f, camera.getCameraVelocity());

    // stop camera below starting point
    if (camera.getCenter().y > height / 2.f)
    {
        camera.setCenter(width / 2.f, height / 2.f);
        camera.setCameraVelocity(0.f);
    }
}

void drawWindow(sf::RenderWindow& window, Ball& ball, Shape* shapes[], int shapeCount)
{
    window.clear(sf::Color::Blue);  // background colour only for test 

    for (int i = 0; i < shapeCount; i++)
    {
        shapes[i]->draw(window);
    }
    ball.draw(window);
    window.display();
}

int main()
{
    const int width = 800;
    const int height = 600;
    const float groundY = 580.f;   // where the ball is placed initaially

    float gravity = 0.5f;          // speed of gravity
    float jumpStrength = -6.f;     // speed gained by ball on pressing spacebar

    sf::RenderWindow window(sf::VideoMode({ width, height }), "SFML Window");
    window.setFramerateLimit(60);
    srand(time(0));
    float centerX = width / 2.f;

    float x1 = centerX;
    float x2 = centerX;
    float x3 = centerX;

    float baseY = 100.f;
    float gap = 700.f;

    float y1 = baseY + (rand() % 50);
    float y2 = baseY - gap + (rand() % 50);
    float y3 = baseY - 2 * gap + (rand() % 50);
    Ball ball(20.0f, width / 2.f, groundY);
    Camera camera((float)width, (float)height);
    CircleShapeObj circle1(x1, y1, sf::Color::Red);
    RectangleShapeObj square1(x2, y2, sf::Color::Green);
    TriangleShapeObj triangle1(x3, y3, sf::Color::Yellow);

    Shape* shapes[3] = {
        &circle1,
        &square1,
        &triangle1,
      
    };

    while (window.isOpen())
    {
        handleEvents(window);
        handleInput(ball, jumpStrength);
        applyGravity(ball, gravity);
        moveBall(ball);
        resetBallOnGround(ball, groundY);
        updateCamera(camera, ball, width, height);

        window.setView(camera.getView());
        drawWindow(window, ball,shapes,3);
    }

    return 0;
}
