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
	const inline int getNpcId() const { return this->npcID; }

	void npcSelection(Graphics &graphics, int posX, int posY, int selection);
	void setNpcIcon(Graphics &graphics, const std::string name, int posX, int posY);
	void drawNpcIcon(Graphics &graphics, const std::string name, int posX, int posY);
	int playScript(int npcID, Graphics &graphics, int posX, int posY);
	int playNext(int npcID, Graphics &graphics, int posX, int posY, Player &player);
	int repeatScript(Graphics &graphics, int posX, int posY);
	int playQuest(int npcID, int selection, Graphics &graphics, int posX, int posY, Player &player);
	int playNextQuest(int npcID, Graphics &graphics, int posX, int posY, Player &player);
	int repeatQuestScript(Graphics &graphics, int posX, int posY);
	inline bool const getChatStatus() { return this->endOfChat; }
	inline void setChatStatus(bool condition) { this->endOfChat = condition; }
	inline bool const getNpcTalk() { return this->npcTalking; }
	inline void setNpcTalk(bool condition) { this->npcTalking = condition; }
	void resetScripts();
	
	int loadQuests(int npcID);
	void displayQuests(Graphics &grpahics, int npcID, int posX, int posY, Player &player);
	void questSelection(Graphics &graphics, int posX, int posY, int selection);
	void acceptQuest(Graphics &graphics, int npcID, int posX, int posY, Player &player, int selection);
	void giveRewards(Player &player);
	bool checkQuest(Player &player);
	inline bool const getQuestState() { return this->questState; }
	inline void setQuestState(bool condition) { this->questState = condition; }
	inline bool const checkQuestDone() { return this->questDone; }
	inline bool const getQuestMenuState() { return this->questMenu; }
	inline void const setQuestMenuState(bool condition) { this->questMenu = condition; }
	inline void setQuestDone(bool condition) { this->questDone = condition; }
	inline bool const checkNoQuests() { return this->noQuest; }
	inline void setNoQuest(bool condition) { this->noQuest = condition; }
	/*inline const std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> getQuestLog() const { return this->questLog; }
	inline void setQuestLog(std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> table) { this->questLog = table; }*/
	inline const std::vector<std::pair<std::string, int>> getNpcIdTable() const { return this->npcIDTable; }
	const std::string getNpcNameID(int npcID);
	inline bool const checkQuestBlocked() { return this->blockSelectedQuest; }
protected:
	Direction _direction;
	// (0)QuestName, (1)type, (2)object, (3)amount, (4)dialogueText(Vector of strings), (5)finishText(Vector of strings),
	// (6)npcID (formerly npcName), (7)rewardItemID, (8)rewardAmount, (9)exp, (10)cels, (11)isCompleted
	std::vector<std::tuple<std::string, int, std::string, int, std::vector<std::string>, std::vector<std::string>,
	int, int, int, int, int, bool>> questTable;
	// QuestName, type, object, amount, isCompleted, isRewarded, 
	//std::vector<std::tuple<std::string, int, std::string, int, bool, bool, int, std::string, int, int>> questLog;
	// ID of NPC, DialogueID player has reached
	std::vector<std::pair<int, int>> npcDialogueTable;
	// NPC ID table - Stores the name with the corresponding ID
	std::vector<std::pair<std::string, int>> npcIDTable;
	// Store the currently selected quest dialogue to be accessed throughout the chat
	std::vector<std::string> currentQuestDialogue;

	int _maxHealth;
	int _currentHealth;

	std::string _npcName;
	// NPC's will never have more than 2 quests per NPC ID.
	std::string questName1, questName2;
	std::string storedQuestName;
	std::string questDialogueText;
	std::string reward;
	std::string dialogueText;
	int expReward, celsReward, itemReward, itemQuantityReward, npcID;
	//int levelReward, rewardType, exp, cels, npcID;

	Sprite* _chatSelection;
	Sprite* _questSelection;
	Sprite* _txtBox;
	Sprite* _npcBox;
	Sprite* _npcIcon;

	int lines = 1;
	int lineCounter = 1;
	int questDialogueSize;
	int questDialogueLine = 0;
	char lineChar = 'a';
	bool endOfChat = false;
	bool questState = false;
	bool questDone = false;
	bool npcTalking = false;
	bool questMenu = false;
	bool noQuest = false;
	bool isQuestDone = false;
	bool isQuestRewarded = false;
	bool blockSelectedQuest = false;
};

class Luna : public Npc {
public:
	Luna();
	Luna(Graphics &graphics, Vector2 spawPoint, std::string name, int npcId);

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