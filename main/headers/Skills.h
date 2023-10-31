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
	virtual void updateSkillStats(Player &player) = 0;
	virtual void draw(Graphics & graphics);
	virtual void drawSkills(Graphics &graphics, float x, float y);
	virtual const inline std::string getSkillName() { return this->name; }
	virtual void raiseEventMsg(Player &player) = 0;
	virtual const inline int getSkillLevel() { return this->skillLevel; }
	virtual inline void setSkillLevel(int skillLevel) { this->skillLevel = skillLevel; }
	virtual inline void raiseSkillLevel(int skillLevel) { this->skillLevel = skillLevel; }
	virtual const std::map<std::string, std::variant<int, float, std::string>> getProperties() = 0;

	~Skills();

private:
	std::vector<std::pair<std::string, int>> skillIDTable;

protected:
	SkillID _skillID;
	std::string name;
	int skillLevel;
	float skillCost;
	std::map<std::string, std::variant<int, float, std::string>> properties_;
};


class LifeSteal : public Skills {
public:
	LifeSteal();
	LifeSteal(Graphics &graphics, Vector2 spawnPoint);
	~LifeSteal() override;

	static const SkillID ID = 0;

	void use(Player &player) override {
		// Implement HP pot

		//player.handleLifeSteal

	}

	void updateSkillStats(Player &player) override {
		this->updateLifeSteal();
	}

	Skills* clone() const override { return new LifeSteal(*this); }

	void raiseEventMsg(Player &player) override {
		player.setEventMessage("Life Steal has been learned");
	}

	void update(int elapsedTime, Player & player) override;

	void draw(Graphics & graphics) override;
	void drawSkills(Graphics &graphics, float x, float y) override;

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	const inline std::string getSkillName() override { return this->name; }
	const inline int getSkillLevel() override { return this->skillLevel; }
	inline void setSkillLevel(int skillLevel) override { this->skillLevel = skillLevel; }
	inline void raiseSkillLevel(int skillLevel) override { this->skillLevel += skillLevel; setLifeSteal(); updateLifeSteal(); }
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
			if (std::holds_alternative<int>(it->second)) {
				this->skillCost = std::get<int>(it->second);
			}
		}
	}

	inline void setLifeSteal() {
		auto it = this->properties_.find("Life Steal");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<float>(it->second)) {
				std::get<float>(it->second) = ((static_cast<float>(this->skillLevel) * 0.002f) + 0.003f);
			}
		}
		it = this->properties_.find("Cost");
		if (it != this->properties_.end()) {
			if (std::holds_alternative<int>(it->second)) {
				int currentCost = std::get<int>(it->second);
				currentCost += this->skillLevel + 2 * 5;
				it->second = currentCost;
			}
		}
	}
	
private:
	float lifeSteal = 0.002f;
	std::string name = "Life Steal";
	int skillLevel = 0;
	float skillCost = 5.0f;
	std::map<std::string, std::variant<int, float, std::string>> properties_ = {
		{"Level", skillLevel},
		{"Life Steal", lifeSteal},
		{"Cost", std::to_string(skillCost) + "% of max health"},
		{"Description", std::string("Drain the life from your enemies by > Skill LS " + std::to_string((lifeSteal * 1)) + "%" " + Base Life Steal <") }
	};
};

