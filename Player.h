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

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void drawCurrentMapName(Graphics &graphics);
	void update(float elapsedTime);

	//Move player left by -dx
	void moveLeft();
	//Move player right by +dx
	void moveRight();
	//Player jump
	void jump();
	//Stop moving the player and play idle animation
	void stopMoving();
	//Look up and down
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

	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics);
	void handleEnemyCollisions(std::vector<Enemy*> &others);

	void handleItemCollisions(std::vector<std::string*>& others);

	void handleNpcCollisions(std::vector<Npc*>& others, Graphics &graphics, int lineCount);

	std::string getNpcName(std::vector<Npc*>& others, Graphics & graphics);

	void setIFrame(bool condition);

	void gainHealth(float amount);

	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt

	void render(int camX, int camY);

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }

	void gainExp(int exp);
	int getMaxExp();
	int getCurrentExp();
	int getLevel();
	void setLevel(int num);
	void addLevel(int num);

	//void gainHealth(float amount);
	void addKillCount(int num);
	void setKillCount(int num);
	int getKillCount();
	int killCount = 0;

	Direction facingDir();
	Direction _facing;

private:
	float _dx, _dy; //(dx (delta x) is change in x postition during frame)

	

	bool _grounded; //true if we are, false if we are in the air

	bool _lookingUp;
	bool _lookingDown;

	bool iFrame;

	float _maxHealth;
	float _currentHealth;

	int _exp = 0;
	int _playerLevel = 0;

	int mPosX, mPosY;
	double _timeElapsed = 0; //for timer
	double _mapTimeElapsed = 0;

protected:
	double _timeToUpdate = 2500;
	double _timeForMapName = 3000;
};

#endif
