#pragma once

#include "AnimatedSprite.h"
#include "Global.h"
#include "Player.h"

#include <string>

class Graphics;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player* player) = 0; //pure virtual implement in derived classes
	virtual void setTimer() = 0;
	virtual void playDeath() = 0;

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	void bulletHit(float dmg);
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;

	bool deathCreated = false;
};

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawPoint);
	
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void setTimer();
	Uint32 setHit(Uint32 interval, void* param);
	bool canHit();
	//void bulletHit();

	void playDeath();

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if bat should move
	bool hasHit = false;
	int iFrameCount = 0;
};