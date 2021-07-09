#pragma once

#include "AnimatedSprite.h"
#include "Player.h"
#include <vector>
#include <map>
#include <string>

class TextManager;
class Graphics;

class Projectile : public AnimatedSprite
{
public:
	Projectile();
	Projectile(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual ~Projectile();

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);

	virtual Direction getProjectileDirection() { return this->_direction; }
	
private:

protected:
	float _startingX, _startingY;
	Direction _direction;
};

class SilverBullet : public Projectile {
public:
	SilverBullet();
	SilverBullet(Graphics & graphics, Vector2 spawnPoint, Direction dir);
	~SilverBullet();

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);

	Direction getProjectileDirection() { return this->_direction; }

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
protected:
	float _startingX, _startingY;
	Direction _direction;
};


