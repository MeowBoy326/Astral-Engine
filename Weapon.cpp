#include "Weapon.h"
#include <iostream>
using namespace std;


namespace weapon_constants {
	const float BULLET_VELOCITY = 0.2f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;
}

Weapon::Weapon()
{
}
Weapon::~Weapon()
{
}

Weapon::Weapon(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(0),
	_currentHealth(0)
{}

void Weapon::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Weapon::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

//Gun class
Gun::Gun() {}

Gun::Gun(Graphics &graphics, Vector2 spawnPoint) :
	Weapon(graphics, "Bullet.png", 1, 6, 16, 16, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	this->setupAnimations();
	this->playAnimation("FlyLeft");
}

void Gun::update(int elapsedTime, Player &player) {
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "FlyRight" : "FlyLeft");

	//Move up or down
	this->_y += this->_shouldMoveUp ? -.06 : .06;
	if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}

	Weapon::update(elapsedTime, player);
}

void Gun::draw(Graphics &graphics) {
	Weapon::draw(graphics);
}

void Gun::animationDone(std::string currentAnimation) {

}

void Gun::setupAnimations() {
	this->addAnimation(3, 2, 32, "FlyLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(3, 2, 48, "FlyRight", 16, 16, Vector2(0, 0));
}

void Gun::touchPlayer(Player* player) {
	player->gainHealth(-1);
}

//Bullet class
/*
Bullet::Bullet()
{
}

Bullet::Bullet(Graphics &graphics, Player &player)
{
	this->_bullet = Sprite(graphics, "Bullet.png", 0, 6, 16, 16, 66, 70);
}

Bullet::Bullet(Graphics &graphics, Vector2 spawnPoint) :
	//Weapon(graphics, "Bullet.png", 1, 6, 16, 16, spawnPoint, 140),
	AnimatedSprite(graphics, "Bullet.png", 1, 6, 16, 16, spawnPoint.x, spawnPoint.y, 140),
	_shootDx(0),
	_shootDy(0)

	//_shouldMoveUp(false)
{
	graphics.loadImage("Bullet.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("Idle");
	this->currentVec;
	//this->setupAnimations();
	//this->playAnimation("FlyLeft");
}

void Bullet::setupAnimations() {
	//int x = player.getX;
	//int y = player.getY;
	//this->addAnimation(3, 0, 0, "RunLeft", 16, 16, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(1, 0, 6, "shootRight", 16, 16, Vector2(0, 0));
	//debug
	this->addAnimation(1, 0, 24, "Idle", 6, 6, Vector2(0, 0));
}

void Bullet::shootRight(Player &player, int id)
{
	//Graphics graphics;
	//graphics.loadImage("Bullet.png"); //loads sprite sheet in
	int sposX = player.getX();
	int sposY = player.getY();
	//Sprite(graphics, "Bullet.png", 0, 6, 16, 16, sposX, sposY);
	active = true;
	this->_shootDx = weapon_constants::BULLET_VELOCITY;
	this->_shootDy = weapon_constants::BULLET_VELOCITY;
	//Sprite(graphics, "TextBox.png", 81, 81, 11, 7, 38, 55);
	this->playBulletAnimation("shootRight");
}

void Bullet::draw(Graphics &graphics, Player &player) {
	if (active == true) {
		this->_x = player.getX();
		this->_y = player.getY();
		AnimatedSprite::drawBullet(graphics, this->_x, this->_y);
	}

	else {
		
		//debug cout << "X: " << this->_x << " Y: " << this->_y;
		AnimatedSprite::drawBullet(graphics, this->_x, this->_y);
	}

	active = false;

	//if (active == true) {
	//	this->_bullet.draw(graphics, this->_bullet.getX() + 17, this->_bullet.getY() + 17);
	//}
	//else {
	//	
	//}
	//AnimatedSprite::draw(graphics, this->_x, this->_y);
	//active = false;
}

void Bullet::update(float ElapsedTime, Player &player) {

	//Move by dx
	this->_x += this->_shootDx * ElapsedTime; //elapsedTime will move by a certain amount based on frame rate keeping thing moving smoothly
	//if (this->_x == globals::SCREEN_WIDTH / 2) {
	//	this->_x = globals::SCREEN_WIDTH / 2;
	//}
	this->_y += this->_shootDy * 0; //Gravity move them by Y
	//this->_x += 25 * 2;
										 //Move by dy
	//this->_y += this->_shootDy * elapsedTime; //Gravity move them by Y

	AnimatedSprite::updateBullet(ElapsedTime);
}

void Bullet::animationDone(std::string currentAnimation) {
	cout << "done bullet" << endl;
}


const float Bullet::getX() const {
	return this->_x;
}

const float Bullet::getY() const {
	return this->_y;
}
*/
