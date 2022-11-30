#pragma once
#include "AnimatedSprite.h"
#include "Global.h"
#include "Player.h"

#include <string>

class Graphics;

class Effects : public AnimatedSprite {
public:
	Effects();
	virtual ~Effects();
	Effects(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	Effects& operator=(const Enemy& levelMap);
	Effects(const Enemy&);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

private:
	bool isVisible = true;
};

class ArenaEffect : public Effects {
public:
	ArenaEffect();
	~ArenaEffect();
	ArenaEffect(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	float _startingX, _startingY;
	bool isVisible = true;
};

