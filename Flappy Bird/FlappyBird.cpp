#include "FlappyBird.hpp"

#include <SFML/Graphics.hpp> 
#include <iostream>
#include <vector>

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

	void moveBall(const float gravity) {
		position.y += velocityY;

		if (position.y >= 600) {
			position.y = 600;
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

class Pipes {
public:
	std::vector<sf::RectangleShape> pipesList; 
	float velocity; 

	Pipes() {
		velocity = -3.0f;
	}

	void generateRandomPipe(float& timeSinceLastNewPipe) {
		float width = 180; 
		float gapBetweenPipes = 250; 

		if (timeSinceLastNewPipe > 2.2f) {
			srand((unsigned)time(NULL)); 
			float height = 50 + (100 * (rand() % 3)); // between 50 and 350

			sf::RectangleShape newTopPipe(sf::Vector2f(width, height)); 
			newTopPipe.setPosition(800, 0); 

			sf::RectangleShape newLowPipe(sf::Vector2f(width, 600)); 
			newLowPipe.setPosition(800, height + gapBetweenPipes); 

			pipesList.push_back(newTopPipe);
			pipesList.push_back(newLowPipe); 

			timeSinceLastNewPipe = 0; 
		}
	}

	void ballTouchesPipe(BouncingBall& ball) {
		for (int i = 0; i < pipesList.size(); i++) {
			if (ball.circle.getGlobalBounds().intersects(pipesList[i].getGlobalBounds())) {
				std::cout << "biag"; 
			}
		}
	}

	void movePipes() {
		for (int i = 0; i < pipesList.size(); i++) {
			sf::RectangleShape& curPipe = pipesList[i]; 
			sf::Vector2f newPosition(curPipe.getPosition().x + velocity, curPipe.getPosition().y);
			pipesList[i].setPosition(newPosition); 
		}
	}

	void drawPipes(sf::RenderWindow& window) {
		for (int i = 0; i < pipesList.size(); i++) {
			window.draw(pipesList[i]);
		}
	}
};

void gameLoop(sf::RenderWindow& window) {
	// initializing circle
	BouncingBall ball(window);
	Pipes pipes; 

	// defining gravity, velocity, and initial jump velocity variables
	const float gravity = +1.0f;
	const float initialJumpVelocity = -12.0f;

	// defining jump cooldown variables
	float jumpCooldown = .15f;
	float timeSinceLastJump = 0.0f;

	// pipe cooldown variables
	float timeSinceLastNewPipe = 0.0f;



	sf::Clock ballClock;
	sf::Clock pipeClock;
	while (window.isOpen()) {
		float ballDeltaTime = ballClock.restart().asSeconds();
		timeSinceLastJump += ballDeltaTime;

		float pipeDeltaTime = pipeClock.restart().asSeconds(); 
		timeSinceLastNewPipe += pipeDeltaTime; 

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
		ball.moveBall(gravity); 

		pipes.ballTouchesPipe(ball); 
		pipes.movePipes(); 
		pipes.generateRandomPipe(timeSinceLastNewPipe); 

		window.clear();
		window.draw(ball.circle);
		pipes.drawPipes(window); 
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