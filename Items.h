#pragma once

#include <string>
#include <vector>
#include "AnimatedSprite.h"
#include "Player.h"
#include "Inventory.h"


class Graphics;


class Items : public AnimatedSprite, public Inventory
{
public:
	Items();
	Items(Graphics &graphics);
	virtual void update(int elapsedTime, Player & player);
	virtual void draw(Graphics & graphics);
	void addToInventory(int type);
	Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	~Items();
private:
	//std::vector<std::vector<int>> lootTable;
	std::vector<std::pair<std::string, int>> lootTable;
};

class HealthPotion : public Items {
public:
	HealthPotion();
	HealthPotion(Graphics &graphics, Vector2 spawnPoint);

	void addToInventory();

	void update(int elapsedTime, Player & player);

	void draw(Graphics & graphics);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	int hpGain;
};

class PermHP : public Items {
public:
	PermHP();
	PermHP(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player & player);
	void draw(Graphics & graphics);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	int maxHPgain;
};

class Key : public Items {
public:
	Key();
	Key(Graphics &graphics, Vector2 spawnPoint);

	void draw(Graphics &graphics);
	void update(int elapsedTime, Player & player);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	std::string keyName;
};
