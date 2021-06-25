#include "Items.h"

#include "Graphics.h"
#include <iostream>

namespace itemConstants {
	Inventory::InventoryItem* hPot = new Inventory::InventoryItem;
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
	Items(graphics, "NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	hpGain(3)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
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
	Items(graphics, "NpcSym.png", 288, 1, 14, 15, spawnPoint, 140),
	maxHPgain(5)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	//this->addToInventory();
	this->setupAnimations();
	this->playAnimation("Blink");
}

void PermHP::update(int elapsedTime, Player & player) {
	this->playAnimation("Blink");
	Items::update(elapsedTime, player);
}

void PermHP::draw(Graphics & graphics) {
	Items::draw(graphics);
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
	Items(graphics, "NpcSym.png", 194, 4, 16, 16, spawnPoint, 140)
{
	graphics.loadImage("NpcSym.png"); //loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Blink");
}

void Key::draw(Graphics &graphics) {
	Items::draw(graphics);
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
	Items(graphics, "loot.png", 1, 1, 16, 16, spawnPoint, 140),
	amountGain(100)
{
	graphics.loadImage("loot.png"); //loads sprite sheet in
	this->setupAnimations();
	this->playAnimation("Rotate");
}

void GoldCoin::update(int elapsedTime, Player & player)
{
	this->playAnimation("Rotate");
	this->_y += 0.4;
	Items::update(elapsedTime, player);
}

void GoldCoin::draw(Graphics & graphics)
{
	//Items::draw(graphics);
	Items::drawLoot(graphics, this->_x, this->_y);
}

void GoldCoin::animationDone(std::string currentAnimation)
{
}

void GoldCoin::setupAnimations()
{
	this->addAnimation(4, 1, 1, "Rotate", 16, 16, Vector2(0, 0));
}
