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
	this->_txtBox = new Sprite(graphics, "npcTextBox.png", 0, 0, 76, 24, 505, 499);
	//this->_npcBox = Sprite(graphics, "npcTextBox.png", 63, 149, 32, 32, 505, 499);
	//this->_chatSelection = Sprite(graphics, "npcTextBox.png", 0, 147, 46, 18, 15, 15);
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

void Npc::npcSelection(Graphics & graphics, int posX, int posY, int selection)
{
	this->drawQuestText(graphics, posX + 20, posY, "Chat");
	this->drawQuestText(graphics, posX + 20, posY + 50, "Quests");
	if (selection == 1) {
		this->_chatSelection->drawStatSelection(graphics, posX - 250, posY + 92);
	}
	else {
		this->_chatSelection->drawStatSelection(graphics, posX - 250, posY + 148);
	}
}

void Npc::setNpcIcon(Graphics & graphics, const std::string name, int posX, int posY)
{
	this->_chatSelection = new Sprite(graphics, "npcTextBox.png", 0, 147, 46, 18, 15, 15);
	//graphics.loadImage(name+".png");
	this->_npcIcon = new Sprite(graphics, name + ".png", 0, 0, 28, 28, posX, posY);
	this->_npcBox = new Sprite(graphics, "npcTextBox.png", 63, 149, 32, 32, posX, posY);
}

void Npc::drawNpcIcon(Graphics & graphics, const std::string name, int posX, int posY)
{
	this->_npcBox->draw(graphics, posX - 270, posY + 5);
	this->_npcIcon->draw(graphics, posX - 265, posY + 5);
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

int Npc::playNext(std::string name, Graphics & graphics, int posX, int posY, Player &player)
{
	//if (questState == false) {
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
			//this->checkQuest(graphics, name, posX, posY, player);
		}
	//}
	//else {
	//	this->checkQuest(graphics, name + ".xml", posX, posY, player);
	//}
}

void Npc::resetScripts()
{
	this->lines = 1;
	this->lineCounter = 1;
	this->lineChar = 'a';
	this->endOfChat = false;
}

int Npc::loadQuests(std::string name)
{
	std::cout << "loading quests for: " << name << std::endl;
	XMLDocument xml;
	XMLError result;
	std::string fileName = name + ".xml";
	result = xml.LoadFile(fileName.c_str());
	if (result != tinyxml2::XML_SUCCESS)
		std::cout << "Could not read the file!";
	XMLNode* root = xml.FirstChild();
	if (root == nullptr) {
		std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	XMLElement* element = root->FirstChildElement("Quest");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	int amount, type, numOfQuest;
	//element->QueryIntAttribute("bat", &killRequired);
	element->QueryIntAttribute("numOfQuest", &numOfQuest);
	const char* textPtr = nullptr, *descPtr = nullptr, *objPtr = nullptr, *npcPtr = nullptr;
	for (int counter = 0; counter < numOfQuest; ++counter) {
		//obtain data from XML
		element->QueryIntAttribute("amount", &amount);
		element->QueryIntAttribute("type", &type);
		textPtr = element->Attribute("questName");
		descPtr = element->Attribute("description");
		objPtr = element->Attribute("object");
		std::string text = textPtr, descText = descPtr, objText = objPtr;

		//push to vector
		if (this->questTable.empty()) {
			std::cout << "table empty...adding quest" << std::endl;
			this->questTable.push_back(std::make_tuple(text, type, objText, amount, descText, name));
		}
		else { //find_if algorithm with lambda function
			auto it = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
			if (it == questTable.end())
				this->questTable.push_back(std::make_tuple(text, type, objText, amount, descText, name));
		/*	for (auto iter = this->questTable.begin(); iter != this->questTable.end(); iter++) {
				if (std::get<1>(*iter) == text);
			}*/
		}

		if (element->NextSiblingElement("Quest") != nullptr)
			element = element->NextSiblingElement("Quest");
	}
}

void Npc::displayQuests(Graphics & graphics, std::string npcName, int posX, int posY, Player & player)
{
	//if (this->questTable.empty())
	//	std::cout << "Quest table is empty! Oh no!" << std::endl;
	//for (auto &t : this->questTable) {
	//	std::cout << "quest table<5> = " << std::get<5>(t);
	//}
	/*for (auto iter = this->questTable.begin(); iter != this->questTable.end(); iter++) {
		if (std::get<6>(*iter) == npcName);*/
	for (auto &t : this->questTable) {
		if (npcName == std::get<5>(t)) {
			//std::cout << "drawing quests to screen" << std::endl;
			this->drawNpcText(graphics, 100, 100, std::get<0>(t), posX, posY);
		}
	}
}



bool Npc::checkQuest(Graphics & graphics, std::string name, int posX, int posY, Player &player)
{
	XMLDocument xml;
	XMLError result;
	result = xml.LoadFile(name.c_str());
	if (result != tinyxml2::XML_SUCCESS)
		std::cout << "Could not read the file!";
	XMLNode* root = xml.FirstChild();
	if (root == nullptr) {
		std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	XMLElement* element = root->FirstChildElement("Quest");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	int killRequired, type, numOfQuest;
	//element->QueryIntAttribute("bat", &killRequired);
	element->QueryIntAttribute("amount", &killRequired);
	element->QueryIntAttribute("type", &type);
	element->QueryIntAttribute("numOfQuest", &numOfQuest);
	const char* textPtr = nullptr, *descPtr = nullptr, *objPtr = nullptr;
	//const char* descPtr = nullptr;
	//const char* objPtr = nullptr;
	textPtr = element->Attribute("questName");
	descPtr = element->Attribute("description");
	objPtr = element->Attribute("object");
	std::string text = textPtr, descText = descPtr, objText = objPtr;
	if (player.checkKillQuestComplete("bat", 4)) {
		this->drawQuestText(graphics, posX, posY, "Thank you for killing those bats!");
		if (questDone == false) {
			player.addLevel(1);
		}
		questDone = true;
	}
	else {
		this->drawQuestText(graphics, posX, posY, "Please kill 4 bats!");
		questState = true;
	}
		
	return false;
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
	//this->loadQuests(name);
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




