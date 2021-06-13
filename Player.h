#ifndef PLAYER_H
#define PLAYER_H


#include "AnimatedSprite.h"
#include "Global.h"
#include "Slope.h"
#include "Level.h"
#include "Enemy.h"

//#include "Npc.h"

class Graphics;
class Npc;
class Items;
class Inventory;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void drawStatMenu(Graphics &graphics, Player &player);
	void drawCurrentMapName(Graphics &graphics);
	void update(float elapsedTime);

	//Player handling
	void moveLeft(); 	//Move player left by -dx
	void moveRight(); 	//Move player right by +dx
	void jump();
	void stopMoving();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();
	virtual void animationDone(std::string currentAnimation);
	std::string getMap();
	bool lookingUp();
	bool lookingDown();
	bool lookingRight();
	bool lookingLeft();
	virtual void setupAnimations();
	Direction facingDir();
	Direction _facing;

	//Event handling
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent);
	void handleEnemyCollisions(std::vector<Enemy*> &others);
	void handleItemCollisions(std::vector<std::string*>& others);
	void handleNpcCollisions(std::vector<Npc*>& others, Graphics &graphics, int lineCount);
	std::string getNpcName(std::vector<Npc*>& others, Graphics & graphics);
	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt

	//Health handling
	void setIFrame(bool condition);
	void gainHealth(float amount);
	void gainMaxHealth(float amount);
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	//Exp & Level handling
	void gainExp(float exp);
	float getCurrentExp();
	void setCurrentExp(float exp);
	float getRequiredExp();
	int getLevel();
	void setLevel(int num);
	void addLevel(int num);
	int getSoulLevel();
	void setSoulLevel(int num);
	void addSoulLevel(int num);
	double getDmgMod();

	//void gainHealth(float amount);
	void addKillCount(int num);
	void setKillCount(int num);
	int getKillCount();
	int getRequiredKills();
	int killCount = 0;
private:
	float _dx, _dy; //(dx (delta x) is change in x postition during frame)
	float _maxHealth;
	float _currentHealth;
	float _exp = 0;
	float STAT_AGIL = 1;
	float STAT_STR = 1;
	float STAT_AVOID = 1;

	bool _grounded; //true if we are, false if we are in the air
	bool _lookingUp;
	bool _lookingDown;
	bool iFrame;

	int _requiredExp;
	int _statPoints = 0;
	int _playerLevel = 0;
	int _soulLevel = 0;
	int _requiredKill;
	int mPosX, mPosY;

	double _soulStrength = 1;
	double _dmgMod = 0;
	double _timeElapsed = 0; //for timer
	double _mapTimeElapsed = 0;

	Sprite _statMenu;
protected:
	double _timeToUpdate = 2500;
	double _timeForMapName = 3000;
};

#endif
