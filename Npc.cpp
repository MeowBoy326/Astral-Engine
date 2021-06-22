#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif
#include "Npc.h"
#include "Input.h"
#include "tinyxml2.h";

using namespace tinyxml2;

Npc::Npc() {}

Npc::Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(10),
	_currentHealth(10)
{
	this->_txtBox = Sprite(graphics, "npcTextBox.png", 0, 0, 76, 24, 505, 499);
	this->_npcBox = Sprite(graphics, "npcTextBox.png", 63, 149, 32, 32, 505, 499);
	graphics.loadImage("npcTextBox.png"); //loads sprite sheet in
}

void Npc::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Npc::draw(Graphics &graphics) {
	AnimatedSprite::drawNpc(graphics, this->_x, this->_y);
}

void Npc::animationDone(std::string currentAnimation){}

void Npc::setupAnimations() {}

void Npc::setNpcIcon(Graphics & graphics, const std::string name, int posX, int posY)
{
	graphics.loadImage(name+".png");
	this->_npcIcon = Sprite(graphics, name + ".png", 0, 0, 28, 28, posX, posY);
	this->_npcBox = Sprite(graphics, "npcTextBox.png", 63, 149, 32, 32, posX, posY);
}

void Npc::drawNpcIcon(Graphics & graphics, const std::string name, int posX, int posY)
{
	this->_npcBox.draw(graphics, posX - 270, posY + 5);
	this->_npcIcon.draw(graphics, posX - 265, posY + 5);
	this->drawNpcName(graphics, posX, posY, name);
}

int Npc::playScript(std::string name, Graphics & graphics, int posX, int posY)
{
	this->endOfChat = false;
	XMLDocument xml;
	XMLError result;
	name += ".xml";
	result = xml.LoadFile(name.c_str());
	if (result != tinyxml2::XML_SUCCESS)
		std::cout << "Could not read the file!";
	XMLNode* root = xml.FirstChild();
	if (root == nullptr) {
		std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	XMLElement* element = root->FirstChildElement("Talk");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	//get number of lines here
	result = element->QueryIntAttribute("lines", &this->lines);
	const char* textPtr = nullptr;
	textPtr = element->Attribute((char*)&this->lineChar);
	std::string text = textPtr;
	this->drawNpcText(graphics, 100, 100, text, posX, posY);
	XMLCheckResult(result);
}

int Npc::playNext(std::string name, Graphics & graphics, int posX, int posY)
{
	XMLDocument xml;
	name += ".xml";
	xml.LoadFile(name.c_str());
	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	XMLElement* element = root->FirstChildElement("Talk");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	result = element->QueryIntAttribute("lines", &this->lines);
	const char* textPtr = nullptr;
	this->lineChar++;
	this->lineCounter++;
	if (this->lineCounter <= this->lines) {
		textPtr = element->Attribute((char*)&this->lineChar);
		std::string text;
		text = textPtr;
		this->drawNpcText(graphics, 100, 100, text, posX, posY);
		XMLCheckResult(result);
	}
	else {
		this->lineCounter = 1;
		this->lineChar = 'a';
		this->endOfChat = true;
	}
}

void Npc::resetScripts()
{
	this->lines = 1;
	this->lineCounter = 1;
	this->lineChar = 'a';
	this->endOfChat = false;
}

//Clock class
Clock::Clock() {}

Clock::Clock(Graphics &graphics, Vector2 spawnPoint, std::string name) :
	Npc(graphics, "female.png", 2, 33, 30, 30, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	_npcName = name;
	this->setupAnimations();
	this->playAnimation("clock");
}

void Clock::update(int elapsedTime, Player &player) {
	/*
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "clock" : "clockD");


	//Move up or down
	this->_y += this->_shouldMoveUp ? -.06 : .06;
	if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}*/

	Npc::update(elapsedTime, player);
}

void Clock::draw(Graphics &graphics) {
	Npc::draw(graphics);
}

void Clock::animationDone(std::string currentAnimation) {

}

void Clock::setupAnimations() {
	this->addAnimation(1, 1, 1, "clock", 33, 31, Vector2(0, 0));
	this->addAnimation(1, 2, 33, "clockD", 33, 31, Vector2(0, 0));
}

void Clock::bulletHit() {
	this->_currentHealth += -1;
	std::cout << "hit! HP = " << this->_currentHealth << std::endl;
}

void Clock::touchPlayer(Player* player) {
	player->gainHealth(-1);
}




