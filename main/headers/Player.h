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
class Skills;
class SkillFactory;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics &graphics, Vector2 spawnPoint);
	void draw(Graphics &graphics);
	void drawGun(Graphics &graphics);
	void drawJetPack(Graphics &graphics);
	void drawStatMenu(Graphics &graphics, Player &player, int selection);
	void drawSaveMenu(Graphics &graphics, Player &player, int selection);
	void drawCurrentMapName(Graphics &graphics);
	void drawStatusEffect(Graphics &graphics, const std::string text);
	void drawEventMessage(Graphics &graphics, std::string text);
	void drawBattleMessage(Graphics &graphics, std::string text);
	void drawPlayerOutline(Graphics &graphics);
	void showSceneDialogue(Graphics &graphics, std::string text);
	void update(float elapsedTime);
	bool checkDeathPlayed() const { return this->deathPlayed; }

	// Player handling
	void moveLeft(); 	// Move player left by -dx
	void moveRight(); 	// Move player right by +dx
	void moveUp(); // For ladders
	void moveDown();
	void jump();
	void useJetPack();
	void stopMoving();
	void lookUp();
	void stopLookingUp();
	void lookDown();
	void stopLookingDown();
	void startDeath();
	void resetDeath() { this->deathPlayed = false; }
	void setPlayerDX(float dx) { this->_dx = dx; }
	void setPlayerDY(float dy) { this->_dy = dy; }
	float getPlayerDX() { return this->_dx; }
	float getPlayerDY() { return this->_dy; }
	void setPlayerPosition(float x, float y);
	void resetPlayer();
	virtual void animationDone(std::string currentAnimation);
	std::string getMap();
	bool lookingUp();
	bool lookingDown();
	bool lookingRight();
	bool lookingLeft();
	bool canShortJump() const { return this->_canShortJump; }
	virtual void setupAnimations();
	Direction facingDir();
	Direction _facing;
	Surface _currentSurface = NOTHING;
	Rectangle _lastCollidedFloorRect;
	Slope _lastCollidedSlope;

	// Event handling
	void setEventMessage(std::string text);
	void handleTileCollisions(std::vector<Rectangle> &others);
	void handleArenaCollisions(std::vector<Rectangle> &others);
	void handleLavaCollisions(std::vector<Rectangle> &others);
	void handlePoisonCollisions(std::vector<Rectangle> &others);
	void handleWaterCollisions(std::vector<Rectangle> &others);
	void handleDeadzoneCollisions(std::vector<Rectangle> &others);
	bool handleLadderCollisions(std::vector<Rectangle> &others);
	void handleSlopeCollisions(std::vector<Slope> &others);
	void handlePlayerSlopeCollision(Slope &slope);
	void handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent, Player &player);
	void handleLockedDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent, Player &player);
	void handleEnemyCollisions(std::vector<Enemy*> &others);
	void handleRestoreableHealth(float damage);
	void handleHex(float elapsedTime);
	void applyHex(int hexID, double duration, bool isStackable);
	void nullifyHex(int hexID);
	const int getHex() const { return this->_hexID; }
	std::string getNpcName(std::vector<Npc*>& others, Graphics & graphics);
	const int getNpcId(std::vector<Npc*> &others, Graphics &graphics);
	const float getX() const;
	const float getY() const; // Getting variables not changes const make sure it doesnt
	const float getPreviousY() const;
	std::string enemyName = "";
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
	bool checkEnemyDespawn(std::string name, std::string mapName, float x, float y);
	void addDespawnTable(std::string name, std::string mapName, float x, float y);
	void clearDespawnTable() { this->despawnEnemy.clear(); }
	inline const std::vector<std::string> getSceneTable() const { return this->cutSceneTable; }
	inline void setCutsceneTable(std::vector<std::string> table) { this->cutSceneTable = table; }
	void addCutSceneTable(std::string name);
	bool checkCutSceneCompleted(std::string name);
	inline const std::vector<std::string> getLockedDoorTable() const { return this->lockedDoorTable; }
	inline void setLockedDoorTable(std::vector<std::string> table) { this->lockedDoorTable = table; }
	void addLockedDoorTable(std::string name);
	bool checkLockedDoorCompleted(std::string name);
	void addItem(int itemID, int quantity, Inventory &invent);
	inline const std::vector<std::string> getEquipmentTable() const { return this->equipmentTable; }
	inline void setEquipmentTable(std::vector<std::string> table) { this->equipmentTable = table; }
	void addEquipment(std::string name);
	bool checkEquipmentUnlocked(std::string name);
	void storeLevel(Level &level);
	void overwriteLevel(Level &level, std::string mapName);
	const std::string getMapHash(std::string mapName);
	bool isGrounded() const { return this->_grounded; }
	bool isWalking() { return (this->_dx != 0) ? true : false; }
	void setClimbing(bool condition) { this->_climbing = condition; }
	void setShortJump(bool condition) { this->_canShortJump = condition; }
	void setBreakableCollision(bool condition) { this->isBreakableCollision = condition; }
	

	// Health handling
	void drawHPNumbers(Graphics &graphics, int x, int y);
	void drawHPPotAvailable(Graphics &graphics, int x, int y);
	void drawHPPotStrength(Graphics &graphics, int x, int y);
	void setIFrame(bool condition);
	void gainHealth(float amount);
	void gainHPFromStatus(float amount);
	void gainMaxHealth(float amount);
	void setBurning(bool condition) { this->isBurning = condition; }
	void setDrowning(bool condition) { this->isDrowning = condition; }
	void setHex(bool condition) { this->isHexed = condition; }
	void setPlayerHit(bool condition) { this->gotHit = condition; }
	const inline bool getPlayerHit() const { return this->gotHit; }
	const inline float getMaxHealth() const { return this->_maxHealth; }
	const inline float getCurrentHealth() const { return this->_currentHealth; }
	const inline float getRestorableHealth() const { return this->_restorableHealth; }
	inline void setMaxHealth(float hp) { this->_maxHealth = hp; }
	inline void setCurrentHealth(float hp) { this->_currentHealth = hp; }
	inline void subtractHpPot() { this->_hpPotAmount -= 1; }
	inline void refillHpPot() { this->_hpPotAmount = this->_hpPotCapacity; }
	inline bool hasHpPot() const { if (this->_hpPotAmount > 0) { return true; } return false; }
	inline int getHpPotCapacity() const { return this->_hpPotCapacity; }
	inline void setHpPotCapacity(int capacity) { this->_hpPotCapacity = capacity; }
	inline int getHpPotStrength() const { return this->_hpPotStrength; }
	inline void setHpPotStrength(int strength) { this->_hpPotStrength = strength; }

	// Exp, Level, & currency handling
	void drawExpNumbers(Graphics &graphics, int x, int y);
	void gainExp(float exp);
	void gainExpFromEnemy(int enemyLevel, float exp);
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

	// Stat menu
	void statChoice(int selection);
	inline void setDmgMod(double mod) { this->_dmgMod = mod; }
	inline void setDefense(double def) { this->_defense = def; }
	inline void setStatPoints(int points) { this->_statPoints = points; }
	inline void setSoulStr(double str) { this->_soulStrength = str; }
	inline void setLifeSteal(float amt) { this->STAT_LIFESTEAL = amt; }
	inline bool hasLifeStealActive() { return this->lifeStealActive; }
	inline void setLifeStealActive(bool active) { this->lifeStealActive = active; }
	inline void raiseLifeSteal(float amt) { this->STAT_LIFESTEAL += amt; }
	double getDmgMod();
	double getDmgReduction() { return this->_dmgReduction; }
	double getDefense();
	double getSoulStr();
	float getLifeSteal() { return this->STAT_LIFESTEAL; }
	int getStatPoints();
	int selectX = 0;
	int selectY = 0;
private:
	float _dx, _dy; //(dx (delta x) is change in x postition during frame)
	float fallHeight = 0;
	float previousY = 0;
	float _maxHealth;
	float _currentHealth;
	float _restorableHealth;
	float _air = 100;
	float _fuel = 100;
	float _exp = 0;
	float STAT_AGIL = 1;
	float STAT_STR = 1;
	float STAT_AVOID = 1;
	float STAT_LIFESTEAL = 0.002f;

	bool _grounded; // True if we are, false if we are in the air
	bool _canShortJump = true;
	bool _climbing;
	bool _lookingUp;
	bool _lookingDown;
	bool iFrame;
	bool isRestorableHealth = false;
	bool isPoisoned = false;
	bool isBurning = false;
	bool isDrowning = false;
	bool isHexed = false;
	bool isFlying = false;
	bool isBreakableCollision = false;
	bool drawExhaust = false;
	bool gotHit = false;
	bool _playerDeathSound = false;
	bool deathPlayed = false;
	bool showEventMsg = false;
	bool lifeStealActive = false;

	int _hpPotAmount = 2;
	int _hpPotCapacity = 2;
	int _hpPotStrength = 0;
	int _requiredExp;
	int _statPoints = 1;
	int _playerLevel = 0;
	int _soulLevel = 0;
	int _requiredKill;
	int mPosX, mPosY;
	int currency = 0;
	int _hexID = 0;

	double _soulStrength = 1;
	double _defense = 1.05;
	double _dmgMod = 1;
	double _dmgReduction = 0;
	double _finalDamage = 0;
	double _timeElapsed = 0; // For timer
	double _mapTimeElapsed = 0;

	std::string eventMessage;

	TextManager* _txt;
	Sprite _statMenu;
	Sprite _statSelection;
	Sprite _saveMenu;
	Sprite _saveSelection;
	Sprite _Gun;
	Sprite _JetPack;

	// enemy name, amount killed
	std::vector<std::pair<std::string, int>> killTable;
	// Name, mapName, initial x & y, spawn
	std::vector<std::tuple<std::string, std::string, float, float, bool>> bossTable;
	// Name, mapName, initial x & y - Despawn enemy until the game is Saved
	std::vector<std::tuple<std::string, std::string, float, float>> despawnEnemy;
	// map name, level object (stores the state of the level I.E: item was taken already)
	std::map<std::string, Level> mapStorage;
	std::vector<std::string> cutSceneTable;
	std::vector<std::string> lockedDoorTable;
	std::vector<std::string> equipmentTable;
	// message, x-pos, y-pos, duration
	std::vector<std::tuple<std::string, float, float, double>> battleMessages;
	// hex id, duration, can be stacked, stack count, timer
	std::vector<std::tuple<int, double, bool, int, double>> hexTable;
protected:
	double _timeToUpdate = 2500;
	double _timeForMapName = 3000;
	double _timeForEventMsg = 0;
	double _timeForBattleMsg = 0;
	double _timeForRestoreHealth = 0;
	double _poisonDOTTimer = 0;
	double _poisonDuration = 0;
	double _hexDOTTimer = 0;
	double _hexDuration = 0;
	double _hexTimer = 0;
	double _deathAnimationTimer = 0;


	std::map<std::string, std::string> mapHash = {
		{"cave depths",				"7DE64BB85B2390D2059057A20DDEAC9A9892C1D2EC78C9FEEF02303B38AFD455"},
		{"cave",					"8360B397746A75B5FA02588532590E2D2FAAF5A2F3858B5FC61B7EB84463766B"},
		{"caveFork",				"B799B724E27A1364475C3AC3065F266AD01D5AF50E1518D9BADE68403DB0FF43"},
		{"caverns",					"9B3CF0FF7808927BCDD776DE33364FE43B57B57FAB45CFFF7F8023CDEA82CCA0"},
		{"Collapsed Cave",			"9E872647A4662F99D73C442B733D3ADCA56A602B1F229E27E735E19EDEDB64BB"},
		{"Profaned Capital",		"FD53C70E2D45BC6122328A6309A6D560D355319D147FB2017FA33D495A63CC1F"},
		{"Up To The Forest",		"C991610C4B5A585142456B012AFC435E090B745DF9A01936326651DC072D5436"},
		{"New York",				"547313162592760B5D648D09D1291FA55E5418A88459FF8AF0571FBFC1308BBB"},
		{"The Mines",				"6D377F741AF307288022D864B1D3F53E873F60974C0E39554AF9FCD703A78B1D"}
	};
};

#endif
