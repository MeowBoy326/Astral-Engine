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
	virtual void drawDrops(Graphics &graphics, float x, float y);
	void addToInventory(int type);
	virtual const inline bool isDroppedItem() { return this->wasDropped; }
	virtual const inline int getAmount() { return this->currencyAmount; }
	Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	~Items();
private:
	std::vector<std::pair<std::string, int>> lootTable;
	std::vector<std::pair<Items*, std::string>> mobDrops; 
	bool wasDropped;
	int currencyAmount;
};

class HealthPotion : public Items {
public:
	HealthPotion();
	HealthPotion(Graphics &graphics, Vector2 spawnPoint);

	void addToInventory();
	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void update(int elapsedTime, Player & player);

	void draw(Graphics & graphics);
	void drawDrops(Graphics &graphics, float x, float y);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	int hpGain;
	bool wasDropped = false;
	int currencyAmount = 0;
};

class PermHP : public Items {
public:
	PermHP();
	PermHP(Graphics &graphics, Vector2 spawnPoint);
	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void update(int elapsedTime, Player & player);
	void draw(Graphics & graphics);
	void drawDrops(Graphics &graphics, float x, float y);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	int maxHPgain;
	int currencyAmount = 0;
	bool wasDropped = false;
};

class GoldCoin : public Items {
public:
	GoldCoin();
	GoldCoin(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
private:
	int amountGain = 100;
	int currencyAmount = 100;
	bool wasDropped = true;
};

class RedCoin : public Items {
public:
	RedCoin();
	RedCoin(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
private:
	int amountGain = 1000;
	int currencyAmount = 1000;
	bool wasDropped = true;
};

class BronzeCoin : public Items {
public:
	BronzeCoin();
	BronzeCoin(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
private:
	int amountGain = 1;
	int currencyAmount = 1;
	bool wasDropped = true;
};

class SilverGem : public Items {
public:
	SilverGem();
	SilverGem(Graphics &graphics, Vector2 spawnPoint);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
private:
	int amountGain = 1;
	int currencyAmount = 0;
	bool wasDropped = true;
	bool initialPosDone = false;
};

class Key : public Items {
public:
	Key();
	Key(Graphics &graphics, Vector2 spawnPoint);
	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	void update(int elapsedTime, Player & player);

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	std::string keyName = "key";
	int currencyAmount = 0;
	bool wasDropped = false;
};
