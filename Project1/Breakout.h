/**
 *	@file breakout.h
 *	@author Chris Tio
 */
 #ifndef BREAKOUT_H_
#define BREAKOUT_H_
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

// The brick structure and functions
struct Brick
{
	sf::RectangleShape shape;
	bool visible; // brick disappears when collision with ball occurs
};

// The paddle structure
struct Paddle
{
	sf::RectangleShape shape;
};

// The Ball structure
struct Ball
{
	sf::CircleShape shape;
	Vector2f velocity;
};

struct Game
{
	sf::RenderWindow * window = nullptr;
	std::vector<Brick> bricks;
	Paddle paddle;
	Ball ball;
	int turnsLeft;
};

void init(Game& game, sf::RenderWindow * win);
void run(Game& game);

/** Width of application window in pixels. */
const float APP_WIDTH = 400;
/** Height of application window in pixels. */
const float APP_HEIGHT = 600;

/** Dimensions of the paddle. */
const float PADDLE_WIDTH = 60;
const float PADDLE_HEIGHT = 10;

/** Offset of the paddle up from the bottom. */
const float PADDLE_Y_OFFSET = 30;

/** Speed of the paddle */
const float PADDLE_VELOCITY = 6;

/** Number of bricks per row. */
const int NBRICKS_PER_ROW = 10;

/** Number of rows of bricks. */
const int NBRICK_ROWS = 10;

/** Separation between bricks. */
const float BRICK_SEP = 4;

/** Width of a brick. */
const float BRICK_WIDTH =
(APP_WIDTH - (NBRICKS_PER_ROW - 1) * BRICK_SEP)
/ NBRICKS_PER_ROW;

/** Height of a brick. */
const float BRICK_HEIGHT = 8;

/** Radius of the ball in pixels. */
const float BALL_RADIUS = 10;

/** Velocity of the ball */
const float BALL_VELOCITY = 4;

/** Offset of the top brick row from the top. */
const float BRICK_Y_OFFSET = 70;

/** Number of turns. */
const int NTURNS = 3;

#endif /* BREAKOUT_H_ */
#pragma once
