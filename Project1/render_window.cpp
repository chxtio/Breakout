/**
	Opening and managing a SFML window
	https://www.sfml-dev.org/tutorials/2.5/window-window.php
*/
#include <SFML/Graphics.hpp>

int main()
{
	// Create the main rendering window
	sf::RenderWindow App(sf::VideoMode(800, 600, 32), "SFML Graphics");

	// Start game Loop- run program as long as the window is open
	while (App.isOpen())
	{
		// Check all the window's events that were triggered since last iteration of the loop
		sf::Event event;
		while (App.pollEvent(event))
		{
			// "Close requested" event: close window
			if (event.type == sf::Event::Closed)
				App.close();

		}

		// Clear the screen (fill it with red color)
		App.clear(sf::Color(200, 0, 0));

		// Display window contents on screen
		App.display();
	}

	return 0;

}

