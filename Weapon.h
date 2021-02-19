#pragma once

#include "AnimatedSprite.h"
#include "Global.h"
#include "Player.h"
#include "Enemy.h"
#include "Graphics.h"
#include <vector>

#include <string>

class Graphics;
class Player;

class Weapon : public AnimatedSprite {
public:
		Weapon();

		Weapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

		virtual void update(int elapsedTime, Player &player);
		virtual void draw(Graphics &graphics);
		//virtual void touchPlayer(Player* player) = 0; //pure virtual implement in derived classes

		const inline int getMaxHealth() const { return this->_maxHealth; }
		const inline int getCurrentHealth() const { return this->_currentHealth; }
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;

		~Weapon();

};



class Gun : public Weapon {
public:
	Gun();
	Gun(Graphics &graphics, Vector2 spawPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);

	void animationDone(std::string currentAnimation);
	virtual void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if bat should move
};

class Bullet : public AnimatedSprite {
public:
	Bullet();
	Bullet(Graphics &graphics, Player &player);
	Bullet(Graphics &graphics, Vector2 spawnPoint);
	//Bullet(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	void draw(Graphics &graphics, Player &player);
	void update(float elapsedTime, Player &player);
	void animationDone(std::string currentAnimation);
	virtual void setupAnimations();


	//Move player left by -dx
	//void shootLeft();
	//Move player right by +dx
	void shootRight(Player &player, int id);

	//void handleEnemyCollisions(std::vector<Enemy*> &others);
	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt
	bool active = false;
	std::vector<int> currentVec;

private:
	float _shootDx, _shootDy; //(dx (delta x) is change in x postition during frame)
	float _startingX, _startingY;
	Sprite _bullet;
};