#pragma once

#include "sprite.h"
#include "Player.h"
#include <vector>
#include "TextManager.h"

class Graphics;
//class Player; Can't foward declare player here because Player_player is not a pointer

class Inventory : public TextManager
{
public:
	Inventory();
	Inventory(Graphics &graphics, Player &player);

	
	enum ItemType {
		HEALTH_POT,
		BUFF
		// ...
	};

	struct InventoryItem {
		ItemType type;
		std::string imagePath;
	};

	void update(int elapsedTime, Player &player);
	void useItem(int type, Player & player);
	void draw(Graphics &graphics, Player &player);
	void drawQuantity(Graphics &graphics, int x, int y, int quantity);
	void addInstancedLoot(std::string mapName, int type);
	inline const std::vector<std::pair<std::string, int>> getLootTable() const { return this->lootTable; }
	inline void setLootTable(std::vector<std::pair<std::string, int>> table) { this->lootTable = table; }
	std::vector<Inventory::InventoryItem*> getInventory();
	void setInventory(std::vector<Inventory::InventoryItem*> invent);
	bool isLooted(std::string map, int iType);

	Inventory(struct Inventory::InventoryItem);
	void addItem(Inventory::InventoryItem *item);
	
	~Inventory();

private:
	Player _player;

	//Health Sprites
	Sprite _iMenu;
	Sprite _hpPot;

	int hpToGain;

	std::vector<std::pair<std::string, int>> lootTable;
protected:
	
};

