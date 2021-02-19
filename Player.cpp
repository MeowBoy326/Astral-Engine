#include "Player.h"
#include "Graphics.h"
#include "Slope.h"
#include "Door.h"
#include "Npc.h"
#include "Items.h"

namespace player_constants {
	const float WALK_SPEED = 0.2f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	bool iFrame = false;
	std::string mapName = "";
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
	_maxHealth(3.0f),
	_currentHealth(3.0f)
{
	graphics.loadImage("MyChar.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("RunRight");
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
	else if (this->_lookingUp == false) {
		return false;
	}
}

bool Player::lookingDown() {
	if (this->_lookingDown == true) {
		return true;
	}
	else if (this->_lookingDown == false) {
		return false;
	}

}

bool Player::lookingRight() {
	if (this->_facing == RIGHT) {
		return true;
	}
	else if (this->_facing == LEFT) {
		return false;
	}
}

bool Player::lookingLeft() {
	if (this->_facing == LEFT) {
		return true;
	}
	else if (this->_facing == RIGHT) {
		return false;
	}
}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
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

void Player::handleDoorCollision(std::vector<Door> &others, Level &level, Graphics &graphics) {
	//Check if the player is grounded and holding the down arrow
	//If so, go through the door
	//If not, do nothing
	for (int i = 0; i < others.size(); i++) {
		if (this->_grounded == true && this->_lookingDown == true) {
			level = Level(others.at(i).getDestination(), graphics);
			player_constants::mapName = others.at(i).getDestination();
			this->_x = level.getPlayerSpawnPoint().x;
			this->_y = level.getPlayerSpawnPoint().y;
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
			others.at(i)->runScript(others.at(i)->getName(), graphics, this->_x, this->_y);
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

void Player::setIFrame(bool condition) {
	iFrame = condition;
	cout << "player iFrame = " << condition << endl;
}

void Player::gainHealth(float amount) {
	if (amount < 0 && player_constants::iFrame == false) {
		this->_currentHealth += amount; //can be used to take away hp as well
		cout << "lost " << _currentHealth << endl;
		player_constants::iFrame = true;
	}
	else if (amount > 0) {
		this->_currentHealth += amount; //can be used to take away hp as well
		cout << "gained " << _currentHealth << endl;
	}
}

int Player::getCurrentExp()
{

	return 0;
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

void Player::addKillCount(int num) {
	this->killCount += num;
	cout << this->getKillCount() << endl;
}

void Player::setKillCount(int num) {
	this->killCount = num;
}

int Player::getKillCount() {
	return this->killCount;
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

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ remove text?
		this->_timeElapsed -= this->_timeToUpdate;
		player_constants::iFrame = false;
	}

	if ((this->getKillCount() >= 3 && this->getKillCount() < 4) && this->getLevel() == 0) {
		this->addLevel(1);
		cout << "leveled up to: " << this->getLevel() << endl;
	}

	else if (this->getKillCount() >= 4 && this->getKillCount() < 8 && this->getLevel() == 1) {
		this->addLevel(1);
		cout << "leveled up to: " << this->getLevel() << endl;
	}

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}