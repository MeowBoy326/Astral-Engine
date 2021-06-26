#pragma once
#include "TextManager.h"
#include "Player.h"
#include "Graphics.h"
#include "Slope.h"
#include "Door.h"
#include "Npc.h"
#include "Items.h"
#include <time.h>

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	bool iFrame = false;
	bool showMapName = false;

	std::string mapName = "CAVE";
	/*
	* Index 0 = the exp needed from lvl 0 (starting level) to level 1.
	* So, to get to level 1 you need 20 exp. Level 2 needs 40 exp. With every new level exp is set to 0 
	*/
	std::vector<float> expTable = {20, 40, 60, 80, 100, 120, 140, 160, 180, 200};
	std::vector<float> dmgTable = { 6, 20, 67, 150, 222, 437, 700, 1000 }; 
	// same as above but with kills. this increased damage per kill needed
}

Player::Player() {}

Player::Player(Graphics &graphics, Vector2 spawnPoint) :
	//graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
	AnimatedSprite(graphics, "MyChar.png", 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100), 
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_grounded(false),
	_lookingUp(false),
	_lookingDown(false),
	_maxHealth(100.0f),
	_currentHealth(100.0f)
{
	graphics.loadImage("MyChar.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("RunRight");

	this->_statMenu = Sprite(graphics, "npcTextBox.png", 40, 88, 71, 52, this->_x, (this->_y - 10));
	this->_statSelection = Sprite(graphics, "npcTextBox.png", 0, 147, 46, 18, 15, 15);
	graphics.loadImage("npcTextBox.png");
	//graphics.loadImage("")
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
}

void Player::animationDone(std::string currentAnimation) {}

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
			std::cout << "Appropiate amount of mobs killed!" << std::endl;
			return true;
		}
			
	}
	return false;
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



void Player::jump() {
	if (this->_grounded) {
		this->_dy = 0;
		this->_dy -= player_constants::JUMP_DISTANCE;
		this->_grounded = false;
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

//void handleTileCollisions
//handles collisions with all tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle> &others) {
	//Figure out what side the collision happened on and move the player accordingly
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
	}
}

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics, Inventory &invent) {
	//Check if the player is grounded and holding the down arrow
	//If so, go through the door
	//If not, do nothing
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded == true && this->_lookingDown == true) {
			level = Level(others.at(i).getDestination(), graphics, invent);
			player_constants::mapName = others.at(i).getDestination();
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
			player_constants::showMapName = true;
		}
	}
}

void Player::handleLockedDoorCollision(std::vector<Door>& others, Level & level, Graphics & graphics, Inventory & invent)
{
	//Check if the player is grounded and holding the down arrow
	//Also check to see if the player has a "Key"
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded == true && this->_lookingDown == true && invent.hasKeyStored() == true) {
			level = Level(others.at(i).getDestination(), graphics, invent);
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
			amount = -0.15;
		this->_currentHealth += amount;
		std::cout << "lost " << amount << std::endl;
		player_constants::iFrame = true;
	}
	else if (amount > 0) {
		this->_currentHealth += amount;
		std::cout << "gained " << _currentHealth << std::endl;
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
	std::cout << "required exp for level " << this->getLevel() << " is:" << this->_requiredExp << std::endl;
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
		this->_dmgMod += 1.02;
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
	//Apply gravity
	if (this->_dy <= player_constants::GRAVITY_CAP) {
		//dy is change in y over this frame Delta Y if dy is less than or equal to gravity cap then we need to increase cuz we are not at the cap
		this->_dy += player_constants::GRAVITY * elapsedTime;
	}

	//Move by dx
	this->_x += this->_dx * elapsedTime; //elapsedTime will move by a certain amount based on frame rate keeping thing moving smoothly
	//Move by dy
	this->_y += this->_dy * elapsedTime; //Gravity move them by Y

	//cout << this->_x << " , " << this->_y << endl;

	//iFrame timer
	if (player_constants::iFrame == true) {
		this->_timeElapsed += elapsedTime;
		if (this->_timeElapsed > this->_timeToUpdate) {
			this->_timeElapsed -= this->_timeToUpdate;
			player_constants::iFrame = false;
		}
	}

	if (this->getCurrentExp() >= this->getRequiredExp()) {
		this->addLevel(1);
		this->setCurrentExp(0);
		this->_defense += 0.05 + (this->_soulLevel * 0.14);
		this->_statPoints += 2;
		std::cout << "Level up to: " << this->getLevel() << std::endl;
	}

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
	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
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

void Player::drawCurrentMapName(Graphics &graphics) {
	if (player_constants::showMapName == true) {
		this->_txt->drawMapName(graphics, player_constants::mapName, this->getX(), this->getY());
	}
}