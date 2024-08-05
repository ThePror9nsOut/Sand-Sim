#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <sstream>

struct PositionHash {
    std::size_t operator()(const sf::Vector2f& pos) const {
        return std::hash<float>()(pos.x) ^ (std::hash<float>()(pos.y) << 1);
    }
};

struct PositionEqual {
    bool operator()(const sf::Vector2f& lhs, const sf::Vector2f& rhs) const {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 700), "0");
    sf::Clock deltaClock;
    sf::Clock updClock;

    const int parSize = 10;

    sf::RectangleShape pShape(sf::Vector2f(parSize, parSize));
    pShape.setFillColor(sf::Color(255, 255, 255));

    const float updDelay = 0.01f;

    sf::VertexArray particles(sf::Points);
    std::unordered_set<sf::Vector2f, PositionHash, PositionEqual> particleSet;

    sf::Time dt;
    int frameCount = 0;
    float timeElapsed = 0.0f;

    while (window.isOpen())
    {
        sf::Time dt = deltaClock.restart();
        float deltaTime = dt.asSeconds();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f gridMouse = sf::Vector2f(mousePos.x - mousePos.x % parSize, mousePos.y - mousePos.y % parSize);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && particleSet.find(gridMouse) == particleSet.end())
        {
            particles.append(sf::Vertex(gridMouse));
            particleSet.insert(gridMouse);
        }

        if (updClock.getElapsedTime().asSeconds() > updDelay)
        {
            window.clear(sf::Color::Black);

            for (size_t i = 0; i < particles.getVertexCount(); i++)
            {
                sf::Vector2f& pos = particles[i].position;
                pShape.setPosition(pos);

                if (pos.y < 700 - parSize)
                {
                    sf::Vector2f newPos = pos;

                    if (particleSet.find(sf::Vector2f(pos.x, pos.y + parSize)) == particleSet.end() &&
                        pos.y + parSize < 700)
                    {
                        newPos.y += parSize;
                    }
                    else if (particleSet.find(sf::Vector2f(pos.x - parSize, pos.y + parSize)) == particleSet.end() &&
                             pos.x - parSize >= 0 && pos.y + parSize < 700)
                    {
                        newPos.x -= parSize;
                        newPos.y += parSize;
                    }
                    else if (particleSet.find(sf::Vector2f(pos.x + parSize, pos.y + parSize)) == particleSet.end() &&
                             pos.x + parSize < 1400 && pos.y + parSize < 700)
                    {
                        newPos.x += parSize;
                        newPos.y += parSize;
                    }

                    if (newPos != pos)
                    {
                        particleSet.erase(pos);
                        particleSet.insert(newPos);
                        pos = newPos;
                    }
                }

                window.draw(pShape);
            }

            updClock.restart();
            window.display();
        }

        frameCount++;
        timeElapsed += deltaTime;

        if (timeElapsed >= 1.0f) {
            float fps = frameCount / timeElapsed;
            std::stringstream ss;
            ss << fps;
            window.setTitle(ss.str());
            frameCount = 0;
            timeElapsed = 0.0f;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
