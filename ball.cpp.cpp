#include <SFML/Graphics.hpp>
#include <optional>
using namespace std;

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

void drawWindow(sf::RenderWindow& window, Ball& ball)
{
    window.clear(sf::Color::Blue);  // background colour only for test 
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

    Ball ball(20.0f, width / 2.f, groundY);
    Camera camera((float)width, (float)height);

    while (window.isOpen())
    {
        handleEvents(window);
        handleInput(ball, jumpStrength);
        applyGravity(ball, gravity);
        moveBall(ball);
        resetBallOnGround(ball, groundY);
        updateCamera(camera, ball, width, height);

        window.setView(camera.getView());
        drawWindow(window, ball);
    }

    return 0;
}
