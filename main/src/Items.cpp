#include "../headers/Items.h"
#include "../headers/Graphics.h"

#include <iostream>

namespace itemConstants {
}

Items::Items()
{
}

Items::Items(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate) {}

Items::Items(Graphics & graphics)
{
}

void Items::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Items::draw(Graphics &graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Items::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}



void Items::addToInventory(int type) {
}

Items::~Items()
{
}

/*
*	Health Potion Item
*/

HealthPotion::HealthPotion()
{
}

HealthPotion::HealthPotion(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	hpGain(3)
{
	graphics.loadImage("data\\maps\\NpcSym.png"); // Loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

HealthPotion::~HealthPotion()
{
	this->destroySprite();
}

void HealthPotion::addToInventory() {
}

void HealthPotion::update(int elapsedTime, Player &player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void HealthPotion::draw(Graphics &graphics) {
	Items::draw(graphics);
}

void HealthPotion::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void HealthPotion::animationDone(std::string currentAnimation)
{
}

void HealthPotion::setupAnimations()
{
	this->addAnimation(2, 2, 84, "Blink", 16, 16, Vector2(0, 0));
}

/*
*	Permenant HP Item
*/
PermHP::PermHP() {};

PermHP::PermHP(Graphics &graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	maxHPgain(5)
{
	graphics.loadImage("data\\maps\\NpcSym.png"); // Loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

PermHP::~PermHP()
{
	this->destroySprite();
}

void PermHP::update(int elapsedTime, Player & player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void PermHP::draw(Graphics & graphics) {
	Items::draw(graphics);
}

void PermHP::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void PermHP::animationDone(std::string currentAnimation) {
}

void PermHP::setupAnimations() {
	this->addAnimation(2, 2, 101, "Blink", 16, 16, Vector2(0, 0));
}

/*
*	Key Item
*/

Key::Key() {};

Key::Key(Graphics &graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\NpcSym.png", 194, 4, 16, 16, spawnPoint, 140)
{
	graphics.loadImage("data\\maps\\NpcSym.png"); // Loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Blink");
}

Key::~Key()
{
	this->destroySprite();
}

void Key::draw(Graphics &graphics) {
	Items::draw(graphics);
}

void Key::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void Key::update(int elapsedTime, Player & player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void Key::animationDone(std::string currentAnimation) {

}

void Key::setupAnimations() {
	this->addSpecialAnimation(2, 194, 1, "Blink", 15, 14, Vector2(0, 0));
	//special animation for large X-value frames
}

/*
*  Gold Coin
*/

GoldCoin::GoldCoin()
{
}

GoldCoin::GoldCoin(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\loot.png", 1, 1, 16, 16, spawnPoint, 140),
	amountGain(100)
{
	graphics.loadImage("data\\maps\\loot.png"); // Loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Rotate");
}

GoldCoin::~GoldCoin()
{
	this->destroySprite();
}

void GoldCoin::update(int elapsedTime, Player & player)
{
	this->playAnimation("Rotate");
	this->_y += 0.12f;
	Items::update(elapsedTime, player);
}

void GoldCoin::draw(Graphics & graphics)
{
	Items::drawLoot(graphics, this->_x, this->_y);
}

void GoldCoin::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void GoldCoin::animationDone(std::string currentAnimation)
{
}

void GoldCoin::setupAnimations()
{
	this->addAnimation(4, 1, 1, "Rotate", 16, 16, Vector2(0, 0));
}

/*
*  Red Coin
*/

RedCoin::RedCoin()
{
}

RedCoin::RedCoin(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\loot.png", 1, 23, 16, 16, spawnPoint, 140),
	amountGain(1000)
{
	graphics.loadImage("data\\maps\\loot.png"); // Loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Rotate");
}

RedCoin::~RedCoin()
{
	this->destroySprite();
}

void RedCoin::update(int elapsedTime, Player & player)
{
	this->playAnimation("Rotate");
	this->_y += 0.12f;
	Items::update(elapsedTime, player);
}

void RedCoin::draw(Graphics & graphics)
{
	Items::drawLoot(graphics, this->_x, this->_y);
}

void RedCoin::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void RedCoin::animationDone(std::string currentAnimation)
{
}

void RedCoin::setupAnimations()
{
	this->addAnimation(4, 1, 23, "Rotate", 16, 16, Vector2(0, 0));
}

/*
*  Bronze Coin
*/

BronzeCoin::BronzeCoin()
{
}

BronzeCoin::BronzeCoin(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\loot.png", 96, 23, 16, 16, spawnPoint, 140),
	amountGain(1)
{
	graphics.loadImage("data\\maps\\loot.png"); // Loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Rotate");
}

BronzeCoin::~BronzeCoin()
{
	this->destroySprite();
}

void BronzeCoin::update(int elapsedTime, Player & player)
{
	this->playAnimation("Rotate");
	this->_y += 0.12f;
	Items::update(elapsedTime, player);
}

void BronzeCoin::draw(Graphics & graphics)
{
	Items::drawLoot(graphics, this->_x, this->_y);
}

void BronzeCoin::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void BronzeCoin::animationDone(std::string currentAnimation)
{
}

void BronzeCoin::setupAnimations()
{
	this->addSpecialAnimation(4, 96, 23, "Rotate", 16, 16, Vector2(0, 0));
}

SilverGem::SilverGem()
{
}

SilverGem::SilverGem(Graphics & graphics, Vector2 spawnPoint) :
	Items(graphics, "data\\maps\\loot.png", 72, 50, 16, 16, spawnPoint, 140),
	amountGain(1)
{
	graphics.loadImage("data\\maps\\loot.png"); // Loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Rotate");
}

SilverGem::~SilverGem()
{
	this->destroySprite();
}

void SilverGem::update(int elapsedTime, Player & player)
{
	this->_y += 0.12f;
	this->playAnimation("Rotate");
	Items::update(elapsedTime, player);
}

void SilverGem::draw(Graphics & graphics)
{
	Items::drawLoot(graphics, this->_x, this->_y);
}

void SilverGem::drawDrops(Graphics & graphics, float x, float y)
{
	AnimatedSprite::drawLoot(graphics, x, y);
}

void SilverGem::animationDone(std::string currentAnimation)
{
}

void SilverGem::setupAnimations()
{
	this->addSpecialAnimation(3, 70, 50, "Rotate", 16, 16, Vector2(0, 0));
}
