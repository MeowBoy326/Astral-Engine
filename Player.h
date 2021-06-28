#ifndef PLAYER_H
#define PLAYER_H

#include "AnimatedSprite.h"
#include "Global.h"
#include "Slope.h"
#include "Level.h"
#include "Enemy.h"

//#include "Npc.h"
class TextManager;
class Graphics;
class Npc;
class Items;
class Inventory;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void drawStatMenu(Graphics &graphics, Player &player, int selection);
	void drawCurrentMapName(Graphics &graphics);
	void update(float elapsedTime);

	//Player handling
	void moveLeft(); 	//Move player left by -dx
	void moveRight(); 	//Move player right by +dx
	void jump();
	void stopMoving();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();
	virtual void animationDone(std::string currentAnimation);
	std::string getMap();
	bool lookingUp();
	bool lookingDown();
	bool lookingRight();
	bool lookingLeft();
	virtual void setupAnimations();
	Direction facingDir();
	Direction _facing;

	//Event handling
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent);
	void handleLockedDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent);
	void handleEnemyCollisions(std::vector<Enemy*> &others);
	void handleNpcCollisions(std::vector<Npc*>& others, Graphics &graphics, int lineCount);
	std::string getNpcName(std::vector<Npc*>& others, Graphics & graphics);
	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt
	std::string enemyName;
	void addKillTable(std::string name);
	void addMultiKill(std::string name, int amount);
	inline const std::vector<std::pair<std::string, int>> getKillTable() const { return this->killTable; }
	inline void setKillTable(std::vector<std::pair<std::string, int>> table) { this->killTable = table; }
	bool checkKillQuestComplete(std::string name, int count);
	void storeLevel(Level &level);
	void overwriteLevel(Level &level, std::string mapName);
	

	//Health handling
	void drawHPNumbers(Graphics &graphics);
	void setIFrame(bool condition);
	void gainHealth(float amount);
	void gainMaxHealth(float amount);
	const inline float getMaxHealth() const { return this->_maxHealth; }
	const inline float getCurrentHealth() const { return this->_currentHealth; }
	inline void setMaxHealth(float hp) { this->_maxHealth = hp; }
	inline void setCurrentHealth(float hp) { this->_currentHealth = hp; }

	//Exp, Level, & currency handling
	void drawExpNumbers(Graphics &graphics);
	void gainExp(float exp);
	float getCurrentExp();
	void setCurrentExp(float exp);
	float getRequiredExp();
	int getLevel();
	void setLevel(int num);
	void addLevel(int num);
	int getSoulLevel();
	void setSoulLevel(int num);
	void addSoulLevel(int num);
	void addKillCount(int num);
	void setKillCount(int num);
	int getKillCount();
	int getRequiredKills();
	int killCount = 0;
	void gainCurrency(int num);
	int getCurrency() { return this->currency; }
	void setCurrency(int num) { this->currency = num; }

	//Stat menu
	void statChoice(int selection);
	inline void setDmgMod(double mod) { this->_dmgMod = mod; }
	inline void setDefense(double def) { this->_defense = def; }
	inline void setStatPoints(int points) { this->_statPoints = points; }
	inline void setSoulStr(double str) { this->_soulStrength = str; }
	double getDmgMod();
	double getDefense();
	double getSoulStr();
	int getStatPoints();
	int selectX = 0;
	int selectY = 0;
private:
	float _dx, _dy; //(dx (delta x) is change in x postition during frame)
	float _maxHealth;
	float _currentHealth;
	float _exp = 0;
	float STAT_AGIL = 1;
	float STAT_STR = 1;
	float STAT_AVOID = 1;

	bool _grounded; //true if we are, false if we are in the air
	bool _lookingUp;
	bool _lookingDown;
	bool iFrame;

	int _requiredExp;
	int _statPoints = 25;
	int _playerLevel = 0;
	int _soulLevel = 0;
	int _requiredKill;
	int mPosX, mPosY;
	int currency = 0;

	double _soulStrength = 1;
	double _defense = 1.05;
	double _dmgMod = 1;
	double _timeElapsed = 0; //for timer
	double _mapTimeElapsed = 0;

	TextManager* _txt;
	Sprite _statMenu;
	Sprite _statSelection;

	std::vector<std::pair<std::string, int>> killTable;
	std::map<std::string, Level> mapStorage;
protected:
	double _timeToUpdate = 2500;
	double _timeForMapName = 3000;
};

#endif
