#include <SFML/Graphics.hpp> 
#include <iostream>
#include "FlappyBird.hpp" 

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Bird");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		startScreen(window); 
		gameScreen(window); 
	}
}