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

	virtual const std::map<std::string, std::variant<int, std::string>> getProperties() = 0;

	~Skills();

private:
	std::vector<std::pair<std::string, int>> skillIDTable;

protected:
	SkillID _skillID;
	std::string name;
	std::map<std::string, std::variant<int, std::string>> properties_;
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
		this->properties_["Level"] = player.getHpPotStrength();
		this->properties_["Life Steal"] = player.getHpPotCapacity();
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

	const inline std::map<std::string, std::variant<int, std::string>> getProperties() override { return this->properties_; }

private:
	float lifeSteal;
	std::string name = "Life Steal";
	std::map<std::string, std::variant<int, std::string>> properties_ = {
		{"Level", 2},
		{"Life Steal", 4},
		{"Value", 0},
		{"Description", std::string("Drain the life from your enemies by >" + std::string("3%") ) }
	};
};

