#pragma once

#include <string>
#include <vector>
#include "AnimatedSprite.h"
#include "Player.h"
#include <iostream>
#include <variant>

class Graphics;

class Skills : public AnimatedSprite {
public:
	typedef int SkillID;

	Skills();
	Skills(Graphics &graphics);
	Skills(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	SkillID getSkillID() const { return _skillID; }

	virtual Skills* clone() const = 0;
	virtual void use(Player &player) = 0;
	virtual void update(int elapsedTime, Player & player);
	virtual void updateSkillStats(Player &player, int elapsedTime) = 0;
	virtual void draw(Graphics & graphics);
	virtual void drawSkills(Graphics &graphics, float x, float y);
	virtual const inline std::string getSkillName() { return this->name; }
	virtual void raiseEventMsg(Player &player) = 0;
	virtual void raiseCustomMsg(Player &player, std::string message) = 0;
	virtual const inline int getSkillLevel() { return this->skillLevel; }
	virtual void refreshSkillsFromTable() = 0;
	virtual inline void setSkillLevel(int skillLevel) { this->skillLevel = skillLevel; }
	virtual inline void raiseSkillLevel(int skillLevel) { this->skillLevel += skillLevel; }
	virtual inline void setSkillActive(bool active) { this->skillActive = active; }
	virtual inline bool getSkillActive(bool active) { return this->skillActive; }
	virtual inline bool checkSkillCooldown() { if (this->skillCooldown == 0) { return true; } return false; }
	virtual const std::map<std::string, std::variant<int, float, std::string>> getProperties() = 0;

	~Skills();

private:
	std::vector<std::pair<std::string, int>> skillIDTable;

protected:
	SkillID _skillID;
	std::string name;
	int skillLevel;
	float skillCost;
	bool skillActive;
	bool skillOffCD;
	int skillCooldown;
	std::map<std::string, std::variant<int, float, std::string>> properties_;
};


class LifeSteal : public Skills {
public:
	LifeSteal();
	LifeSteal(Graphics &graphics, Vector2 spawnPoint);
	~LifeSteal() override;

	static const SkillID ID = 0;

	void use(Player &player) override {
		if (!skillActive && skillOffCD) {
			if (this->skillLevel > 0) {
				float hpReduc = this->skillCost / 100.0f;
				float lifeStealAdd = this->lifeSteal + this->baseLifeSteal;
				this->raiseCustomMsg(player, "Life Steal Activated | HP Reduction: " +
					globals::to_string_with_precision(hpReduc, 1) +
					"%, Lifesteal: " +
					globals::to_string_with_precision(lifeStealAdd, 1) +
					"%"
				);
				player.setMaxHealth(player.getMaxHealth() - (player.getMaxHealth() * hpReduc));
				if (player.getCurrentHealth() >= player.getMaxHealth())
					player.setCurrentHealth(player.getMaxHealth());
				player.raiseLifeSteal(lifeStealAdd);
				player.setLifeStealActive(true);
				skillActive = !skillActive;
				skillCooldown = 180000;
				skillOffCD = !skillOffCD;
			}
			else {
				this->raiseCustomMsg(player, "LifeSteal is not level 1+");
			}
		}
		else if (skillActive && skillOffCD) {
			float hpReduc = this->skillCost / 100.0f;
			float lifeStealAdd = this->lifeSteal + this->baseLifeSteal;
			this->raiseCustomMsg(player, "Life Steal Off");
			player.setMaxHealth(player.getMaxHealth() + (player.getMaxHealth() * hpReduc));
			if (player.getCurrentHealth() >= player.getMaxHealth())
				player.setCurrentHealth(player.getMaxHealth());
			player.setLifeSteal(0.002f);
			player.setLifeStealActive(false);
			skillActive = !skillActive;
			skillCooldown = 180000;
			skillOffCD = !skillOffCD;
		}

	}

	void updateSkillStats(Player &player, int elapsedTime) override {
		// Called every update
		// Use for returning updated values to player class

		if (!this->skillOffCD) {
			this->skillCooldown -= elapsedTime;
			this->skillOffCD = false;
			if (this->skillCooldown <= 0) {
				this->skillCooldown = 0;
				this->skillOffCD = !this->skillOffCD;
			}
		}
	}

	Skills* clone() const override { return new LifeSteal(*this); }

	void raiseEventMsg(Player &player) override {
		player.setEventMessage("Life Steal has been learned");
	}

	void raiseCustomMsg(Player &player, std::string message) override { player.setEventMessage(message); }

	void update(int elapsedTime, Player & player) override;

	void draw(Graphics & graphics) override;
	void drawSkills(Graphics &graphics, float x, float y) override;

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	const inline std::string getSkillName() override { return this->name; }
	const inline int getSkillLevel() override { return this->skillLevel; }
	virtual void refreshSkillsFromTable() override { setLifeSteal(); }
	inline void setSkillLevel(int skillLevel) override { this->skillLevel = skillLevel; }
	inline void raiseSkillLevel(int skillLevel) override { this->skillLevel += skillLevel; setLifeSteal(); }
	const inline std::map<std::string, std::variant<int, float, std::string>> getProperties() override { return this->properties_; }

	inline void updateLifeSteal() {
		auto it = this->properties_.find("Life Steal");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<float>(it->second)) {
				this->lifeSteal = std::get<float>(it->second);
			}
		}
		it = it = this->properties_.find("Cost");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<float>(it->second)) {
				this->skillCost = std::get<float>(it->second);
			}
		}
		it = it = this->properties_.find("Level");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<int>(it->second)) {
				this->skillLevel = std::get<int>(it->second);
			}
		}
		it = it = this->properties_.find("Description");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<std::string>(it->second)) {
				this->description = std::get<std::string>(it->second);
			}
		}
	}

	inline void setLifeSteal() {
		auto it = this->properties_.find("Life Steal");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<std::string>(it->second)) {
				this->lifeSteal = ((static_cast<float>(this->skillLevel) * 0.002f) + 0.003f);
				this->lifeStealText = globals::to_string_with_precision(this->lifeSteal * 100, 1) + "%";
				it->second = this->lifeStealText;
			}
		}
		it = this->properties_.find("Cost");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<std::string>(it->second)) {
				//float currentCost = std::get<float>(it->second);
				this->skillCost = (static_cast<float>(this->skillLevel) + 2.0f) * 5.0f;
				if (this->skillLevel == 0)
					this->skillCost = (static_cast<float>(1) + 2.0f) * 5.0f;
				this->skillCostText = globals::to_string_with_precision(this->skillCost, 1) + "% of max health";
				it->second = skillCostText;
			}
		}
		it = this->properties_.find("Level");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<int>(it->second)) {
				it->second = this->skillLevel;
			}
		}
		it = this->properties_.find("Description");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<std::string>(it->second)) {
				this->description = "Drain the life from your enemies by > Skill LifeSteal " +
					globals::to_string_with_precision(this->lifeSteal * 100, 1) + "%" " + Base LifeSteal " +
					globals::to_string_with_precision(this->baseLifeSteal * 100, 1) + "% < " +
					globals::to_string_with_precision((this->lifeSteal + this->baseLifeSteal) * 100, 1) + "%";
				it->second = this->description;
			}
		}
	}
	
private:
	bool skillActive = false;
	bool skillOffCD = true;
	int skillCooldown = 0;
	float lifeSteal = 0.005f;
	float baseLifeSteal = 0.002f;
	float skillCost = 15.0f;
	int skillLevel = 0;
	std::string name = "Life Steal";
	std::string lifeStealText = globals::to_string_with_precision(this->lifeSteal * 100, 1) + "%";
	std::string description = "Drain the life from your enemies by > Skill LifeSteal " + 
		globals::to_string_with_precision(this->lifeSteal * 100, 1) + "%" " + Base LifeSteal " + 
		globals::to_string_with_precision(this->baseLifeSteal * 100, 1) +  "% < " +
		globals::to_string_with_precision((this->lifeSteal + this->baseLifeSteal) * 100, 1) + "%";
	std::string skillCostText = globals::to_string_with_precision(this->skillCost, 1) + "% of max health";
	std::map<std::string, std::variant<int, float, std::string>> properties_ = {
		{"Level", skillLevel},
		{"Life Steal", lifeStealText},
		{"Cost", skillCostText},
		{"Cooldown (Toggle On/Off)", "180s"},
		{"Description", description}
	};
};

