/* Game Class
* This class holds all information for our main game loop
*/
#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif

#include <algorithm>﻿
#include <SDL.h>
#include <SDL_mixer.h>
#include "tinyxml2.h"
#include "game.h"
#include "Graphics.h"
#include "Input.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace tinyxml2;

bool activeProjectile = false;
namespace {
	const int FPS = 50;
	// remove 5 *
	const int MAX_FRAME_TIME = 5* 1000 / FPS; //Max amount of time a frame is allowed to last
	bool title = true;
	bool GAMEOVER = false;
	bool activeTalk = false;
	bool activeInventory = false;
	bool activeStatMenu = false;
	int selection = 1;
	int npcSelection = 1;
	int questSelection = 1;
	bool pickUp = false;
	bool nextLine = false;
	std::string npcName;
	int lineNum = 0;
	int currentLine = 0;
}

Game::Game() { //constructor
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		
	}
	//TTF_Font *font = TTF_OpenFont("Arcadia.ttf", 24);
	this->gameLoop(); //start game
}

Game::~Game() { //deconstructor
	
}

std::ifstream& GoLine(std::ifstream& file, unsigned int num) {
	file.seekg(std::ios::beg);
	for (int i = 0; i < num - 1; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	//music
	Mix_Music *gMusic = NULL;
	gMusic = Mix_LoadMUS("Astral.wav");;
	if (gMusic == NULL)
	{
		printf("Failed to load specified music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	// else if ! == NULL ?
	//Mix_PlayMusic(gMusic, -1); uncomment to play music

	//Load savefile
	std::ifstream data;
	int x, y;
	Vector2 spawn;
	std::string mapname;

	this->_title = Title(graphics, input, event);
	std::cout << "title loop has finished" << std::endl;
	this->_gameOver = GameOver(graphics);
	//this->_title.Start();
	//this->_level = Level("cave", graphics); //intialize level: Map name , spawn point, graphics
	//this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_camera = Camera();
	//this->_bullet = Bullet(graphics, this->_level.getBulletSpawnPoint());
	this->_bullet = Projectile(graphics, this->_player);
	this->_chatBox = TextManager(graphics, this->_player);
	//this->_player = Player(graphics, 100, 100); //We only now need x, y of char when we create. intialize player
	this->_hud = HUD(graphics, this->_player);
	this->_inventory = Inventory(graphics, this->_player);
	//Camera::Init();

	int LAST_UPDATE_TIME = SDL_GetTicks(); 
	//Above ^ gets the amount of miliseconds since the SDL library was intialized
	//must start before loop
	//Start the game loop
	while (true) {
		input.beginNewFrame(); //resets our released key pressed keys first time around doesnt matter already set to false but good to reset anyway
		if (title == true) {
			title = _title.Start(graphics, input, event);
			if (_title.getMenuChoice() == 0) {
				std::cout << "No save data found...Starting new game!" << std::endl;
				this->_level = Level("cave", graphics, this->_inventory); //intialize level: Map name , spawn point, graphics
				this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
				this->saveGame(graphics);
			}
			else {
				this->loadGame(graphics);
			}
		}
		if (title == false && GAMEOVER == true) {
			const int CURRENT_TIME_MS = SDL_GetTicks();
			int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.repeat == 0) {
						input.keyDownEvent(event); //if we are holding key start keydown event
					}
				}
				else if (event.type == SDL_KEYUP) { // if key was released
					input.keyUpEvent(event);
				}
				else if (event.type == SDL_QUIT) {
					return; //when the game ends or user exits
				}
			}
			if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true && GAMEOVER == true) {
				this->_level = Level("cave", graphics, this->_inventory); //intialize level: Map name , spawn point, graphics
				this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
				GAMEOVER = false;
			}

			this->_graphics = graphics; //updated graphics
			this->updateGameOver(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME)); //take standard min : elapsed time ms and max frame time

			LAST_UPDATE_TIME = CURRENT_TIME_MS; //loop will go again and current time - new last update will tell us how long next frame will take

			this->drawGameOver(graphics);
		}

		if (title == false && GAMEOVER == false) {


		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event); //if we are holding key start keydown event
				}
			}
			else if (event.type == SDL_KEYUP) { // if key was released
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return; //when the game ends or user exits
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			this->saveGame(graphics);
			std::cout << "Quitting Game..." << std::endl;
			return; //quit game if ESC was pressed
		}
		else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) { 
			if (activeTalk == false && activeInventory == false && activeStatMenu == false) {
				this->_player.moveLeft();
			}
			else if (activeTalk == true) {
				std::cout << "impaired action" << std::endl;
			}
			 
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false) {
				this->_player.moveRight();
			}
			else if (activeTalk == true) {
				std::cout << "impaired action" << std::endl;
			}
			
		}

		if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false)
				this->_player.lookUp();
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false)
				this->_player.lookDown();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_UP) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false)
				this->_player.stopLookingUp();
		}
		if (input.wasKeyReleased(SDL_SCANCODE_DOWN) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false)
				this->_player.stopLookingDown();
		}

		if (input.wasKeyPressed(SDL_SCANCODE_SPACE) == true){ //IF is imporannt make sure not to do else if otherwise u cannot move and jump!
			if (activeTalk == false && activeInventory == false && activeStatMenu == false) {
				this->_player.jump();
			}
			else if (activeTalk == true) {
				std::cout << "impaired action" << std::endl;
			}
			
		}
		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)) { //if player isnt moving left or right(at all) do stopMoving function
			this->_player.stopMoving();
		}
		//bullet
		if (input.wasKeyPressed(SDL_SCANCODE_E) == true) {
			if (activeTalk == false && activeInventory == false && activeStatMenu == false) {

				if (_player.lookingUp() == true) {
					this->_bullet.shootUp(graphics, this->_player);;
				}
				else if (_player.lookingDown() == true) {
					this->_bullet.shootDown(graphics, this->_player);;
				}

				else if (_player.lookingLeft() == true) {
					this->_bullet.shootLeft(graphics, this->_player);;
				}

				else if (_player.lookingRight() == true) {
					this->_bullet.shootBullet(graphics, this->_player);
				}
				//this->_bullet.shootBullet(graphics, this->_player);
			}
			
			else if (activeTalk == true) {
				std::cout << "impaired action" << std::endl;
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Q) == true && activeInventory == false && activeStatMenu == false) {
			if (activeTalk == false && npcName != ""){
				activeTalk = true;
				this->_chatBox.setTextStatus(true);
				this->_chatBox.drawChatBox(graphics, this->_player);
				this->_npc.setNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
				this->_npc.drawNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
				this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				this->_npc.loadQuests(npcName);
			}
			else if (activeTalk == true) {
				activeTalk = false;
				this->_chatBox.setTextStatus(false);
				this->_npc.resetScripts();
			}
		}

		if (activeTalk) {
			if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
				//chat
				if (this->_npc.getQuestState() == true) {
					activeTalk = false;
					this->_chatBox.setTextStatus(false);
					this->_npc.setNpcTalk(false);
					this->_npc.setQuestMenuState(false);
					this->_npc.setQuestState(false);
					this->_npc.setQuestDone(false);
					this->_npc.setNoQuest(false);
					this->_npc.resetScripts();
					npcSelection = 1;
					questSelection = 1;
				}
				else if (npcSelection == 1 && this->_npc.getNpcTalk() == false) {
					this->_npc.setNpcTalk(true);
					this->_npc.playScript(npcName, graphics, this->_player.getX(), this->_player.getY());
				}
				else if (npcSelection == 1 && this->_npc.getNpcTalk()) {
					this->_npc.playNext(npcName, graphics, this->_player.getX(), this->_player.getY(), this->_player);
					if (this->_npc.getChatStatus()) {
						this->_npc.setNpcTalk(false);
						activeTalk = false;
						this->_chatBox.setTextStatus(false);
						this->_npc.resetScripts();
					}
				}
				//quests
				else if (npcSelection == 2 && this->_npc.getNpcTalk() == false) {
					this->_npc.setNpcTalk(true);
					this->_npc.setQuestMenuState(true);
					this->_npc.displayQuests(graphics, npcName, this->_player.getX(), this->_player.getY(), this->_player);
				}
				else if (npcSelection == 2 && this->_npc.getNpcTalk() && this->_npc.checkNoQuests() == false) {
					if (questSelection == 1) {
						this->_npc.acceptQuest(graphics, npcName, this->_player.getX(), this->_player.getY(), this->_player, questSelection - 1);
					}
					else if (questSelection == 2) {
						this->_npc.acceptQuest(graphics, npcName, this->_player.getX(), this->_player.getY(), this->_player, questSelection - 1);
					}
				}
				else if (this->_npc.checkNoQuests()) {
					activeTalk = false;
					this->_chatBox.setTextStatus(false);
					this->_npc.setNpcTalk(false);
					this->_npc.setQuestMenuState(false);
					this->_npc.setQuestState(false);
					this->_npc.setQuestDone(false);
					this->_npc.setNoQuest(false);
					this->_npc.resetScripts();
					npcSelection = 1;
					questSelection = 1;
				}
					
			}

			if (input.wasKeyPressed(SDL_SCANCODE_DELETE) == true) {
				this->_npc.setNpcTalk(false);
				this->_chatBox.setTextStatus(false);
				activeTalk = false;
				this->_npc.setQuestMenuState(false);
				this->_npc.resetScripts();
			}

			if (input.wasKeyPressed(SDL_SCANCODE_UP) == true && this->_npc.getNpcTalk() == false && this->_npc.getQuestMenuState() == false) {
				if (npcSelection == 1)
					this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				else if (npcSelection != 1) {
					npcSelection--;
					this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				}
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true && this->_npc.getNpcTalk() == false && this->_npc.getQuestMenuState() == false) {
				if (npcSelection == 2)
					this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				else if (npcSelection != 2) {
					npcSelection++;
					this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				}
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_UP) == true && this->_npc.getQuestMenuState() == true) {
				if (questSelection == 1)
					this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				else {
					questSelection--;
					this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				}
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true && this->_npc.getQuestMenuState() == true) {
				if (questSelection == 2)
					this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				else {
					questSelection++;
					this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
				}
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_F1) == true) {
			if (!activeStatMenu) {
				selection = 1;
				activeStatMenu = true;
				this->_player.drawStatMenu(graphics, this->_player, selection);
			}
			else
				activeStatMenu = false;
		}

		if (activeStatMenu) {
			if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
				this->_player.statChoice(selection);
			}
			if (input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
				if (selection == 1)
					this->_player.drawStatMenu(graphics, this->_player, selection);
				else if (selection != 1) {
					selection--;
					this->_player.drawStatMenu(graphics, this->_player, selection);
				}
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
				if (selection == 3)
					this->_player.drawStatMenu(graphics, this->_player, selection);
				else if (selection != 3) {
					selection++;
					this->_player.drawStatMenu(graphics, this->_player, selection);
				}
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_TAB) == true && activeStatMenu == false && activeTalk == false) {
			if (activeInventory == false) {
				activeInventory = true;
				this->_inventory.draw(graphics, this->_player);
			}

			else if (activeInventory == true) {
				activeInventory = false;
			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_Z) == true) {
			if (pickUp == false) {
				pickUp = true;
			}
			else if (pickUp == true) {
				pickUp = false;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_1) == true) {
			_inventory.useItem(0, this->_player);
			std::cout << "game: useItem called" << std::endl;
		}

		if (input.wasKeyPressed(SDL_SCANCODE_2) == true) {
			if (_bullet.checkDmg() == 1) {
				_bullet.setDmg(2);
			}
			else if (_bullet.checkDmg() == 2) {
				_bullet.setDmg(1);
			}
		}

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->_graphics = graphics; //updated graphics
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics); //take standard min : elapsed time ms and max frame time
																 //if (this->_bullet.isActive() == true) {
																 //	this->updateBullet(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
																 //}
																 //this->updateBullet(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS; //loop will go again and current time - new last update will tell us how long next frame will take
		//cout << "last update Time: " << LAST_UPDATE_TIME << endl;
		this->draw(graphics);
		/*if (activeTalk == true) {
			this->drawNpcChat(graphics);
		}*/
		}//title


	}

}



void Game::drawBullet(Graphics &graphics) {
	this->_bullet.draw(graphics, this->_player);

	graphics.flip(); 
}

void Game::drawTitle(Graphics &graphics) {
	graphics.clear(); //clear any drawings MUST do

	this->_title.draw(graphics);

	graphics.flip(); 
}

void Game::updateTitle(float elapsedTime) { 
	this->_title.update(elapsedTime);
}

void Game::updateGameOver(float elapsedTime) {
	this->_gameOver.update(elapsedTime);
}

void Game::drawGameOver(Graphics &graphics) {
	graphics.clear();

	this->_gameOver.draw(graphics);

	graphics.flip();
}

void Game::draw(Graphics &graphics) {
	graphics.clear(); //clear any drawings MUST do

	this->_level.draw(graphics, this->_player); //need to draw level before player (below) so player is on top of level and not behind it!
	this->_player.draw(graphics); //what and where to draw
	this->_bullet.draw(graphics, this->_player);
	this->_bullet.drawUp(graphics, this->_player);
	this->_bullet.drawDown(graphics, this->_player);
	this->_bullet.drawLeft(graphics, this->_player);
	this->_bullet.drawGun(graphics, this->_player);
	this->_chatBox.drawChatBox(graphics, this->_player);
	this->_bullet.drawDmgText(graphics);

	if (activeTalk == true) {
		//if (this->_npc.getQuestState())
		//	this->_npc.checkQuest(graphics, npcName + ".xml", this->_player.getX(), this->_player.getY(), this->_player);
		//else
		//	this->_npc.playScript(npcName, graphics, this->_player.getX(), this->_player.getY());
		if (this->_npc.getNpcTalk() && npcSelection == 1) {
			//std::cout << "draw first option" << std::endl;
			this->_npc.playScript(npcName, graphics, this->_player.getX(), this->_player.getY());
		}
		else if (this->_npc.getNpcTalk() && npcSelection == 2 && this->_npc.getQuestMenuState() && !this->_npc.getQuestState()) {
			this->_npc.displayQuests(graphics, npcName, this->_player.getX(), this->_player.getY(), this->_player);
			this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), questSelection);
		}
		else if (this->_npc.getNpcTalk() && npcSelection == 2 && this->_npc.getQuestMenuState() && this->_npc.getQuestState() && this->_npc.checkNoQuests() == false) {
			this->_npc.acceptQuest(graphics, npcName, this->_player.getX(), this->_player.getY(), this->_player, questSelection - 1);
		}
			//this->_npc.checkQuest(graphics, npcName + ".xml", this->_player.getX(), this->_player.getY(), this->_player);
		else {
			this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
		}
		this->_npc.drawNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
	}
	/*if (activeTalk == true && nextLine == false) {
		this->_npc.runScript(npcName, graphics, this->_player.getX(), this->_player.getY());
	}

	if (activeTalk == true && nextLine == true) {
		this->_npc.playNextScript(npcName, graphics, this->_player.getX(), this->_player.getY(), currentLine);
	}*/

	this->_hud.draw(graphics, this->_player);
	if (activeInventory == true) {
		this->_inventory.draw(graphics, this->_player);
	}
	if (activeStatMenu)
		this->_player.drawStatMenu(graphics, this->_player, selection);
		
	this->_player.drawHPNumbers(graphics);
	this->_player.drawCurrentMapName(graphics);

	graphics.flip(); //Render everything above
}

void Game::updateBullet(float elapsedTime) {
	this->_bullet.update(elapsedTime, this->_player);
}

int Game::saveGame(Graphics & graphics)
{
	XMLDocument xml;
	XMLNode* root = xml.NewElement("Root");
	xml.InsertFirstChild(root);
	std::cout << "Creating the XML Document..." << std::endl;

	//Save player location
	XMLElement* element = xml.NewElement("Spawn");
	element->SetAttribute("mapName", this->_level.getMapName().c_str());
	element->SetAttribute("xCoordinate", this->_player.getX());
	element->SetAttribute("yCoordinate", this->_player.getY());
	root->InsertEndChild(element);
	//Save player stats
	element = xml.NewElement("Stats");
	element->SetAttribute("MaxVitality", this->_player.getMaxHealth());
	element->SetAttribute("Vitality", this->_player.getCurrentHealth());
	element->SetAttribute("Damage", this->_player.getDmgMod());
	element->SetAttribute("Defense", this->_player.getDefense());
	element->SetAttribute("Points", this->_player.getStatPoints());
	element->SetAttribute("Level", this->_player.getLevel());
	element->SetAttribute("Exp", this->_player.getCurrentExp());
	element->SetAttribute("SoulStr", this->_player.getSoulStr());
	element->SetAttribute("SoulLevel", this->_player.getSoulLevel());
	element->SetAttribute("KillCount", this->_player.getKillCount());
	element->SetAttribute("Celestial", this->_player.getCurrency());
	root->InsertEndChild(element);
	//Save loot table
	element = xml.NewElement("Loot");
	std::vector<std::pair<std::string, int>> tempVec = this->_inventory.getLootTable();
	for (auto iter = tempVec.begin(); iter != tempVec.end(); iter++) {
		auto first = iter->first;
		auto second = iter->second;
		XMLElement* ptrElement = xml.NewElement("Table");
		ptrElement->SetAttribute("value", second);
		ptrElement->SetAttribute("key", first.c_str());
		element->InsertEndChild(ptrElement);
	}
	root->InsertEndChild(element);
	//Save inventory
	element = xml.NewElement("Inventory");
	std::vector<std::pair<int, int>> iVec = this->_inventory.getInventoryTable();
	for (auto iter = iVec.begin(); iter != iVec.end(); iter++) {
		auto first = iter->first, second = iter->second;
		XMLElement* ptrElement = xml.NewElement("iTable");
		ptrElement->SetAttribute("type", second);
		ptrElement->SetAttribute("quantity", first);
		element->InsertEndChild(ptrElement);
	}
	root->InsertEndChild(element);
	element = xml.NewElement("QuestLog");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	std::vector<std::tuple<std::string, int, std::string, int, bool, bool>> qVec = this->_npc.getQuestLog();
	for (int counter = 0; counter < qVec.size(); ++counter) {
		XMLElement* qElement = xml.NewElement("Quest");
		qElement->SetAttribute("questName", std::get<0>(qVec[counter]).c_str());
		qElement->SetAttribute("type", std::get<1>(qVec[counter]));
		qElement->SetAttribute("object", std::get<2>(qVec[counter]).c_str());
		qElement->SetAttribute("amount", std::get<3>(qVec[counter]));
		qElement->SetAttribute("completed", std::get<4>(qVec[counter]));
		qElement->SetAttribute("rewarded", std::get<5>(qVec[counter]));
		element->InsertEndChild(qElement);
	}
	root->InsertEndChild(element);
	element = xml.NewElement("KillTable");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	std::vector<std::pair<std::string, int>> kVec = this->_player.getKillTable();
	for (auto iter = kVec.begin(); iter != kVec.end(); iter++) {
		auto first = iter->first;
		auto second = iter->second;
		XMLElement* ptrElement = xml.NewElement("kTable");
		ptrElement->SetAttribute("amount", second);
		ptrElement->SetAttribute("Name", first.c_str());
		element->InsertEndChild(ptrElement);
	}
	root->InsertEndChild(element);
	XMLError result = xml.SaveFile("SF-LOC.xml");
	XMLCheckResult(result);
}

int Game::loadGame(Graphics & graphics)
{
	XMLDocument xml;
	xml.LoadFile("SF-LOC.xml");
	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	//Load loot table
	XMLElement* element = root->FirstChildElement("Loot");
	XMLElement* ptrVec = element->FirstChildElement("Table");
	std::vector<std::pair<std::string, int>> tempVec;
	while (ptrVec != nullptr) {
		int type; std::string map; const char * getText = nullptr;
		result = ptrVec->QueryIntAttribute("value", &type);
		getText = ptrVec->Attribute("key");
		map = getText;
		tempVec.push_back(std::make_pair(map, type));
		ptrVec = ptrVec->NextSiblingElement("Table");
	}
	this->_inventory.setLootTable(tempVec);
	//Load Inventory
	element = root->FirstChildElement("Inventory");
	ptrVec = element->FirstChildElement("iTable");
	std::vector<std::pair<int, int>> iVec;
	while (ptrVec != nullptr) {
		int type, quantity;
		result = ptrVec->QueryIntAttribute("type", &type);
		result = ptrVec->QueryIntAttribute("quantity", &quantity);
		iVec.push_back(std::make_pair(quantity, type));
		ptrVec = ptrVec->NextSiblingElement("iTable");
	}
	this->_inventory.setInventoryTable(iVec);
	//Load QuestLog
	element = root->FirstChildElement("QuestLog");
	ptrVec = element->FirstChildElement("Quest");
	std::vector<std::tuple<std::string, int, std::string, int, bool, bool>> qVec;
	while (ptrVec != nullptr) {
		int amount, type;
		const char* textPtr = nullptr, *objPtr = nullptr;
		std::string qName, objName;
		bool completed, rewarded;
		result = ptrVec->QueryIntAttribute("type", &type);
		result = ptrVec->QueryIntAttribute("amount", &amount);
		textPtr = ptrVec->Attribute("questName");
		qName = textPtr;
		objPtr = ptrVec->Attribute("object");
		objName = objPtr;
		result = ptrVec->QueryBoolAttribute("completed", &completed);
		result = ptrVec->QueryBoolAttribute("rewarded", &rewarded);
		qVec.push_back(std::make_tuple(qName, type, objName, amount, completed, rewarded));
		ptrVec = ptrVec->NextSiblingElement("Quests");
	}
	this->_npc.setQuestLog(qVec);
	//Load Map
	element = root->FirstChildElement("Spawn");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	const char* textPtr = nullptr;
	textPtr = element->Attribute("mapName");
	std::string mapName = textPtr;
	this->_level = Level(mapName, graphics, this->_inventory); //intialize level: Map name , spawn point, graphics
	//Load coordinates
	Vector2 spawn;
	int x, y;
	result = element->QueryIntAttribute("xCoordinate", &x);
	result = element->QueryIntAttribute("yCoordinate", &y);
	spawn = Vector2((int)std::ceil(x), (int)std::ceil(y));
	//spawn = this->_level.getPlayerSpawnPoint();
	this->_player = Player(graphics, spawn);
	//Load KillTable
	element = root->FirstChildElement("KillTable");
	ptrVec = element->FirstChildElement("kTable");
	std::vector<std::pair<std::string, int>> kVec;
	while (ptrVec != nullptr) {
		int amount; std::string name; const char * getText = nullptr;
		result = ptrVec->QueryIntAttribute("amount", &amount);
		getText = ptrVec->Attribute("Name");
		name = getText;
		kVec.push_back(std::make_pair(name, amount));
		ptrVec = ptrVec->NextSiblingElement("kTable");
	}
	this->_player.setKillTable(kVec);
	//Load stats
	element = root->FirstChildElement("Stats");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	float fValue; double dValue; int iValue;
	result = element->QueryFloatAttribute("MaxVitality", &fValue);
	this->_player.setMaxHealth(fValue);
	result = element->QueryFloatAttribute("Vitality", &fValue);
	this->_player.setCurrentHealth(fValue);
	result = element->QueryDoubleAttribute("Damage", &dValue);
	this->_player.setDmgMod(dValue);
	result = element->QueryDoubleAttribute("Defense", &dValue);
	this->_player.setDefense(dValue);
	result = element->QueryIntAttribute("Points", &iValue);
	this->_player.setStatPoints(iValue);
	result = element->QueryIntAttribute("Level", &iValue);
	this->_player.setLevel(iValue);
	result = element->QueryFloatAttribute("Exp", &fValue);
	this->_player.setCurrentExp(fValue);
	result = element->QueryDoubleAttribute("SoulStr", &dValue);
	this->_player.setSoulStr(dValue);
	result = element->QueryIntAttribute("SoulLevel", &iValue);
	this->_player.setSoulLevel(iValue);
	result = element->QueryIntAttribute("KillCount", &iValue);
	this->_player.setKillCount(iValue);
	result = element->QueryIntAttribute("Celestial", &iValue);
	this->_player.setCurrency(iValue);

	XMLCheckResult(result);
}

void Game::update(float elapsedTime, Graphics &graphics) {
	this->_player.update(elapsedTime); 
	/*if (this->_player.getCurrentHealth() <= 0) {
		GAMEOVER = true;
	}*/
	this->_camera.Update(elapsedTime, this->_player);
	this->_level.update(elapsedTime, this->_player);
	this->_bullet.update(elapsedTime, this->_player);
	this->_bullet.updateUp(elapsedTime, this->_player);
	this->_bullet.updateDown(elapsedTime, this->_player);
	this->_bullet.updateLeft(elapsedTime, this->_player);
	this->_bullet.updateDmgText(elapsedTime);
    //hud goes on top of everything
	this->_hud.update(elapsedTime, this->_player);

	std::vector<Npc*> otherNpc;
	if ((otherNpc = this->_level.checkNpcCollisions(this->_player.getBoundingBox(), graphics)).size() > 0) {
		//this->_player.getNpcName(otherNpc, graphics);
		npcName = this->_player.getNpcName(otherNpc, graphics);

	}
	//this will ensure when we are no long colliding with npc, set name to blank so we cant talk to an npc far away
	if (otherNpc.size() == 0) {
		npcName = "";
	}
	
	//Check collisions
	std::vector<Rectangle> others;
	//set vector = the result of the checkTileCollisions function
	//checkTile wants another rectangle to check against. So its going to check all the collisions rect
	//against whatever we give it (player bounding box)
	//if it returns at least 1, handle Tile collision!
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		//Player collided with atleast 1 title
		this->_player.handleTileCollisions(others);
	}

	//Check slope
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	//Check doors
	std::vector<Door> otherDoors;
	if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics, this->_inventory);
	}

	std::vector<Items*> otherItems;
	//if ((otherItems = this->_level.checkItemFloorCollisions(this->_level.)))
	/*
	* TODO: Fix HP loss and find other usages in the future (currently handling in enemy class)
	*/
	//Check enemies
	//std::vector<Enemy*> otherEnemies;
	//if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
	//	this->_player.handleEnemyCollisions(otherEnemies);
	//}

	std::vector<Enemy*> projectileHit;
	if ((projectileHit = this->_level.checkEnemyCollisions(this->_bullet.getProjectileBBox())).size() > 0) {
		this->_bullet.handleProjectileCollisions(projectileHit, graphics);
	}
	this->_level.checkEnemyHP(this->_player, this->_graphics);

	if (pickUp == true) {
		std::vector<Items*> itemPickUp;
		std::vector<std::string*> dropPick;
		if ((itemPickUp = this->_level.checkItemCollisions(this->_player, this->_player.getBoundingBox(), graphics, this->_inventory)).size() > 0) {
		}
		pickUp = false; 
	}

	/*
	std::vector<Npc*> otherNpc;
	if ((otherNpc = this->_level.checkNpcCollisions(this->_player.getBoundingBox(), graphics)).size() > 0) {
		this->_player.handleNpcCollisions(otherNpc, graphics);
	}*/


}


