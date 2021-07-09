#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif
#include "Npc.h"
#include "Input.h"
#include "tinyxml2.h";
#include <filesystem>

using namespace tinyxml2;

Npc::Npc() {}

Npc::Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(10),
	_currentHealth(10)
{
	this->_txtBox = new Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 0, 76, 24, 505, 499);
	//this->_npcBox = Sprite(graphics, "data\\npc\\npcTextBox.png", 63, 149, 32, 32, 505, 499);
	//this->_chatSelection = Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 147, 46, 18, 15, 15);
	graphics.loadImage("data\\npc\\npcTextBox.png"); //loads sprite sheet in
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
	this->_chatSelection = new Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 147, 46, 18, 15, 15);
	this->_questSelection = new Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 147, 46, 18, 15, 15);
	//graphics.loadImage(name+".png");
	this->_npcIcon = new Sprite(graphics, "data\\npc\\" + name + ".png", 0, 0, 28, 28, posX, posY);
	this->_npcBox = new Sprite(graphics, "data\\npc\\npcTextBox.png", 63, 149, 32, 32, posX, posY);
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
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
	cwd.append(name + ".xml");
	result = xml.LoadFile(cwd.string().c_str());
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
	return 0;
}

int Npc::playNext(std::string name, Graphics & graphics, int posX, int posY, Player &player)
{
	//if (questState == false) {
		XMLDocument xml;
		std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
		cwd.append(name + ".xml");
		xml.LoadFile(cwd.string().c_str());
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
			XMLCheckResult(result);
		}
		XMLCheckResult(result);
		return 0;
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

	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
	cwd.append(name + ".xml");
	result = xml.LoadFile(cwd.string().c_str());

	if (result != tinyxml2::XML_SUCCESS)
		std::cout << "Could not read the file!";
	XMLNode* root = xml.FirstChild();
	if (root == nullptr) {
		std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
		return XML_ERROR_FILE_READ_ERROR;
	}
	XMLElement* element = root->FirstChildElement("QuestTable");
	XMLElement* ptrElement = element->FirstChildElement("Quest");
	if (ptrElement == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	int amount, type, numOfQuest;
	ptrElement->QueryIntAttribute("numOfQuest", &numOfQuest);
	const char* textPtr = nullptr, *descPtr = nullptr, *objPtr = nullptr, *npcPtr = nullptr;
	for (int counter = 0; counter < numOfQuest; ++counter) {
		//obtain data from XML
		ptrElement->QueryIntAttribute("amount", &amount);
		ptrElement->QueryIntAttribute("type", &type);
		textPtr = ptrElement->Attribute("questName");
		descPtr = ptrElement->Attribute("description");
		objPtr = ptrElement->Attribute("object");
		std::string text = textPtr, descText = descPtr, objText = objPtr;
		bool completed = true;
		auto logIt = std::find_if(questLog.begin(), questLog.end(), [&completed](const auto& t) {return std::get<5>(t) == completed; });
		//push to vector
		if (this->questTable.empty() && logIt == questLog.end()) { //check if its not in the accepted quests already
			this->questTable.push_back(std::make_tuple(text, type, objText, amount, descText, name));
		}
		else { //find_if algorithm with lambda function
			auto it = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
			if (it == questTable.end() && logIt == questLog.end()) {
				this->questTable.push_back(std::make_tuple(text, type, objText, amount, descText, name));
			}
		}
		if (ptrElement->NextSiblingElement("Quest") != nullptr)
			ptrElement = ptrElement->NextSiblingElement("Quest");
	}
	for (auto &t : this->questLog) {
		if (std::get<5>(t) == true && std::get<4>(t) == false) {
			std::get<4>(t) = true;
			//once rewarded and player opens menu again, mark as completed.
		}
	}
	XMLCheckResult(result);
	return 0;
}

void Npc::displayQuests(Graphics & graphics, std::string npcName, int posX, int posY, Player & player)
{
	int dy = 0;
	if (!this->questTable.empty()) {
		for (auto &t : this->questTable) {
			if (npcName == std::get<5>(t)) {
				this->drawQuestText(graphics, posX + 10, posY + dy, std::get<0>(t));
				dy += 40;
			}
		}
	}
	if (this->questTable.empty()) {
		this->drawQuestText(graphics, posX + 10, posY + dy, "I have no quests for you at this time.");
		this->noQuest = true;
	}
}

void Npc::questSelection(Graphics & graphics, int posX, int posY, int selection)
{
	if (selection == 1) {
		this->_questSelection->drawQuestSelection(graphics, posX - 262, posY + 92);
	}
	else {
		this->_questSelection->drawQuestSelection(graphics, posX - 262, posY + 132);
	}
}

void Npc::acceptQuest(Graphics & graphics, std::string npcName, int posX, int posY, Player & player, int selection)
{
	this->questState = true;
	if (questDone) {
		this->giveRewards(graphics, this->storedQuestName, posX, posY, player, selection);
		return;
	}
	bool isCompleted;
	if (questLog.empty() && questTable.empty())
		return;
	if (selection >= this->questTable.size()) { //If player tries to select and empty quest index
		std::cout << "Cannot find specified index..." << std::endl;
		return;
	}	
	std::string text = std::get<0>(this->questTable[selection]);
	this->storedQuestName = text;
	if (!questLog.empty() && !questTable.empty()) {
		isCompleted = this->checkQuest(graphics, text, posX, posY, player);
		if (isCompleted == true) {
			this->questDone = true;
			this->giveRewards(graphics, this->storedQuestName, posX, posY, player, selection);
			return;
		}
	}
	std::string completeMsg;
	auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
	auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
	auto distance = std::distance(this->questLog.begin(), it);
	 if (!questTable.empty()) {
		this->drawNpcText(graphics, 100, 100, std::get<4>(this->questTable[selection]), posX, posY);
		if (this->questLog.empty()) {
			this->questLog.push_back(std::make_tuple(std::get<0>(this->questTable[selection]), std::get<1>(this->questTable[selection]),
				std::get<2>(this->questTable[selection]), std::get<3>(this->questTable[selection]), false, false));
		}
		else { //find_if algorithm with lambda function
			auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
			if (it == questLog.end()) {
				this->questLog.push_back(std::make_tuple(std::get<0>(this->questTable[selection]), std::get<1>(this->questTable[selection]),
					std::get<2>(this->questTable[selection]), std::get<3>(this->questTable[selection]), false, false));
			}
		}
	}
}

void Npc::giveRewards(Graphics & graphics, std::string npcName, int posX, int posY, Player & player, int selection)
{
	std::string completeMsg;
	auto it = std::find_if(questLog.begin(), questLog.end(), [&npcName](const auto& t) {return std::get<0>(t) == npcName; });
	auto distance = std::distance(this->questLog.begin(), it);
	if (questDone) {
		completeMsg = "Thank you! Here is your reward.$e"; //TODO: push rewards into vector to distibute any reward for any quest.
		this->drawNpcText(graphics, 100, 100, completeMsg, posX, posY);
		if (std::get<5>(this->questLog[distance]) != true)
			player.addLevel(1);
		std::get<5>(this->questLog[distance]) = true;
	}
}


bool Npc::checkQuest(Graphics & graphics, std::string name, int posX, int posY, Player &player)
{
	if (!this->questLog.empty()) {
		std::string text = name;
		std::string completeMsg;
		auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
		auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
		auto distanceTable = std::distance(this->questTable.begin(), tableIt);
		auto distance = std::distance(this->questLog.begin(), it);
		if (it != questLog.end() && std::get<4>(this->questLog[distance]) == false) {
			if (std::get<1>(this->questLog[distance]) == 1) { //kill quests
				if (player.checkKillQuestComplete(std::get<2>(this->questLog[distance]), std::get<3>(this->questLog[distance]))) {
					std::cout << "Quest is completed! Remove from available quests!" << std::endl;
					if (tableIt != questTable.end()) {
						this->questTable.erase(this->questTable.begin() + distanceTable);
					}
					return true;
				}
			}
		}
	}
	return false;
}

//Clock class
Clock::Clock() {}

Clock::Clock(Graphics &graphics, Vector2 spawnPoint, std::string name) :
	Npc(graphics, "data\\npc\\female.png", 2, 33, 30, 30, spawnPoint, 140),
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




