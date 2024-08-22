#include "SFML/Graphics.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Tessera");
	sf::Event event;

	sf::CircleShape circle(100);
	circle.setFillColor(sf::Color::Red);
	circle.setPosition(100, 100);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}	