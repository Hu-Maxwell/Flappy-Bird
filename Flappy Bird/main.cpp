#include <SFML/Graphics.hpp> 
#include <iostream>
#include "FlappyBird.hpp" 

// todo: 
// check - is the 2 clock system ineffcient? 
// instead of using clock, use distance from right of screen
// change game screen once ball hits pipe
// give everything a texture
// create a background


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Bird");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		startScreen(window); 
		gameScreen(window); 
	}
}