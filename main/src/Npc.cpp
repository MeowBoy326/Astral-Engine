#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif

#include "../headers/Npc.h"
#include "../headers/Input.h"
#include "../headers/tinyxml2.h"
#include "../headers/AESCipher.h"

#include <filesystem>
#include <sstream>
#include <iostream>

#include <LuaBridge/detail/LuaHelpers.h>
#include <lua.h>
#include <LuaBridge/LuaBridge.h>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

using namespace tinyxml2;

Npc::Npc() {}

Npc::Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(10),
	_currentHealth(10)
{
	this->_txtBox = new Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 0, 76, 24, 505, 499);
	// This->_npcBox = Sprite(graphics, "data\\npc\\npcTextBox.png", 63, 149, 32, 32, 505, 499);
	// This->_chatSelection = Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 147, 46, 18, 15, 15);
	graphics.loadImage("data\\npc\\npcTextBox.png"); // Loads sprite sheet in
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
	// Graphics.loadImage(name+".png");
	this->_npcIcon = new Sprite(graphics, "data\\npc\\" + name + ".png", 0, 0, 28, 28, posX, posY);
	this->_npcBox = new Sprite(graphics, "data\\npc\\npcTextBox.png", 63, 149, 32, 32, posX, posY);
}

void Npc::drawNpcIcon(Graphics & graphics, const std::string name, int posX, int posY)
{
	this->_npcBox->draw(graphics, posX - 270, posY + 5);
	this->_npcIcon->draw(graphics, posX - 265, posY + 5);
	this->drawNpcName(graphics, posX, posY, name);
}

int Npc::playScript(int npcID, Graphics & graphics, int posX, int posY)
{
	this->endOfChat = false;
	//XMLDocument xml;
	//XMLError result;
	//std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
	//cwd.append(name + ".xml");
	//std::filesystem::path nCwd = std::filesystem::current_path() / "data" / "npc";
	//nCwd.append("temp.xml");
	//AESCipher cipher;
	//cipher.AESDecrypt(cwd.string(), nCwd.string());
	//result = xml.LoadFile(nCwd.string().c_str());
	//if (result != tinyxml2::XML_SUCCESS)
	//	std::cout << "Could not read the file!";
	//XMLNode* root = xml.FirstChild();
	//if (root == nullptr) {
	//	std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
	//	return XML_ERROR_FILE_READ_ERROR;
	//}
	//XMLElement* element = root->FirstChildElement("Talk");
	//if (element == nullptr)
	//	return XML_ERROR_PARSING_ELEMENT;
	//// Get number of lines here
	//result = element->QueryIntAttribute("lines", &this->lines);
	//const char* textPtr = nullptr;
	//textPtr = element->Attribute((char*)&this->lineChar);
	//std::string text = textPtr;
	////this->drawNpcText(graphics, 100, 100, text, posX, posY);
	//this->drawNpcDialogue(graphics, 100, 100, text, posX, posY);
	//cipher.AESEncrypt(nCwd.string(), cwd.string());
	//XMLCheckResult(result);

	//if (this->npcDialogueTable)


	lua_State* L = luaL_newstate(); // create a new Lua state
	luaL_openlibs(L); // load Lua standard libraries
	std::string scriptName = "data/npc/" + std::to_string(npcID) + ".lua";
	std::cout << scriptName << std::endl;
	luaL_dofile(L, scriptName.c_str()); // load the dialogue script file

	// retrieve the number and string from the script file
	lua_getglobal(L, "lines"); // push the "num" variable onto the stack
	int lines = lua_tonumber(L, -1); // retrieve the number from the top of the stack
	lua_pop(L, 1); // remove the number from the stack

	int lineID;

	auto it = std::find_if(npcDialogueTable.begin(), npcDialogueTable.end(), [&npcID](const auto& t) {return t.first == npcID; });
	auto distance = std::distance(this->npcDialogueTable.begin(), it);
	if (it != npcDialogueTable.end()) {
		if (npcDialogueTable[distance].second > lines) {
			// Set to +1 of the max amount of lines so we don't have to deal with lineID becoming a large number.
			lineID = lines + 1; 
			npcDialogueTable[distance].second = lineID;
		}
		lineID = npcDialogueTable[distance].second;
	}
	else {
		this->npcDialogueTable.push_back(std::make_pair(npcID, 1));
		lineID = 1;
	}

	// push the function and arguments onto the stack
	lua_getglobal(L, "getDialogue"); // push the function
	lua_pushinteger(L, lineID); // push the NPC ID argument

	// call the function with 1 argument and 1 result
	lua_call(L, 1, 1);

	// retrieve the result from the top of the stack
	this->dialogueText = lua_tostring(L, -1);

	// clean up the stack and close the Lua state
	lua_pop(L, 1);
	lua_close(L);

	this->drawNpcDialogue(graphics, 100, 100, this->dialogueText, posX, posY);
	return 0;
}

int Npc::playNext(int npcID, Graphics & graphics, int posX, int posY, Player &player)
{
	// If (questState == false) {
		/*XMLDocument xml;
		std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
		cwd.append(name + ".xml");
		std::filesystem::path nCwd = std::filesystem::current_path() / "data" / "npc";
		nCwd.append("temp.xml");
		AESCipher cipher;
		cipher.AESDecrypt(cwd.string(), nCwd.string());
		xml.LoadFile(nCwd.string().c_str());
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
			cipher.AESEncrypt(nCwd.string(), cwd.string());
			XMLCheckResult(result);
		}
		else {
			this->lineCounter = 1;
			this->lineChar = 'a';
			this->endOfChat = true;
			cipher.AESEncrypt(nCwd.string(), cwd.string());
			XMLCheckResult(result);
		}
		XMLCheckResult(result);*/

	int lineID;

	auto it = std::find_if(npcDialogueTable.begin(), npcDialogueTable.end(), [&npcID](const auto& t) {return t.first == npcID; });
	auto distance = std::distance(this->npcDialogueTable.begin(), it);
	if (it != npcDialogueTable.end()) {
		npcDialogueTable[distance].second += 1;
		lineID = npcDialogueTable[distance].second;
	}
	else {
		this->npcDialogueTable.push_back(std::make_pair(npcID, 1));
		lineID = 1;
	}

	lua_State* L = luaL_newstate(); // create a new Lua state
	luaL_openlibs(L); // load Lua standard libraries
	std::string scriptName = "data/npc/" + std::to_string(npcID) + ".lua";
	std::cout << scriptName << std::endl;
	luaL_dofile(L, scriptName.c_str()); // load the dialogue script file

	// retrieve the number and string from the script file
	lua_getglobal(L, "lines"); // push the "num" variable onto the stack
	int lines = lua_tonumber(L, -1); // retrieve the number from the top of the stack
	lua_pop(L, 1); // remove the number from the stack

	// push the function and arguments onto the stack
	lua_getglobal(L, "getDialogue"); // push the function
	lua_pushinteger(L, lineID); // push the NPC ID argument

	// call the function with 1 argument and 1 result
	lua_call(L, 1, 1);

	// retrieve the result from the top of the stack
	this->dialogueText = lua_tostring(L, -1);

	// clean up the stack and close the Lua state
	lua_pop(L, 1);
	lua_close(L);

	this->drawNpcDialogue(graphics, 100, 100, this->dialogueText, posX, posY);

	if (lineID > lines) {
		this->endOfChat = true;
	}

	return 0;
}

int Npc::repeatScript(Graphics& graphics, int posX, int posY) {
	this->drawNpcDialogue(graphics, 100, 100, this->dialogueText, posX, posY);
	return 0;
}

int Npc::playQuest(int npcID, int selection, Graphics& graphics, int posX, int posY, Player &player) {
	this->endOfChat = false;

	

	if (selection == 1)
		this->storedQuestName = this->questName1;
	else
		this->storedQuestName = this->questName2;

	this->currentQuestDialogue.clear();

	// Check if quest is completed now. If so, do not use the quest dialogue get<4> use quest finish get<5> and set to complete at playNext() for rewards
	if (checkQuest(player)) {
		if (this->blockSelectedQuest) {
			return 0;
		}
		this->isQuestDone = true;
		if (!this->questTable.empty()) {
			for (auto &t : this->questTable) {
				if (this->storedQuestName == std::get<0>(t)) {
					this->questDialogueSize = std::get<5>(t).size();
					for (int i = 0; i < this->questDialogueSize; i++) {
						this->currentQuestDialogue.push_back(std::get<5>(t)[i]);
					}
					//this->questDialogueText = std::get<4>(t)[0];
				}
			}
		}
	}

	else {
		// Retrieve the text and store into a vector that serves only the currently selected quest
		// As many quests will be in the game, searching through all the quests each time can become expensive
		if (!this->questTable.empty()) {
			for (auto &t : this->questTable) {
				if (this->storedQuestName == std::get<0>(t)) {
					this->questDialogueSize = std::get<4>(t).size();
					for (int i = 0; i < this->questDialogueSize; i++) {
						this->currentQuestDialogue.push_back(std::get<4>(t)[i]);
					}
					//this->questDialogueText = std::get<4>(t)[0];
				}
			}
		}
	}

	// Start at line 0
	this->questDialogueLine = 0;

	//lua_State* L = luaL_newstate(); // create a new Lua state
	//luaL_openlibs(L); // load Lua standard libraries
	//std::string scriptName = "data/npc/" + std::to_string(npcID) + ".lua";
	//std::cout << scriptName << std::endl;
	//luaL_dofile(L, scriptName.c_str()); // load the dialogue script file

	//luabridge::LuaRef questsTable = luabridge::getGlobal(L, "quests");
	//luabridge::LuaRef quest = questsTable;
	//luabridge::LuaRef dialogueTable = quest["qDialogue"];

	//int lineID, lines;
	//lines = dialogueTable.length();


	//auto it = std::find_if(npcDialogueTable.begin(), npcDialogueTable.end(), [&npcID](const auto& t) {return t.first == npcID; });
	//auto distance = std::distance(this->npcDialogueTable.begin(), it);
	//if (it != npcDialogueTable.end()) {
	//	if (npcDialogueTable[distance].second > lines) {
	//		// Set to +1 of the max amount of lines so we don't have to deal with lineID becoming a large number.
	//		lineID = lines + 1;
	//		npcDialogueTable[distance].second = lineID;
	//	}
	//	lineID = npcDialogueTable[distance].second;
	//}
	//else {
	//	this->npcDialogueTable.push_back(std::make_pair(npcID, 1));
	//	lineID = 1;
	//}

	//// push the function and arguments onto the stack
	//lua_getglobal(L, "getDialogue"); // push the function
	//lua_pushinteger(L, lineID); // push the NPC ID argument

	//// call the function with 1 argument and 1 result
	//lua_call(L, 1, 1);

	//// retrieve the result from the top of the stack
	//this->dialogueText = lua_tostring(L, -1);

	//// clean up the stack and close the Lua state
	//lua_pop(L, 1);
	//lua_close(L);

	this->drawNpcDialogue(graphics, 100, 100, this->currentQuestDialogue[this->questDialogueLine], posX, posY);
	return 0;
}

int Npc::playNextQuest(int npcID, Graphics& graphics, int posX, int posY, Player &player) {
	this->questDialogueLine++;
	if (this->questDialogueLine <= this->questDialogueSize - 1) {
		this->drawNpcDialogue(graphics, 100, 100, this->currentQuestDialogue[this->questDialogueLine], posX, posY);
	}
	else if (this->questDialogueLine == this->questDialogueSize && this->isQuestDone && !this->isQuestRewarded)
	{
		this->giveRewards(player);
		this->isQuestRewarded = true;
		this->endOfChat = true;
	}
	else {
		this->endOfChat = true;
	}

	return 0;
}

int Npc::repeatQuestScript(Graphics& graphics, int posX, int posY) {
	this->drawNpcDialogue(graphics, 100, 100, this->currentQuestDialogue[this->questDialogueLine], posX, posY);
	return 0;
}

void Npc::resetScripts()
{
	this->lines = 1;
	this->lineCounter = 1;
	this->lineChar = 'a';
	this->endOfChat = false;
}

int Npc::loadQuests(int npcID) {
	// If the quest for the NPC hasn't been loaded yet then get the data and insert into the questTable.
	// This will save a lot of time checking this first and also prevent duplicate quests.
	auto it = std::find_if(questTable.begin(), questTable.end(), [&npcID](const auto& t) {return std::get<6>(t) == npcID; });
	if (this->questTable.empty() || it == questTable.end()) {
		lua_State* L = luaL_newstate(); // create a new Lua state
		luaL_openlibs(L); // load Lua standard libraries
		std::string scriptName = "data/npc/" + std::to_string(npcID) + ".lua";
		std::cout << scriptName << std::endl;
		luaL_dofile(L, scriptName.c_str()); // load the dialogue script file

		// Create the references here so that they may be destroyed after the for loop
		// to prevent any memory leaks and allow the lua state to close properly.
		luabridge::LuaRef questsTable = luabridge::getGlobal(L, "quests");
		luabridge::LuaRef quest = questsTable;
		luabridge::LuaRef dialogueTable = quest["qDialogue"];
		luabridge::LuaRef finishTable = quest["finish"];

		// Get the number of quests in the quests table
		int numQuests = questsTable.length();

		for (int i = 1; i <= numQuests; i++) {
			// Get a reference to the current quest
			quest = questsTable[i];

			// Get the name of the quest
			std::string qName = quest["name"].cast<std::string>();

			std::cout << "Quest name: " << qName << std::endl;

			// Get a reference to the dialogue table for the quest
			dialogueTable = quest["qDialogue"];

			// Get the number of dialogue strings for the quest
			int numDialogue = dialogueTable.length();
			// Create the vector to store the dialogue
			std::vector<std::string> questDialogueVec;

			for (int j = 1; j <= numDialogue; j++) {
				// Get the current dialogue string
				std::string dialogue = dialogueTable[j].cast<std::string>();
				questDialogueVec.push_back(dialogue);
				std::cout << dialogue << std::endl;
			}

			// Get a reference to the quest finish dialogue table for the quest
			finishTable = quest["finish"];

			// Get the number of dialogue strings for the quest
			int numFinish = finishTable.length();
			// Create the vector to store the quest finished dialogue
			std::vector<std::string> questFinishDialogueVec;

			for (int k = 1; k <= numFinish; k++) {
				// Get the current dialogue string
				std::string finish = finishTable[k].cast<std::string>();
				questFinishDialogueVec.push_back(finish);
				std::cout << finish << std::endl;
			}

			// Get the quest object (enemy/loot to kill/collect)
			std::string object = quest["object"].cast<std::string>();

			// Get the quest type (1 = Kill and 2 = collect)
			int qType = quest["qType"].cast<int>();

			// Get the amount needed (kill/collect)
			int qAmount = quest["amount"].cast<int>();

			// Get the exp reward
			int rExp = quest["rExp"].cast<int>();

			// Get the celestial reward
			int rCels = quest["rCels"].cast<int>();

			// Get the item reward (as a itemID that we can check in a Item vector for to give)
			int rItem = quest["rItem"].cast<int>();

			// Get the reward quantity
			int rQuantity = quest["rQuantity"].cast<int>();

			std::cout << "Quest object: " << object << std::endl;
			std::cout << "Quest type: " << qType << std::endl;
			std::cout << "Quest amount: " << qAmount << std::endl;
			std::cout << "Reward exp: " << rExp << std::endl;
			std::cout << "Reward cels: " << rCels << std::endl;
			std::cout << "Reward item: " << rItem << std::endl;
			std::cout << "Item quantity: " << rQuantity << std::endl;

			this->questTable.push_back(std::make_tuple(qName, qType, object, qAmount, questDialogueVec, questFinishDialogueVec, npcID, rItem, rQuantity, rExp, rCels, false));
		}
		// Set all the LuaRef to Nil so that when lua_close(L) is called it can close properly by dereferencing Nil
		// since the original values are no longer accessible after the for loop
		questsTable = luabridge::Nil();
		quest = luabridge::Nil();
		dialogueTable = luabridge::Nil();
		finishTable = luabridge::Nil();

		// close the Lua state
		lua_close(L);
	}

	// Once rewarded and player opens menu again, mark as completed.
	//for (auto &t : this->questLog) {
	//	if (std::get<5>(t) == true && std::get<4>(t) == false) {
	//		std::get<4>(t) = true;

	//	}
	//}


	//std::cout << "loading quests for: " << name << std::endl;
	//XMLDocument xml;
	//XMLError result;

	//std::filesystem::path cwd = std::filesystem::current_path() / "data" / "npc";
	//cwd.append(name + ".xml");
	//std::filesystem::path nCwd = std::filesystem::current_path() / "data" / "npc";
	//nCwd.append("temp.xml");
	//AESCipher cipher;
	//cipher.AESDecrypt(cwd.string(), nCwd.string());
	//result = xml.LoadFile(nCwd.string().c_str());

	//if (result != tinyxml2::XML_SUCCESS)
	//	std::cout << "Could not read the file!";
	//XMLNode* root = xml.FirstChild();
	//if (root == nullptr) {
	//	std::cout << XML_ERROR_FILE_READ_ERROR << std::endl;
	//	return XML_ERROR_FILE_READ_ERROR;
	//}
	//XMLElement* element = root->FirstChildElement("QuestTable");
	//XMLElement* ptrElement = element->FirstChildElement("Quest");
	//if (ptrElement == nullptr)
	//	return XML_ERROR_PARSING_ELEMENT;
	//int amount, type, numOfQuest, rewardType, rewardLevel, exp, cels;
	//ptrElement->QueryIntAttribute("numOfQuest", &numOfQuest);
	//const char* textPtr = nullptr, *descPtr = nullptr, *objPtr = nullptr, *npcPtr = nullptr, *rewardItem;
	//for (int counter = 0; counter < numOfQuest; ++counter) {
	//	// Obtain data from XML
	//	ptrElement->QueryIntAttribute("amount", &amount);
	//	ptrElement->QueryIntAttribute("type", &type);
	//	textPtr = ptrElement->Attribute("questName");
	//	descPtr = ptrElement->Attribute("description");
	//	objPtr = ptrElement->Attribute("object");
	//	ptrElement->QueryIntAttribute("rewardType", &rewardType);
	//	// If (rewardType == 0) {
	//	// PtrElement->QueryIntAttribute("reward", &rewardLevel);
	//	//}
	//	// Else
	//	rewardItem = ptrElement->Attribute("reward");
	//	ptrElement->QueryIntAttribute("exp", &exp);
	//	ptrElement->QueryIntAttribute("cels", &cels);
	//	std::string text = textPtr, descText = descPtr, objText = objPtr;

	//cipher.AESEncrypt(nCwd.string(), cwd.string());
	//XMLCheckResult(result);
	return 0;
}

void Npc::displayQuests(Graphics & graphics, int npcID, int posX, int posY, Player & player)
{
	int dy = 0;
	int qNum = 1;
	if (!this->questTable.empty()) {
		for (auto &t : this->questTable) {
			if (npcID == std::get<6>(t)) {
				std::string qtName = std::get<0>(t);
				// If Quest was completed. Show a green visual indicator that it is done.
				if (std::get<11>(t) == true)
					this->drawQuestText(graphics, posX + 10, posY + dy, qtName, { 0, 255, 0, 0 });
				else
					this->drawQuestText(graphics, posX + 10, posY + dy, qtName);
				if (qNum == 1)
					this->questName1 = qtName;
				else
					this->questName2 = qtName;
				qNum++;
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

//void Npc::acceptQuest(Graphics & graphics, int npcID, int posX, int posY, Player & player, int selection)
//{
//	this->questState = true;
//	if (questDone) {
//		this->giveRewards(graphics, this->storedQuestName, posX, posY, player, selection);
//		return;
//	}
//	bool isCompleted;
//	if (questTable.empty())
//		return;
//	if (questName1 || questName2)
//	if (selection >= this->questTable.size()) { // If player tries to select and empty quest index
//		std::cout << "Cannot find specified index..." << std::endl;
//		return;
//	}	
//	//std::string text = std::get<0>(this->questTable[selection]);
//	//this->storedQuestName = text;
//
//
//
//
//	if (!questLog.empty() && !questTable.empty()) {
//		isCompleted = this->checkQuest(graphics, text, posX, posY, player);
//		if (isCompleted == true) {
//			this->questDone = true;
//			this->giveRewards(graphics, this->storedQuestName, posX, posY, player, selection);
//			return;
//		}
//	}
//	std::string completeMsg;
//	auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
//	auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
//	auto distance = std::distance(this->questLog.begin(), it);
//	 if (!questTable.empty()) {
//		this->drawNpcText(graphics, 100, 100, std::get<4>(this->questTable[selection]), posX, posY);
//		if (this->questLog.empty()) {
//			this->questLog.push_back(std::make_tuple(std::get<0>(this->questTable[selection]), std::get<1>(this->questTable[selection]),
//				std::get<2>(this->questTable[selection]), std::get<3>(this->questTable[selection]), false, false, 
//				std::get<6>(this->questTable[selection]), std::get<7>(this->questTable[selection]), 
//				std::get<8>(this->questTable[selection]), std::get<9>(this->questTable[selection])));
//		}
//		else { // Find_if algorithm with lambda function
//			auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
//			if (it == questLog.end()) {
//				this->questLog.push_back(std::make_tuple(std::get<0>(this->questTable[selection]), std::get<1>(this->questTable[selection]),
//					std::get<2>(this->questTable[selection]), std::get<3>(this->questTable[selection]), false, false,
//					std::get<6>(this->questTable[selection]), std::get<7>(this->questTable[selection]),
//					std::get<8>(this->questTable[selection]), std::get<9>(this->questTable[selection])));
//			}
//		}
//	}
//}

void Npc::giveRewards(Player & player)
{
	if (!this->questTable.empty()) {
		std::string qstName = this->storedQuestName;
		auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&qstName](const auto& t) {return std::get<0>(t) == qstName; });
		auto distanceTable = std::distance(this->questTable.begin(), tableIt);
		if (tableIt != questTable.end() && std::get<11>(this->questTable[distanceTable]) == false) {
			std::cout << "Quest was already checked for completion. Mark completed now and distribute rewards." << std::endl;
			std::get<11>(this->questTable[distanceTable]) = true;

			// Store the rewards and check if not 0 (Stored to draw rewards later).
			this->itemReward = std::get<7>(this->questTable[distanceTable]);
			this->itemQuantityReward = std::get<8>(this->questTable[distanceTable]);
			this->expReward = std::get<9>(this->questTable[distanceTable]);
			this->celsReward = std::get<10>(this->questTable[distanceTable]);

			if (itemReward != 0) {
				// TODO: Change equipment to itemID + handle inventory items such as hp pots
				player.addEquipment("JetPack");
			}
			if (expReward != 0) {
				player.gainExp(this->expReward);
			}
			if (celsReward != 0) {
				player.gainCurrency(this->celsReward);
			}
		}
	}


	//std::string completeMsg;
	//std::stringstream ss;
	//auto it = std::find_if(questLog.begin(), questLog.end(), [&questName](const auto& t) {return std::get<0>(t) == questName; });
	//auto distance = std::distance(this->questLog.begin(), it);
	//// Auto itQT = std::find_if(questTable.begin(), questTable.end(), [&npcName](const auto& t) {return std::get<0>(t) == npcName; });
	//// If (itQT == questTable.end()) {
	////	std::cout << "COULD NOT FIND QUEST IN QUESTABLE!!!" << std::endl;
	////}
	//// Auto distanceQT = std::distance(questTable.begin(), itQT);
	//if (questDone) {
	//	if (this->exp >= 0) {
	//		if (std::get<6>(this->questLog[distance]) == 0) {
	//			ss << "Here is your reward!$n" << this->levelReward << " level," << this->exp << "exp," << this->cels << "cels.$e";
	//			this->drawNpcText(graphics, 100, 100, ss.str(), posX, posY);
	//		}
	//		else if (std::get<6>(this->questLog[distance]) == 1) {
	//			ss << "Here is your reward!$n" << this->reward << " item," << this->exp << "exp," << this->cels << "cels.$e";
	//			this->drawNpcText(graphics, 100, 100, ss.str(), posX, posY);
	//		}
	//	}
	//	if (std::get<5>(this->questLog[distance]) != true) {
	//		if (std::get<6>(this->questLog[distance]) == 0) {
	//			std::cout << "rewardType = " << std::get<6>(this->questLog[distance]) << std::endl;
	//			this->levelReward = std::stoi(std::get<7>(this->questLog[distance]));
	//			this->exp = std::get<8>(this->questLog[distance]);
	//			this->cels = std::get<9>(this->questLog[distance]);
	//			player.addLevel(levelReward);
	//			player.gainExp(exp);
	//			player.gainCurrency(cels);
	//			ss << "Here is your reward!$n" << levelReward << " level," << exp << "exp," << cels << "cels.$e";
	//			// CompleteMsg = "Thank you! Here is your reward.$n" + levelGain + "level, "; // TODO: push rewards into vector to distibute any reward for any quest.
	//			this->drawNpcText(graphics, 100, 100, ss.str(), posX, posY);
	//		}
	//		else if (std::get<6>(this->questLog[distance]) == 1) {
	//			std::cout << "rewardType = " << std::get<6>(this->questLog[distance]) << std::endl;
	//			this->exp = std::get<8>(this->questLog[distance]);
	//			this->cels = std::get<9>(this->questLog[distance]);
	//			this->reward = std::get<7>(this->questLog[distance]);
	//			player.addEquipment(reward);
	//			player.gainExp(exp);
	//			player.gainCurrency(cels);
	//			ss << "Here is your reward!$n" << reward << " item," << exp << "exp," << cels << "cels.$e";
	//			this->drawNpcText(graphics, 100, 100, ss.str(), posX, posY);
	//		}
	//	}
	//		
	//	std::get<5>(this->questLog[distance]) = true;
	//}
}


bool Npc::checkQuest(Player &player)
{
	this->blockSelectedQuest = false;
	if (!this->questTable.empty()) {
		std::string qstName = this->storedQuestName;
		auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&qstName](const auto& t) {return std::get<0>(t) == qstName; });
		auto distanceTable = std::distance(this->questTable.begin(), tableIt);
		if (tableIt != questTable.end() && std::get<11>(this->questTable[distanceTable]) == false) {
			if (std::get<1>(this->questTable[distanceTable]) == 1) { // Kill quests
				if (player.checkKillQuestComplete(std::get<2>(this->questTable[distanceTable]), std::get<3>(this->questTable[distanceTable]))) {
					std::cout << "Quest is completed. Return true to setup finish dialogue and rewards" << std::endl;
					return true;
				}
			}
			// else if {} TODO: Collect quest type handled here
		}
		// Quest was completed already and rewards were given. So, block from selecting it.
		else {
			this->blockSelectedQuest = true;
		}
	}

	//if (!this->questLog.empty()) {
	//	std::string text = name;
	//	std::string completeMsg;
	//	auto it = std::find_if(questLog.begin(), questLog.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
	//	auto tableIt = std::find_if(questTable.begin(), questTable.end(), [&text](const auto& t) {return std::get<0>(t) == text; });
	//	auto distanceTable = std::distance(this->questTable.begin(), tableIt);
	//	auto distance = std::distance(this->questLog.begin(), it);
	//	if (it != questLog.end() && std::get<4>(this->questLog[distance]) == false) {
	//		if (std::get<1>(this->questLog[distance]) == 1) { // Kill quests
	//			if (player.checkKillQuestComplete(std::get<2>(this->questLog[distance]), std::get<3>(this->questLog[distance]))) {
	//				std::cout << "Quest is completed! Remove from available quests!" << std::endl;
	//				if (tableIt != questTable.end()) {
	//					this->questTable.erase(this->questTable.begin() + distanceTable);
	//				}
	//				return true;
	//			}
	//		}
	//	}
	//}
	return false;
}

const std::string Npc::getNpcNameID(int npcID) {
	auto it = std::find_if(this->npcIDTable.begin(), this->npcIDTable.end(), [&npcID](const auto& t) {return t.second == npcID; });
	auto distance = std::distance(this->npcIDTable.begin(), it);
	if (it != npcIDTable.end()) {
		return npcIDTable[distance].first;
	}
	std::printf("Error resolving NPC name with ID(%s) at file:%s:%d\n",npcID, __FILE__, __LINE__);
	return "NULL";
}

// Luna class
Luna::Luna() {}

Luna::Luna(Graphics &graphics, Vector2 spawnPoint, std::string name, int npcId) :
	Npc(graphics, "data\\npc\\female.png", 2, 33, 30, 30, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	_npcName = name;
	npcID = npcId;
	this->setupAnimations();
	this->playAnimation("Luna");
	// This->loadQuests(name);
}

void Luna::update(int elapsedTime, Player &player) {
	/*
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "Luna" : "LunaD");


	// Move up or down
	this->_y += this->_shouldMoveUp ? -.06 : .06;
	if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}*/

	Npc::update(elapsedTime, player);
}

void Luna::draw(Graphics &graphics) {
	Npc::draw(graphics);
}

void Luna::animationDone(std::string currentAnimation) {

}

void Luna::setupAnimations() {
	this->addAnimation(1, 1, 1, "Luna", 33, 31, Vector2(0, 0));
	this->addAnimation(1, 2, 33, "LunaD", 33, 31, Vector2(0, 0));
}

void Luna::bulletHit() {
	this->_currentHealth += -1;
	std::cout << "hit! HP = " << this->_currentHealth << std::endl;
}

void Luna::touchPlayer(Player* player) {
	player->gainHealth(-1);
}




