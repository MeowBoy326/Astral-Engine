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
	virtual void playDeath() = 0;
	virtual float enemyExpAmount();

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	virtual bool isRemoveable();
	virtual void setRemoveable();

	virtual void bulletHit(float dmg);
	virtual std::string getName();

	bool removeEnemy = false;

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name;
	bool deathCreated = false;
};

class Bat : public Enemy {
public:
	Bat();
	Bat(Graphics &graphics, Vector2 spawPoint);
	
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void playDeath();
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void dropLoot();
	void playAttack();

	void bulletHit(float dmg);
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	bool isRemoveable();
	void setRemoveable();

	bool removeEnemy = false;

	float enemyExpAmount();

	std::string getName();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if bat should move
	bool hasHit = false;
	int iFrameCount = 0;
	float batExp = 10;
	Sprite _fireBall;
	float _fireX, _fireY;

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name = "bat";
	bool deathCreated = false;
};

class Shade : public Enemy {
public:
	Shade();
	Shade(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void playDeath();
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void dropLoot();
	void playAttack();

	void bulletHit(float dmg);
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	bool isRemoveable();
	void setRemoveable();

	bool removeEnemy = false;
	float enemyExpAmount();

	std::string getName();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if bat should move
	bool hasHit = false;
	int iFrameCount = 0;
	float shadeExp = 100;
	Sprite _shadeBall;
	//float _fireX, _fireY;

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name = "shade";
	bool deathCreated = false;
};