#include "Enemy.h"
#include <iostream>
#include "SDL_timer.h"
#include "TextManager.h"

using namespace std;

TextManager txt;
//Base enemy class
Enemy::Enemy() {}

Enemy::Enemy(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(3),
	_currentHealth(3)
{}

void Enemy::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Enemy::playDeath() {
	if (deathCreated == false) {

	}
}

void Enemy::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

//Bat class
Bat::Bat() {}

Bat::Bat(Graphics &graphics, Vector2 spawnPoint) :
	Enemy(graphics, "NpcCemet.png", 32, 32, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Bat::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	if (this->getCurrentHealth() > 0) {
		this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");
		//Move up or down
		this->_y += this->_shouldMoveUp ? -.06 : .06;
		if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
			this->_shouldMoveUp = !this->_shouldMoveUp;
		}
	}
	
	else if (this->getCurrentHealth() <= 0) {
		this->playAnimation("BatDie", true);
		this->_currentHealth--;
	}

	Enemy::update(elapsedTime, player);
}

void Bat::draw(Graphics &graphics) {
	Enemy::draw(graphics);
}

void Bat::animationDone(std::string currentAnimation) {

}

void Bat::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
	this->addAnimation(5, 6, 32, "BatDie", 19, 16, Vector2(0, 0));
}

void Bat::bulletHit() {
	this->_currentHealth += -1; 

	cout << "hit! HP = " << this->_currentHealth << endl;
}

void Bat::playDeath() {
	this->playAnimation("BatDie", true);
}

void Bat::touchPlayer(Player* player) {

	if (hasHit == false) {
		player->gainHealth(-0.01f);
	}

	else if (hasHit == true) {
		cout << "Cannot hit again!" << endl;
	}


}

void Bat::setTimer() {
	hasHit = true;
	//SDL_TimerID timerID = SDL_AddTimer(2 * 1000, Bat::setHit, "yes");
	SDL_Delay(2 * 1000);
	hasHit = false;
}

Uint32 Bat::setHit(Uint32 interval, void* param) {
	return 0;
}

bool Bat::canHit() {
	if (hasHit == true) {
		return false;
	}
	else if (hasHit == false) {
		return true;
	}
}
/*
void Bat::lockDamage(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->touchPlayer(this);
		others.at(i)->lockDamage();
	}
}*/