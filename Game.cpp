#include "Game.h"
#include "Timer.h"
#include <random>
#include <vector>

using namespace std;

bool hit = false;

vector<Brick> bricks;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	int windowWidth = window.getWidth();
	int windowHeight = window.getHeight();
	topWall = { 0, 0, static_cast<float>(windowWidth), wallThickness };
	topWall = { 0, 0, static_cast<float>(windowWidth), wallThickness };
	rightWall = { windowWidth - wallThickness, 0, wallThickness, static_cast<float>(windowHeight)};
	leftWall = { 0, 0, wallThickness, static_cast<float>(windowHeight) };

	float tmpX = 50; float tmpY = 50;
	
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
		{
			Brick tmpB;
			tmpB.pos.x = tmpX;
			tmpB.pos.y = tmpY;
			bricks.push_back(tmpB);
			tmpX += 65;
		}
		tmpY += 20;
	}

	return isWindowInit && isRendererInit; // Return bool && bool && bool ...to detect error
}

void Game::processInput()
{
	// SDL Event
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		}
	}
	// Keyboard state
	const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
	// Escape: quit game
	if (keyboardState[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}
	// Paddle1 move
	if ((ballVelocity.x != 0) || (ballVelocity.y != 0)) {
		if (keyboardState[SDL_SCANCODE_A])
		{
			paddle1Direction = -1;
		}
		if (keyboardState[SDL_SCANCODE_D])
		{
			paddle1Direction = 1;
		}
	}

	// Ball launch
	if (keyboardState[SDL_SCANCODE_SPACE])
	{
		if ((ballVelocity.x == 0) && (ballVelocity.y == 0))
		{
			ballVelocity.x = 200;
			ballVelocity.y = -200;		
		}
	}
}

void Game::update(float dt)
{
	// Paddle move
	paddle1Pos += paddle1Velocity * dt * paddle1Direction;
	if (paddle1Pos.x < wallThickness + paddle1Width / 2)
	{
		paddle1Pos.x = wallThickness + paddle1Width / 2;
	}
	if (paddle1Pos.x > window.getWidth() - paddle1Width / 2 - wallThickness)
	{
		paddle1Pos.x = window.getWidth() - paddle1Width / 2 - wallThickness;
	}

	// Ball move
	ballPos += ballVelocity * dt;
	if (ballPos.y < ballSize / 2 + wallThickness) {
		ballPos.y = ballSize / 2 + wallThickness;
		ballVelocity.y *= -1;
	}
	else if (ballPos.x > window.getWidth() - ballSize / 2 - wallThickness) {
		ballPos.x = window.getWidth() - ballSize / 2 - wallThickness;
		ballVelocity.x *= -1;
	}
	else if (ballPos.x < wallThickness) {
		ballPos.x = wallThickness;
		ballVelocity.x *= -1;
	}

	// Ball-Paddle collision
	Vector2 diff = ballPos - paddle1Pos;
	if (fabsf(diff.y) <= paddle1Height / 2 && fabsf(diff.x) <= paddle1Width / 2)
	{
		ballVelocity.y *= -1;
	}

	// Ball - Rect collision
		for (int i = 0; i <= bricks.size(); i++)
		{
			Vector2 collideDiff = ballPos - bricks[i].pos;
			if (hit == false && fabsf(collideDiff.y) <= bricks[i].height / 2 && fabsf(collideDiff.x) <= bricks[i].width / 2)
			{
				hit = true;
				ballVelocity.y *= -1;
			}
		}

	// Restart automatically
	if (ballPos.y > window.getHeight()) {
		ballPos.y = paddle1Pos.y - paddle1Height;
		ballPos.x = paddle1Pos.x;
		ballVelocity.x = 0; ballVelocity.y = 0;
	}
}

void Game::render()
{
	renderer.beginDraw();

	renderer.drawRectRed(topWall);
	renderer.drawRectRed(rightWall);
	renderer.drawRectRed(leftWall);


	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 255);

	const int r = dist6(rng);
	const int g = dist6(rng);
	const int b = dist6(rng);

	for (int i = 0; i < bricks.size(); i++)
	{
		if (bricks[i].hit == false)
		{
			Rectangle rect = { bricks[i].pos.x - bricks[i].width / 2, bricks[i].pos.x - bricks[i].width / 2, bricks[i].width, bricks[i].height };
			renderer.drawRectRandom(rect, r, g, b);
		}
	}

	Rectangle ballRect = { ballPos.x - ballSize / 2, ballPos.y - ballSize / 2, ballSize, ballSize };
	renderer.drawRectWhite(ballRect);

	Rectangle paddle1Rect = { paddle1Pos.x - paddle1Width / 2, paddle1Pos.y - paddle1Height / 2, paddle1Width, paddle1Height };
	renderer.drawRectRed(paddle1Rect);

	renderer.endDraw();
}

void Game::loop()
{
	Timer timer;
	float dt = 0;
	while (isRunning)
	{
		float dt = timer.computeDeltaTime() / 1000.0f;
		processInput();
		update(dt);
		render();
		timer.delayTime();
	}
}

void Game::close()
{
	renderer.close();
	window.close();
	SDL_Quit();
}