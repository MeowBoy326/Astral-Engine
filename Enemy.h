#pragma once

#include "AnimatedSprite.h"
#include "Global.h"
#include "Player.h"

#include <string>

class Graphics;
class Items;
class GoldCoin;

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
	virtual const inline int getCurrentHealth() const { return this->_currentHealth; }
	virtual bool isRemoveable();
	virtual void setRemoveable();
	virtual const inline bool getDyingState() const { return this->dyingAnimation; }
	virtual void setDyingState(bool condition) { this->dyingAnimation = condition; }
	virtual void dropLoot(Player &player);
	const inline bool getLootState() const { return this->canDropLoot; }
	inline void setLootState(bool condition) { this->canDropLoot = condition; }
	virtual const inline std::string getCoinDropType() { return this->coinType; }

	virtual void bulletHit(float dmg);
	virtual std::string getName();

	bool removeEnemy = false;

protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name;
	std::string coinType;
	bool deathCreated = false;
	bool canDropLoot = false;
	bool dyingAnimation = true;
	double _deathAnimationTime = 2500;
	double _deathTimeElapsed = 0;
	double respawnTime = 6000;
	double timerRespawn = 0;
	bool isVisible = true;
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
	void dropLoot(Player &player);
	void playAttack();

	void bulletHit(float dmg);
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	const inline bool getDyingState() const { return this->dyingAnimation; }
	void setDyingState(bool condition) { this->dyingAnimation = condition; }
	inline void setVisible(bool condition) { this->isVisible = condition; }

	const inline std::string getCoinDropType() { return this->coinType; }

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
	float batExp = 2.15;
	Sprite _fireBall;
	float _fireX, _fireY;
	double _deathAnimationTime = 800;
	double _deathTimeElapsed = 0;
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name = "bat";
	std::string coinType = "bronze";
	bool deathCreated = false;
	bool canDropLoot = false;
	bool dyingAnimation = true;
	double respawnTime = 6000;
	double timerRespawn = 0;
	bool isVisible = true;
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
	void dropLoot(Player &player);
	void playAttack();

	void bulletHit(float dmg);
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	const inline bool getDyingState() const { return this->dyingAnimation; }
	void setDyingState(bool condition) { this->dyingAnimation = condition; }

	const inline std::string getCoinDropType() { return this->coinType; }

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
	double _deathAnimationTime = 2500;
	double _deathTimeElapsed = 0;
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	std::string name = "shade";
	std::string coinType = "red";
	bool deathCreated = false;
	bool canDropLoot = false;
	bool dyingAnimation = true;
	double respawnTime = 6000;
	double timerRespawn = 0;
	bool isVisible = true;
};