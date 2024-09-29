#include "FlappyBird.hpp"

#include <SFML/Graphics.hpp> 
#include <iostream>

class BouncingBall {
public: 
	sf::CircleShape circle;
	sf::Vector2f position; 
	float velocityY; 
	bool jumped;


	BouncingBall(sf::RenderWindow& window) {
		circle.setRadius(50);
		circle.setOrigin(0, circle.getRadius() * 2);

		position.x = window.getSize().x / 2 - circle.getRadius();
		position.y = window.getSize().y / 2 + circle.getRadius();
		velocityY = 0; 
		jumped = false; 

		circle.setPosition(position); 
	}

	void colorBall(const float timeSinceLastJump) {
		// sets the color to be red after jump
		if (timeSinceLastJump <= .15 && jumped == true && circle.getFillColor() != sf::Color::Red) {
			circle.setFillColor(sf::Color::Red);
		}
		else if (timeSinceLastJump >= .15 && circle.getFillColor() != sf::Color::White) {
			circle.setFillColor(sf::Color::White);
		}
	}

	void moveBall(const float gravity, const float groundHeight) {
		position.y += velocityY;

		if (position.y >= groundHeight) {
			position.y = groundHeight;
			velocityY = 0;
		} // if it is touching roof, stop its velocity
		else if (position.y < circle.getRadius() * 2) {
			position.y = circle.getRadius() * 2;
			velocityY = 0;
		}
		else { // if it is neither on ground or touching roof, velocity is being affected by gravity
			velocityY += gravity;
		}

		circle.setPosition(position.x, position.y);
	}

};

void gameLoop(sf::RenderWindow& window) {
	// initializing circle
	BouncingBall ball(window);

	// defining gravity, velocity, and initial jump velocity variables
	const float gravity = +1.0f;
	const float initialJumpVelocity = -12.0f;

	// defining jump cooldown variables
	float jumpCooldown = .15f;
	float timeSinceLastJump = 0.0f;

	// defining ground
	sf::RectangleShape rectangle(sf::Vector2f(800, 100));
	const float groundHeight = 500; // 100 above the bottom
	rectangle.setPosition(0, groundHeight);

	sf::Clock clock;
	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();
		timeSinceLastJump += deltaTime;

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && timeSinceLastJump > jumpCooldown) {
			timeSinceLastJump = 0;
			ball.jumped = true;
			ball.velocityY = initialJumpVelocity;
		}

		ball.colorBall(timeSinceLastJump); 
		ball.moveBall(gravity, groundHeight); 

		window.clear();
		window.draw(ball.circle);
		window.draw(rectangle);
		window.display();
	}
}

void drawBackground() {
	sf::Texture texture;
	if (!texture.loadFromFile("C:/Users/maxwe/source/repos/SFML images app/billyman.png")) {
		return;
	}

	sf::Sprite sprite; 
	sprite.setTexture(texture);
}