#include "Items.h"

#include "Graphics.h"
#include <iostream>

using namespace std;

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
	if (type == 0) {
		itemConstants::hPot->type = HEALTH_POT;
		this->addItem(itemConstants::hPot);
		cout << "Items::addTo success!" << endl;
	}

	if (itemConstants::hPot->type != NULL) {
		cout << "null..." << endl;
	}
}


Items::~Items()
{
}

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
	InventoryItem* hPot = new InventoryItem;
	hPot->type = HEALTH_POT;
	hPot->imagePath = "";
	//hPot.type = HEALTH_POT;
	cout << "add Item: hPot" << endl;
	//this->addItem(hPot);
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
