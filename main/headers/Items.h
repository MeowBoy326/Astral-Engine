#pragma once

#include <string>
#include <vector>
#include "AnimatedSprite.h"
#include "Player.h"
#include <iostream>

class Graphics;

class Items : public AnimatedSprite
{
public:
	Items();
	Items(Graphics &graphics);

	typedef int ItemID;
	ItemID getID() const { return _id; }
	bool isCurrency() const { return currencyItem; }
	virtual Items* clone() const = 0;
	virtual void use(Player &player) = 0;
	virtual ~Items() {};

	/* old functions */
	virtual void update(int elapsedTime, Player & player);
	virtual void draw(Graphics & graphics);
	virtual void drawDrops(Graphics &graphics, float x, float y);
	//void addToInventory(int type);
	virtual const inline bool isDroppedItem() { return this->wasDropped; }
	virtual const inline int getAmount() { return this->currencyAmount; }
	Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
private:
	std::vector<std::pair<std::string, int>> lootTable;
	std::vector<std::pair<Items*, std::string>> mobDrops; 
	bool wasDropped;
	int currencyAmount;
protected:
	ItemID _id;
	bool currencyItem;
};

class HealthPotion : public Items {
public:
	HealthPotion();
	HealthPotion(Graphics &graphics, Vector2 spawnPoint);
	~HealthPotion();

	static const ItemID ID = 0;

	void use(Player &player) override {
		// Implement HP pot
		std::cout << "NEW HEAL USED!" << std::endl;
		player.gainHealth(player.getMaxHealth() / 3);
	}

	Items* clone() const override { return new HealthPotion(*this); }


	//void addToInventory();
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
	~PermHP();

	static const ItemID ID = 1;

	void use(Player &player) override {}

	Items* clone() const override { return new PermHP(*this); }


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
	~GoldCoin();

	static const ItemID ID = 3001;

	void use(Player &player) override {}

	Items* clone() const override { return new GoldCoin(*this); }


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
	~RedCoin();

	static const ItemID ID = 3002;

	void use(Player &player) override {}

	Items* clone() const override { return new RedCoin(*this); }


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
	~BronzeCoin();

	static const ItemID ID = 3000;

	void use(Player &player) override {}

	Items* clone() const override { return new BronzeCoin(*this); }

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
	~SilverGem();

	static const ItemID ID = 1100;

	void use(Player &player) override {}

	Items* clone() const override { return new SilverGem(*this); }


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
	~Key();

	static const ItemID ID = 1001;

	void use(Player &player) override {}

	Items* clone() const override { return new Key(*this); }

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
