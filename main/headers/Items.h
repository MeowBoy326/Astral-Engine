#pragma once

#include <string>
#include <vector>
#include "AnimatedSprite.h"
#include "Player.h"
#include <iostream>
#include <variant>

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
	virtual void raiseEventMsg(Player &player) = 0;
	virtual void use(Player &player) = 0;
	virtual void updateItemStats(Player &player) = 0;
	virtual ~Items() {};

	/* old functions */
	virtual void update(int elapsedTime, Player & player);
	virtual void draw(Graphics & graphics);
	virtual void drawDrops(Graphics &graphics, float x, float y);
	//void addToInventory(int type);
	virtual const inline bool isDroppedItem() { return this->wasDropped; }
	virtual const inline int getAmount() { return this->currencyAmount; }
	virtual const inline std::string getItemName() { return this->name; }
	Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);
	virtual const std::map<std::string, std::variant<int, std::string>> getProperties() = 0;
private:
	std::vector<std::pair<std::string, int>> lootTable;
	std::vector<std::pair<Items*, std::string>> mobDrops; 
	bool wasDropped;
	int currencyAmount;
protected:
	ItemID _id;
	bool currencyItem;
	std::string name;
	std::map<std::string, std::variant<int, std::string>> properties_;
};

class HealthPotion : public Items {
public:
	HealthPotion();
	HealthPotion(Graphics &graphics, Vector2 spawnPoint);
	~HealthPotion();
	
	static const ItemID ID = 0;

	void use(Player &player) override {
		// Implement HP pot
		if (player.hasHpPot()) {
			player.subtractHpPot();
			player.gainHealth(((player.getMaxHealth() / 3)) + (player.getHpPotStrength() * 8));
		}
		
	}

	void updateItemStats(Player &player) {
		this->properties_["Strength"] = player.getHpPotStrength();
		this->properties_["Capacity"] = player.getHpPotCapacity();
	}

	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new HealthPotion(*this); }

	//void addToInventory();
	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void update(int elapsedTime, Player & player);

	void draw(Graphics & graphics);
	void drawDrops(Graphics &graphics, float x, float y);

	void animationDone(std::string currentAnimation);
	void setupAnimations();

	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int hpGain;
	bool wasDropped = false;
	int currencyAmount = 0;
	std::string name = "Health Vial";
	std::map<std::string, std::variant<int, std::string>> properties_ = {
		{"Strength", 2},
		{"Capacity", 4},
		{"Value", 0},
		{"Description", "A damaged vial that recovers your shattered soul. Can be repaired using fragments to increase it's healing properties."}
	};
	
};

class PermHP : public Items {
public:
	PermHP();
	PermHP(Graphics &graphics, Vector2 spawnPoint);
	~PermHP();

	static const ItemID ID = 1;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new PermHP(*this); }


	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void update(int elapsedTime, Player & player);
	void draw(Graphics & graphics);
	void drawDrops(Graphics &graphics, float x, float y);

	void animationDone(std::string currentAnimation);
	void setupAnimations();

	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int maxHPgain;
	int currencyAmount = 0;
	bool wasDropped = false;
	std::string name = "PermHP";
	std::map<std::string, std::variant<int, std::string>> properties_;
};

class GoldCoin : public Items {
public:
	GoldCoin();
	GoldCoin(Graphics &graphics, Vector2 spawnPoint);
	~GoldCoin();

	static const ItemID ID = 3001;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new GoldCoin(*this); }


	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int amountGain = 100;
	int currencyAmount = 100;
	std::string name = "Gold Coin";
	bool wasDropped = true;
	std::map<std::string, std::variant<int, std::string>> properties_;
};

class RedCoin : public Items {
public:
	RedCoin();
	RedCoin(Graphics &graphics, Vector2 spawnPoint);
	~RedCoin();

	static const ItemID ID = 3002;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new RedCoin(*this); }


	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int amountGain = 1000;
	int currencyAmount = 1000;
	bool wasDropped = true;
	std::string name = "Red Coin";
	std::map<std::string, std::variant<int, std::string>> properties_;
};

class BronzeCoin : public Items {
public:
	BronzeCoin();
	BronzeCoin(Graphics &graphics, Vector2 spawnPoint);
	~BronzeCoin();

	static const ItemID ID = 3000;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new BronzeCoin(*this); }

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int amountGain = 1;
	int currencyAmount = 1;
	bool wasDropped = true;
	std::string name = "Bronze Coin";
	std::map<std::string, std::variant<int, std::string>> properties_;
};

class SilverGem : public Items {
public:
	SilverGem();
	SilverGem(Graphics &graphics, Vector2 spawnPoint);
	~SilverGem();

	static const ItemID ID = 1100;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new SilverGem(*this); }


	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	const inline bool isDroppedItem() { return this->wasDropped; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getAmount() { return this->currencyAmount; }
	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	int amountGain = 1;
	int currencyAmount = 0;
	bool wasDropped = true;
	bool initialPosDone = false;
	std::string name = "Silver Gem";
	std::map<std::string, std::variant<int, std::string>> properties_ = {
		{"Value", 40},
		{"Description", "A refined silver ore that has a slight shine to it. Possibly used for trading or crafting."}
	};
};

class Key : public Items {
public:
	Key();
	Key(Graphics &graphics, Vector2 spawnPoint);
	~Key();

	static const ItemID ID = 1001;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {}

	Items* clone() const override { return new Key(*this); }

	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void draw(Graphics &graphics);
	void drawDrops(Graphics &graphics, float x, float y);
	void update(int elapsedTime, Player & player);

	void animationDone(std::string currentAnimation);
	void setupAnimations();

	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	std::string keyName = "key";
	int currencyAmount = 0;
	bool wasDropped = false;
	std::string name = "Key";
	std::map<std::string, std::variant<int, std::string>> properties_ = {
		{"Value", 0},
		{"Description", "Rusty key used to open a simple locked door."}
	};
};

class JetPack : public Items {
public:
	JetPack();
	JetPack(Graphics &graphics, Vector2 spawnPoint);
	~JetPack();

	static const ItemID ID = 2100;

	void use(Player &player) override {}
	void updateItemStats(Player &player) {}
	void raiseEventMsg(Player &player) override {
		player.setEventMessage("Press 3 to use the JetPack");
	}

	Items* clone() const override { return new JetPack(*this); }


	//void addToInventory();
	const inline bool isDroppedItem() { return this->wasDropped; }
	const inline int getAmount() { return this->currencyAmount; }
	void update(int elapsedTime, Player & player);

	void draw(Graphics & graphics);
	void drawDrops(Graphics &graphics, float x, float y);

	void animationDone(std::string currentAnimation);
	void setupAnimations();

	const inline std::string getItemName() override { return this->name; }
	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }
private:
	bool wasDropped = false;
	int currencyAmount = 0;
	std::string name = "Jet Pack";
	std::map<std::string, std::variant<int, std::string>> properties_ = {
		{"Fuel", 80},
		{"Value", 0},
		{"Description", "If pigs could fly."}
	};
};
