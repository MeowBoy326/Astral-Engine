#pragma once

#include "sprite.h"
#include "Player.h"
#include <vector>
#include "TextManager.h"
#include "Items.h"
#include <map>

class Graphics;
// Class Player; Can't forward declare player here because Player_player is not a pointer

class Inventory : public TextManager
{
public:
	Inventory();
	Inventory(Graphics &graphics, Player &player);

	 void initPrototypes() {
		 /* Create a map for the ID system. Create items based off the mappings in map items*/
		 item_prototypes_[HealthPotion::ID] = new HealthPotion();
		 item_prototypes_[PermHP::ID] = new PermHP();
		 item_prototypes_[Key::ID] = new Key();
		 item_prototypes_[JetPack::ID] = new JetPack();
	}

	void addItem(Items::ItemID id, int quantity, Player &player) {
		/* Find the item in the prototype map and clone into the inventory map items */
		std::cout << "Adding Item: " << id << std::endl;
		/* Check if the ItemID exist */
		auto it = item_prototypes_.find(id);
		if (it != item_prototypes_.end()) {
			/* Now check to see if the player has one already */
			auto itr = items.find(id);
			if (itr != items.end()) {
				std::cout << "Item exist already, so increasing quantity" << std::endl;
				std::cout << "Pre-Quantity: " << itr->second << std::endl;
				items[id] += quantity;
				std::cout << "Post-Quantity: " << itr->second << std::endl;
			}
			else {
				std::cout << "Item did not exist. Adding it now" << std::endl;
				items.insert({ id, quantity });
				item_prototypes_[id]->raiseEventMsg(player);
			}
		}
	}

	void addItem(Items::ItemID id, int quantity) {
		/* Find the item in the prototype map and clone into the inventory map items */
		std::cout << "Adding Item: " << id << std::endl;
		/* Check if the ItemID exist */
		auto it = item_prototypes_.find(id);
		if (it != item_prototypes_.end()) {
			/* Now check to see if the player has one already */
			auto itr = items.find(id);
			if (itr != items.end()) {
				std::cout << "Item exist already, so increasing quantity" << std::endl;
				std::cout << "Pre-Quantity: " << itr->second << std::endl;
				items[id] += quantity;
				std::cout << "Post-Quantity: " << itr->second << std::endl;
			}
			else {
				std::cout << "Item did not exist. Adding it now" << std::endl;
				items.insert({ id, quantity });
			}
		}
	}

	bool checkItem(Items::ItemID id, int quantity) {
		auto itr = items.find(id);
		if (itr != items.end()) {
			if (items[id] >= quantity) {
				return true;
			}
			return false;
		}
		return false;
	 }

	void useItem(Items::ItemID id, Player &player) {
		/* Call the overriden function use that the item has */
		auto it = items.find(id);
		if (it != items.end()) {
			auto itr = item_prototypes_.find(id);
			if (itr != item_prototypes_.end()) {
				itr->second->use(player);
			}
		}
	}

	
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
	void draw(Graphics &graphics, Player &player);
	void drawQuantity(Graphics &graphics, int x, int y, int quantity);
	void addInstancedLoot(std::string mapName, int type);
	inline const std::vector<std::pair<std::string, int>> getLootTable() const { return this->lootTable; }
	inline void setLootTable(std::vector<std::pair<std::string, int>> table) { this->lootTable = table; }
	inline const std::map<Items::ItemID, int> getInventoryTable() const { return this->items; }
	inline void setInventoryTable(std::map<Items::ItemID, int> table) { this->items = table; }
	bool isLooted(std::string map, int iType);
	void storeItem(int type);
	bool hasKeyStored(int keyID);
	
	~Inventory();

private:
	std::map<Items::ItemID, int> items;
	std::map<Items::ItemID, Items*> item_prototypes_;

	// Health Sprites
	Sprite _iMenu;
	Sprite _hpPot;
	Sprite _key;
	Sprite _silverGem;

	int hpToGain;
	std::vector<std::pair<int, int>> inventoryTable;
	std::vector<std::pair<std::string, int>> lootTable;
protected:
	
};

