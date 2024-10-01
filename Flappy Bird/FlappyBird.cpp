#include "FlappyBird.hpp"

#include <SFML/Graphics.hpp> 
#include <iostream>
#include <vector>

class Bird {
public: 
	sf::RectangleShape rectangle;
	sf::Vector2f position; 
	float velocityY; 
	bool jumped;
	sf::Texture texture; 
	sf::Texture animation;

	Bird(sf::RenderWindow& window) {
		rectangle.setSize(sf::Vector2f(100.0f, 100.0f));
		rectangle.setOrigin(0, rectangle.getSize().y);

		position.x = window.getSize().x / 2 - rectangle.getSize().x / 2;
		position.y = window.getSize().y / 2 + rectangle.getSize().x / 2;
		velocityY = 0; 
		jumped = false; 

		rectangle.setPosition(position);

		if (!texture.loadFromFile("assets/flappyBird.png")) {
			return;
		}
		else {
			rectangle.setTexture(&texture);
		}

		if (!animation.loadFromFile("assets/flappyBirdAnimation.png")) {
			return;
		}
	}

	void colorBird(const float timeSinceLastJump) {

		// changes texture during jump
		if (timeSinceLastJump <= .15 && jumped == true && rectangle.getTexture() != &animation) {
			rectangle.setTexture(&animation);
		}
		else if (timeSinceLastJump >= .15 && rectangle.getTexture() != &texture) {
			rectangle.setTexture(&texture);
		}
	}

	void moveBird(const float gravity) {
		position.y += velocityY;

		if (position.y >= 600) {
			position.y = 600;
			velocityY = 0;
		} // if it is touching roof, stop its velocity
		else if (position.y < rectangle.getSize().y) {
			position.y = rectangle.getSize().y;
			velocityY = 0;
		}
		else { // if it is neither on ground or touching roof, velocity is being affected by gravity
			velocityY += gravity;
		}

		rectangle.setPosition(position.x, position.y);
	}

};

class Pipes {
public:
	std::vector<sf::RectangleShape> pipesList; 
	float velocity; 
	sf::Texture textureTop;
	sf::Texture textureLow; 

	Pipes() {
		velocity = -3.0f;
		if (!textureTop.loadFromFile("assets/topPipe.png")) {
			return;
		}

		if (!textureLow.loadFromFile("assets/lowPipe.png")) {
			std::cout << "biag"; 
			return;
		}
	}

	void generateRandomPipe(float& timeSinceLastNewPipe) {
		float width = 180; 
		float distanceYPipe = 250; 
		float distanceXPipe = 300; 
		float curDistanceXPipe = 0; 

		if (pipesList.size() >= 1) {
			curDistanceXPipe = 800 - (pipesList[pipesList.size() - 1]).getPosition().x - 200;
		} else {
			curDistanceXPipe = 1000; 
		}
		
		if (curDistanceXPipe > distanceXPipe) {
			srand((unsigned)time(NULL)); 
			float height = 50 + (100 * (rand() % 3)); // between 50 and 350

			sf::RectangleShape newTopPipe(sf::Vector2f(width, height)); 
			newTopPipe.setPosition(800, 0); 
			newTopPipe.setTexture(&textureTop); 


			sf::RectangleShape newLowPipe(sf::Vector2f(width, 600 - (height + distanceYPipe))); 
			newLowPipe.setPosition(800, height + distanceYPipe);
			newLowPipe.setTexture(&textureLow);


			pipesList.push_back(newTopPipe);
			pipesList.push_back(newLowPipe); 

			timeSinceLastNewPipe = 0; 
		}
	}

	bool ballTouchesPipe(Bird& ball) {
		for (int i = 0; i < pipesList.size(); i++) {
			if (ball.rectangle.getGlobalBounds().intersects(pipesList[i].getGlobalBounds())) {
				return true;
			}
		}
		return false; 
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

void drawBackground(sf::RenderWindow& window) {
	sf::Texture texture;
	if (!texture.loadFromFile("assets/bg.png")) {
		return;
	}

	float scaleX = static_cast<float>(window.getSize().x) / texture.getSize().x;
	float scaleY = static_cast<float>(window.getSize().y) / texture.getSize().y;


	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale(scaleX, scaleY); 

	window.draw(sprite);
}

void gameScreen(sf::RenderWindow& window) {
	// initializing circle
	Bird bird(window);
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
			bird.jumped = true;
			bird.velocityY = initialJumpVelocity;
		}

		bird.colorBird(timeSinceLastJump);
		bird.moveBird(gravity);

		pipes.movePipes(); 
		pipes.generateRandomPipe(timeSinceLastNewPipe); 
		if (pipes.ballTouchesPipe(bird)) {
			return; 
		}

		window.clear();
		drawBackground(window); 
		window.draw(bird.rectangle);
		pipes.drawPipes(window); 
		window.display();
	}
}

void startScreen(sf::RenderWindow& window) {

	sf::Font font;
	if (!font.loadFromFile("C:/Users/maxwe/source/repos/TicTacToe Redo/TicTacToe Redo/fonts/semibold.ttf")) {
		std::cerr << "Failed to load font" << std::endl;
		return; 
	}

	sf::Text text("Press left click to play.", font); 
	text.setCharacterSize(24); 

	sf::FloatRect textRect = text.getLocalBounds(); 
	text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f); 
	text.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (event.mouseButton.button == sf::Mouse::Left) {
			return;
		}

		window.clear(); 
		window.draw(text);
		window.display();
	}
}