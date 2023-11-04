#include "../headers/Projectile.h"
#include "../headers/TextManager.h"

#include <iostream>

namespace projectileConstants {
	const float BULLET_VELOCITY = 0.6f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	double BULLET_DMG = 1.0;

	bool active = false;

	int count = 0;

	float enemyX = 0;
	float enemyY = 0;
}

Projectile::Projectile()
{
}

Projectile::Projectile(Graphics & graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate)
{
}

Projectile::~Projectile()
{
}

void Projectile::update(float elapsedTime, Player & player)
{
	AnimatedSprite::update(elapsedTime);
}

void Projectile::draw(Graphics & graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

SilverBullet::SilverBullet()
{
}

SilverBullet::SilverBullet(Graphics & graphics, Vector2 spawnPoint, Direction dir) :
	Projectile(graphics, "data\\graphics\\Bullet.png", 12, 2, 17, 9, spawnPoint, 140),
	_direction(dir),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y)
{
	graphics.loadImage("data\\graphics\\Bullet.png");
	this->setupAnimations();
}

SilverBullet::~SilverBullet()
{
	this->destroySprite();
}

void SilverBullet::update(float elapsedTime, Player & player)
{
	if (_direction == LEFT) {
		this->playAnimation("ShootLeft");
		this->_x -= 0.40f * elapsedTime;
	}
	else if (_direction == RIGHT) {
		this->playAnimation("ShootRight");
		this->_x += 0.40f * elapsedTime;
	}
	else if (_direction == UP) {
		this->playAnimation("ShootUp");
		this->_y -= 0.40f * elapsedTime;
	}
	else if (_direction == DOWN) {
		this->playAnimation("ShootDown");
		this->_y += 0.40f * elapsedTime;
	}

	Projectile::update(elapsedTime, player);
}

void SilverBullet::draw(Graphics & graphics)
{
	Projectile::draw(graphics);
}

void SilverBullet::animationDone(std::string currentAnimation)
{
}

void SilverBullet::setupAnimations()
{
	this->addSpecialAnimation(1, 51, 2, "ShootLeft", 17, 9, Vector2(0, 0));
	this->addSpecialAnimation(1, 78, 2, "ShootRight", 17, 9, Vector2(0, 0));
	this->addSpecialAnimation(1, 62, 2, "ShootUp", 17, 13, Vector2(0, 0));
	this->addSpecialAnimation(1, 95, 2, "ShootDown", 17, 13, Vector2(0, 0));
}
