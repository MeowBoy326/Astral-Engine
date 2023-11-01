#pragma once

#include "sprite.h"
#include "Player.h"
#include <vector>
#include "TextManager.h"
#include "Skills.h"
#include <map>

class Graphics;
// Class Player; Can't forward declare player here because Player_player is not a pointer

class SkillFactory : public TextManager {
public:
	SkillFactory();
	SkillFactory(Graphics &graphics, Player &player);

	void initPrototypes() {
		/* Create a map for the ID system. Create skills based off the mappings in map skills*/
		skill_prototypes_[LifeSteal::ID] = new LifeSteal();
	}

	void addSkill(Skills::SkillID id, int quantity, Player &player) {
		/* Find the skill in the prototype map and clone into the inventory map skills */
		std::cout << "Adding Skill: " << id << std::endl;
		/* Check if the skillID exist */
		auto it = skill_prototypes_.find(id);
		if (it != skill_prototypes_.end()) {
			/* Now check to see if the player has one already */
			auto itr = skills.find(id);
			if (itr != skills.end()) {
				std::cout << "Skill exist already, so increasing quantity" << std::endl;
				std::cout << "Pre-Quantity: " << itr->second << std::endl;
				skills[id] += quantity;
				std::cout << "Post-Quantity: " << itr->second << std::endl;
			}
			else {
				std::cout << "Skill did not exist. Adding it now" << std::endl;
				skills.insert({ id, quantity });
				skill_prototypes_[id]->raiseEventMsg(player);
			}
		}
	}

	void addSkill(Skills::SkillID id, int quantity) {
		/* Find the skill in the prototype map and clone into the inventory map skills */
		std::cout << "Adding Skill: " << id << std::endl;
		/* Check if the skillID exist */
		auto it = skill_prototypes_.find(id);
		if (it != skill_prototypes_.end()) {
			/* Now check to see if the player has one already */
			auto itr = skills.find(id);
			if (itr != skills.end()) {
				std::cout << "Skill exist already, so increasing quantity" << std::endl;
				std::cout << "Pre-Quantity: " << itr->second << std::endl;
				skills[id] += quantity;
				std::cout << "Post-Quantity: " << itr->second << std::endl;
			}
			else {
				std::cout << "Skill did not exist. Adding it now" << std::endl;
				skills.insert({ id, quantity });
			}
		}
	}

	bool checkSkill(Skills::SkillID id, int quantity) {
		auto itr = skills.find(id);
		if (itr != skills.end()) {
			if (skills[id] >= quantity) {
				return true;
			}
			return false;
		}
		return false;
	}

	bool checkSkillSlot(int skillIndex) {
		std::vector<int> keys;
		for (const auto&[skillID, skillCount] : skills) {
			keys.push_back(skillID);
		}
		std::sort(keys.begin(), keys.end());

		if (keys.size() >= skillIndex) {
			/* Now that the index is validated do -1 to get the correct skillID*/
			skillIndex -= 1;
			this->currentSkill = keys[skillIndex];
			return true;
		}
		return false;
	}

	void useSkillFromInvent(Player &player) {
		this->useSkill(this->currentSkill, player);
	}

	void bindSkillToKey(int hotkey) {
		auto it = skillHotkeys.find(hotkey);
		if (it != skillHotkeys.end()) {
			// Replace current hotkey with this new skill (rebind)
			it->second = this->currentSkill;
		}
		else if (it == skills.end()) {
			// Hotkey is not assigned, insert skill bind
			skillHotkeys.insert(hotkey, this->currentSkill);
		}
	}

	void resetCurrentSkill() { this->currentSkill = 0; }

	void useSkill(Skills::SkillID id, Player &player) {
		/* Call the overriden function use that the skill has */
		auto it = skills.find(id);
		if (it != skills.end()) {
			auto itr = skill_prototypes_.find(id);
			if (itr != skill_prototypes_.end()) {
				itr->second->use(player);
				it->second = itr->second->getSkillLevel();
			}
		}
	}

	void drawSkillProperties(Graphics &graphics, Player& player, int skillID) {
		/* Get the map that holds the properties for each skill. */
		const std::map<std::string, std::variant<int, float, std::string>> properties = skill_prototypes_[skillID]->getProperties();
		std::string iName = "NULL";

		skill_prototypes_[skillID]->updateSkillStats(player);
		iName = skill_prototypes_[skillID]->getSkillName();

		this->drawMultiLine(graphics, player.getX() - 320, player.getY() - 110, iName);

		int x = player.getX() - 320;
		int y = player.getY() - 95;
		int newY = 0;
		int counter = 0;
		std::string description = "Description: NULL";
		/* Get each properties of the skill and draw */
		for (const auto& property : properties) {
			if (property.second.index() == 0) {
				int num = std::get<int>(property.second);
				std::string text = property.first + ": " + std::to_string(num);
				/* Convert to string and draw */
				newY = this->drawMultiLine(graphics, x, y, text);
			}
			else if (std::holds_alternative<float>(property.second)) {
				float fnum = std::get<float>(property.second);
				std::string text = property.first + ": " + std::to_string(fnum);
				newY = this->drawMultiLine(graphics, x, y, text);
			}
			else if (std::holds_alternative<int>(property.second)) {
				int inum = std::get<int>(property.second);
				std::string text = property.first + ": " + std::to_string(inum);
				newY = this->drawMultiLine(graphics, x, y, text);
			}
			else {
				std::string text = property.first + ": " + std::get<std::string>(property.second);
				if (property.first == "Description") {
					description = text;
					continue;
				}
				/* Draw here */
				newY = this->drawMultiLine(graphics, x, y, text);
			}
			y += newY;
		}
		this->drawInventItem(graphics, x, y, description);
	}


	enum SkillType {
		HEALTH_POT,
		BUFF
		// ...
	};

	struct SkillItem {
		SkillType type;
		std::string imagePath;
	};

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics, Player &player);
	void drawSkillSelection(Graphics &graphics, int x, int y);
	void drawQuantity(Graphics &graphics, int x, int y, int quantity);
	void addInstancedLoot(std::string mapName, int type);
	inline const std::vector<std::pair<std::string, int>> getLootTable() const { return this->lootTable; }
	inline void setLootTable(std::vector<std::pair<std::string, int>> table) { this->lootTable = table; }
	inline const std::map<Skills::SkillID, int> getSkillTable() const { return this->skills; }
	inline void setSkillTable(std::map<Skills::SkillID, int> table) { this->skills = table; }
	inline const std::map<int, Skills::SkillID> getSkillHotkeys() const { return this->skillHotkeys; }
	inline void setSkillHotkeys(std::map<int, Skills::SkillID> table) { this->skillHotkeys = table; }
	bool isLooted(std::string map, int iType);
	void storeSkill(int type);
	bool hasKeyStored(int keyID);

	~SkillFactory();

private:
	// Skill ID, Skill Level
	std::map<Skills::SkillID, int> skills;
	std::map<int, Skills::SkillID> skillHotkeys;
	std::map<Skills::SkillID, Skills*> skill_prototypes_;
	std::map<Skills::SkillID, Sprite> skillSprites;

	// Health Sprites
	Sprite _sMenu;
	Sprite _sSelection;
	Sprite _lifeSteal;

	int hpToGain;
	int currentSkill = 0;
	std::vector<std::pair<int, int>> skillTable;
	std::vector<std::pair<std::string, int>> lootTable;
protected:
};

