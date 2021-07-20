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
	std::vector<float> expTable = {20, 150, 369, 715, 1000, 1420, 2840, 4160, 7180, 12420};
	std::vector<float> dmgTable = { 40, 185, 420, 850, 1522, 2437, 4700, 6000 }; 
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

void Player::addKillTable(std::string name)
{
	if (this->killTable.empty()) {
		this->killTable.push_back(std::make_pair(name, 1));
	}
	else {
		auto it = std::find_if(killTable.begin(), killTable.end(), [&name](const auto& t) {return t.first == name; });
		auto distance = std::distance(this->killTable.begin(), it);
		if (it != killTable.end()) {
			killTable[distance].second += 1;
		}
		else {
			this->killTable.push_back(std::make_pair(name, 1));
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
	this->_dx = -player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunLeft");
	}
	this->_facing = LEFT;
}

void Player::moveRight() {
	if (this->_lookingDown == true && this->_grounded == true) 
	{ return; }
	this->_dx = player_constants::WALK_SPEED;
	if (this->_lookingUp == false) {
		this->playAnimation("RunRight");
	}
	this->_facing = RIGHT;
}

void Player::moveUp()
{
	this->_dy = -player_constants::WALK_SPEED;
}

void Player::moveDown()
{
	this->_dy = +player_constants::WALK_SPEED;
}



void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_DISTANCE;
		this->_grounded = false;
		this->_currentSurface = NOTHING;
	}
}

/*bool Player::isGrounded() {
	if (this->_dy == 0) {
		return true;
	}
	if (this->_dy > 0) {
		return false;
	}
} */

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

//void handleTileCollisions
//handles collisions with all tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collision happened on and move the player accordingly
	if (this->_climbing)
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
		//this->gainHPFromStatus(-0.02);
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
				break; //going down the ladder
				//this->_dy = 0; //reset all gravity, if we arent grounded we fall to the ground
				//this->_y = others.at(i).getBottom() + 1; //no longer go through things, stops us
				//if (this->_grounded) { //only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
				//	this->_dx = 0; //stop movement on x-axis
				//	this->_x -= this->_facing == RIGHT ? 0.5f : -0.5f; //if we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				//}
				//this->_climbing = false;
				//return false;
				//break;
			case sides::BOTTOM: //going up the ladder
				//hit the top (bottom) of tile push us back up ontop of tile
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true; //we are on ground since it pushed it back up
				this->_climbing = false;
				return false;
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
				return;
			}
			if (level.isArenaActive())
				return;
			this->overwriteLevel(level, level.getMapName());
			std::map<std::string, Level>::iterator it;
			it = this->mapStorage.find(others.at(i).getDestination());
			if (it != this->mapStorage.end()) {
				std::cout << "found the saved map:" << others.at(i).getDestination() << std::endl;
				level = it->second;
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
			}
			else {
				level = Level(others.at(i).getDestination(), graphics, invent);
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
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
				return;
			}
			if (!invent.hasKeyStored()) {
				return;
			}
			if (level.isArenaActive())
				return;
			this->overwriteLevel(level, level.getMapName());
			std::map<std::string, Level>::iterator it;
			it = this->mapStorage.find(others.at(i).getDestination());
			if (it != this->mapStorage.end()) {
				std::cout << "found the saved map:" << others.at(i).getDestination() << std::endl;
				level = it->second;
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
			}
			else {
				level = Level(others.at(i).getDestination(), graphics, invent);
				level.generateMapItems(graphics, level.getMapName(), invent);
				level.generateEnemies(graphics, level.getMapName(), player);
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

void Player::handleEnemyCollisions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		if (others.at(i)->getCurrentHealth() > 0) {
			others.at(i)->touchPlayer(this);
		}

		//others.at(i)->setTimer();
	}
}

/*
void Player::handleItemCollisions(std::vector<std::string*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->
	}
}*/

void Player::handleNpcCollisions(std::vector<Npc*> &others, Graphics &graphics, int lineCount) {
	for (int i = 0; i < others.size(); i++) {
		//others.at(i)->touchPlayer(this);
		//others.at(i)->say(graphics, this);
		//cout << "handleNpcColl: This npc is: " << others.at(i)->getName() << endl;
		if (lineCount == 0) {
			//others.at(i)->playScript(others.at(i)->getName(), graphics, this->_x, this->_y);
		}
	/*	else if (lineCount >= 1) {
			others.at(i)->playNextScript(others.at(i)->getName(), graphics, this->_x, this->_y, lineCount);
		} */
	}
}

std::string Player::getNpcName(std::vector<Npc*> &others, Graphics &graphics) {
	std::string name;
	for (int i = 0; i < others.size(); i++) {
		//others.at(i)->runScript(others.at(i)->getName(), graphics, this->_x, this->_y);
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
		amount = amount + (this->_defense * 0.5);
		if (amount >= 0)
			amount = -0.15f;
		this->_currentHealth += amount;
		std::cout << "lost " << amount << std::endl;
		player_constants::iFrame = true;
		this->gotHit = true;
	}
	else if (amount > 0) {
		this->_currentHealth += amount;
		std::cout << "gained " << _currentHealth << std::endl;
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
	this->_currentHealth += _maxHealth - _currentHealth;
	std::cout << "Max health is now: " << this->_maxHealth << std::endl;
}

void Player::gainExp(float exp) {
	this->_exp += exp;
	std::cout << "current exp = " << this->_exp << std::endl;
	std::cout << "required exp for level " << this->getLevel()+1 << " is:" << this->_requiredExp << std::endl;
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
		this->_maxHealth += 10 + (this->_soulLevel * 0.5 + 0.08);
		this->_currentHealth = this->_maxHealth;
		this->_statPoints--;
	}
	else if (selection == 2 && this->_statPoints > 0) {
		this->_dmgMod += 1.014;
		this->_statPoints--;
		//
	}
	else if (selection == 3 && this->_statPoints > 0) {
		this->_defense += 0.25 + (this->_soulLevel * 0.5);
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
	this->_defense += 0.03 + (this->_soulLevel * 0.11);
	this->_statPoints += 2;
	std::cout << "Level up to: " << this->getLevel() << std::endl;
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
		if (_currentSurface == RECTANGLE) {
			if (getBoundingBox().getRight() < _lastCollidedFloorRect.getLeft() || getBoundingBox().getLeft() > _lastCollidedFloorRect.getRight()) {
				_grounded = false;
				_currentSurface = NOTHING;
			}
		}
		else if (_currentSurface == SLOPE) {
			if (getBoundingBox().getLeft() < _lastCollidedSlope.getP1().x && getBoundingBox().getRight() < _lastCollidedSlope.getP1().x
				&& getBoundingBox().getLeft() < _lastCollidedSlope.getP2().x && getBoundingBox().getRight() < _lastCollidedSlope.getP2().x) {
				_grounded = false;
			}
			if (getBoundingBox().getLeft() > _lastCollidedSlope.getP1().x && getBoundingBox().getRight() > _lastCollidedSlope.getP1().x
				&& getBoundingBox().getLeft() > _lastCollidedSlope.getP2().x && getBoundingBox().getRight() > _lastCollidedSlope.getP2().x) {
				_grounded = false;
			}
		}
		//Apply gravity
		//if (this->_dy <= player_constants::GRAVITY_CAP && !_climbing) {
		//	//dy is change in y over this frame Delta Y if dy is less than or equal to gravity cap then we need to increase cuz we are not at the cap
		//	this->_dy += player_constants::GRAVITY * elapsedTime;
		//}
		if (!_grounded || _currentSurface == SLOPE || !_climbing) {
			if (this->_dy <= player_constants::GRAVITY_CAP) {
				this->_dy += player_constants::GRAVITY * elapsedTime;
			}
		}

		//Move by dx
		this->_x += this->_dx * elapsedTime; //elapsedTime will move by a certain amount based on frame rate keeping thing moving smoothly
		//Move by dy
		this->_y += this->_dy * elapsedTime; //Gravity move them by Y

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

		if (this->getCurrentExp() >= this->getRequiredExp())
			this->addLevel(1);

		if (this->getKillCount() >= this->getRequiredKills()) {
			this->addSoulLevel(1);
			this->_soulStrength += 1;
			srand((unsigned)time(NULL));
			this->_dmgMod += this->_soulStrength + ((double)(rand() % 90 + 10) / 100);
			std::cout << "damage mod is: " << this->_dmgMod << std::endl;
			//cout << "damage mod is: " << this->dmgMod;
			std::cout << "Soul Level increased to: " << this->getSoulLevel() << std::endl;
		}

		//Show map name timer
		if (player_constants::showMapName == true) {
			this->_mapTimeElapsed += elapsedTime;
			if (this->_mapTimeElapsed > this->_timeForMapName) {
				this->_mapTimeElapsed -= this->_timeForMapName;
				player_constants::showMapName = false;
			}
		}
	}
	
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
	this->drawGun(graphics);
	if (this->isPoisoned)
		this->drawStatusEffect(graphics, "POISONED");
	if (this->isBurning)
		this->drawStatusEffect(graphics, "BURNING");
	if (this->isDrowning)
		this->drawStatusEffect(graphics, "AIR: " + std::to_string((int)this->_air));
	if (!this->isDrowning && this->_air != 100.0f)
		this->drawStatusEffect(graphics, "AIR: " + std::to_string((int)this->_air));
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

void Player::showSceneDialogue(Graphics & graphics, std::string text)
{
	this->_txt->drawSceneDialogue(graphics, this->getX(), this->getY(), text);
}
