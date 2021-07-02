#include "Enemy.h"
#include <iostream>
#include "SDL_timer.h"
#include "TextManager.h"
#include "Player.h"

TextManager txt;
//Base enemy class
Enemy::Enemy() {}

Enemy::~Enemy()
{
}

namespace enemyConstants {
	//bool removeEnemy = false;
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

//Bat class
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_maxHealth(5),
	_currentHealth(5)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
	this->_fireX = this->_startingX;
	this->_fireY = this->_startingY;
	this->_fireBall = Sprite(graphics, "NpcCemet.png", 220, 33, 13, 10, this->_fireX, (this->_fireY + 10));
}

Bat::~Bat()
{
	std::cout << "BAT destructor" << std::endl;
	this->destroySprite();
	this->_fireBall.destroySprite();
}

void Bat::update(int elapsedTime, Player &player) {
	this->_fireBall.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");
		//Move up or down
		this->_y += this->_shouldMoveUp ? -.06 : .06;
		if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
			this->_shouldMoveUp = !this->_shouldMoveUp;
		}
		if (this->getBoundingBox().collidesWith(player.getBoundingBox())) {
			player.gainHealth(-4.5);
		}
		if (_fireBall.getBoundingBox().collidesWith(player.getBoundingBox())) {
			this->_fireBall.setX(this->_x);
			this->_fireBall.setY(this->_y);
			player.gainHealth(-8.5);
		}
		else if (_fireBall.getY() > this->_y + 160) {
			this->_fireBall.setX(this->_x);
			this->_fireBall.setY(this->_y);
		}
		else {
			this->_fireBall.addX(player.getX() > this->_x ? .2 : -.2);
			this->_fireBall.addY(player.getY() > this->_y ? .2 : -.2);
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
		this->_fireBall.draw(graphics, this->_fireBall.getX(), this->_fireBall.getY());
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
		player->gainHealth(-1.0f);
}

void Bat::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
	//std::cout << "BAT hit! HP = " << this->_currentHealth << std::endl;
}

void Bat::setRemoveable() {
	if (removeEnemy == true) {
		removeEnemy = false;
	}
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
	std::cout << "Shade destructor" << std::endl;
	this->destroySprite();
	this->_shadeBall.destroySprite();
}

Shade::Shade(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "shade.png", 27, 7, 30, 28, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false),
	_currentHealth(100),
	_maxHealth(100)
{
	this->setupAnimations();
	this->playAnimation("shadeRight");

	this->_shadeBall = Sprite(graphics, "shadeAttack.png", 0, 0, 22, 19, this->_startingX, (this->_startingY + 60));
	graphics.loadImage("shadeAttack.png");
}

void Shade::update(int elapsedTime, Player &player) {
	this->_shadeBall.update();
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0 && this->isVisible == true) {
		this->playAnimation(this->_direction == RIGHT ? "shadeRight" : "shadeLeft");

		if (this->_direction == RIGHT) {
			this->_x += 0.2;
		}
		else {
			this->_x -= 0.2;
		}
		if (this->getBoundingBox().collidesWith(player.getBoundingBox())) {
			player.gainHealth(-12.64);
		}
		if (_shadeBall.getBoundingBox().collidesWith(player.getBoundingBox())) {
			this->_shadeBall.setX(this->_x);
			this->_shadeBall.setY(this->_y + 60);
			player.gainHealth(-26.69);
		}
		else if (_shadeBall.getX() > this->_x + 200) {
			this->_shadeBall.setX(this->_x);
			this->_shadeBall.setY(this->_y + 60);
		}
		else {
			this->_shadeBall.addX(player.getX() > this->_x ? .8 : -.8);
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
			this->isVisible = true;
			this->_currentHealth = this->_maxHealth;
			this->removeEnemy = false;
			this->canDropLoot = false;
			_deathTimeElapsed = 0;
		}
	}

	AnimatedSprite::updateBoss(elapsedTime, player.getY());
}

void Shade::bulletHit(float dmg) {
	if (this->_currentHealth >= 1)
		this->_currentHealth -= dmg;
}

void Shade::draw(Graphics &graphics) {
	if (isVisible == true) {
	AnimatedSprite::drawBoss(graphics, this->_x, this->_y);
	this->_shadeBall.draw(graphics, this->_shadeBall.getX(), this->_shadeBall.getY());
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
	this->addAnimation(4, 1, 9, "shadeIdle", 30, 28, Vector2(0, 0));
	this->addAnimation(4, 1, 73, "shadeLeft", 30, 28, Vector2(0, 0));
	this->addAnimation(4, 1, 118, "shadeRight", 30, 28, Vector2(0, 0));
	this->addAnimation(6, 1, 168, "shadeDie", 30, 28, Vector2(0, 0));
}

void Shade::playDeath() {
	this->playAnimation("shadeDie", true);
}

void Shade::playAttack() {

}

void Shade::touchPlayer(Player* player) {
	player->gainHealth(-1.0f);
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
