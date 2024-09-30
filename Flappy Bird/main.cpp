#include <SFML/Graphics.hpp> 
#include <iostream>
#include "FlappyBird.hpp" 

// todo: 
// delete floor and change it to rectangles that move left
// change game screen once ball hits pipe
// make pipes infinitely generating
// give pipes a texture
// create a background


int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Flappy Bird");
	window.setFramerateLimit(60);

	while (window.isOpen()) {
		gameLoop(window); 
	}
}