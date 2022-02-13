#include "Game.h"
#include "Timer.h"
#include <random>
#include <vector>
#include <Math.h>
#include <iostream>
using namespace std;



int BRICK_W = 120;
int BRICK_H = 50;
int BRICK_GAP = 10;
const int BRICK_COLS = 7;
const int BRICK_ROWS = 7;

vector<Brick> bricks;

bool Game::initialize()
{
	bool isWindowInit = window.initialize();
	bool isRendererInit = renderer.initialize(window);

	int windowWidth = window.getWidth();
	int windowHeight = window.getHeight();
	topWall = { 0, 0, static_cast<float>(windowWidth), wallThickness };
	topWall = { 0, 0, static_cast<float>(windowWidth), wallThickness };
	rightWall = { windowWidth - wallThickness, 0, wallThickness, static_cast<float>(windowHeight) };
	leftWall = { 0, 0, wallThickness, static_cast<float>(windowHeight) };




	Brick tmpB({ 120, 200 }, BRICK_W, BRICK_H);
	for (int i = 0; i < BRICK_ROWS; i++)
	{
		for (int j = 0; j < BRICK_COLS; j++)
		{
			bricks.push_back(tmpB);
			tmpB.pos.x += BRICK_W;
		}
		tmpB.pos.x = 120;
		tmpB.pos.y += BRICK_H;

	}

	for (int i = 0; i < BRICK_ROWS * BRICK_COLS; i++)
	{
		cout << "(" << bricks[i].pos.x << ";" << bricks[i].pos.y << ")" << endl;
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

	// Ball - rect collision
	int caseX = floor(ballPos.x / BRICK_W) - 1;
	int caseY = floor(ballPos.y / BRICK_H) - 4;
	int index = caseX + caseY * 7;
	if (index >= 0 && index < BRICK_ROWS * BRICK_COLS)
	{
		Vector2 surfBall = ballPos; surfBall.x += -10; surfBall.y += -10;
		Vector2 collideDiff = surfBall - bricks[index].pos;
		if (bricks[index].hit == false && fabsf(collideDiff.y) <= bricks[index].height - BRICK_GAP && fabsf(collideDiff.x) <= bricks[index].width - BRICK_GAP)
		{
			int prevBallX = ballPos.x - ballVelocity.x;
			int prevBallY = ballPos.y - ballVelocity.y;

			int prevCaseX = floor(prevBallX / BRICK_W);
			int prevCaseY = floor(prevBallY / BRICK_H);

			if (prevCaseX != caseX)
			{
				ballVelocity.x *= -1;
			}
			if (prevCaseY != caseY)
			{
				ballVelocity.y *= -1;
			}

			bricks[index].hit = true;
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


	for (int i = 0; i < BRICK_ROWS * BRICK_COLS; i++)
	{
		if (bricks[i].hit == false)
		{
			Rectangle rect = { bricks[i].pos.x, bricks[i].pos.y, bricks[i].width - BRICK_GAP, bricks[i].height - BRICK_GAP };
			renderer.drawRectRandom(rect, 0, 0, 255);
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