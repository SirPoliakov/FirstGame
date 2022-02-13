#pragma once
#include "Window.h"
#include "Renderer.h"
#include "Vector2.h"

struct Brick {

	Brick() : pos(0, 0), width(0), height(0) {}
	Brick(const int w, const int h) : pos(0, 0), width(w), height(h) {}
	Brick(Vector2 position, const int w, const int h) : pos(position), width(w), height(h) {}

	void operator=(Brick maBrick)
	{
		this->pos = maBrick.pos;
		this->width = maBrick.width;
		this->height = maBrick.height;
	}

	Vector2 pos;
	int width = 65;
	int height = 20;
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

