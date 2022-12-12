#include "../headers/Enemy.h"
#include "../headers/TextManager.h"
#include "../headers/Player.h"

#include <iostream>
#include "SDL_timer.h"

TextManager txt;
// Base enemy class
Enemy::Enemy() {}

Enemy::~Enemy()
{
}

namespace enemyConstants {
	// Bool removeEnemy = false;
}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT)
	//_maxHealth(3),
	//_currentHealth(3)
{}

void Enemy::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::handleEnemyTileCollision(std::vector<Rectangle>& others)
{
}

void Enemy::bulletHit(float dmg) {
	this->_currentHealth -= dmg;
	std::cout << "hit! HP = " << this->_currentHealth << std::endl;
}

std::string Enemy::getName()
{
	return this->name;
}

void Enemy::playDeath() {
	if (deathCreated == false) {

	}
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Enemy::isRemoveable() {
	return removeEnemy;
}

void Enemy::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
}

void Enemy::dropLoot(Player & player)
{
	this->canDropLoot = true;
}

float Enemy::enemyExpAmount() {
	return 0;
}

// Bat class
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "data\\enemy\\NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_maxHealth(8),
	_currentHealth(8)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
	this->_fireX = this->_startingX;
	this->_fireY = this->_startingY;
	this->_HPBar._x = this->_startingX;
	this->_HPBar._y = this->_startingY;
	this->_fireBall = Sprite(graphics, "data\\enemy\\NpcCemet.png", 220, 33, 13, 10, this->_fireX, (this->_fireY + 10));
	this->_HPBar = Sprite(graphics, "data\\enemy\\NpcCemet.png", 2, 157, 17, 7, this->_HPBar._x, this->_HPBar._y - 15);
	this->_HPValue = Sprite(graphics, "data\\enemy\\NpcCemet.png", 3, 167, 17, 5, this->_HPBar._x + 1, this->_HPBar._y + 2);
}

Bat::~Bat()
{
	this->destroySprite();
	this->_fireBall.destroySprite();
	this->_HPBar.destroySprite();
	this->_HPValue.destroySprite();
}

void Bat::update(int elapsedTime, Player &player) {
	this->_fireBall.updateFBall();
	this->_HPBar.update();
	this->_HPValue.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");
		// Move up or down
		this->_y += this->_shouldMoveUp ? -.06 : .06;
		this->_x += this->_shouldMoveLeft ? -.016 : .016;
		this->_HPBar._y += this->_shouldMoveUp ? -.06 : .06;
		this->_HPBar._x += this->_shouldMoveLeft ? -.016 : .016;
		this->_HPValue._y += this->_shouldMoveUp ? -.06 : .06;
		this->_HPValue._x += this->_shouldMoveLeft ? -.016 : .016;
		float hpNum = (float)this->getCurrentHealth() / this->getMaxHealth();
		this->_HPValue.setSourceRectW(std::floor(hpNum * 17));

		if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
			this->_shouldMoveUp = !this->_shouldMoveUp;
		}
		if (this->_x > (this->_startingX + 6) || this->_x < this->_startingX - 6) {
			this->_shouldMoveLeft = !this->_shouldMoveLeft;
		}
		if (this->getBoundingBox().collidesWith(player.getBoundingBox())) {
			player.gainHealth(-11.5);
		}
		if (_fireBall.getBoundingBox().collidesWith(player.getBoundingBox())) {
			this->_fireBall.setX(this->_x);
			this->_fireBall.setY(this->_y);
			player.gainHealth(-15.5);
		}
		else if (_fireBall.getY() > this->_y + 100 || _fireBall.getY() < this->_y - 100) {
			this->_fireBall.setX(this->_x);
			this->_fireBall.setY(this->_y);
		}
		else {
			this->_fireBall.addX(player.getX() > this->_x ? .04 : -.04);
			this->_fireBall.addY(player.getY() > this->_y ? .04 : -.04);
		}
	}
	else if (this->getCurrentHealth() <= 0) {
		_deathTimeElapsed += elapsedTime;
		if (isVisible == false)
			timerRespawn += elapsedTime;
		if (_deathTimeElapsed > _deathAnimationTime && isVisible == true && dyingAnimation == true) {
			this->dyingAnimation = false;
			this->_deathTimeElapsed = 0;
			this->isVisible = false;
		}
		else if (_deathTimeElapsed < _deathAnimationTime && isVisible == true) {
			this->dyingAnimation = true;
			this->playAnimation("BatDie");
		}
		if (timerRespawn > respawnTime && isVisible == false && dyingAnimation == false) {
			timerRespawn = 0;
			this->isVisible = true;
			this->_currentHealth = this->_maxHealth;
			this->removeEnemy = false;
			this->canDropLoot = false;
			_deathTimeElapsed = 0;
		}
	}

	Enemy::update(elapsedTime, player);
}


void Bat::draw(Graphics &graphics) {
	if (isVisible == true) {
		Enemy::draw(graphics);
		this->_fireBall.drawFBall(graphics, this->_fireBall.getX(), this->_fireBall.getY());
		if (this->getCurrentHealth() < this->getMaxHealth()) {
			this->_HPBar.draw(graphics, this->_HPBar._x, this->_HPBar._y);
			this->_HPValue.draw(graphics, this->_HPValue._x, this->_HPValue._y);
		}

	}
}

void Bat::animationDone(std::string currentAnimation) {
	if (this->getCurrentHealth() <= 0) {
		if (this->canDropLoot == false) {
			this->removeEnemy = true;
			this->canDropLoot = true;
		}
	}
}

void Bat::dropLoot(Player &player) {
	
}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
	this->addAnimation(7, 6, 32, "BatDie", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 220, 33, "FireBall", 13, 10, Vector2(0, 0));
}

void Bat::playDeath() {
	this->playAnimation("BatDie", true);
}

void Bat::playAttack() {
	
}

void Bat::touchPlayer(Player* player) {
		player->gainHealth(-11.0f);
}

void Bat::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
}

void Bat::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
}

void Bat::handleEnemyTileCollision(std::vector<Rectangle>& others)
{
}

bool Bat::isRemoveable() {
	return removeEnemy;
}

float Bat::enemyExpAmount() {
	return Bat::batExp;
}

std::string Bat::getName()
{
	return this->name;
}


//Shade class
Shade::Shade() {}

Shade::~Shade()
{
	this->destroySprite();
	this->_shadeBall.destroySprite();
	this->_oppositeShadeBall.destroySprite();
	this->_HPBar.destroySprite();
	this->_HPValue.destroySprite();
}

Shade::Shade(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "data\\enemy\\shade.png", 27, 7, 32, 32, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_currentHealth(170),
	_maxHealth(170)
{
	this->setupAnimations();
	this->playAnimation("shadeRight");

	this->_shadeBall = Sprite(graphics, "data\\enemy\\shadeAttack.png", 0, 0, 22, 19, this->_startingX, (this->_startingY - 10));
	this->_oppositeShadeBall = Sprite(graphics, "data\\enemy\\shadeAttack.png", 0, 0, 22, 19, this->_startingX - 5, (this->_startingY + 130));
	this->_HPBar = Sprite(graphics, "data\\enemy\\NpcCemet.png", 2, 157, 17, 7, this->_startingX + 56, this->_startingY - 15);
	this->_HPValue = Sprite(graphics, "data\\enemy\\NpcCemet.png", 3, 174, 17, 5, this->_startingX + 57, this->_startingY - 12);
	graphics.loadImage("data\\enemy\\shadeAttack.png");

	this->sBallX = .33;
	this->osBallX = -.33;
}

void Shade::update(int elapsedTime, Player &player) {
	if (this->_dy <= this->GRAVITY_CAP) {
		// Dy is change in y over this frame Delta Y if dy is less than or equal to gravity cap then we need to increase cuz we are not at the cap
		this->_dy += this->GRAVITY * elapsedTime;
	}
	this->_y += this->_dy * elapsedTime; // Gravity move them by Y
	this->_shadeBall.update();
	this->_oppositeShadeBall.update();
	this->_HPBar.update();
	this->_HPValue.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		if (this->_currentHealth == this->_maxHealth && this->isIdle == true && (this->_direction == RIGHT && player.getX() > this->_x + 350 || this->_direction == LEFT &&
			player.getX() <= this->_x - 350)) {
			this->playAnimation("shadeIdle");
			isIdle = true;
		}
		else {
			isIdle = false;
			this->playAnimation(this->_direction == RIGHT ? "shadeRight" : "shadeLeft");

			float hpNum = (float)this->getCurrentHealth() / this->getMaxHealth();
			this->_HPValue.setSourceRectW(std::floor(hpNum * 17));
			if (player.isGrounded() && player.getY() < this->_y + 15 && !this->_hasTeleported) {
				this->_y = player.getY();
				this->_x = player.getX();
				this->_shadeBall._y = this->_y - 10;
				this->_shadeBall._x = this->_x;
				this->_oppositeShadeBall._y = this->_y + 30;
				this->_oppositeShadeBall._x = this->_x;
				this->_HPBar._x = this->_x + 56;
				this->_HPBar._y = this->_y - 15;
				this->_HPValue._x = this->_x + 57;
				this->_HPValue._y = this->_y - 12;
				this->_hasTeleported = true;
			}
			if (this->_hasTeleported) {
				this->_teleportTimer += elapsedTime;
				if (this->_teleportTimer >= 1400)
					this->_hasTeleported = false;
			}
			if (this->_direction == RIGHT) {
				this->_x += 0.07f;
				if (this->_boundingBox.collidesWith(player.getBoundingBox()))
					this->_x -= 0.07f;
				this->_HPBar._x = this->_x + 56;
				this->_HPValue._x = this->_x + 56;
				this->_HPBar._y = this->_y - 65;
				this->_HPValue._y = this->_y - 62;
			}
			else {
				this->_x -= 0.07f;
				if (this->_boundingBox.collidesWith(player.getBoundingBox()))
					this->_x += 0.07f;
				this->_HPBar._x = this->_x + 36;
				this->_HPValue._x = this->_x + 36;
				this->_HPBar._y = this->_y - 65;
				this->_HPValue._y = this->_y - 62;
			}
			if (this->getBoundingBox().collidesWith(player.getBoundingBox())) {
				player.gainHealth(-9.64f);
			}
			if (_shadeBall.getBoundingBox().collidesWith(player.getBoundingBox())) {
				player.gainHealth(-38.69f);
			}
			if (_shadeBall.getX() >= this->_startingX + 1180) {
				this->sBallX = -.33 * sBallMultiplyer;
				sBallMultiplyer += 0.4;
				if (sBallMultiplyer >= 4.0)
					sBallMultiplyer = 1.0;
			}
			if (this->_startingX - 430 >= _shadeBall._x) {
				this->sBallX = .33 * sBallMultiplyer;
				sBallMultiplyer += 0.4;
				if (sBallMultiplyer >= 4.0)
					sBallMultiplyer = 1.0;
			}
			if (_oppositeShadeBall.getBoundingBox().collidesWith(player.getBoundingBox())) {
				player.gainHealth(-38.69f);
			}
			if (_oppositeShadeBall.getX() >= this->_startingX + 1180) {
				this->osBallX = -.33 * osBallMultiplyer;
				osBallMultiplyer += 0.4;
				if (osBallMultiplyer >= 4.0)
					osBallMultiplyer = 1.0;
			}
			if (this->_startingX - 430 >= _oppositeShadeBall._x) {
				this->osBallX = .33 * osBallMultiplyer;
				osBallMultiplyer += 0.4;
				if (osBallMultiplyer >= 4.0)
					osBallMultiplyer = 1.0;
			}
			this->_shadeBall._x += this->sBallX;
			this->_oppositeShadeBall._x += this->osBallX;

			// 75% HP threshold
			if (this->getCurrentHealth() <= (this->getMaxHealth() * 75) / 100) {
				if (this->_hex1Timer == 0) {
					// Apply a hex to the player if the shades HP reaches the above threshold.
					player.applyHex(1, 12000, false);
				}
				this->_hex1Timer += elapsedTime;
			}

			if (this->getCurrentHealth() <= (this->getMaxHealth() * 35) / 100 && this->canHeal == false) {
				if (this->_healCDTimer == 0) {
					this->_currentHealth += 50;
					this->canHeal = true;
				}
			}

			if (this->canHeal) {
				this->_healTimer += elapsedTime;
				if (this->_healTimer >= this->_healDelay) {
					this->_currentHealth += 15;
					this->healCount++;
					this->_healTimer = 0;
				}
			}

			if (this->healCount >= 5) {
				this->canHeal = false;
				this->_healCDTimer += elapsedTime;
				if (this->_healCDTimer >= this->_healCooldown) {
					this->_healCDTimer = 0;
					this->healCount = 0;
				}
			}

			if (this->_hex1Timer >= this->_hex1Cooldown) {
				this->_hex1Timer = 0;
			}
		}
	}

	else if (this->getCurrentHealth() <= 0) {
		_deathTimeElapsed += elapsedTime;
		if (isVisible == false)
			timerRespawn += elapsedTime;
		if (_deathTimeElapsed > _deathAnimationTime && isVisible == true && dyingAnimation == true) {
			this->dyingAnimation = false;
			this->_deathTimeElapsed = 0;
			this->isVisible = false;
		}
		else if (_deathTimeElapsed < _deathAnimationTime && isVisible == true) {
			this->dyingAnimation = true;
			this->playAnimation("shadeDie");
		}
		if (timerRespawn > respawnTime && isVisible == false && dyingAnimation == false) {
			timerRespawn = 0;
			_deathTimeElapsed = 0;
		}
	}

	AnimatedSprite::updateBoss(elapsedTime, this->scaleFactor);
}

void Shade::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
}

void Shade::draw(Graphics &graphics) {
	if (isVisible == true) {
	AnimatedSprite::drawBoss(graphics, this->_x, this->_y, this->scaleFactor);
	if (isIdle == false) {
		this->_shadeBall.draw(graphics, this->_shadeBall.getX(), this->_shadeBall.getY());
		this->_oppositeShadeBall.draw(graphics, this->_oppositeShadeBall.getX(), this->_oppositeShadeBall.getY());
	}
		
	if (this->getCurrentHealth() < this->getMaxHealth()) {
		this->_HPBar.draw(graphics, this->_HPBar._x, this->_HPBar._y);
		this->_HPValue.draw(graphics, this->_HPValue._x, this->_HPValue._y);
		}
	}
}

void Shade::animationDone(std::string currentAnimation) {
	if (this->getCurrentHealth() <= 0) {
		if (this->canDropLoot == false) {
			this->removeEnemy = true;
			this->canDropLoot = true;
		}
	}
}

void Shade::dropLoot(Player &player) {

}

void Shade::setupAnimations() {
	this->addSpecialAnimation(3, 0, 16, "shadeIdle", 29, 39, Vector2(0, -60));
	this->addAnimation(4, 1, 73, "shadeLeft", 30, 28, Vector2(0, -60));
	this->addAnimation(4, 1, 118, "shadeRight", 30, 28, Vector2(0, -60));
	this->addAnimation(6, 1, 168, "shadeDie", 30, 28, Vector2(0, -60));
}

void Shade::playDeath() {
	this->playAnimation("shadeDie", true);
}

void Shade::playAttack() {

}

void Shade::touchPlayer(Player* player) {
	player->gainHealth(-1.0f);
}

void Shade::handleEnemyTileCollision(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0; // Reset all gravity, if we arent grounded we fall to the ground
				this->_y = others.at(i).getBottom() + 1; // No longer go through things, stops us
				//this->_HPBar._y = others.at(i).getTop() + 1;
				//this->_HPValue._y = others.at(i).getTop() + 1;
				if (this->_grounded) { // Only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
					//this->_dx = 0; //stop movement on x-axis
					this->_x -= this->_direction == RIGHT ? 0.5f : -0.5f; // If we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				}
				break;
			case sides::BOTTOM: // Hit the top (bottom) of tile push us back up ontop of tile
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() / 2;
				//this->_HPBar._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				//this->_HPValue._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true; // we are on ground since it pushed it back up
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() / 2;
				break;
			}
		}
	}
}

bool Shade::isRemoveable() {
	return removeEnemy;
}

void Shade::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
}

float Shade::enemyExpAmount() {
	return Shade::shadeExp;
}

std::string Shade::getName()
{
	return this->name;
}


// JellyFish class
JellyFish::JellyFish() {}

JellyFish::JellyFish(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "data\\enemy\\NpcWeed.png", 273, 0, 45, 24, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_maxHealth(10),
	_currentHealth(10)
{
	this->setupAnimations();
	this->playAnimation("JellyLeft");
	this->_fireX = this->_startingX;
	this->_fireY = this->_startingY;
	this->_HPBar._x = this->_startingX;
	this->_HPBar._y = this->_startingY;
	this->_HPBar = Sprite(graphics, "data\\enemy\\NpcCemet.png", 2, 157, 17, 7, this->_HPBar._x + 25, this->_HPBar._y - 15);
	this->_HPValue = Sprite(graphics, "data\\enemy\\NpcCemet.png", 3, 167, 17, 5, this->_HPBar._x + 1, this->_HPBar._y + 2);
}

JellyFish::~JellyFish()
{
	this->destroySprite();
	this->_HPBar.destroySprite();
	this->_HPValue.destroySprite();
}

void JellyFish::update(int elapsedTime, Player &player) {
	this->_y += this->_dy * elapsedTime; // Gravity move them by Y
	this->_HPBar.update();
	this->_HPValue.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		this->playAnimation(this->_direction == RIGHT ? "JellyRight" : "JellyLeft");
		// Move up or down
		if (this->_JellyAttack == false && this->_resettingPos == false && this->_delayAttack == false) {
			this->_y += this->_shouldMoveUp ? -.06 : .06;
			this->_HPBar._y += this->_shouldMoveUp ? -.06 : .06;
			this->_HPValue._y += this->_shouldMoveUp ? -.06 : .06;
			if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
				this->_shouldMoveUp = !this->_shouldMoveUp;
			}
		}
		if (this->_delayAttack == false && this->_resettingPos == false && this->_y - 15 < player.getY() && 
			//(int)this->_x + 20 == (int)player.getX()) This was the center. Now we use a range with the checkInRange.
		    checkInRange(this->_x - 15, this->_x + this->_sourceRect.w + 15, player.getX())) {
				this->_JellyAttack = true;
		}
		if (this->_resettingPos == true && this->_delayAttack == false) {
			this->_y -= 0.18;
			if (this->_y <= this->_startingY) {
				this->_resettingPos = false;
				this->_delayAttack = true;
			}
		}
		if (this->_delayAttack == true && this->_resettingPos == false) {
			this->_delayTimer += elapsedTime;
			if (this->_delayTimer >= 88) {
				this->_delayAttack = false;
				this->_delayTimer = 0;
			}
		}
		if (this->_JellyAttack == true) {
			this->_y += 1.25;
		}


		float hpNum = (float)this->getCurrentHealth() / this->getMaxHealth();
		this->_HPValue.setSourceRectW(std::floor(hpNum * 17));
		if (this->getBoundingBox().collidesWith(player.getBoundingBox())) {
			player.gainHealth(-35.5);
		}
	}
	else if (this->getCurrentHealth() <= 0) {
		_deathTimeElapsed += elapsedTime;
		if (isVisible == false)
			timerRespawn += elapsedTime;
		if (_deathTimeElapsed > _deathAnimationTime && isVisible == true && dyingAnimation == true) {
			this->dyingAnimation = false;
			this->_deathTimeElapsed = 0;
			this->isVisible = false;
		}
		else if (_deathTimeElapsed < _deathAnimationTime && isVisible == true) {
			this->dyingAnimation = true;
			this->playAnimation("JellyFishDie");
		}
		if (timerRespawn > respawnTime && isVisible == false && dyingAnimation == false) {
			timerRespawn = 0;
			this->isVisible = true;
			this->_currentHealth = this->_maxHealth;
			this->removeEnemy = false;
			this->canDropLoot = false;
			_deathTimeElapsed = 0;
		}
	}

	Enemy::update(elapsedTime, player);
}


void JellyFish::draw(Graphics &graphics) {
	if (isVisible == true) {
		Enemy::draw(graphics);
		if (this->getCurrentHealth() < this->getMaxHealth()) {
			this->_HPBar.draw(graphics, this->_HPBar._x, this->_HPBar._y);
			this->_HPValue.draw(graphics, this->_HPValue._x, this->_HPValue._y);
		}
	}
}

void JellyFish::animationDone(std::string currentAnimation) {
	if (this->getCurrentHealth() <= 0) {
		if (this->canDropLoot == false) {
			this->removeEnemy = true;
			this->canDropLoot = true;
		}
	}
}

void JellyFish::dropLoot(Player &player) {

}

void JellyFish::setupAnimations() {
	this->addSpecialAnimation(1, 228, 0, "JellyLeft", 45, 24, Vector2(0, 0));
	this->addSpecialAnimation(1, 228, 24, "JellyRight", 45, 24, Vector2(0, 0));
	this->addAnimation(7, 0, 270, "JellyFishDie", 16, 16, Vector2(0, 0));
}

void JellyFish::playDeath() {
	this->playAnimation("JellyFishDie", true);
}

void JellyFish::playAttack() {

}

void JellyFish::touchPlayer(Player* player) {
	player->gainHealth(-1.0f);
}

void JellyFish::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
}

void JellyFish::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
}

void JellyFish::handleEnemyTileCollision(std::vector<Rectangle>& others)
{
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0; // Reset all gravity, if we arent grounded we fall to the ground
				//this->_y = others.at(i).getBottom() + 1; // No longer go through things, stops us
				//this->_JellyAttack = false;
				//this->_resettingPos = true;
				//this->_HPBar._y = others.at(i).getTop() + 1;
				//this->_HPValue._y = others.at(i).getTop() + 1;
				// If (this->_grounded) { // Only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
				//	//this->_dx = 0; //stop movement on x-axis
				//	this->_x -= this->_direction == RIGHT ? 0.5f : -0.5f; // If we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				//}
				break;
			case sides::BOTTOM: // Hit the top (bottom) of tile push us back up ontop of tile
				//this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_y -= 10;
				//this->_HPBar._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				//this->_HPValue._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				//this->_grounded = true; // we are on ground since it pushed it back up
				this->_JellyAttack = false;
				this->_resettingPos = true;
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

bool JellyFish::isRemoveable() {
	return removeEnemy;
}

float JellyFish::enemyExpAmount() {
	return JellyFish::JellyFishExp;
}

std::string JellyFish::getName()
{
	return this->name;
}

// Ghost class
Ghost::Ghost() {}

Ghost::Ghost(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "data\\enemy\\NpcWeed.png", 0, 186, 22, 29, spawnPoint, 280),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_maxHealth(10),
	_currentHealth(10)
{
	this->setupAnimations();
	this->playAnimation("GhostLeft");
	this->_HPBar._x = this->_startingX;
	this->_HPBar._y = this->_startingY;
	this->_HPBar = Sprite(graphics, "data\\enemy\\NpcCemet.png", 2, 157, 17, 7, this->_HPBar._x + 25, this->_HPBar._y - 15);
	this->_HPValue = Sprite(graphics, "data\\enemy\\NpcCemet.png", 3, 167, 17, 5, this->_HPBar._x + 1, this->_HPBar._y + 2);
}

Ghost::~Ghost()
{
	this->destroySprite();
	this->_HPBar.destroySprite();
	this->_HPValue.destroySprite();
}

void Ghost::update(int elapsedTime, Player &player) {
	this->_y += this->_dy * elapsedTime; // Gravity move them by Y
	this->_HPBar.update();
	this->_HPValue.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		if (!this->_GhostAttack && !this->_delayAttack)
			this->playAnimation(this->_direction == RIGHT ? "GhostRight" : "GhostLeft");
		if (this->_direction == LEFT && checkInRange(this->_y - 45, this->_y + 45, player.getY())) {
			//Stay idle frame until player is in range
			if (!this->_delayAttack && player.getX() < this->_x - 225) {
				this->playAnimation("GhostLeft");
			}
			else {
				this->_delayAttack = true;
			}

			if (this->_delayAttack) {
				this->_sourceRect.w = 50;
				this->_sourceRect.h = 50;
				this->_x = this->_startingX - 15;
				this->_y = this->_startingY - 35;
				this->playAnimation("GhostAttackLeft");
				this->_GhostAttack = true;
			}
			if (this->_GhostAttack) {
				this->_delayTimer += elapsedTime;
				this->setBoundingBox(this->_x - 500, this->_y, 5500, 5500);
				if (this->_delayTimer >= 940 && this->getBoundingBox().collidesWith(player.getBoundingBox())) {
					player.gainHealth(-150);
					this->_delayTimer = 0;
					this->_delayAttack = false;
				}
			}
		}
		else if (this->_direction == RIGHT && checkInRange(this->_y - 45, this->_y + 45, player.getY())) {
			//Stay idle frame until player is in range
			if (!this->_delayAttack && this->_x + 225 > player.getX()) {
				this->playAnimation("GhostRight");
			}
			else {
				this->_delayAttack = true;
			}

			if (this->_delayAttack) {
				this->_sourceRect.w = 50;
				this->_sourceRect.h = 50;
				this->_x = this->_startingX - 15;
				this->_y = this->_startingY - 35;
				this->playAnimation("GhostAttackRight");
				this->_GhostAttack = true;
			}
			if (this->_GhostAttack) {
				this->_delayTimer += elapsedTime;
				this->setBoundingBox(this->_x, this->_y, 5500, 5500);
				if (this->_delayTimer >= 940 && this->getBoundingBox().collidesWith(player.getBoundingBox())) {
					player.gainHealth(-150);
					this->_delayTimer = 0;
					this->_delayAttack = false;
				}
			}
		}

		float hpNum = (float)this->getCurrentHealth() / this->getMaxHealth();
		this->_HPValue.setSourceRectW(std::floor(hpNum * 17));
	}
	else if (this->getCurrentHealth() <= 0) {
		_deathTimeElapsed += elapsedTime;
		if (isVisible == false)
			timerRespawn += elapsedTime;
		if (_deathTimeElapsed > _deathAnimationTime && isVisible == true && dyingAnimation == true) {
			this->dyingAnimation = false;
			this->_deathTimeElapsed = 0;
			this->isVisible = false;
		}
		else if (_deathTimeElapsed < _deathAnimationTime && isVisible == true) {
			this->dyingAnimation = true;
			this->playAnimation("GhostDie");
		}
		if (timerRespawn > respawnTime && isVisible == false && dyingAnimation == false) {
			timerRespawn = 0;
			this->isVisible = true;
			this->_currentHealth = this->_maxHealth;
			this->removeEnemy = false;
			this->canDropLoot = false;
			_deathTimeElapsed = 0;
		}
	}

	Enemy::update(elapsedTime, player);
}


void Ghost::draw(Graphics &graphics) {
	if (isVisible == true) {
		Enemy::draw(graphics);
		if (this->getCurrentHealth() < this->getMaxHealth()) {
			this->_HPBar.draw(graphics, this->_HPBar._x, this->_HPBar._y);
			this->_HPValue.draw(graphics, this->_HPValue._x, this->_HPValue._y);
		}
	}
}

void Ghost::animationDone(std::string currentAnimation) {
	if (this->getCurrentHealth() <= 0) {
		if (this->canDropLoot == false) {
			this->removeEnemy = true;
			this->canDropLoot = true;
		}
	}
	if (this->_GhostAttack) {
		this->isVisible = false;
	}
}

void Ghost::dropLoot(Player &player) {

}

void Ghost::setupAnimations() {
	this->addAnimation(1, 0, 185, "GhostLeft", 22, 29, Vector2(0, 0));
	this->addAnimation(1, 0, 217, "GhostRight", 22, 29, Vector2(0, 0));
	this->addAnimation(5, 0, 185, "GhostAttackLeft", 22, 29, Vector2(0, 0));
	this->addAnimation(5, 0, 217, "GhostAttackRight", 22, 29, Vector2(0, 0));
	this->addAnimation(7, 0, 270, "GhostDie", 16, 16, Vector2(0, 0));
}

void Ghost::playDeath() {
	this->playAnimation("GhostDie", true);
}

void Ghost::playAttack() {

}

void Ghost::touchPlayer(Player* player) {
	player->gainHealth(-1.0f);
}

void Ghost::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
}

void Ghost::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
}

void Ghost::handleEnemyTileCollision(std::vector<Rectangle>& others)
{
	if (this->_GhostAttack)
		return;
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_dy = 0; // Reset all gravity, if we arent grounded we fall to the ground
				this->_y = others.at(i).getBottom() + 1; // No longer go through things, stops us
				//this->_HPBar._y = others.at(i).getTop() + 1;
				//this->_HPValue._y = others.at(i).getTop() + 1;
				if (this->_grounded) { // Only time we hit a top tile is if we are on a slope, (we are grounded on a slope)
					//this->_dx = 0; //stop movement on x-axis
					this->_x -= this->_direction == RIGHT ? 0.5f : -0.5f; // If we face right, subtract .5 from x pos otherwise subtract -.5 (adds .5)
				}
				break;
			case sides::BOTTOM: // Hit the top (bottom) of tile push us back up ontop of tile
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				//this->_HPBar._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				//this->_HPValue._y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				this->_grounded = true; // we are on ground since it pushed it back up
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

bool Ghost::isRemoveable() {
	return removeEnemy;
}

float Ghost::enemyExpAmount() {
	return Ghost::GhostExp;
}

std::string Ghost::getName()
{
	return this->name;
}
