/* Game Class
* This class holds all information for our main game loop
*/
#include <algorithm>﻿
#include <SDL.h>
#include <SDL_mixer.h>

#include "game.h"
#include "Graphics.h"
#include "Input.h"
#include <vector>
#include <iostream>
#include <fstream>


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


	//data.open("savefile1.txt");
	//if (data.good()) {
	//	if (!data.peek() == data.eof()) {
	//		while (data >> x >> y >> mapname) {
	//			cout << "data: " << x << " , " << y << " " << mapname << endl;
	//			this->_level = Level(mapname, graphics); //intialize level: Map name , spawn point, graphics
	//			spawn = Vector2(std::ceil(x), std::ceil(y));
	//			this->_player = Player(graphics, spawn);
	//			cout << "data retrieved successfully!" << endl;
	//		}
	//	}
	//	else if (data.peek() == std::ifstream::traits_type::eof()) {
	//		cout << "No save data found...Starting new game!" << endl;
	//		this->_level = Level("cave", graphics); //intialize level: Map name , spawn point, graphics
	//		this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	//	}
	//}


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
				std::ofstream ofs("savefile1.txt", std::ios::out | std::ios::trunc);
				ofs.close();

			}
			else {
				data.open("savefile1.txt");
				if (!data.peek() == data.eof()) {
					while (data >> x >> y >> mapname) {
						std::cout << "data: " << x << " , " << y << " " << mapname << std::endl;
						this->_level = Level(mapname, graphics, this->_inventory); //intialize level: Map name , spawn point, graphics
						spawn = Vector2(std::ceil(x), std::ceil(y));
						this->_player = Player(graphics, spawn);
						std::cout << "data retrieved successfully!" << std::endl;
						data.close();
					}
				}
				else {
					std::cout << "No save data found...Starting new game!" << std::endl;
					this->_level = Level("cave", graphics, this->_inventory); //intialize level: Map name , spawn point, graphics
					this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
				}
			}
			
		}
		//if (title == true) {
		//		const int CURRENT_TIME_MS = SDL_GetTicks();
		//		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		//		if (SDL_PollEvent(&event)) {
		//			if (event.type == SDL_KEYDOWN) {
		//				if (event.key.repeat == 0) {
		//					input.keyDownEvent(event); //if we are holding key start keydown event
		//				}
		//			}
		//			else if (event.type == SDL_KEYUP) { // if key was released
		//				input.keyUpEvent(event);
		//			}
		//			else if (event.type == SDL_QUIT) {
		//				return; //when the game ends or user exits
		//			}
		//		}
		//		if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) { //IF is imporannt make sure not to do else if otherwise u cannot move and jump!
		//			title = false;
		//		}

		//		this->_graphics = graphics; //updated graphics
		//		this->updateTitle(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME)); //take standard min : elapsed time ms and max frame time

		//		LAST_UPDATE_TIME = CURRENT_TIME_MS; //loop will go again and current time - new last update will tell us how long next frame will take

		//		this->drawTitle(graphics);
		//}
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
			std::ofstream saveFile;
			int x, y, saveNum;
			std::string mapName;

			x = this->_player.getX();
			y = this->_player.getY();
			mapName = this->_player.getMap();

			if (saveFile.good()) {
				saveFile.open("savefile1.txt");
				saveFile << x << std::endl << y << std::endl << mapName;
				saveFile.close();
				std::cout << "Save file succesfully overwritten!" << std::endl;
			}
			else if (!saveFile.good()) {
				std::cout << "Save file not found!" << std::endl;
			}

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
				std::cout << "status of talk: " << activeTalk << std::endl;
				this->_chatBox.setTextStatus(true);
				this->_chatBox.drawChatBox(graphics, this->_player);
				this->_npc.runScript(npcName, graphics, this->_player.getX(), this->_player.getY());
			}
			else if (activeTalk == true) {
				activeTalk = false;
				std::cout << "status of talk: " << activeTalk << std::endl;
				this->_chatBox.setTextStatus(false);

			}
		}

		if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true && activeTalk == true) {
			lineNum = _npc.getLineAmount();
			currentLine++;
			std::cout << "lineNum: " << lineNum << std::endl;
			std::cout << "currentLine: " << currentLine << std::endl;
			if (currentLine < lineNum) {
				this->_npc.playNextScript(npcName, graphics, this->_player.getX(), this->_player.getY(), currentLine);
				nextLine = true;
			}
			else if (currentLine >= lineNum) {
				nextLine = false;
				activeTalk = false;
				this->_chatBox.setTextStatus(false);
				this->_npc.setEmpty();
				std::cout << "ran out of lines" << std::endl;
				currentLine = 0;
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

			if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
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
				//this->_inventory.draw(graphics, this->_player);
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

	this->_level.draw(graphics); //need to draw level before player (below) so player is on top of level and not behind it!
	this->_player.draw(graphics); //what and where to draw
	this->_bullet.draw(graphics, this->_player);
	this->_bullet.drawUp(graphics, this->_player);
	this->_bullet.drawDown(graphics, this->_player);
	this->_bullet.drawLeft(graphics, this->_player);
	this->_bullet.drawGun(graphics, this->_player);
	this->_chatBox.drawChatBox(graphics, this->_player);
	this->_bullet.drawDmgText(graphics);

	if (activeTalk == true && nextLine == false) {
		this->_npc.runScript(npcName, graphics, this->_player.getX(), this->_player.getY());
	}

	if (activeTalk == true && nextLine == true) {
		this->_npc.playNextScript(npcName, graphics, this->_player.getX(), this->_player.getY(), currentLine);
	}

	this->_hud.draw(graphics, this->_player);
	if (activeInventory == true) {
		this->_inventory.draw(graphics, this->_player);
	}
	if (activeStatMenu)
		this->_player.drawStatMenu(graphics, this->_player, selection);
		

	this->_player.drawCurrentMapName(graphics);

	graphics.flip(); //Render everything above
}

void Game::updateBullet(float elapsedTime) {
	this->_bullet.update(elapsedTime, this->_player);
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

	//Check enemies
	std::vector<Enemy*> otherEnemies;
	if ((otherEnemies = this->_level.checkEnemyCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleEnemyCollisions(otherEnemies);
	}

	std::vector<Enemy*> projectileHit;
	if ((projectileHit = this->_level.checkEnemyCollisions(this->_bullet.getProjectileBBox())).size() > 0) {
		this->_bullet.handleProjectileCollisions(projectileHit, graphics);
	}
	this->_level.checkEnemyHP(this->_player);

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


