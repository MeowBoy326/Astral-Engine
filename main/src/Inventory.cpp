#include "../headers/Inventory.h"

#include <iostream>

Inventory::Inventory()
{
}

Inventory::Inventory(Graphics & graphics, Player & player)
{
	this->_player = player;
	this->_iMenu = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 87, 40, 45, 35, 70); // x, y, width, height, screen pos x, screen pos y
	this->_hpPot = Sprite(graphics, "data\\maps\\NpcSym.png", 32, 83, 13, 11, 35, 70);
	this->_key = Sprite(graphics, "data\\maps\\NpcSym.png", 194, 4, 12, 10, 35, 70);
	this->_silverGem = Sprite(graphics, "data\\maps\\loot.png", 72, 50, 16, 16, 35, 70);
}

void Inventory::storeItem(int type)
{
	std::cout << "Stored item to inventory" << std::endl;
	bool found = false;
	if (this->inventoryTable.size() == 0)
		this->inventoryTable.push_back(std::make_pair(1, type));
	else {
		for (int index = 0; index < this->inventoryTable.size(); ++index) {
			if (this->inventoryTable[index].second == type) {
				this->inventoryTable[index].first += 1;
				found = true;
			}
		}
		if (!found)
			this->inventoryTable.push_back(std::make_pair(1, type));
	}
}

bool Inventory::hasKeyStored()
{
	std::cout << "Finding Key in Inventory" << std::endl;
	bool found = false;
	if (this->inventoryTable.size() == 0)
		return false;
	else {
		for (int index = 0; index < this->inventoryTable.size(); ++index) {
			if (this->inventoryTable[index].second == 3) {
				if (this->inventoryTable[index].first >= 2) {
					this->inventoryTable[index].first -= 1;
					return true;
				}
				else if (this->inventoryTable[index].first == 1) {
					this->inventoryTable.erase(this->inventoryTable.begin() + index);
					return true;
				}
			}
		}
	}
	return false;
}

void Inventory::update(int elapsedTime, Player & player)
{
}

void Inventory::useItem(int type, Player &player) {
	if (type == 0 && this->inventoryTable.size() > 0) {
		for (int index = 0; index < this->inventoryTable.size(); ++index) {
			if (this->inventoryTable[index].second == 0 && this->inventoryTable[index].first >=1 && 
				player.getCurrentHealth() < player.getMaxHealth()) {
				player.gainHealth(player.getMaxHealth() / 3);
				this->inventoryTable[index].first -= 1;
				if (this->inventoryTable[index].first == 0)
					this->inventoryTable.erase(this->inventoryTable.begin() + index);
			}
			else {
				std::cout << "Either your HP is full or you do not have a Health Potion!" << std::endl;
			}
		}
	}
	else {
		std::cout << "Inventory is empty" << std::endl;
	}
}

void Inventory::draw(Graphics & graphics, Player & player)
{
	this->_iMenu.drawiMenu(graphics, player.getX()-130, player.getY() - 130);
	std::string cels = "Celestials:"+std::to_string(player.getCurrency());
	this->drawCurrency(graphics, player.getX() - 100, player.getY() + 125, cels);
	for (int index = 0; index < this->inventoryTable.size(); ++index) {
		if (this->inventoryTable[index].second == 0) {
			this->_hpPot.draw(graphics, player.getX() - 120, player.getY() - 110);
			this->drawQuantity(graphics, player.getX() - 110, player.getY() - 100, this->inventoryTable[index].first);
		}
		else if (this->inventoryTable[index].second == 3) {
			this->_key.draw(graphics, player.getX() - 90, player.getY() - 110);
			this->drawQuantity(graphics, player.getX() - 80, player.getY() - 100, this->inventoryTable[index].first);
		}
		else if (this->inventoryTable[index].second == 4) {
			this->_silverGem.draw(graphics, player.getX() - 60, player.getY() - 110);
			this->drawQuantity(graphics, player.getX() - 50, player.getY() - 100, this->inventoryTable[index].first);
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
