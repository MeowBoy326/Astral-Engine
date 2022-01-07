#pragma once
#include "TextManager.h"
#include "Player.h"
#include "Graphics.h"
#include "Slope.h"
#include "Door.h"
#include "Npc.h"
#include "Items.h"
#include <time.h>
#include "AESCipher.h"
#include <sstream>
#include <iomanip>

namespace player_constants {
	float WALK_SPEED = 0.2f;
	float JUMP_DISTANCE = 0.7f;

	float GRAVITY = 0.002f;
	float GRAVITY_CAP = 0.8f;

	bool iFrame = false;
	bool showMapName = false;

	std::string mapName = "CAVE";
	/*
	* Index 0 = the exp needed from lvl 0 (starting level) to level 1.
	* So, to get to level 1 you need 20 exp. Level 2 needs 40 exp. With every new level exp is set to 0 
	*/
	std::vector<float> expTable = {20, 175, 369, 715, 1000, 1420, 2840, 4160, 7180, 12420};
	std::vector<float> dmgTable = { 20, 100, 420, 850, 1522, 2437, 4700, 6000 }; 
	// same as above but with kills. this increased damage per kill needed
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	//graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
	AnimatedSprite(graphics, "data\\graphics\\MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100), 
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(100.0f),
	_currentHealth(100.0f)
{
	graphics.loadImage("data\\graphics\\MyChar.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("RunRight");

	this->_statMenu = Sprite(graphics, "data\\npc\\npcTextBox.png", 40, 88, 71, 52, this->_x, (this->_y - 10));
	this->_statSelection = Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 147, 46, 18, 15, 15);
	this->_saveMenu = Sprite(graphics, "data\\npc\\npcTextBox.png", 155, 51, 76, 26, this->_x, (this->_y - 10));
	this->_saveSelection = Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 173, 31, 18, 15, 15);
	graphics.loadImage("data\\npc\\npcTextBox.png");
	this->_Gun = Sprite(graphics, "data\\graphics\\Arms.png", 52, 10, 10, 5, (this->_x - 10), (this->_y + 10));
	graphics.loadImage("data\\graphics\\Arms.png");
	this->_JetPack = Sprite(graphics, "data\\graphics\\exhaust.png", 54, 1, 13, 20, (this->_x - 10), (this->_y + 10));
	graphics.loadImage("data\\graphics\\exhaust.png");
}

void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 0, 16, "IdleRight", 16, 16, Vector2(0, 0)); // we are 16 pixels down now on sprite sheet
	this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(3, 0, 16, "RunRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 0, "IdleLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 3, 16, "IdleRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 0, "RunLeftUp", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 3, 16, "RunRightUp", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 0, "LookDownLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 6, 16, "LookDownRight", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 0, "LookBackwardsLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 7, 16, "LookBackwardsRight", 16, 16, Vector2(0, 0));
	this->addAnimation(7, 0, 72, "PlayerDead", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {
	if (this->_currentHealth <= 0) {
		if (!this->deathPlayed)
			this->deathPlayed = true;
	}
}

std::string Player::getMap() {
	return player_constants::mapName;
}

bool Player::lookingUp() {
	if (this->_lookingUp == true) {
		return true;
	}
	else  {
		return false;
	}
}

bool Player::lookingDown() {
	if (this->_lookingDown == true) {
		return true;
	}
	else  {
		return false;
	}

}

bool Player::lookingRight() {
	if (this->_facing == RIGHT) {
		return true;
	}
	else  {
		return false;
	}
}

bool Player::lookingLeft() {
	if (this->_facing == LEFT) {
		return true;
	}
	else  {
		return false;
	}
}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

const float Player::getPreviousY() const
{
	return this->previousY;
}

void Player::addKillTable(std::string name)
{
	if (this->killTable.empty()) {
		this->killTable.push_back(std::make_pair(name, 1));
		std::string msg = "Slain " + name + " (1)";
		this->battleMessages.push_back(std::make_tuple(msg, 0, 0, 0));
	}
	else {
		auto it = std::find_if(killTable.begin(), killTable.end(), [&name](const auto& t) {return t.first == name; });
		auto distance = std::distance(this->killTable.begin(), it);
		if (it != killTable.end()) {
			killTable[distance].second += 1;
			std::string msg = "Slain " + name + " (" + std::to_string(killTable[distance].second) + ")";
			this->battleMessages.push_back(std::make_tuple(msg, 0, 0, 0));
		}
		else {
			this->killTable.push_back(std::make_pair(name, 1));
			std::string msg = "Slain " + name + " (1)";
			this->battleMessages.push_back(std::make_tuple(msg, 0, 0, 0));
		}
	}
}

void Player::addBossTable(std::string name, std::string mapName, float x, float y)
{
	if (this->bossTable.empty())
		this->bossTable.push_back(std::make_tuple(name, mapName, x, y, false));
	else {
		auto bossIt = std::find_if(this->bossTable.begin(), this->bossTable.end(), [&name](const auto& t) {return std::get<0>(t) == name; });
		auto bDistance = std::distance(this->bossTable.begin(), bossIt);
		if (bossIt != this->bossTable.end()) {
			if (std::get<1>(this->bossTable[bDistance]) == mapName && (std::get<2>(this->bossTable[bDistance]) != x
				|| std::get<3>(this->bossTable[bDistance]) != y)) //same boss/mini-boss (2 on same level) so we check the x or y as well 
			{
				this->bossTable.push_back(std::make_tuple(name, mapName, x, y, false));
			}
			else if (std::get<1>(this->bossTable[bDistance]) != mapName) {
				this->bossTable.push_back(std::make_tuple(name, mapName, x, y, false));
			}
		}
		else
			this->bossTable.push_back(std::make_tuple(name, mapName, x, y, false));
	}
}

void Player::completeBossTable(std::string name, std::string mapName, float x, float y)
{
	auto bossIt = std::find_if(this->bossTable.begin(), this->bossTable.end(), [&name](const auto& t) {return std::get<0>(t) == name; });
	auto bDistance = std::distance(this->bossTable.begin(), bossIt);
	if (bossIt != this->bossTable.end()) {
		if (std::get<1>(this->bossTable[bDistance]) == mapName && std::get<2>(this->bossTable[bDistance]) == x
			&& std::get<3>(this->bossTable[bDistance]) == y) //same boss/mini-boss (2 on same level) so we check the x or y as well 
		{
			std::get<4>(this->bossTable[bDistance]) = true;
		}
	}
}

void Player::addMultiKill(std::string name, int amount)
{
	if (this->killTable.empty()) {
		this->killTable.push_back(std::make_pair(name, amount));
	}
	else {
		auto it = std::find_if(killTable.begin(), killTable.end(), [&name](const auto& t) {return t.first == name; });
		auto distance = std::distance(this->killTable.begin(), it);
		if (it != killTable.end()) {
			killTable[distance].second += amount;
		}
		else {
			this->killTable.push_back(std::make_pair(name, amount));
		}
	}
}

bool Player::checkKillQuestComplete(std::string name, int count)
{
	for (int index = 0; index < this->killTable.size(); ++index) {
		if (this->killTable[index].first == name && this->killTable[index].second >= count) {
			return true;
		}
	}
	return false;
}

bool Player::checkBossCompleted(std::string name, std::string mapName, float x, float y)
{
	auto bossIt = std::find_if(this->bossTable.begin(), this->bossTable.end(), [&name](const auto& t) {return std::get<0>(t) == name; });
	auto bDistance = std::distance(this->bossTable.begin(), bossIt);
	if (bossIt != this->bossTable.end()) {
		if (std::get<1>(this->bossTable[bDistance]) == mapName && std::get<2>(this->bossTable[bDistance]) == x
			&& std::get<3>(this->bossTable[bDistance]) == y && std::get<4>(this->bossTable[bDistance]) == true) {
			return true;
		}
		else
			return false;
	}
	else
		return false;
	return false;
}

void Player::addCutSceneTable(std::string name)
{
	this->cutSceneTable.push_back(name);
}

bool Player::checkCutSceneCompleted(std::string name)
{
	for (int i = 0; i < this->cutSceneTable.size(); ++i) {
		if (this->cutSceneTable[i] == name) {
			return true;
		}
	}
	return false;
}

void Player::addLockedDoorTable(std::string name)
{
	if (std::find(this->lockedDoorTable.begin(), this->lockedDoorTable.end(), name) != this->lockedDoorTable.end())
		return;
	else
		this->lockedDoorTable.push_back(name);
}

bool Player::checkLockedDoorCompleted(std::string name)
{
	for (int i = 0; i < this->lockedDoorTable.size(); ++i) {
		if (this->lockedDoorTable[i] == name) {
			return true;
		}
	}
	return false;
}

void Player::addEquipment(std::string name)
{
	if (std::find(this->equipmentTable.begin(), this->equipmentTable.end(), name) != this->equipmentTable.end())
		return;
	else {
		this->equipmentTable.push_back(name);
		std::cout << "Added " << name << " to equipment" << std::endl;
		if (name == "JetPack") {
			this->eventMessage = "Press 3 to toggle the Jetpack.";
			this->showEventMsg = true;
			this->_timeForEventMsg = 0;
		}
	}

}

bool Player::checkEquipmentUnlocked(std::string name)
{
	for (int i = 0; i < this->equipmentTable.size(); ++i) {
		if (this->equipmentTable[i] == name) {
			return true;
		}
	}
	return false;
}

void Player::storeLevel(Level & level)
{
	this->mapStorage[level.getMapName()] = level;
}

void Player::overwriteLevel(Level & level, std::string mapName)
{
	//update the level that is stored already
	std::map<std::string, Level>::iterator it;
	it = this->mapStorage.find(mapName);
	if (it != this->mapStorage.end()) {
		std::cout << "found & overwritten the saved map:" << mapName << std::endl;
		it->second = level;
	}
}

const std::string Player::getMapHash(std::string mapName)
{
	std::map<std::string, std::string>::iterator it;
	it = this->mapHash.find(mapName);
	if (it != this->mapHash.end())
		return it->second;
	else
		return "NULL";
}

void Player::moveLeft() {
	if (this->_lookingDown == true && this->_grounded == true) //while facing backwards if we are on the ground and looking down that means char
	//is turned around and interacting with something so don't allow movement!
	{
		return;
	}
	if (this->_climbing)
		return;
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown == true && this->_grounded == true) 
	{ return; }
	if (this->_climbing)
		return;
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::moveUp()
{
	this->_dy = -player_constants::WALK_SPEED;
	this->_dx = 0;
}

void Player::moveDown()
{
	this->_dy = +player_constants::WALK_SPEED;
	this->_dx = 0;
}

void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_DISTANCE;
		this->_grounded = false;
		this->_currentSurface = NOTHING;
	}
	if (this->_climbing) {
		this->_dy = 0;
		this->_dy -= 0.07; //Don't set to any higher or else player can traverse quickly using jump
		this->_grounded = false;
		this->_climbing = false;
	}
}

void Player::useJetPack()
{
	if (this->_fuel <= 0.0f) {
		this->_fuel = 0.0f;
		isFlying = false;
	}
	else {
		if (!this->_climbing && !this->isDrowning) {
			isFlying = true;
			this->_dy -= 0.2f;
			this->_fuel -= 0.0423;
			this->_grounded = false;
			this->_climbing = false;
		}
	}
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	if (this->_currentHealth <= 0)
		return;
	if (this->_lookingUp == false && this->_lookingDown == false) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRight" : "IdleLeft"); //if player is facing right call IdleRight otherwise use IdleLeft
	}
}

void Player::lookUp() {
	this->_lookingUp = true;
	if (this->_dx == 0) {
		this->playAnimation(this->_facing == RIGHT ? "IdleRightUp" : "IdleLeftUp");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "RunRightUp" : "RunLeftUp");
	}
}

void Player::stopLookingUp() {
	this->_lookingUp = false;
}

void Player::lookDown() {
	this->_lookingDown = true;
	if (this->_grounded == true) {
		//We need to interact (turn backwards)
		this->playAnimation(this->_facing == RIGHT ? "LookBackwardsRight" : "LookBackwardsLeft");
	}
	else {
		this->playAnimation(this->_facing == RIGHT ? "LookDownRight" : "LookDownLeft");
	}
}

void Player::stopLookingDown() {
	this->_lookingDown = false;
}

void Player::startDeath()
{
	this->playAnimation("PlayerDead");
}

void Player::setEventMessage(std::string text)
{
	this->eventMessage = text;
	this->showEventMsg = true;
	this->_timeForEventMsg = 0;
}

//handles collisions with all tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collision happened on and move the player accordingly
	if (this->_climbing && this->isBreakableCollision == false)
		return;
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0; //reset all gravity, if we arent grounded we fall to the ground
				this->_y = others.at(i).getBottom() + 1; //no longer go through things, stops us
				if (this->_grounded) { //only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
					this->_dx = 0; //stop movement on x-axis
					this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f; //if we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				}
				break;
			case sides::BOTTOM: //hit the top (bottom) of tile push us back up ontop of tile
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true; //we are on ground since it pushed it back up
				this->_lastCollidedFloorRect = others.at(i);
				this->_currentSurface = RECTANGLE;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}
		}
	}
}

void Player::handleArenaCollisions(std::vector<Rectangle>& others)
{
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getArenaCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0; //reset all gravity, if we arent grounded we fall to the ground
				this->_y += 2; //no longer go through things, stops us
				if (this->_grounded) { //only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
					this->_dx = 0; //stop movement on x-axis
					this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f; //if we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				}
				break;
			case sides::BOTTOM: //hit the top (bottom) of tile push us back up ontop of tile
				this->_y -= 2;
				this->_dy = 0;
				this->_grounded = true; //we are on ground since it pushed it back up
				this->_lastCollidedFloorRect = others.at(i);
				this->_currentSurface = RECTANGLE;
				break;
			case sides::LEFT:
				this->_x += 2;
				break;
			case sides::RIGHT:
				this->_x -= 2;
				break;
			}
		}
	}
}

void Player::handleLavaCollisions(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++) {
		this->gainHPFromStatus(-0.02);
		this->isBurning = true;
	}
}

void Player::handlePoisonCollisions(std::vector<Rectangle>& others)
{
	if (!this->isPoisoned)
		this->isPoisoned = true;
}

void Player::handleWaterCollisions(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++) {
		if (this->_air >= 0.0f)
			this->_air -= 0.0168;
		this->isDrowning = true;
	}
}

bool Player::handleLadderCollisions(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_y -= 1;
				if (this->_facing == LEFT)
					this->moveLeft();
				if (this->_facing == RIGHT)
					this->moveRight();
				this->_grounded = false;
				this->_climbing = false;
				break; //going down the ladder
			case sides::BOTTOM: //going up the ladder
				//hit the top (bottom) of tile push us back up ontop of tile
				if (this->_climbing) {
					this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
					this->_dy = 0;
					this->_grounded = false; //we are on ground since it pushed it back up
					this->_climbing = false;
					if (this->_facing == LEFT)
						this->moveLeft();
					if (this->_facing == RIGHT)
						this->moveRight();
					this->_dy = player_constants::GRAVITY_CAP;
					return false;
				}
				break;
			}
		}
	}
	return true;
}

//handles collisions with all slopes the player is colliding with
void Player::handleSlopeCollisions(std::vector<Slope> &others) {
	for (int i = 0; i < others.size(); i++) {
		//Calculate where on the slope the player's bottom center is touching
		//and use y=mx+b to figure out the y position to place him at
		//First calculate "b" (slope) intercept) using one of the points (b = y - mx)
		int b = (others.at(i).getP1().y - (others.at(i).getSlope() * fabs(others.at(i).getP1().x)));

		//Now get the players center x
		int centerX = this->_boundingBox.getCenterX();

		//Now pass that X into the equation y = mx + b (using our newly found b and x) to get the new y position
		int newY = (others.at(i).getSlope() * centerX) + b - 8; //8 is temporary to fix a problem

		//Reposition the player to the correct "y"
		if (this->_grounded) {
			this->_y = newY - this->_boundingBox.getHeight();
			this->_grounded = true;
		}
		this->_lastCollidedSlope = others.at(i);
		this->_currentSurface = SLOPE;
	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent, Player &player) {
	//Check if the player is grounded and holding the down arrow
	//If so, go through the door
	//If not, do nothing
	AESCipher cipher;
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded == true && this->_lookingDown == true) {
			if (!cipher.verifyHash(others.at(i).getDestination(), player)) {
				std::cout << "Unable to set level to new level : Hash check failed!" << std::endl;
				this->eventMessage = "Anti-Cheat: File has been modified.";
				this->showEventMsg = true;
				this->_timeForEventMsg = 0;
				return;
			}
			if (level.isArenaActive()) {
				this->eventMessage = "Boss Battle still in-progress";
				this->showEventMsg = true;
				this->_timeForEventMsg = 0;
				return;
			}
			this->overwriteLevel(level, level.getMapName());
			std::map<std::string, Level>::iterator it;
			it = this->mapStorage.find(others.at(i).getDestination());
			if (it != this->mapStorage.end()) {
				std::cout << "found the saved map:" << others.at(i).getDestination() << std::endl;
				level = it->second;
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
				level.generateEffects(graphics, player);
			}
			else {
				level = Level(others.at(i).getDestination(), graphics, invent);
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
				level.generateEffects(graphics, player);
				this->storeLevel(level);
				std::cout << "stored level" << std::endl;
			}
			player_constants::mapName = others.at(i).getDestination();
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
			player_constants::showMapName = true;
		}
	}
}

void Player::handleLockedDoorCollision(std::vector<Door>& others, Level & level, Graphics & graphics, Inventory & invent, Player &player)
{
	//Check if the player is grounded and holding the down arrow
	//Also check to see if the player has a "Key"
	AESCipher cipher;
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded == true && this->_lookingDown == true) {
			if (!cipher.verifyHash(others.at(i).getDestination(), player)) {
				std::cout << "Unable to set level to new level : Hash check failed!" << std::endl;
				this->eventMessage = "Anti-Cheat: File has been modified.";
				this->showEventMsg = true;
				this->_timeForEventMsg = 0;
				return;
			}
			if (!this->checkLockedDoorCompleted(others.at(i).getDestination()) && !invent.hasKeyStored()) {
				this->eventMessage = "Key required";
				this->showEventMsg = true;
				this->_timeForEventMsg = 0;
				return;
			}
			if (level.isArenaActive()) {
				this->eventMessage = "Boss Battle still in-progress";
				this->showEventMsg = true;
				this->_timeForEventMsg = 0;
				return;
			}
				
			this->overwriteLevel(level, level.getMapName());
			std::map<std::string, Level>::iterator it;
			it = this->mapStorage.find(others.at(i).getDestination());
			if (it != this->mapStorage.end()) {
				std::cout << "found the saved map:" << others.at(i).getDestination() << std::endl;
				level = it->second;
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
				level.generateEffects(graphics, player);
			}
			else {
				level = Level(others.at(i).getDestination(), graphics, invent);
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
				level.generateEffects(graphics, player);
				this->storeLevel(level);
				std::cout << "stored level" << std::endl;
			}
			player_constants::mapName = others.at(i).getDestination();
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
			player_constants::showMapName = true;
			this->addLockedDoorTable(others.at(i).getDestination());
		}
	}
}

void Player::handleEnemyCollisions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		if (others.at(i)->getCurrentHealth() > 0) {
			others.at(i)->touchPlayer(this);
		}
	}
}

void Player::handleHex(float elapsedTime)
{
	for (int i = 0; i < this->hexTable.size(); i++) {
		// If the timer for the hex is the same or has exceeded the hex duration, remove it.
		if (std::get<4>(this->hexTable[i]) >= std::get<1>(this->hexTable[i])) {
			this->nullifyHex(std::get<0>(this->hexTable[i]));
			this->hexTable.erase(this->hexTable.begin() + i);
			--i;
		}
		else {
			// hexID 1: Damage reduction
			if (std::get<0>(this->hexTable[i]) == 1) {
				// For non-stackable hexes, just check if the timer is 0
				// That will let us know that this is the first iteration
				// As nothing can reduce the timer. So it should always be above 0 after
				// the initial cast.
				if (std::get<4>(this->hexTable[i]) == 0) {
					this->_dmgReduction += this->_dmgMod / 3;
					std::get<4>(this->hexTable[i]) += elapsedTime;
				}
				else {
					std::get<4>(this->hexTable[i]) += elapsedTime;
				}
			}
			// hedID 2: TBD
			// if (std::get<0>(this->hexTable[i]) == 2 {

			// }
		}
	}	
}

void Player::applyHex(int hexID, double duration, bool isStackable)
{
	if (hexID == 0) {
		// Nullify any hexes (I.E: Using an item that will remove it)
		this->setHex(false);
		this->nullifyHex(0);
	}
	else {
		this->setHex(true);
		if (this->hexTable.empty()) {
			this->hexTable.push_back(std::make_tuple(hexID, duration, isStackable, 1, 0));
		}
		else {
			auto hexIT = std::find_if(this->hexTable.begin(), this->hexTable.end(), [&hexID](const auto& t) {return std::get<0>(t) == hexID; });
			auto hexDistance = std::distance(this->hexTable.begin(), hexIT);
			if (hexIT != this->hexTable.end()) {
				if (std::get<2>(this->hexTable[hexDistance]) == true) {
					// Add a stack to the hex
					std::get<3>(this->hexTable[hexDistance]) += 1;
					// Increase the duration by half of the original duration
					std::get<1>(this->hexTable[hexDistance]) += duration / 2;
				}
			}
			else {
				// If the hex table isn't empty and the hex does not exist in the table already simply add the hex
				// Hexes are deleted once the duration has ended
				this->hexTable.push_back(std::make_tuple(hexID, duration, isStackable, 1, 0));
			}
		}
	}
}

void Player::nullifyHex(int hexID)
{
	if (hexID == 0) {
		// If all hexes are being nullified then we need to individually remove them all
		// by checking the hex table and seeing which ones are active and apply the
		// appropiate removal method.
		for (int i = 0; i < this->hexTable.size(); i++) {
			if (std::get<0>(this->hexTable[i]) == 1) {
				this->_dmgReduction -= this->_dmgMod / 3;
			}
			//if (std::get<0>(this->hexTable[i]) == 2) {
			//	Remove hexID 2 here
			//}
		}
		this->hexTable.clear();
	}
	else if (hexID == 1) {
		this->_dmgReduction -= this->_dmgMod / 3;
	}
	// Check if the table is empty after everything has been handled and if it is set the hex state to false
	if (this->hexTable.empty()) {
		this->isHexed = false;
	}
}

std::string Player::getNpcName(std::vector<Npc*> &others, Graphics &graphics) {
	std::string name;
	for (int i = 0; i < others.size(); i++) {
		name = others.at(i)->getName();
	}
	return name;
}

void Player::drawHPNumbers(Graphics & graphics)
{
	double percent = ((double)this->_currentHealth / (double)this->_maxHealth) * 100;
	this->_txt->drawPercentNumber(graphics, this->_x - 245, this->_y - 215, percent);
}

void Player::drawExpNumbers(Graphics & graphics)
{
	double percent = ((double)this->getCurrentExp() / (double)this->getRequiredExp()) * 100;
	this->_txt->drawPercentNumber(graphics, this->_x - 245, this->_y - 200, percent);
}

void Player::setIFrame(bool condition) {
	iFrame = condition;
	std::cout << "player iFrame = " << condition << std::endl;
}

void Player::gainHealth(float amount) {
	if (amount < 0 && player_constants::iFrame == false) {
		if (amount < -35)
			amount = amount + (this->_defense * 1.5);
		else
			amount = amount + (this->_defense * 0.5);
		if (amount >= 0)
			amount = -1.0f;
		this->_currentHealth += amount;
		std::cout << "lost " << amount << std::endl;
		player_constants::iFrame = true;
		this->gotHit = true;
		//Knock back player
		if (_facing == LEFT)
			this->_x += 14;
		else if (_facing == RIGHT)
			this->_x -= 14;
		this->_dx = 0;
	}
	else if (amount > 0) {
		this->_currentHealth += amount;
		if (this->_currentHealth > this->_maxHealth)
			this->_currentHealth = this->_maxHealth;
		std::cout << "gained " << amount << std::endl;
	}
}

void Player::gainHPFromStatus(float amount)
{   //This function is a constant tick.
	//It does not give the player iFrames
	if (amount < 0) {
		this->_currentHealth += amount;
	}
}

void Player::gainMaxHealth(float amount) { 
	this->_maxHealth += amount; 
	this->_currentHealth += _maxHealth / 8;
	if (this->_currentHealth > this->_maxHealth)
		this->_currentHealth = this->_maxHealth;
	std::cout << "Max health is now: " << this->_maxHealth << std::endl;
	this->eventMessage = "Max health increased by " + std::to_string((int)amount);
	this->showEventMsg = true;
	this->_timeForEventMsg = 0;
}

void Player::gainExp(float exp) {
	this->_exp += exp;
	std::string msg = "Gained (+" + std::to_string((int)exp) + ") exp";
	this->battleMessages.push_back(std::make_tuple(msg, 0, 0, 0));
	std::cout << "current exp = " << this->_exp << std::endl;
	std::cout << "required exp for level " << this->getLevel()+1 << " is:" << this->_requiredExp << std::endl;
}

void Player::gainExpFromEnemy(int enemyLevel, float exp)
{
	if (this->_playerLevel == 0) {
		this->_exp += exp;
	}
	else {
		exp -= 0.25;
		if (this->_playerLevel > enemyLevel) {
			int lvlDiff = this->_playerLevel - enemyLevel;
			if (exp <= 10)
				exp -= lvlDiff + this->_playerLevel * 0.12;
			else
				exp -= lvlDiff + this->_playerLevel * 0.30 + (exp / 3);
		}
		if (exp <= 0) {
			exp = 0.1;
		}
		this->_exp += exp;
	}
	std::cout << "exp gain: " << exp << std::endl;
	std::stringstream streamExp;
	streamExp << std::fixed << std::setprecision(2) << exp;
	std::string fixedExp = streamExp.str();
	std::string msg = "Gained (+" + fixedExp + ") exp";
	this->battleMessages.push_back(std::make_tuple(msg, 0, 0, 0));
	std::cout << "current exp = " << this->_exp << std::endl;
	std::cout << "required exp for level " << this->getLevel() + 1 << " is:" << this->_requiredExp << std::endl;
}

float Player::getCurrentExp()
{
	return this->_exp;
}

void Player::setCurrentExp(float exp) {
	this->_exp = exp;
}

float Player::getRequiredExp() { 
	for (int i = 0; i < this->getLevel() + 1; i++) {
		if (this->getLevel() == i) {
			this->_requiredExp = player_constants::expTable[i];
		}
	}
	return this->_requiredExp; 
}

int Player::getRequiredKills() {
	for (int i = 0; i < this->getSoulLevel() + 1; i++) {
		if (this->getSoulLevel() == i) {
			this->_requiredKill = player_constants::dmgTable[i];
		}
	}
	return this->_requiredKill;
}

void Player::gainCurrency(int num)
{
	this->currency += num;
	std::cout << "Player gained " << num << " money\n Total is: " << getCurrency() << std::endl;
}

void Player::statChoice(int selection)
{
	if (selection == 1 && this->_statPoints > 0) {
		this->_maxHealth += 10 + (this->_soulLevel * 0.7 + 0.38);
		this->_currentHealth += this->_maxHealth / 9;
		if (this->_currentHealth > this->_maxHealth)
			this->_currentHealth = this->_maxHealth;
		this->_statPoints--;
	}
	else if (selection == 2 && this->_statPoints > 0) {
		if (this->_dmgMod < 3)
			this->_dmgMod += 0.55;
		else
			this->_dmgMod += 0.25 + (this->_soulLevel * 0.2);
		this->_statPoints--;
	}
	else if (selection == 3 && this->_statPoints > 0) {
		if (this->_defense <= 7.65)
			this->_defense += 2.25 + (this->_soulLevel * 0.5);
		else
			this->_defense += 1.15 + (this->_soulLevel * 0.2);
		this->_statPoints--;
	}
}

int Player::getLevel()
{
	return this->_playerLevel;
}

void Player::setLevel(int num) {
	this->_playerLevel = num;
}

void Player::addLevel(int num) {
	this->_playerLevel += num;
	this->setCurrentExp(0);
	this->_defense += 0.25 + (this->_soulLevel * 0.11);
	this->_maxHealth += 2 + ((this->_soulLevel * 0.3) + 0.2);
	this->_currentHealth += this->_maxHealth / 4;
	if (this->_currentHealth > this->_maxHealth)
		this->_currentHealth = this->_maxHealth;
	this->_statPoints += 1;
	std::cout << "Level up to: " << this->getLevel() << std::endl;
	this->eventMessage = "Leveled Up!";
	this->showEventMsg = true;
	this->_timeForEventMsg = 0;
}

int Player::getSoulLevel() {
	return this->_soulLevel;
}

void Player::setSoulLevel(int num) {
	this->_soulLevel = num;
}
void Player::addSoulLevel(int num) {
	this->_soulLevel += num;
}

void Player::addKillCount(int num) {
	this->killCount += num;
	std::cout << this->getKillCount() << std::endl;
}

void Player::setKillCount(int num) {
	this->killCount = num;
}

int Player::getKillCount() {
	return this->killCount;
}

double Player::getDmgMod() {
	return this->_dmgMod;
}

double Player::getDefense()
{
	return this->_defense;
}

double Player::getSoulStr()
{
	return this->_soulStrength;
}

int Player::getStatPoints()
{
	return this->_statPoints;
}


Direction Player::facingDir()
{
	return this->_facing;
}

void Player::update(float elapsedTime) {

	if (this->_currentHealth <= 0) {
		this->_deathAnimationTimer += elapsedTime;
	}
	else {
		//Apply gravity
		if (this->_dy <= player_constants::GRAVITY_CAP && !_climbing && !this->isFlying) {
		//dy is change in y over this frames dy. If dy is less than or equal to gravity cap then we need to increase 
	    //the DY by gravity because we are not at the cap.
			this->_dy += player_constants::GRAVITY * elapsedTime;
		}
		/*if (!isFlying && !_climbing && (!_grounded || _currentSurface == SLOPE)) {
			if (this->_dy <= player_constants::GRAVITY_CAP) {
				this->_dy += player_constants::GRAVITY * elapsedTime;
			}
		}*/
		//Move by dx
		this->_x += this->_dx * elapsedTime; //elapsedTime will move by a certain amount based on frame rate keeping thing moving smoothly
		//Move by dy
		this->previousY = this->_y;
		this->_y += this->_dy * elapsedTime; //Gravity move them by Y

		if (this->isFlying) {
			this->_dy = 0;
			this->drawExhaust = true;
			this->isFlying = false;
		}
		else if (!this->isFlying && this->_fuel != 100.0f && this->_grounded) {
			this->_fuel += 0.05;
			if (this->_fuel >= 99.0f)
				this->_fuel = 100.0f;
		}
		else if (!this->isFlying || this->_fuel != 100.0f)
			this->drawExhaust = false;

		if (_climbing)
			this->_dy = 0;

		if (this->isDrowning) {
			player_constants::WALK_SPEED = 0.09f;
			player_constants::JUMP_DISTANCE = 0.55f;
			player_constants::GRAVITY = 0.0009;
			player_constants::GRAVITY_CAP = 0.45f;
		}
		if (!this->isDrowning && this->_air != 100.0f) {
			this->_air += 0.05;
			if (this->_air >= 99.0f)
				this->_air = 100.0f;
		}
		if (this->isDrowning && this->_air <= 0.0f)
			this->_currentHealth -= 0.08;
		if (!this->isDrowning && player_constants::WALK_SPEED != 0.2f) {
			player_constants::WALK_SPEED = 0.2f;
			player_constants::JUMP_DISTANCE = 0.7f;
			player_constants::GRAVITY = 0.002f;
			player_constants::GRAVITY_CAP = 0.8f;
		}
		//Poison timer
		if (this->isPoisoned) {
			this->_poisonDuration += elapsedTime;
			this->_poisonDOTTimer += elapsedTime;
			if (this->_poisonDOTTimer >= 1000) {
				this->gainHPFromStatus(-3.45);
				this->_poisonDOTTimer = 0;
			}
			if (this->_poisonDuration >= 8000) {
				this->_poisonDOTTimer = 0;
				this->_poisonDuration = 0;
				this->isPoisoned = false;
			}
		}

		//Hex timer
		if (this->isHexed) {
			this->handleHex(elapsedTime);
		}

		//iFrame timer
		if (player_constants::iFrame == true && this->_currentHealth > 0) {
			this->_timeElapsed += elapsedTime;
			if (this->getBlink())
				this->showBlink(false);
			else if (!this->getBlink())
				this->showBlink(true);

			if (this->_timeElapsed > this->_timeToUpdate) {
				this->_timeElapsed -= this->_timeToUpdate;
				player_constants::iFrame = false;
			}
		}
		else {
			this->showBlink(true);
		}

		for (int i = this->battleMessages.size(); i--;) {
			std::get<3>(this->battleMessages[i]) += elapsedTime; //increase timer
			//Set X to end of screen on the right then subtract by the width of the texture in TextManager
			//This will get the correct x position so that the last letter of the string ends right before the end of the screen
			std::get<1>(this->battleMessages[i]) = this->_x + 320; //Set to player x + screen width / 2 (320)
			std::get<2>(this->battleMessages[i]) = this->_y + 220; //Set to player y + the offset
			//Formula by Nataru(J.L)
			if (i < this->battleMessages.size() - 1) {
				if (i % 2 == 0) {
					std::get<2>(this->battleMessages[i]) = std::get<2>(this->battleMessages[i + 1]) - 15;
				}
				else {
					std::get<2>(this->battleMessages[i]) = std::get<2>(this->battleMessages[i + 1]) - 12;
				}
			}
			//Delete from vector if the message exceeds its lifespan
			if (std::get<3>(this->battleMessages[i]) >= 3100) {
				this->battleMessages.erase(this->battleMessages.begin() + i);
			}
		}

		if (this->getCurrentExp() >= this->getRequiredExp())
			this->addLevel(1);

		if (this->getKillCount() >= this->getRequiredKills()) {
			this->addSoulLevel(1);
			this->_soulStrength += 0.12;
			srand((unsigned)time(NULL));
			double corruptMod = this->_soulStrength + ((double)(rand() % 90 + 10) / 100 - 1.28);
			if (corruptMod <= 0.29) {
				corruptMod = 0.30;
			}
			else if (corruptMod >= 0.81) {
				corruptMod = 0.80;
			}
			this->_dmgMod += corruptMod;
			std::cout << "damaged gained: " << this->_dmgMod + this->_soulStrength + ((double)(rand() % 90 + 10) / 100) <<
				std::endl;
			std::cout << "damage mod is: " << this->_dmgMod << std::endl;
			std::cout << "Soul Level increased to: " << this->getSoulLevel() << std::endl;
			std::stringstream streamDmg;
			streamDmg << std::fixed << std::setprecision(2) << (double)this->_dmgMod;
			std::string fixedDmg = streamDmg.str();
			this->eventMessage = "Corruption spread. Damage increased to " + fixedDmg;
			this->showEventMsg = true;
			this->_timeForEventMsg = 0;
		}

		//Show map name timer
		if (player_constants::showMapName == true) {
			this->_mapTimeElapsed += elapsedTime;
			if (this->_mapTimeElapsed > this->_timeForMapName) {
				this->_mapTimeElapsed -= this->_timeForMapName;
				player_constants::showMapName = false;
			}
		}

		if (this->showEventMsg) {
			this->_timeForEventMsg += elapsedTime;
			if (this->_timeForEventMsg >= 5000) {
				this->eventMessage.clear();
				this->showEventMsg = false;
				this->_timeForEventMsg = 0;
			}
		}
	}
	
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
	this->drawJetPack(graphics);
	this->drawGun(graphics);
	if (this->isPoisoned)
		this->drawStatusEffect(graphics, "POISONED");
	if (this->isBurning)
		this->drawStatusEffect(graphics, "BURNING");
	if (this->isDrowning)
		this->drawStatusEffect(graphics, "AIR: " + std::to_string((int)this->_air));
	if (!this->isDrowning && this->_air != 100.0f)
		this->drawStatusEffect(graphics, "AIR: " + std::to_string((int)this->_air));
	if (this->isFlying)
		this->drawStatusEffect(graphics, "FUEL: " + std::to_string((int)this->_fuel));
	if (!this->isFlying && this->_fuel != 100.0f)
		this->drawStatusEffect(graphics, "FUEL: " + std::to_string((int)this->_fuel));
	if (this->showEventMsg)
		this->drawEventMessage(graphics, this->eventMessage);
	if (!this->battleMessages.empty()) {
		for (int i = 0; i < this->battleMessages.size(); ++i) {
			this->_txt->drawBattleMessages(graphics, std::get<1>(this->battleMessages[i]), std::get<2>(this->battleMessages[i]),
				std::get<0>(this->battleMessages[i]));
		}
	}
}

void Player::drawGun(Graphics & graphics)
{
	if (this->_currentHealth <= 0)
		return;
	if (this->lookingLeft() && !this->lookingUp() && !this->lookingDown()) {
		_Gun.setSourceRectX(52);
		_Gun.setSourceRectY(10);
		this->_Gun.draw(graphics, this->getX() - 8, this->getY() + 14);
		}
	else if (this->lookingRight() && !this->lookingUp() && !this->lookingDown()) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(26);
		_Gun.setSourceRectH(6);
		this->_Gun.draw(graphics, this->getX() + 19, this->getY() + 16);
		}
	else if ((this->lookingUp() == true) && (this->_facing == RIGHT)) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(34);
		this->_Gun.draw(graphics, this->getX() + 19, this->getY() + 16);
		}
	else if ((this->lookingUp() == true) && (this->_facing == LEFT)) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(50);
		this->_Gun.draw(graphics, this->getX() - 1, this->getY() + 14);
	}
	else if ((this->lookingDown() == true) && (this->_facing == LEFT)) {
		_Gun.setSourceRectX(59);
		_Gun.setSourceRectY(70);
		_Gun.setSourceRectH(10);
		this->_Gun.draw(graphics, this->getX() - 4, this->getY() + 14);
	}
	else if ((this->lookingDown() == true) && (this->_facing == RIGHT)) {
		_Gun.setSourceRectX(54);
		_Gun.setSourceRectY(86);
		_Gun.setSourceRectH(10);
		this->_Gun.draw(graphics, this->getX() + 20, this->getY() + 14);
	}	
}

void Player::drawJetPack(Graphics & graphics)
{
	if (this->_currentHealth <= 0)
		return;
	if (this->drawExhaust) {
		this->_JetPack.draw(graphics, this->getX() + 6, this->getY() + 35);
	}
}

void Player::drawStatMenu(Graphics &graphics, Player &player, int selection) {
	_statMenu.drawStatMenu(graphics, this->_x - 300, this->_y - 80);
	if (selection == 1)
		_statSelection.drawStatSelection(graphics, this->_x - 285, this->_y - 70);
	if (selection == 2)
		_statSelection.drawStatSelection(graphics, this->_x - 285, this->_y - 25);
	if (selection == 3)
		_statSelection.drawStatSelection(graphics, this->_x - 285, this->_y - -25);
	this->_txt->drawStats(graphics, this->_x - 115, this->_y - 65, this->_maxHealth, this->_dmgMod, this->_defense, this->_statPoints);
}

void Player::drawSaveMenu(Graphics & graphics, Player & player, int selection)
{
	_saveMenu.drawSaveMenu(graphics, this->_x - 50, this->_y + 80);
	if (selection == 1)
		_saveSelection.drawSaveSelection(graphics, this->_x - 35, this->_y + 85);
	if (selection == 2)
		_saveSelection.drawSaveSelection(graphics, this->_x + 40, this->_y + 85);
}

void Player::drawCurrentMapName(Graphics &graphics) {
	if (player_constants::showMapName == true) {
		this->_txt->drawMapName(graphics, player_constants::mapName, this->getX(), this->getY());
	}
}

void Player::drawStatusEffect(Graphics & graphics, const std::string text)
{
	this->_txt->drawPlayerStatus(graphics, this->_x, this->_y, text);
}

void Player::drawEventMessage(Graphics & graphics, std::string text)
{
	this->_txt->drawEventMessages(graphics, this->_x, this->_y, text);
}

void Player::drawBattleMessage(Graphics & graphics, std::string text)
{
	this->_txt->drawBattleMessages(graphics, this->_x, this->_y, text);
}

void Player::showSceneDialogue(Graphics & graphics, std::string text)
{
	this->_txt->drawSceneDialogue(graphics, this->getX(), this->getY(), text);
}
