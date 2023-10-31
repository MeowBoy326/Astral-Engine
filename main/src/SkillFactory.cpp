#include "../headers/SkillFactory.h"

#include <iostream>

SkillFactory::SkillFactory() {
}

SkillFactory::SkillFactory(Graphics & graphics, Player & player) {
	this->_sMenu = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 87, 40, 45, 35, 70); // x, y, width, height, screen pos x, screen pos y
	this->_sSelection = Sprite(graphics, "data\\graphics\\startGame.png", 0, 63, 19, 12, 185, 275);

	this->_lifeSteal = Sprite(graphics, "data\\graphics\\skills.png", 0, 0, 24, 24, 35, 70);

	graphics.loadImage("data\\graphics\\skills.png");

	skillSprites[LifeSteal::ID] = this->_lifeSteal;

	initPrototypes();
}

void SkillFactory::storeSkill(int type) {
	std::cout << "Stored Skill to inventory" << std::endl;
	bool found = false;
	if (this->skillTable.size() == 0)
		this->skillTable.push_back(std::make_pair(1, type));
	else {
		for (int index = 0; index < this->skillTable.size(); ++index) {
			if (this->skillTable[index].second == type) {
				this->skillTable[index].first += 1;
				found = true;
			}
		}
		if (!found)
			this->skillTable.push_back(std::make_pair(1, type));
	}
}

bool SkillFactory::hasKeyStored(int keyID) {
	std::cout << "Finding Skill in Skill" << std::endl;

	auto itr = skills.find(keyID);
	if (itr != skills.end()) {
		std::cout << "Key found." << std::endl;
		if (skills[keyID] > 0) {
			skills[keyID] -= 1;
			std::cout << "Post-Quantity: " << itr->second << std::endl;
			return true;
		}
		std::cout << "Key found but quantity = 0" << std::endl;
		return false;
	}
	std::cout << "Key not found." << std::endl;
	return false;
}

void SkillFactory::update(int elapsedTime, Player & player) {
}

void SkillFactory::draw(Graphics & graphics, Player & player) {
	this->_sMenu.drawiMenu(graphics, player.getX() - 95, player.getY() - 130);
	std::string cels = "Celestials:" + std::to_string(player.getCurrency());
	this->drawCurrency(graphics, player.getX() - 70, player.getY() + 125, cels);

	int x = player.getX() - 65;
	int y = player.getY() - 105;
	int counter = 0;

	for (const auto&[skillID, skillCount] : skills) {
		// Draw the skill sprite at the current x and y position
		skillSprites[skillID].drawScaled(graphics, x, y, -1.3);
		this->drawQuantity(graphics, x + 6, y + 27, skillCount);

		// Increment the counter and update the x and y positions as necessary
		counter++;
		if (counter % 4 == 0) {
			y += 68;
			x = player.getX() - 65;
		}
		else {
			x += 68;
		}
	}

	this->drawSkillProperties(graphics, player, this->currentSkill);

	/*for (int index = 0; index < this->inventoryTable.size(); ++index) {
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
	}*/
}

void SkillFactory::drawSkillSelection(Graphics& graphics, int x, int y) {
	this->_sSelection.drawScaled(graphics, x - 15, y + 5, 0);
}

void SkillFactory::drawQuantity(Graphics & graphics, int x, int y, int quantity) {
	std::string str = std::to_string(quantity);
	this->drawItemQuantity(graphics, 100, 100, str, x, y);
}

void SkillFactory::addInstancedLoot(std::string mapName, int type) {
	this->lootTable.push_back(std::make_pair(mapName, type));
}

bool SkillFactory::isLooted(std::string map, int iType) {
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

SkillFactory::~SkillFactory() {
}
