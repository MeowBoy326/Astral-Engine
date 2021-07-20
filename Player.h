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
	void drawGun(Graphics &graphics);
	void drawStatMenu(Graphics &graphics, Player &player, int selection);
	void drawSaveMenu(Graphics &graphics, Player &player, int selection);
	void drawCurrentMapName(Graphics &graphics);
	void drawStatusEffect(Graphics &graphics, const std::string text);
	void showSceneDialogue(Graphics &graphics, std::string text);
	void update(float elapsedTime);
	bool checkDeathPlayed() const { return this->deathPlayed; }

	//Player handling
	void moveLeft(); 	//Move player left by -dx
	void moveRight(); 	//Move player right by +dx
	void moveUp(); //For ladders
	void moveDown();
	void jump();
	void stopMoving();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();
	void startDeath();
	void setPlayerDX(float dx) { this->_dx = dx; }
	void setPlayerDY(float dy) { this->_dy = dy; }
	virtual void animationDone(std::string currentAnimation);
	std::string getMap();
	bool lookingUp();
	bool lookingDown();
	bool lookingRight();
	bool lookingLeft();
	virtual void setupAnimations();
	Direction facingDir();
	Direction _facing;
	Surface _currentSurface = NOTHING;
	Rectangle _lastCollidedFloorRect;
	Slope _lastCollidedSlope;

	//Event handling
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleArenaCollisions(std::vector<Rectangle> &others);
	void handleLavaCollisions(std::vector<Rectangle> &others);
	void handlePoisonCollisions(std::vector<Rectangle> &others);
	void handleWaterCollisions(std::vector<Rectangle> &others);
	bool handleLadderCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent, Player &player);
	void handleLockedDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent, Player &player);
	void handleEnemyCollisions(std::vector<Enemy*> &others);
	void handleNpcCollisions(std::vector<Npc*>& others, Graphics &graphics, int lineCount);
	std::string getNpcName(std::vector<Npc*>& others, Graphics & graphics);
	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt
	std::string enemyName;
	void addKillTable(std::string name);
	void addBossTable(std::string name, std::string mapName, float x, float y);
	void setBossTable(std::vector<std::tuple<std::string, std::string, float, float, bool>> table) { this->bossTable = table; }
	inline const std::vector<std::tuple<std::string, std::string, float, float, bool>> getBossTable() const { return this->bossTable; }
	void completeBossTable(std::string name, std::string mapName, float x, float y);
	void addMultiKill(std::string name, int amount);
	inline const std::vector<std::pair<std::string, int>> getKillTable() const { return this->killTable; }
	inline void setKillTable(std::vector<std::pair<std::string, int>> table) { this->killTable = table; }
	bool checkKillQuestComplete(std::string name, int count);
	bool checkBossCompleted(std::string name, std::string mapName, float x, float y);
	inline const std::vector<std::string> getSceneTable() const { return this->cutSceneTable; }
	inline void setCutsceneTable(std::vector<std::string> table) { this->cutSceneTable = table; }
	void addCutSceneTable(std::string name);
	bool checkCutSceneCompleted(std::string name);
	void storeLevel(Level &level);
	void overwriteLevel(Level &level, std::string mapName);
	const std::string getMapHash(std::string mapName);
	bool isGrounded() const { return this->_grounded; }
	void setClimbing(bool condition) { this->_climbing = condition; }
	

	//Health handling
	void drawHPNumbers(Graphics &graphics);
	void setIFrame(bool condition);
	void gainHealth(float amount);
	void gainHPFromStatus(float amount);
	void gainMaxHealth(float amount);
	void setBurning(bool condition) { this->isBurning = condition; }
	void setDrowning(bool condition) { this->isDrowning = condition; }
	void setPlayerHit(bool condition) { this->gotHit = condition; }
	const inline bool getPlayerHit() const { return this->gotHit; }
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
	float _air = 100;
	float _exp = 0;
	float STAT_AGIL = 1;
	float STAT_STR = 1;
	float STAT_AVOID = 1;

	bool _grounded; //true if we are, false if we are in the air
	bool _climbing;
	bool _lookingUp;
	bool _lookingDown;
	bool iFrame;
	bool isPoisoned = false;
	bool isBurning = false;
	bool isDrowning = false;
	bool gotHit = false;
	bool _playerDeathSound = false;
	bool deathPlayed = false;

	int _requiredExp;
	int _statPoints = 1;
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
	Sprite _saveMenu;
	Sprite _saveSelection;
	Sprite _Gun;

	std::vector<std::pair<std::string, int>> killTable;
	//name, mapName, initial x & y
	std::vector<std::tuple<std::string, std::string, float, float, bool>> bossTable;
	std::map<std::string, Level> mapStorage;
	std::vector<std::string> cutSceneTable;
protected:
	double _timeToUpdate = 2500;
	double _timeForMapName = 3000;
	double _poisonDOTTimer = 0;
	double _poisonDuration = 0;
	double _deathAnimationTimer = 0;


	std::map<std::string, std::string> mapHash = { {"cave", "7B00FCFBE2CC327D32B5B3D58A1BD7DD56C37C2C5533AA1AFE959CE7D3ADBC42"},
	{"caveFork", "9D146228905B72A787A2C64C11C5754EE195C7B8C1A466FE21ED30F220403CA5"},
	{"caverns","7555D4274A94F2C283E8429FB4791CE6CC2C80E070EC1144E67C0902F8BD4027"},
	{"Profaned Capital", "721BDA97C06A64CBCEFDE07E538DDE2C59EA31EA36D39792F4EF9F0F57AB74F8"},
	{"Collapsed Cave", "C6876F557AC3629A4765E10619B471DF1DC81D2CE1EB4766DF95426C607F212B"}, 
	{"cave depths", "A33687C4DCBD21A27A417C64E923D7C34ABE77C501DF0CA69388E728B9FE1314"},
	{"Up To The Forest", "1FAA3D690F77FED028A273ABD47C1A22517EADCA8A319D6EFF2BFD666DB1A719"} };
};

#endif
