#ifndef NPC_H
#define NPC_H

#pragma once

#include "TextManager.h"
#include "AnimatedSprite.h"
//#include "Global.h"
//#include "Player.h"
//#include "Graphics.h"


//#include <string>
//#include <queue>
#include <fstream>
//#include <iostream>
//#include <SDL_ttf.h>

// Class Graphics;
// Class TextManager;

class Npc : public TextManager, public AnimatedSprite  {
public:
	Npc();
	Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	const inline std::string getName() const { return this->_npcName; }

	void npcSelection(Graphics &graphics, int posX, int posY, int selection);
	void setNpcIcon(Graphics &graphics, const std::string name, int posX, int posY);
	void drawNpcIcon(Graphics &graphics, const std::string name, int posX, int posY);
	int playScript(std::string name, Graphics &graphics, int posX, int posY);
	int playNext(std::string name, Graphics &graphics, int posX, int posY, Player &player);
	inline bool const getChatStatus() { return this->endOfChat; }
	inline void setChatStatus(bool condition) { this->endOfChat = condition; }
	inline bool const getNpcTalk() { return this->npcTalking; }
	inline void setNpcTalk(bool condition) { this->npcTalking = condition; }
	void resetScripts();
	
	int loadQuests(std::string name);
	void displayQuests(Graphics &grpahics, std::string npcName, int posX, int posY, Player &player);
	void questSelection(Graphics &graphics, int posX, int posY, int selection);
	void acceptQuest(Graphics &graphics, std::string npcName, int posX, int posY, Player &player, int selection);
	void giveRewards(Graphics &graphics, std::string npcName, int posX, int posY, Player &player, int selection);
	bool checkQuest(Graphics &graphics, std::string name, int posX, int posY, Player &player);
	inline bool const getQuestState() { return this->questState; }
	inline void setQuestState(bool condition) { this->questState = condition; }
	inline bool const checkQuestDone() { return this->questDone; }
	inline bool const getQuestMenuState() { return this->questMenu; }
	inline void const setQuestMenuState(bool condition) { this->questMenu = condition; }
	inline void setQuestDone(bool condition) { this->questDone = condition; }
	inline bool const checkNoQuests() { return this->noQuest; }
	inline void setNoQuest(bool condition) { this->noQuest = condition; }
	inline const std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> getQuestLog() const { return this->questLog; }
	inline void setQuestLog(std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> table) { this->questLog = table; }
protected:
	Direction _direction;
	//questName, type, object, amount, description, npcName, rewardType, reward(can be converted to int for level), exp, cels
	std::vector<std::tuple<std::string, int, std::string, int, std::string, std::string, int, std::string, int, int>> questTable;
	//questName, type, object, amount, isCompleted, isRewarded
	std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> questLog;

	int _maxHealth;
	int _currentHealth;

	std::string _npcName;
	std::string storedQuestName;
	std::string reward;
	int levelReward, rewardType, exp, cels;

	Sprite* _chatSelection;
	Sprite* _questSelection;
	Sprite* _txtBox;
	Sprite* _npcBox;
	Sprite* _npcIcon;

	int lines = 1;
	int lineCounter = 1;
	char lineChar = 'a';
	bool endOfChat = false;
	bool questState = false;
	bool questDone = false;
	bool npcTalking = false;
	bool questMenu = false;
	bool noQuest = false;
};

class Clock : public Npc {
public:
	Clock();
	Clock(Graphics &graphics, Vector2 spawPoint, std::string name);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void bulletHit();

	void animationDone(std::string currentAnimation);
	void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp;
};

#endif