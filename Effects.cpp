#include "Effects.h"
#include "Effects.h"

Effects::Effects()
{
}

Effects::~Effects()
{
}

Effects::Effects(Graphics & graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate)
{
}

void Effects::update(int elapsedTime, Player & player)
{
	AnimatedSprite::update(elapsedTime);
}

void Effects::draw(Graphics & graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Effects::animationDone(std::string currentAnimation)
{
}

void Effects::setupAnimations()
{
}


//Arena Effect

ArenaEffect::ArenaEffect()
{
}

ArenaEffect::~ArenaEffect()
{
}

ArenaEffect::ArenaEffect(Graphics &graphics, Vector2 spawnPoint) :
	Effects(graphics, "data\\graphics\\arenaBlue.png", 0, 0, 16, 16, spawnPoint, 60),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y)
{
	this->setupAnimations();
	this->playAnimation("ArenaRight");
	this->_x = _startingX;
	this->_y = _startingY;
	this->_sourceRect.w = 16;
	this->_sourceRect.h = 280;
}

void ArenaEffect::update(int elapsedTime, Player & player)
{

	this->playAnimation("ArenaRight");
	AnimatedSprite::update(elapsedTime);
}

void ArenaEffect::draw(Graphics & graphics)
{
	if (isVisible == true) {
		Effects::draw(graphics);
	}
}

void ArenaEffect::animationDone(std::string currentAnimation)
{
}

void ArenaEffect::setupAnimations()
{
	this->addAnimation(50, 0, 0, "ArenaRight", 16, 16, Vector2(0, 0));
	//this->addAnimation(3, 2, 32, "ArenaLeft", 16, 16, Vector2(0, 0));
	//this->addAnimation(3, 2, 32, "ArenaUp", 16, 16, Vector2(0, 0));
	//this->addAnimation(3, 2, 32, "ArenaDown", 16, 16, Vector2(0, 0));
}
