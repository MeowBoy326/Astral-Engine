#pragma once

#include "AnimatedSprite.h"
#include "Global.h"
#include "Player.h"

#include <string>

class Graphics;

class Enemy : public AnimatedSprite {
public:
	Enemy();
	virtual ~Enemy();
	Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	Enemy& operator=(const Enemy& levelMap);
	Enemy(const Enemy&);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	virtual void touchPlayer(Player* player) = 0; //pure virtual implement in derived classes
	virtual void playDeath() = 0;
	virtual const inline int getEnemyLevel() const { return this->level; }
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
	virtual const inline bool isBoss() { return this->boss; }
	virtual const inline bool isMiniBoss() { return this->miniBoss; }
	virtual const inline float getStartingX() const { return this->_startingX; }
	virtual const inline float getStartingY() const { return this->_startingY; }

	virtual void handleEnemyTileCollision(std::vector<Rectangle> &others);

	virtual void bulletHit(float dmg);
	virtual std::string getName();

	bool removeEnemy = false;

protected:
	Direction _direction;

	float _startingX, _startingY;
	int _maxHealth;
	int _currentHealth;
	int level;
	std::string name;
	std::string coinType;
	bool boss;
	bool miniBoss;
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
	~Bat();
	Bat& operator=(const Bat& levelMap);
	Bat(const Bat&);
	
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
	const inline int getEnemyLevel() const { return this->level; }
	const inline bool getDyingState() const { return this->dyingAnimation; }
	void setDyingState(bool condition) { this->dyingAnimation = condition; }
	inline void setVisible(bool condition) { this->isVisible = condition; }

	const inline std::string getCoinDropType() { return this->coinType; }
	const inline bool isBoss() { return this->boss; }
	const inline bool isMiniBoss() { return this->miniBoss; }
	const inline float getStartingX() const { return this->_startingX; }
	const inline float getStartingY() const { return this->_startingY; }

	void handleEnemyTileCollision(std::vector<Rectangle> &others);

	bool isRemoveable();
	void setRemoveable();

	bool removeEnemy = false;

	float enemyExpAmount();

	std::string getName();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if bat should move
	bool _shouldMoveLeft;
	bool hasHit = false;
	int iFrameCount = 0;
	float batExp = 1.175f;
	Sprite _fireBall;
	Sprite _HPBar;
	Sprite _HPValue;
	float _fireX, _fireY;
	double _deathAnimationTime = 800;
	double _deathTimeElapsed = 0;
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;
	int level = 0;
	std::string name = "bat";
	std::string coinType = "bronze";
	bool boss = false;
	bool miniBoss = false;
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
	~Shade();
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
	const inline int getEnemyLevel() const { return this->level; }
	const inline bool getDyingState() const { return this->dyingAnimation; }
	void setDyingState(bool condition) { this->dyingAnimation = condition; }

	const inline std::string getCoinDropType() { return this->coinType; }
	const inline bool isBoss() { return this->boss; }
	const inline bool isMiniBoss() { return this->miniBoss; }
	const inline float getStartingX() const { return this->_startingX; }
	const inline float getStartingY() const { return this->_startingY; }

	void handleEnemyTileCollision(std::vector<Rectangle> &others);

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
	Sprite _HPBar;
	Sprite _HPValue;
	//float _fireX, _fireY;
	double _deathAnimationTime = 800;
	double _deathTimeElapsed = 0;
	float _teleportTimer = 0;
protected:
	Direction _direction;
	int _maxHealth;
	int _currentHealth;
	int level = 4;
	std::string name = "shade";
	std::string coinType = "red";
	bool boss = true;
	bool miniBoss = false;
	bool deathCreated = false;
	bool canDropLoot = false;
	bool dyingAnimation = true;
	bool _grounded = true;
	bool _hasTeleported = false;
	float _dy = 0;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	double respawnTime = 6000;
	double timerRespawn = 0;
	bool isVisible = true;
	bool isIdle = true;
};

class JellyFish : public Enemy {
public:
	JellyFish();
	JellyFish(Graphics &graphics, Vector2 spawPoint);
	~JellyFish();
	JellyFish& operator=(const JellyFish& levelMap);
	JellyFish(const JellyFish&);

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
	const inline int getEnemyLevel() const { return this->level; }
	const inline bool getDyingState() const { return this->dyingAnimation; }
	void setDyingState(bool condition) { this->dyingAnimation = condition; }
	inline void setVisible(bool condition) { this->isVisible = condition; }

	const inline std::string getCoinDropType() { return this->coinType; }
	const inline bool isBoss() { return this->boss; }
	const inline bool isMiniBoss() { return this->miniBoss; }
	const inline float getStartingX() const { return this->_startingX; }
	const inline float getStartingY() const { return this->_startingY; }

	void handleEnemyTileCollision(std::vector<Rectangle> &others);

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
	float JellyFishExp = 1.175f;
	Sprite _fireBall;
	Sprite _HPBar;
	Sprite _HPValue;
	float _fireX, _fireY;
	bool _JellyAttack = false;
	bool _resettingPos = false;
	bool _delayAttack = false;
	float _delayTimer = 0;
	double _deathAnimationTime = 800;
	double _deathTimeElapsed = 0;
protected:
	Direction _direction;
	bool _grounded = true;
	float _dy = 0;
	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
	int _maxHealth;
	int _currentHealth;
	int level = 3;
	std::string name = "JellyFish";
	std::string coinType = "bronze";
	bool boss = false;
	bool miniBoss = false;
	bool deathCreated = false;
	bool canDropLoot = false;
	bool dyingAnimation = true;
	double respawnTime = 6000;
	double timerRespawn = 0;
	bool isVisible = true;
};