#ifndef FLAPPYBIRD_HPP
#define FLAPPYBIRD_HPP

#include <SFML/Graphics.hpp> 

/*
* i don't know why this causes an error
class BouncingBall {
public:
	sf::CircleShape circle;
	sf::Vector2f position;
	float velocityY;
	bool jumped;

	BouncingBall(sf::RenderWindow& window);
	void colorBall(const float& timeSinceLastJump);
	void moveBall(const float& gravity, const float& groundHeight);
}; */ 

void gameLoop(sf::RenderWindow& window);


#endif
