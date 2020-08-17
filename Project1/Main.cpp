/**
 *  @file breakout.cpp
 *  @author your name and id goes here
 *  @version your day and time goes here
 */
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace sf;

#include "breakout.h"

/**
#if __cplusplus <= 199711L
template <typename T>
string toString(const T& val)
{
	ostringstream out;
	out << val;
	return out.str();
}
#endif
*/

int main()
{
	// Create the main rendering window
	RenderWindow App(VideoMode(APP_WIDTH, APP_HEIGHT), // mode
		"SFML Graphics");
	Game game;
	init(game, &App);
	run(game);
}

static Brick makeBrick(float x, float y, Color color);
static void initBricks(vector<Brick>&); // local prototype
static void initPaddle(Paddle& paddle);
static void initBall(Ball& ball);

void init(Game& game, RenderWindow * App)
{
	// Initialize Game window
	game.window = App; 
	game.window->setFramerateLimit(60);
	// Initialize other game components
	initBricks(game.bricks);
	initPaddle(game.paddle);
	initBall(game.ball);
	game.turnsLeft = NTURNS;
	// game.window->setTitle("Breakout - " + toString(NTURNS) + " turns left");

	game.window->setTitle("Breakout Game");
}
/**
	Private non-class methods for running the game.
*/
static void processEvents(Game& game);
static void update(Game& game);
static void render(Game& game);

/**
	Runs the game loop
	@param g the initialized game
*/
void run(Game& g)
{
	while (g.window->isOpen())
	{
		processEvents(g);
		update(g);
		render(g);
	}
}
/**
	Processes events
	@param g the game.
*/
static void processEvents(Game& g)
{
	// Check all the window's events that were triggered since last iteration of the loop
	Event e;
	while (g.window->pollEvent(e))
	{
		// "Close requested" event: close window
		if (e.type == Event::Closed)
			g.window->close();
	}
}

/**
	Update window to handle gameplay
	@param g the game.
*/
static void update(Game& g)
{
	// Create a vector to hold movement of paddle initalized to (0,0)
	Vector2f velocity = Vector2f();
	
	// Check whether L/R arrow keys are being pressed
	if (Keyboard::isKeyPressed(Keyboard::Left))
		velocity.x -= PADDLE_VELOCITY;
	if (Keyboard::isKeyPressed(Keyboard::Right))
		velocity.x += PADDLE_VELOCITY;

	// Move the paddle within screen window
	auto pRect = g.paddle.shape.getGlobalBounds();
	pRect.left += velocity.x;
	if (pRect.left > 0 && pRect.left + pRect.width < APP_WIDTH)
		g.paddle.shape.move(velocity);

	// Move the ball
	g.ball.shape.move(g.ball.velocity);
	// Get bounding rectangle of ball
	auto bRect = g.ball.shape.getGlobalBounds();
	auto right = bRect.left + bRect.width;
	auto bottom = bRect.top + bRect.height;
	// Change direction when hitting a wall (bounce)
	if (bRect.left <= 0 || right >= APP_WIDTH)
		g.ball.velocity.x *= -1;
	if (bRect.top <= 0)
		g.ball.velocity.y *= -1;
	else if (bRect.top >= APP_HEIGHT)
	{
		// lost a turn
		if (g.turnsLeft > 0)
		{
			g.window->setTitle("Game Over- Try again");
			// Wait 1 second
			sleep(seconds(1));
			g.turnsLeft--; // take away a turn
			// Reset all bricks
			for (Brick& brick : g.bricks)
				brick.visible = true;
			// Move the ball to the center of the screen
			g.ball.shape.setPosition(APP_WIDTH / 2 - BALL_RADIUS,
				APP_HEIGHT / 2 - BALL_RADIUS);
			// Reset the velocity
			g.ball.velocity = Vector2f(-BALL_VELOCITY, -BALL_VELOCITY);
			// Display the title
			// g.window->setTitle("You have " + toString(g.turnsLeft) + " turns left.");
			g.window->setTitle("Breakout Game");

			return;
		}
	}
	// Paddle collision
	if (pRect.intersects(bRect))
	{
		g.ball.velocity.y *= -1;
		if (pRect.left + pRect.width / 2 > bRect.left + bRect.width / 2)
			g.ball.velocity.x = -BALL_VELOCITY;
		else
			g.ball.velocity.x = BALL_VELOCITY;

		// Make ball rebound off paddle
		Vector2f vec(g.ball.velocity.x, (pRect.top - (bRect.top + bRect.height)) * 2);
		g.ball.shape.move(vec);
		return;
	}
	// Brick collision
	for (Brick& brick : g.bricks)
	{
		if (brick.visible && bRect.intersects(brick.shape.getGlobalBounds()))
		{
			brick.visible = false;
			g.ball.velocity.y *= -1;
		}
	}
}

/**
	Real-time rendering of display
	@param g the game.
*/
static void render(Game& g)
{
	// Clear the in-memory screen pixels entirely to the background color
	g.window->clear(Color::Black);
	//	Loop through vector and make window draw only visible bricks
	for (const Brick& brick : g.bricks)
		if (brick.visible)
			g.window->draw(brick.shape);
	// Draw the paddle
	g.window->draw(g.paddle.shape);
	// Draw the ball
	g.window->draw(g.ball.shape);

	// Draw game components- Send canvas to video card
	g.window->display();

}

/**
	Initializes the bricks in the game.
	@param bricks the bricks vector to be filled in.
*/
static void initBricks(vector<Brick>& bricks)
{
	float x = BRICK_SEP / 2;
	float y = BRICK_Y_OFFSET;
	Color brickColor = Color::Red;

	for (int row = 0; row < NBRICK_ROWS; row++)
	{
		for (int col = 0; col < NBRICKS_PER_ROW; col++)
		{
			// Create a brick
			bricks.push_back(makeBrick(x, y, brickColor));
			x += BRICK_WIDTH + BRICK_SEP;
		}

		y += BRICK_HEIGHT + BRICK_SEP;
		x = BRICK_SEP / 2;

		//Change color for every other row
		switch (row)
		{
			case 1: 
				brickColor = Color(255, 200, 0); // no built-in orange
				break;
			case 3:
				brickColor = Color::Yellow;
				break;
			case 5:
				brickColor = Color::Green;
				break;
			case 7:
				brickColor = Color::Cyan;
				break;
		}
	}
}

static Brick makeBrick(float x, float y, Color color)
{
	Brick b;
	b.visible = true;
	b.shape.setPosition(x, y);
	b.shape.setSize(Vector2f(BRICK_WIDTH, BRICK_HEIGHT));
	b.shape.setFillColor(color);

	return b;
}

/**
	Initializes the paddle in the game
	@param paddle the paddle to be added to the game
*/
static void initPaddle(Paddle& paddle)
{
	paddle.shape.setPosition(APP_WIDTH / 2 - PADDLE_WIDTH / 2,
		APP_HEIGHT - PADDLE_Y_OFFSET);
	paddle.shape.setSize(Vector2f(PADDLE_WIDTH, PADDLE_HEIGHT));
	paddle.shape.setFillColor(Color::White);
}

/**
	Initializes the ball in the game
	@param ball the ball to be added to the game
*/
static void initBall(Ball& ball)
{
	ball.shape.setRadius(BALL_RADIUS);
	ball.shape.setOrigin(BALL_RADIUS, BALL_RADIUS);
	ball.shape.setPosition(APP_WIDTH / 2, APP_HEIGHT / 2);
	ball.shape.setFillColor(Color::Magenta);
	ball.velocity = Vector2f(-BALL_VELOCITY, -BALL_VELOCITY);
}