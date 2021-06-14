#include "Inventory.h"

#include <iostream>

namespace inventoryConstants {
	std::vector<Inventory::InventoryItem*> inventory;
}

Inventory::Inventory()
{
}

Inventory::Inventory(Graphics & graphics, Player & player)
{
	this->_player = player;
	this->_iMenu = Sprite(graphics, "TextBox.png", 0, 87, 40, 45, 35, 70); //x, y, width, height, screen pos x, screen pos y
	this->_hpPot = Sprite(graphics, "NpcSym.png", 32, 83, 13, 11, 35, 70);
}

void Inventory::addItem(struct Inventory::InventoryItem *item) {
	//InventoryItem* hPot = new InventoryItem;
	//hPot->type = item->type;
	//this->inventory.push_back(hPot);
	//inventoryConstants::inventory.push_back(hPot);
	inventoryConstants::inventory.push_back(item);
	std::cout << "size of inventory: " << inventoryConstants::inventory.size() << std::endl;
}

void Inventory::update(int elapsedTime, Player & player)
{
	//this->_player = player;
	//this->_healthNumber1.setSourceRectX(8 * this->_player.getCurrentHealth());
}

void Inventory::useItem(int type, Player &player) {
	if (type == 0) { //hpPot
		if (inventoryConstants::inventory.size() == 0) {
			std::cout << "you do not have anything in your inventory!" << std::endl;
			return;
		}
		for (int i = 0; i < inventoryConstants::inventory.size(); i++) {
			if (inventoryConstants::inventory.at(i)->type == HEALTH_POT) {
				if (player.getCurrentHealth() < player.getMaxHealth()) {
					this->hpToGain = player.getMaxHealth() - player.getCurrentHealth();
					player.gainHealth(hpToGain);
					std::cout << "player gained " << hpToGain << " hp" << std::endl;
					inventoryConstants::inventory.erase(inventoryConstants::inventory.begin() + i);
				}
				else {
					std::cout << "HP is already full!" << std::endl;
					return;
				}
			}
			else if (!(inventoryConstants::inventory.at(i)->type == HEALTH_POT)) {
				std::cout << "you do not have any health pots!" << std::endl;
			}
		}

	}
}

void Inventory::draw(Graphics & graphics, Player & player)
{
	this->_iMenu.drawiMenu(graphics, player.getX()-130, player.getY() - 130);
	for (int i = 0; i < inventoryConstants::inventory.size(); i++) {
		if (inventoryConstants::inventory.at(i)->type == HEALTH_POT) {
			this->_hpPot.draw(graphics, player.getX() - 120, player.getY() - 110);
			this->drawQuantity(graphics, player.getX() - 110, player.getY() - 100, i + 1);
		}
	}
}

void Inventory::drawQuantity(Graphics & graphics, int x, int y, int quantity)
{
	std::string str = std::to_string(quantity);
	this->drawItemQuantity(graphics, 100, 100, str, x, y);
}

void Inventory::addInstancedLoot(std::string mapName, int type)
{
	this->lootTable.push_back(std::make_pair(mapName, type));
}

bool Inventory::isLooted(std::string map, int iType)
{
	for (auto iter = this->lootTable.begin(); iter != this->lootTable.end(); iter++) {
		auto first = iter->first;
		auto second = iter->second;
		if (iter->first == map && iter->second == iType) {
			std::cout << "Loot Table: " << iter->first << " , " << iter->second << std::endl;
			return true;
		}
	}
	return false;
}

Inventory::~Inventory()
{
}
