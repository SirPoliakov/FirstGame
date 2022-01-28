#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"

struct Brick {

	Vector2 pos;
	const float width = 65;
	const float height = 20;
	bool hit = false;
};


class Game
{
public:
	static Game& instance()
	{
		static Game inst;
		return inst;
	}

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;

private:
	Game(): 
		isRunning(true),
		ballPos({ 100, 100 }),
		ballVelocity({ 250, 250 }),
		paddle1Pos({ 500, 700 }),
		paddle1Velocity({ 450, 0 }),
		paddle1Direction(0),
		wallThickness(10),
		topWall(Rectangle()),
		rightWall(Rectangle()),
		leftWall(Rectangle())
	{}

public:
	bool initialize();
	void loop();
	void close();

private:
	void processInput();
	void update(float dt);
	void render();

	bool isRunning;
	Window window;
	Renderer renderer;

	Rectangle topWall;
	Rectangle rightWall;
	Rectangle leftWall;
	const float wallThickness = 10;

	Vector2 ballPos;
	Vector2 ballVelocity;
	const float ballSize = 10;

	Vector2 paddle1Pos;
	Vector2 paddle1Velocity;
	const float paddle1Width = 125;
	const float paddle1Height = 10;
	float paddle1Direction;
};

