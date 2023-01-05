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
		 item_prototypes_[SilverGem::ID] = new SilverGem();
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

	bool checkInventorySlot(int inventIndex) {
		std::vector<int> keys;
		for (const auto&[itemID, itemCount] : items) {
			keys.push_back(itemID);
		}
		std::sort(keys.begin(), keys.end());

		if (keys.size() >= inventIndex) {
			/* Now that the index is validated do -1 to get the correct itemID*/
			inventIndex -= 1;
			this->currentItem = keys[inventIndex];
			return true;
		}
		return false;
	 }

	void useItemFromInvent(Player &player) {
		 this->useItem(this->currentItem, player);
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

	void drawItemProperties(Graphics &graphics, Player& player, int itemID) {
		/* Get the map that holds the properties for each item. */
		const std::map<std::string, std::variant<int, std::string>> properties = item_prototypes_[itemID]->getProperties();
		std::string iName = "NULL";

		iName = item_prototypes_[itemID]->getItemName();

		this->drawInventItem(graphics, player.getX() - 320, player.getY() - 110, iName);

		int x = player.getX() - 320;
		int y = player.getY() - 95;
		int counter = 0;
		std::string description = "Description: NULL";
		/* Get each properties of the item and draw */
		for (const auto& property : properties) {
			if (property.second.index() == 0) {
				int num = std::get<int>(property.second);
				std::string text = property.first + ": " + std::to_string(num);
				/* Convert to string and draw */
				this->drawInventItem(graphics, x, y, text);
			}
			else {
				std::string text = property.first + ": " + std::get<std::string>(property.second);
				if (property.first == "Description") {
					description = text;
					continue;
				}
				/* Draw here */
				this->drawInventItem(graphics, x, y, text);
			}
			y += 14;
		}
		this->drawInventItem(graphics, x, y, description);
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
	void drawInventSelection(Graphics &graphics, int x, int y);
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
	std::map<Items::ItemID, Sprite> itemSprites;

	// Health Sprites
	Sprite _iMenu;
	Sprite _iSelection;
	Sprite _hpPot;
	Sprite _key;
	Sprite _silverGem;

	int hpToGain;
	int currentItem = 0;
	std::vector<std::pair<int, int>> inventoryTable;
	std::vector<std::pair<std::string, int>> lootTable;
protected:
	
};

