/*
* Game Class
* This class holds all information for our main game loop
*/

#ifndef XMLCheckResult
	#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif

#include "../headers/tinyxml2.h"
#include "../headers/game.h"
#include "../headers/Graphics.h"
#include "../headers/Input.h"

#include <algorithm>﻿
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>

using namespace tinyxml2;
using namespace CryptoPP;

namespace {
	const int FPS = 50;
	// remove 5 *
	const int MAX_FRAME_TIME = 5* 1000 / FPS; // Max amount of time a frame is allowed to last
	int bgmVolume = 100; // Refers to all types of sounds (BGM/Effects/etc.)
	int sfxVolume = 100;
	int pauseBlockTimer = 0;
	int selection = 1;
	int npcSelection = 1;
	int questSelection = 1;
	int saveSelection = 1;
	int lineNum = 0;
	int currentLine = 0;
	int sceneTimer = 0;
	int sceneMaxTime = 0;
	int sceneType = 0;
	int sceneLines = 0;
	int sceneLineCounter = 1;
	int npcID = 0;
	char sceneLineChar = 'a';

	bool title = true;
	bool GAMEOVER = false;
	bool activeTalk = false;
	bool activeInventory = false;
	bool activeStatMenu = false;
	bool activeProjectile = false;
	bool activeCutscene = false;
	bool activeSave = false;
	bool activeSaveMenu = false;
	bool pickUp = false;
	bool nextLine = false;
	bool stopScroll = false;
	bool sceneTalkDone = false;
	bool isClimbing = false;
	bool jetPack = false;
	bool resetGame = false;
	bool pauseGame = false;
	bool showPlayerOutline = false;
	bool showEnemyOutline = false;
	bool showCollisionOutline = false;
	bool windowIsBeingDragged = false;
	bool findWindowElapsedTime = false;
	bool hasSelectedQuest = false;

	float sceneX = 0;
	float sceneY = 0;
	float targetX = 0;
	float targetY = 0;
	float xMod = 0;
	float yMod = 0;

	std::string npcName;
	std::string sceneDir;
	std::string sceneName;
	std::string BGM = "Astral.ogg";

	Mix_Music *gMusic = NULL;
	Mix_Chunk *sePlHit = NULL;
	Mix_Chunk *sePlDie = NULL;
	Mix_Chunk *enHurt = NULL;
	Mix_Chunk *gameOver = NULL;
	bool walkSound = false;
	bool deathSound = false;
	bool jumpSound = false;
}

Game::Game() { // Constructor
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	// Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
	}
	// TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 24);
	this->cipher = AESCipher();
	this->gameLoop(); // Start game
}

Game::~Game() {}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;
	// Music
	Mix_AllocateChannels(350);
	Mix_Chunk *sBullet = NULL;
	Mix_Chunk *seWalk = NULL;
	Mix_Chunk *seJump = NULL;

	gMusic = Mix_LoadMUS("data\\sound\\Astral.ogg");
	sBullet = Mix_LoadWAV("data\\sound\\sbullet.ogg");
	seWalk = Mix_LoadWAV("data\\sound\\seWalk.ogg");
	seJump = Mix_LoadWAV("data\\sound\\seJump.ogg");
	sePlHit = Mix_LoadWAV("data\\sound\\sePlHit.ogg");
	sePlDie = Mix_LoadWAV("data\\sound\\sePlDie.ogg");
	enHurt = Mix_LoadWAV("data\\sound\\enemyHurt.ogg");
	gameOver = Mix_LoadWAV("data\\sound\\gameOver.ogg");
	Mix_PlayChannel(321, seWalk, -1);
	Mix_Pause(321);
	Mix_VolumeChunk(seWalk, MIX_MAX_VOLUME + 22);

	this->_title = Title(graphics, input, event);
	this->_gameOver = GameOver(graphics);
	this->_camera = Camera();
	this->_chatBox = TextManager(graphics, this->_player);
	this->_hud = HUD(graphics, this->_player);
	this->_inventory = Inventory(graphics, this->_player);

	this->setSettings();

	if (gMusic == NULL)
	{
		printf("Failed to load specified music! SDL_mixer Error: %s\n", Mix_GetError());
	}
	else
	{
		Mix_PlayMusic(gMusic, -1);
		Mix_VolumeMusic(bgmVolume);
	}
		

	int LAST_UPDATE_TIME = SDL_GetTicks(); 
	// Above ^ gets the amount of milliseconds since the SDL library was initialized
	// Must start before loop
	// Start the game loop
	while (true) {
		input.beginNewFrame();
// Title screen Loop
		if (title == true) {
			title = _title.Start(graphics, input, event);
			if (_title.getMenuChoice() == 0) {
				this->_level = Level("cave", graphics, this->_inventory); // Initialize level: Map name , spawn point, graphics
				this->_level.generateItems(graphics);
				this->_level.generateMapItems(graphics, this->_level.getMapName(), this->_inventory);
				this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
				this->_inventory.addItem(0, 1);
				this->_level.generateEnemies(graphics, this->_level.getMapName(), this->_player);
				this->_level.generateEffects(graphics, this->_player);
				if (!cipher.verifyHash("cave", this->_player)) {
					std::exit(0);
				}
				this->saveGame(graphics);
			}
			else {
				this->loadGame(graphics);
			}
		}
// Death Loop
		if (title == false && GAMEOVER == true) {
			const int CURRENT_TIME_MS = SDL_GetTicks();
			int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

			if (!Mix_PausedMusic())
				Mix_PauseMusic();

			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.repeat == 0) {
						input.keyDownEvent(event); // If we are holding key start keydown event
					}
				}
				else if (event.type == SDL_KEYUP) { // If key was released
					input.keyUpEvent(event);
				}
				else if (event.type == SDL_QUIT) {
					return; // When the game ends or user exits
				}
			}
			if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true && GAMEOVER == true) {
				Mix_ResumeMusic();
				Mix_RewindMusic();
				this->loadGame(graphics);
				deathSound = false;
				GAMEOVER = false;
				resetGame = false;
			}

			this->_graphics = graphics; // Updated graphics
			// Take standard min : elapsed time ms and max frame time
			this->updateGameOver(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));

			// Loop will go again and current time - new last update will tell us how long next frame will take
			LAST_UPDATE_TIME = CURRENT_TIME_MS;
			
			this->drawGameOver(graphics);
		}
// Cutscene Loop
		if (title == false && GAMEOVER == false && activeCutscene == true) {
			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.repeat == 0) {
						input.keyDownEvent(event); // If we are holding key start keydown event
					}
				}
				else if (event.type == SDL_KEYUP) { // If key was released
					input.keyUpEvent(event);
				}
				else if (event.type == SDL_QUIT) {
					return; // When the game ends or user exits
				}
			}

			if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
				this->_level.removeCutscene("");
				activeCutscene = false;
				sceneTimer = 0;
				sceneLineChar = 'a';
				sceneLineCounter = 1;
				sceneMaxTime = 0;
				sceneX = 0;
				sceneY = 0;
				targetX = 0;
				targetY = 0;
				stopScroll = false;
				// Input.beginNewFrame();
				this->_player.stopLookingDown();
				this->_player.addCutSceneTable(sceneName);
				continue;
			}
			const int CURRENT_TIME_MS = SDL_GetTicks();
			int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
			this->_graphics = graphics; // Updated graphics
			// Take standard min : elapsed time ms and max frame time
			this->updateCutscene(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
			LAST_UPDATE_TIME = CURRENT_TIME_MS;
			this->drawCutscene(graphics);
		}
// Main Game Loop
		if (title == false && GAMEOVER == false && activeCutscene == false) {
			if (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					if (event.key.repeat == 0) {
						input.keyDownEvent(event); // If we are holding key start keydown event
					}
				}
				else if (event.type == SDL_KEYUP) { // If key was released
					input.keyUpEvent(event);
				}
				else if (event.type == SDL_QUIT) {
					return; // When the game ends or user exits
				}
				else if (event.type == SDL_WINDOWEVENT) {
					if (event.window.event == SDL_WINDOWEVENT_MOVED) {
						windowIsBeingDragged = true;
					}
					else if (event.window.event == SDL_WINDOWEVENT_EXPOSED) {
						windowIsBeingDragged = false;
						// We don't want to update LAST_UPDATE_TIME just yet. So, set this to true and
						// once we are right before we calculated elapsed time, update LAST_UPDATE_TIME
						// so that it is up to date with the SDL library. Then set this bool to false.
						findWindowElapsedTime = true;
					}
					if (event.window.event == SDL_WINDOWEVENT_MINIMIZED || event.window.event == SDL_WINDOWEVENT_MAXIMIZED) {
						windowIsBeingDragged = true;
					}
					else if (event.window.event == SDL_WINDOWEVENT_RESTORED || event.window.event == SDL_WINDOWEVENT_EXPOSED) {
						windowIsBeingDragged = false;
						// We don't want to update LAST_UPDATE_TIME just yet. So, set this to true and
						// once we are right before we calculated elapsed time, update LAST_UPDATE_TIME
						// so that it is up to date with the SDL library. Then set this bool to false.
						findWindowElapsedTime = true;
					}
				}
			}

			if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) && this->_player.getCurrentHealth() > 0) {
				if (pauseBlockTimer == 0) {
					pauseGame = !pauseGame;
					pauseBlockTimer++;
				}
				else {
					pauseGame = false;
				}
				std::cout << "Game pause state = " << pauseGame << std::endl;
			} 

			if (!pauseGame) {

				if (activeSave && input.wasKeyPressed(SDL_SCANCODE_A) && this->_player.getCurrentHealth() > 0) {
					activeSaveMenu = true;
				}

				if (activeSaveMenu) {
					if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
						if (saveSelection == 1) {
							this->saveGame(graphics);
							this->_player.setEventMessage("SAVED SUCESSFULLY.");
						}
						activeSaveMenu = false;
						activeSave = false;
					}
					if (input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
						if (saveSelection == 1)
							this->_player.drawSaveMenu(graphics, this->_player, saveSelection);
						else if (saveSelection != 1) {
							saveSelection--;
							this->_player.drawSaveMenu(graphics, this->_player, saveSelection);
						}
					}
					else if (input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
						if (saveSelection == 2)
							this->_player.drawSaveMenu(graphics, this->_player, saveSelection);
						else if (saveSelection != 2) {
							saveSelection++;
							this->_player.drawSaveMenu(graphics, this->_player, saveSelection);
						}
					}
				}

				// Bullet
				if (input.wasKeyPressed(SDL_SCANCODE_X) && input.isKeyHeld(SDL_SCANCODE_UP) && input.isKeyHeld(SDL_SCANCODE_RIGHT)
					&& this->_player.getCurrentHealth() > 0 && !activeSaveMenu) {
					std::cout << "Bullet right up diag test..." << std::endl;
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && activeCutscene == false) {
						this->_level.generateProjectile(graphics, this->_player);
						Mix_PlayChannel(-1, sBullet, 0);
					}
				}

				else if (input.wasKeyPressed(SDL_SCANCODE_X) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && activeCutscene == false
						&& !activeSaveMenu) {
						/*if (input.isKeyHeld(SDL_SCANCODE_UP) && input.isKeyHeld(SDL_SCANCODE_RIGHT)) {
							this->_level.generateProjectile(graphics, this->_player);
							Mix_PlayChannel(-1, sBullet, 0);
						}*/
						/*else if (input.isKeyHeld(SDL_SCANCODE_UP) && input.isKeyHeld(SDL_SCANCODE_LEFT)) {
							this->_level.generateProjectile(graphics, this->_player);
							Mix_PlayChannel(-1, sBullet, 0);
						}*/
						this->_level.generateProjectile(graphics, this->_player);
						Mix_PlayChannel(-1, sBullet, 0);
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true && this->_player.getCurrentHealth() > 0) {
					activeInventory = false;
					activeSave = false;
					activeSaveMenu = false;
					activeTalk = false;
					this->_chatBox.setTextStatus(false);
					this->_npc.setNpcTalk(false);
					this->_npc.setQuestMenuState(false);
					this->_npc.resetScripts();
					npcSelection = 1;
					questSelection = 1;
					activeStatMenu = false;
				}
				// If (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true && this->_player.getCurrentHealth() > 0
				//	&& !activeCutscene && !activeInventory && !activeStatMenu && !activeTalk) {

				//	// This->saveGame(graphics);
				//	// Std::cout << "Quitting Game..." << std::endl;
				//	// Return; // Quit game if ESC was pressed
				//}
				else if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.moveLeft();
						if (this->_player.isGrounded() && walkSound == false) {
							// Mix_PlayChannel(321, seWalk, -1);
							Mix_Resume(321);
							walkSound = true;
						}
					}
					else if (activeTalk == true) {
						std::cout << "impaired action" << std::endl;
					}

				}
				else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.moveRight();
						if (this->_player.isGrounded() && walkSound == false) {
							Mix_Resume(321);
							walkSound = true;
						}

					}
					else if (activeTalk == true) {
						std::cout << "impaired action" << std::endl;
					}

				}

				if (input.wasKeyReleased(SDL_SCANCODE_LEFT) == true || input.wasKeyReleased(SDL_SCANCODE_RIGHT) == true) {
					Mix_Pause(321);
					walkSound = false;
				}

				/*if (isClimbing && input.wasKeyPressed(SDL_SCANCODE_UP)) {
					this->_player.setClimbing(true);
				}*/

				if (input.isKeyHeld(SDL_SCANCODE_UP) == true &&
					(!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.wasKeyPressed(SDL_SCANCODE_LEFT)
						&& !input.isKeyHeld(SDL_SCANCODE_RIGHT) && !input.wasKeyPressed(SDL_SCANCODE_RIGHT)
						&& !input.isKeyHeld(SDL_SCANCODE_DOWN) && !input.wasKeyPressed(SDL_SCANCODE_DOWN))
					&& this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.lookUp();
						if (isClimbing) {
							this->_player.setClimbing(true);
							this->_player.moveUp();
						}
					}
				}
				if (input.isKeyHeld(SDL_SCANCODE_UP) == true /*&&
					(!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.wasKeyPressed(SDL_SCANCODE_LEFT)
						&& !input.isKeyHeld(SDL_SCANCODE_RIGHT) && !input.wasKeyPressed(SDL_SCANCODE_RIGHT)
						&& !input.isKeyHeld(SDL_SCANCODE_DOWN) && !input.wasKeyPressed(SDL_SCANCODE_DOWN))*/
					&& this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.lookUp();
					}
				}
				else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true &&
					(!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.wasKeyPressed(SDL_SCANCODE_LEFT)
						&& !input.isKeyHeld(SDL_SCANCODE_RIGHT) && !input.wasKeyPressed(SDL_SCANCODE_RIGHT)
						&& !input.isKeyHeld(SDL_SCANCODE_UP) && !input.wasKeyPressed(SDL_SCANCODE_UP))
					&& this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.lookDown();
						if (isClimbing) {
							this->_player.setClimbing(true);
							this->_player.moveDown();
						}
					}
				}
				if (input.wasKeyReleased(SDL_SCANCODE_UP) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu)
						this->_player.stopLookingUp();
				}
				if (input.wasKeyReleased(SDL_SCANCODE_DOWN) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu)
						this->_player.stopLookingDown();
				}

				if (input.isKeyHeld(SDL_SCANCODE_SPACE) && jetPack && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.useJetPack();
					}
				}

				else if (!jetPack && input.isKeyHeld(SDL_SCANCODE_SPACE) == true && this->_player.getCurrentHealth() > 0) {
					if (activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.jump();
						if (!jumpSound)
							Mix_PlayChannel(-1, seJump, 0);
						jumpSound = true;
					}
					else if (activeTalk == true) {
						std::cout << "impaired action" << std::endl;
					}
				}
				if (input.wasKeyReleased(SDL_SCANCODE_SPACE) && this->_player.getCurrentHealth() > 0) {
					if (this->_player.canShortJump() && activeTalk == false && activeInventory == false && activeStatMenu == false && !activeSaveMenu) {
						this->_player.setPlayerDY(0);
						this->_player.setShortJump(false);
						jumpSound = false;
					}
				}
				if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT) && this->_player.getCurrentHealth() > 0) {
					// If player isnt moving left or right(at all) do stopMoving function
					this->_player.stopMoving();
				}

				if (input.wasKeyPressed(SDL_SCANCODE_A) == true && activeInventory == false && activeStatMenu == false
					&& this->_player.getCurrentHealth() > 0 && !activeSaveMenu) {
					if (activeTalk == false && npcName != "") {
						activeTalk = true;
						this->_chatBox.setTextStatus(true);
						this->_chatBox.drawChatBox(graphics, this->_player);
						this->_npc.setNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
						this->_npc.drawNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
						this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
						this->_npc.loadQuests(npcID);
					}
					else if (activeTalk == true) {
						activeTalk = false;
						this->_chatBox.setTextStatus(false);
						this->_npc.setNpcTalk(false);
						this->_npc.setQuestMenuState(false);
						this->_npc.resetScripts();
						npcSelection = 1;
						questSelection = 1;
						hasSelectedQuest = false;
					}
				}

				if (activeTalk) {
					if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true && this->_player.getCurrentHealth() > 0) {
						/* Chat */
						if (npcSelection == 1 && this->_npc.getNpcTalk() == false) {
							this->_npc.setNpcTalk(true);
							this->_npc.playScript(npcID, graphics, this->_player.getX(), this->_player.getY());
						}
						else if (npcSelection == 1 && this->_npc.getNpcTalk()) {
							this->_npc.playNext(npcID, graphics, this->_player.getX(), this->_player.getY(), this->_player);
							if (this->_npc.getChatStatus()) {
								this->_npc.setNpcTalk(false);
								activeTalk = false;
								this->_chatBox.setTextStatus(false);
								this->_npc.resetScripts();
							}
						}
						/* Quests */
						else if (npcSelection == 2 && this->_npc.getNpcTalk() == false) {
							this->_npc.setNpcTalk(true);
							this->_npc.setQuestMenuState(true);
							this->_npc.displayQuests(graphics, npcID, this->_player.getX(), this->_player.getY(), this->_player);
						}
						else if (npcSelection == 2 && this->_npc.getNpcTalk() && !hasSelectedQuest) {
								// Here we will call playQuest()
								// This function is similar to playScript() as it will initialize the dialogue table
								// How many lines are calculated and the table stores the text(s) and quest name
								// Quest selection is passed to know which quest name for this NPCID

								// Also check using checkQuestDone(int questSelection...etc) within playQuest()
								// to see if the quest is complete so we can show the quest finished dialogue instead

								this->_npc.playQuest(npcID, questSelection, graphics, this->_player.getX(), this->_player.getY(),
									this->_player);

							   if (!this->_npc.checkQuestBlocked())
								   hasSelectedQuest = true;
						}

						else if (npcSelection == 2 && this->_npc.getNpcTalk() && hasSelectedQuest) {
							// Continue to the next quest dialogue (PlayNextQuest() function)
							// This function will handle showing the next dialogue when pressing enter
							// Once all dialogue has been displayed, accept the quest within the same call (only once)
							// It will also read the next line for quest finish dialogue.
							// Once at the end it can give the rewards and set it to complete
							this->_npc.playNextQuest(npcID, graphics, this->_player.getX(), this->_player.getY(),
								this->_player, this->_inventory);
							if (this->_npc.getChatStatus()) {
								activeTalk = false;
								this->_chatBox.setTextStatus(false);
								this->_npc.setNpcTalk(false);
								this->_npc.setQuestMenuState(false);
								this->_npc.resetScripts();
								npcSelection = 1;
								questSelection = 1;
								hasSelectedQuest = false;
							}
						}
					}

					if (input.wasKeyPressed(SDL_SCANCODE_UP) == true && this->_npc.getNpcTalk() == false
						&& this->_npc.getQuestMenuState() == false) {
						if (npcSelection == 1)
							this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
						else if (npcSelection != 1) {
							npcSelection--;
							this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
						}
					}
					else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true && this->_npc.getNpcTalk() == false
						&& this->_npc.getQuestMenuState() == false) {
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

				if (input.wasKeyPressed(SDL_SCANCODE_D) == true && this->_player.getCurrentHealth() > 0) {
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

				if (input.wasKeyPressed(SDL_SCANCODE_S) == true && activeStatMenu == false && activeTalk == false
					&& this->_player.getCurrentHealth() > 0) {
					if (activeInventory == false) {
						activeInventory = true;
						this->_inventory.draw(graphics, this->_player);
					}

					else if (activeInventory == true) {
						activeInventory = false;
					}
				}

				if (input.wasKeyPressed(SDL_SCANCODE_Z) == true && this->_player.getCurrentHealth() > 0) {
					if (pickUp == false) {
						pickUp = true;
					}
					else if (pickUp == true) {
						pickUp = false;
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_C) == true && this->_player.getCurrentHealth() > 0) {
					if (this->_player.hasHpPot()) {
						_inventory.useItem(0, this->_player);
						this->_player.subtractHpPot();
					}
					
				}
				if (input.wasKeyPressed(SDL_SCANCODE_1) == true) {
					// Add weapon swap here
				}
				if (input.wasKeyPressed(SDL_SCANCODE_2) == true) {
					// Add weapon swap here
				}
				if (input.wasKeyPressed(SDL_SCANCODE_3) == true) {
					if (this->_inventory.checkItem(2100, 1)) {
						jetPack = !jetPack;
						std::cout << "JetPack state = " << jetPack << std::endl;
					}
				}
				if (input.wasKeyPressed(SDL_SCANCODE_F5) == true) {
					showPlayerOutline = !showPlayerOutline;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_F6) == true) {
					showEnemyOutline = !showEnemyOutline;
				}
				if (input.wasKeyPressed(SDL_SCANCODE_F7) == true) {
					showCollisionOutline = !showCollisionOutline;
				}
				if (this->_player.getCurrentHealth() <= 0 && !this->_player.checkDeathPlayed()) {
					if (!deathSound) {
						Mix_PlayChannel(-1, sePlDie, 0);
						deathSound = true;
					}
					this->_player.startDeath();
				}
			}
			if (findWindowElapsedTime) {
				// Window was moved, so update the LAST_UPDATE_TIME to be near the SDL_GetTicks() time.
				// This prevents the elapsed time from becoming large thus messing with the game logic.
				LAST_UPDATE_TIME = SDL_GetTicks() - 1;
				findWindowElapsedTime = false;
			}
			const int CURRENT_TIME_MS = SDL_GetTicks();
			int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
			this->_graphics = graphics; // Updated graphics
			// Take standard min : elapsed time ms and max frame time
			this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
			// Loop will go again and current time - new last update will tell us how long next frame will take
			LAST_UPDATE_TIME = CURRENT_TIME_MS;
			this->draw(graphics);
			// SDL_GetTicks() will still increment while pauseGame = this->_title(...) runs it's own loop.
			// To account for the time difference, we simply set LAST_UPDATE_TIME = SDL_GetTicks() - 1
			// This is to ensure a smooth and consistent elapsed time for our next iteration's update/drawing logic.
			if (pauseGame) {
				Mix_PauseMusic();
				pauseGame = this->_title.Pause(graphics, input, event, this->_player);
				this->setSettings();
				Mix_ResumeMusic();
				LAST_UPDATE_TIME = SDL_GetTicks() - 1;
				if (_title.getMenuChoice() == 0 && _title.getReload()) {
					this->loadGame(graphics);
					this->_title.setReload();
					Mix_RewindMusic();
				}
			}
		}
	}
}

void Game::drawTitle(Graphics &graphics) {
	graphics.clear(); // Clear any drawings MUST do
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
	graphics.clear(); // Clear any drawings MUST do
	this->_level.draw(graphics, this->_player);
	// Need to draw level before player (below) so player is on top of level and not behind it!
	this->_player.draw(graphics);
	this->_chatBox.drawChatBox(graphics, this->_player);
	if (activeTalk == true) {
		if (this->_npc.getNpcTalk() && npcSelection == 1) {
			this->_npc.repeatScript(graphics, this->_player.getX(), this->_player.getY());
		}
		else if (this->_npc.getNpcTalk() && npcSelection == 2 && this->_npc.getQuestMenuState() && !hasSelectedQuest) {
			this->_npc.displayQuests(graphics, npcID, this->_player.getX(), this->_player.getY(), this->_player);
			this->_npc.questSelection(graphics, this->_player.getX(), this->_player.getY(), questSelection);
		}
		else if (this->_npc.getNpcTalk() && npcSelection == 2 && this->_npc.getQuestMenuState() && hasSelectedQuest) {
			this->_npc.repeatQuestScript(graphics, this->_player.getX(), this->_player.getY()); 
		}
		else {
			this->_npc.npcSelection(graphics, this->_player.getX(), this->_player.getY(), npcSelection);
		}
		this->_npc.drawNpcIcon(graphics, npcName, this->_player.getX(), this->_player.getY());
	}
	this->_hud.draw(graphics, this->_player);
	if (activeInventory == true)
		this->_inventory.draw(graphics, this->_player);
	if (activeStatMenu)
		this->_player.drawStatMenu(graphics, this->_player, selection);
	if (activeSaveMenu)
		this->_player.drawSaveMenu(graphics, this->_player, saveSelection);
	this->_player.drawHPNumbers(graphics);
	this->_player.drawExpNumbers(graphics);
	this->_player.drawCurrentMapName(graphics);
	if (showPlayerOutline)
		this->_player.drawPlayerOutline(graphics);
	if (showEnemyOutline)
		this->_level.drawEnemyOutline(graphics);
	if (showCollisionOutline)
		this->_level.drawCollisionOutline(graphics);
	graphics.flip(); // Render everything above
}

void Game::drawCutscene(Graphics &graphics) {
	if (sceneType == 1) {
		graphics.clear();
		this->startCutscene(sceneName);
	}

	else if (sceneType == 2) {
		this->_level.draw(graphics, this->_player);
		// Need to draw level before player (below) so player is on top of level and not behind it!
		this->_player.draw(graphics);
	}
	graphics.flip(); // Render everything above
}

int Game::loadCutscene(std::string name)
{
	XMLDocument xml;
	std::string fileName = name + ".xml";

	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "cutscenes";
	cwd.append(name + ".xml");
	xml.LoadFile(cwd.string().c_str());
	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	XMLElement* element = root->FirstChildElement("Type");
	element->QueryIntAttribute("type", &sceneType);
	if (sceneType == 1) {
		element = root->FirstChildElement("Talk");
		element->QueryIntAttribute("textTime", &sceneMaxTime);
		result = element->QueryIntAttribute("lines", &sceneLines);
	}
	if (sceneType == 2) {
		element = root->FirstChildElement("Camera");
		const char* textPtr;
		std::string text;
		textPtr = element->Attribute("mob");
		text = textPtr;
		targetX = this->_level.getSceneX(text);
		targetY = this->_level.getSceneY(text);
		element->QueryFloatAttribute("xmod", &xMod);
		element->QueryFloatAttribute("ymod", &yMod);
		element->QueryIntAttribute("cTime", &sceneMaxTime);
		textPtr = element->Attribute("dir");
		sceneDir = textPtr;
	}
}

int Game::startCutscene(std::string name)
{
	XMLDocument xml;
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "cutscenes";
	cwd.append(name + ".xml");
	xml.LoadFile(cwd.string().c_str());
	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	XMLElement* element = root->FirstChildElement("Type");
	element->QueryIntAttribute("type", &sceneType);
	if (sceneType == 1) {
		element = root->FirstChildElement("Talk");
		result = element->QueryIntAttribute("lines", &sceneLines);
		const char* textPtr = nullptr;
		if (sceneLineCounter <= sceneLines) {
			textPtr = element->Attribute((char*)&sceneLineChar);
			std::string text;
			text = textPtr;
			if (text.length() > 80)
				sceneMaxTime = 6500;
			else
				element->QueryIntAttribute("textTime", &sceneMaxTime);
			this->_player.showSceneDialogue(this->_graphics, text);
			XMLCheckResult(result);
		}
	}
}

int Game::saveGame(Graphics & graphics)
{
	XMLDocument xml;
	XMLNode* root = xml.NewElement("Root");
	xml.InsertFirstChild(root);
	std::cout << "Creating the XML Document..." << std::endl;

	// Save player location
	XMLElement* element = xml.NewElement("Spawn");
	element->SetAttribute("mapName", this->_level.getMapName().c_str());
	element->SetAttribute("xCoordinate", this->_player.getX());
	element->SetAttribute("yCoordinate", this->_player.getY());
	root->InsertEndChild(element);
	// Save player stats
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
	element->SetAttribute("hpPot", this->_player.getHpPotCapacity());
	element->SetAttribute("hpPotStr", this->_player.getHpPotStrength());
	root->InsertEndChild(element);
	// Save loot table
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
	// Save inventory
	element = xml.NewElement("Inventory");
	std::map<Items::ItemID, int> iVec = this->_inventory.getInventoryTable();
	for (auto iter = iVec.begin(); iter != iVec.end(); iter++) {
		auto first = iter->first, second = iter->second;
		XMLElement* ptrElement = xml.NewElement("iTable");
		ptrElement->SetAttribute("itemID", first);
		ptrElement->SetAttribute("quantity", second);
		element->InsertEndChild(ptrElement);
	}
	root->InsertEndChild(element);
	// Save the quest log
	element = xml.NewElement("QuestLog");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	std::vector<std::tuple<std::string, int, std::string, int, std::vector<std::string>, std::vector<std::string>,
		int, int, int, int, int, bool>> qVec = this->_npc.getQuestTable();
	for (int counter = 0; counter < qVec.size(); ++counter) {
		XMLElement* qElement = xml.NewElement("Quest");
		qElement->SetAttribute("questName", std::get<0>(qVec[counter]).c_str());
		qElement->SetAttribute("type", std::get<1>(qVec[counter]));
		qElement->SetAttribute("object", std::get<2>(qVec[counter]).c_str());
		qElement->SetAttribute("amount", std::get<3>(qVec[counter]));
		/* 
		 * Append all the dialogue into one string that is delimited by a tilde (~)
		 * This is because we want the Attribute to store the entire string vector
		 * And when we load the save file, we can use the delimiter to make them
		 * back into their own sentences and store it into the string vector
		 */
		std::stringstream qd, fd;
		for (const auto& qDialogue : std::get<4>(qVec[counter])) {
			qd << qDialogue << "~";
		}
		std::string qDiaStr = qd.str();
		qElement->SetAttribute("questDialogue", qDiaStr.c_str());
		/* Do the same for finishDialogue vector */
		for (const auto& fDialogue : std::get<5>(qVec[counter])) {
			fd << fDialogue << "~";
		}
		std::string fDiaStr = fd.str();
		qElement->SetAttribute("finishDialogue", fDiaStr.c_str());
		qElement->SetAttribute("npcID", std::get<6>(qVec[counter]));
		qElement->SetAttribute("rewardItemID", std::get<7>(qVec[counter]));
		qElement->SetAttribute("rewardAmount", std::get<8>(qVec[counter]));
		qElement->SetAttribute("exp", std::get<9>(qVec[counter]));
		qElement->SetAttribute("cels", std::get<10>(qVec[counter]));
		qElement->SetAttribute("isCompleted", std::get<11>(qVec[counter]));
		element->InsertEndChild(qElement);
	}
	root->InsertEndChild(element);
	// Save the kill table
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
	// Save the boss table
	element = xml.NewElement("BossTable");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	std::vector<std::tuple<std::string, std::string, float, float, bool>> btVec = this->_player.getBossTable();
	for (int counter = 0; counter < btVec.size(); ++counter) {
		XMLElement* bElement = xml.NewElement("bTable");
		bElement->SetAttribute("mob", std::get<0>(btVec[counter]).c_str());
		bElement->SetAttribute("map", std::get<1>(btVec[counter]).c_str());
		bElement->SetAttribute("posX", std::get<2>(btVec[counter]));
		bElement->SetAttribute("posY", std::get<3>(btVec[counter]));
		bElement->SetAttribute("completed", std::get<4>(btVec[counter]));
		element->InsertEndChild(bElement);
	}
	root->InsertEndChild(element);
	// Cutscenes
	element = xml.NewElement("SceneTable");
	std::vector<std::string> csVec = this->_player.getSceneTable();
	for (int counter = 0; counter < csVec.size(); ++counter) {
		XMLElement* csElement = xml.NewElement("csTable");
		csElement->SetAttribute("sceneName", csVec[counter].c_str());
		element->InsertEndChild(csElement);
	}
	root->InsertEndChild(element);
	// Locked Doors (unlocked)
	element = xml.NewElement("LockedDoorTable");
	std::vector<std::string> ldVec = this->_player.getLockedDoorTable();
	for (int counter = 0; counter < ldVec.size(); ++counter) {
		XMLElement* ldElement = xml.NewElement("ldTable");
		ldElement->SetAttribute("lockName", ldVec[counter].c_str());
		element->InsertEndChild(ldElement);
	}
	root->InsertEndChild(element);
	// Equipment
	element = xml.NewElement("EquipmentTable");
	std::vector<std::string> eqVec = this->_player.getEquipmentTable();
	for (int counter = 0; counter < eqVec.size(); ++counter) {
		XMLElement* eqElement = xml.NewElement("eqTable");
		eqElement->SetAttribute("equipName", eqVec[counter].c_str());
		element->InsertEndChild(eqElement);
	}
	root->InsertEndChild(element);
	// Finalize save
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "profile";
	cwd.append("temp.xml");
	XMLError result = xml.SaveFile(cwd.string().c_str());
	std::filesystem::path nCwd = std::filesystem::current_path() / "data" / "profile";
	nCwd.append("SF-LOC.xml");
	this->cipher.AESEncrypt(cwd.string(), nCwd.string());
	XMLCheckResult(result);

	/* Once save is complete, refill the HP Pot and respawn the enemies except bosses */
	this->_player.refillHpPot();
	this->_player.clearDespawnTable();
	return 0;
}

int Game::loadGame(Graphics & graphics)
{
	XMLDocument xml;
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "profile";
	cwd.append("SF-LOC.xml");
	std::filesystem::path nCwd = std::filesystem::current_path() / "data" / "profile";
	nCwd.append("temp.xml");
	this->cipher.AESDecrypt(cwd.string(), nCwd.string());
	xml.LoadFile(nCwd.string().c_str());

	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	// Load loot table
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
	// Load Inventory
	element = root->FirstChildElement("Inventory");
	ptrVec = element->FirstChildElement("iTable");
	std::map<Items::ItemID, int> iVec;
	while (ptrVec != nullptr) {
		int itemID, quantity;
		result = ptrVec->QueryIntAttribute("itemID", &itemID);
		result = ptrVec->QueryIntAttribute("quantity", &quantity);
		iVec.insert({ itemID, quantity });
		ptrVec = ptrVec->NextSiblingElement("iTable");
	}
	this->_inventory.setInventoryTable(iVec);
	// Load QuestLog
	element = root->FirstChildElement("QuestLog");
	ptrVec = element->FirstChildElement("Quest");
	std::vector<std::tuple<std::string, int, std::string, int, std::vector<std::string>, std::vector<std::string>,
		int, int, int, int, int, bool>> qVec;
	while (ptrVec != nullptr) {
		int amount, type, rewardItemID, rewardAmount, exp, cels, npcID;
		std::vector<std::string> dialogueText, finishText;
		std::string qName, objName, dialogueStr;
		bool isCompleted;
		const char* textPtr = nullptr, *objPtr = nullptr, *dialoguePtr = nullptr;

		result = ptrVec->QueryIntAttribute("type", &type);
		result = ptrVec->QueryIntAttribute("amount", &amount);
		textPtr = ptrVec->Attribute("questName");
		qName = textPtr;
		objPtr = ptrVec->Attribute("object");
		objName = objPtr;
		result = ptrVec->QueryBoolAttribute("isCompleted", &isCompleted);
		result = ptrVec->QueryIntAttribute("rewardItemID", &rewardItemID);
		result = ptrVec->QueryIntAttribute("rewardAmount", &rewardAmount);
		result = ptrVec->QueryIntAttribute("npcID", &npcID);
		result = ptrVec->QueryIntAttribute("exp", &exp);
		result = ptrVec->QueryIntAttribute("cels", &cels);

		/* 
		 * Get the "questDialogue" attribute by splitting the string value using the delimiter (~)
		 * This will allow us to form the sentences back and store them invidiually into the string vector
		 */
		const char* questDialogueAttr = ptrVec->Attribute("questDialogue");
		if (questDialogueAttr) {
			// Split the attribute value into separate strings using a delimiter (~)
			std::stringstream ss(questDialogueAttr);
			std::string line;
			while (std::getline(ss, line, '~')) {
				// Trim whitespace from the beginning and end of the string
				line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
					return !std::isspace(ch);
					}));
				line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
					return !std::isspace(ch);
					}).base(), line.end());

				// Add the string to the std::vector<std::string>
				dialogueText.push_back(line);
			}
		}
		/* Do the same for finishDialogue */
		const char* questFinishDialogueAttr = ptrVec->Attribute("finishDialogue");
		if (questFinishDialogueAttr) {
			// Split the attribute value into separate strings using a delimiter (~)
			std::stringstream ss(questFinishDialogueAttr);
			std::string line;
			while (std::getline(ss, line, '~')) {
				// Trim whitespace from the beginning and end of the string
				line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
					return !std::isspace(ch);
					}));
				line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
					return !std::isspace(ch);
					}).base(), line.end());

				// Add the string to the std::vector<std::string>
				finishText.push_back(line);
			}
		}
		qVec.push_back(std::make_tuple(qName, type, objName, amount, dialogueText, finishText, npcID, rewardItemID,
			rewardAmount, exp, cels, isCompleted));
		ptrVec = ptrVec->NextSiblingElement("Quest");
	}
	this->_npc.setQuestTable(qVec);
	// Load Map
	element = root->FirstChildElement("Spawn");
	if (element == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	const char* textPtr = nullptr;
	textPtr = element->Attribute("mapName");
	std::string mapName = textPtr;
	this->_level = Level(mapName, graphics, this->_inventory); // Intialize level: Map name , spawn point, graphics
	// Load coordinates
	Vector2 spawn;
	int x, y;
	result = element->QueryIntAttribute("xCoordinate", &x);
	result = element->QueryIntAttribute("yCoordinate", &y);
	spawn = Vector2((int)std::ceil(x), (int)std::ceil(y));
	this->_level.generateItems(graphics);
	// Spawn = this->_level.getPlayerSpawnPoint();
	this->_player = Player(graphics, spawn);
	// Load KillTable
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
	// Load BossTable
	element = root->FirstChildElement("BossTable");
	ptrVec = element->FirstChildElement("bTable");
	std::vector<std::tuple<std::string, std::string, float, float, bool>> btVec;
	while (ptrVec != nullptr) {
		const char* namePtr = nullptr, *mapPtr = nullptr;
		std::string mobName, mapName;
		float mobX, mobY;
		bool completed;
		namePtr = ptrVec->Attribute("mob");
		mobName = namePtr;
		mapPtr = ptrVec->Attribute("map");
		mapName = mapPtr;
		result = ptrVec->QueryFloatAttribute("posX", &mobX);
		result = ptrVec->QueryFloatAttribute("posY", &mobY);
		result = ptrVec->QueryBoolAttribute("completed", &completed);
		btVec.push_back(std::make_tuple(mobName, mapName, mobX, mobY, completed));
		ptrVec = ptrVec->NextSiblingElement("bTable");
	}
	this->_player.setBossTable(btVec);
	// Load completed cutscenes
	element = root->FirstChildElement("SceneTable");
	ptrVec = element->FirstChildElement("csTable");
	std::vector<std::string> csVec;
	while (ptrVec != nullptr) {
		const char* namePtr = nullptr;
		std::string cScene;
		namePtr = ptrVec->Attribute("sceneName");
		cScene = namePtr;
		csVec.push_back(cScene);
		ptrVec = ptrVec->NextSiblingElement("csTable");
	}
	this->_player.setCutsceneTable(csVec);
	// Load unlocked doors
	element = root->FirstChildElement("LockedDoorTable");
	ptrVec = element->FirstChildElement("ldTable");
	std::vector<std::string> ldVec;
	while (ptrVec != nullptr) {
		const char* namePtr = nullptr;
		std::string ldName;
		namePtr = ptrVec->Attribute("lockName");
		ldName = namePtr;
		ldVec.push_back(ldName);
		ptrVec = ptrVec->NextSiblingElement("ldTable");
	}
	this->_player.setLockedDoorTable(ldVec);
	// Load equipment
	element = root->FirstChildElement("EquipmentTable");
	ptrVec = element->FirstChildElement("eqTable");
	std::vector<std::string> eqVec;
	while (ptrVec != nullptr) {
		const char* namePtr = nullptr;
		std::string eqName;
		namePtr = ptrVec->Attribute("equipName");
		eqName = namePtr;
		eqVec.push_back(eqName);
		ptrVec = ptrVec->NextSiblingElement("eqTable");
	}
	this->_player.setEquipmentTable(eqVec);
	// Load stats
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
	result = element->QueryIntAttribute("hpPot", &iValue);
	this->_player.setHpPotCapacity(iValue);
	result = element->QueryIntAttribute("hpPotStr", &iValue);
	this->_player.setHpPotStrength(iValue);

	this->_level.generateEnemies(graphics, this->_level.getMapName(), this->_player);
	if (!cipher.verifyHash(mapName, this->_player)) {
		std::exit(0);
	}
	// Generate map items and effects after level, enemies and inventory has been initialized.
	// It's also important to do this after the map hash has been verified to prevent any edits appearing by chance.
	this->_level.generateMapItems(graphics, this->_level.getMapName(), this->_inventory);
	this->_level.generateEffects(graphics, this->_player);
	this->saveGame(graphics);
	XMLCheckResult(result);

	/* Refill HP Pot on load and respawn enemies */
	this->_player.refillHpPot();
	this->_player.clearDespawnTable();
	return 0;
}

void Game::setSettings() {
	// Volume
	this->_title.getSettings(bgmVolume, sfxVolume);
	Mix_VolumeMusic(bgmVolume);
	Mix_Volume(-1, sfxVolume);

	// Display
	// ...
}

void Game::update(float elapsedTime, Graphics &graphics) {
	if (!pauseGame && !windowIsBeingDragged) {
		this->_player.update(elapsedTime);
		if (pauseBlockTimer != 0) {
			pauseBlockTimer += elapsedTime;
			if (pauseBlockTimer >= 2000)
				pauseBlockTimer = 0;
		}
		if (this->_player.getCurrentHealth() <= 0 && this->_player.checkDeathPlayed()) {
			GAMEOVER = true;
			Mix_PlayChannel(-1, gameOver, 0);
		}
		else {
			if (this->_player.getPlayerHit()) {
				Mix_PlayChannel(-1, sePlHit, 0);
				this->_player.setPlayerHit(false);
			}
			this->_level.update(elapsedTime, this->_player);

			if (!this->_level.getMapBGM().empty()) {
				if (BGM != this->_level.getMapBGM()) {
					std::string bgmMusic = "data\\sound\\";
					bgmMusic.append(this->_level.getMapBGM());
					BGM = this->_level.getMapBGM();
					gMusic = Mix_LoadMUS(bgmMusic.c_str());
					Mix_PlayMusic(gMusic, -1);
				}
			}


			std::vector<Npc*> otherNpc;
			if ((otherNpc = this->_level.checkNpcCollisions(this->_player.getBoundingBox(), graphics)).size() > 0) {
				npcName = this->_player.getNpcName(otherNpc, graphics);
				npcID = this->_player.getNpcId(otherNpc, graphics);
			}
			// This will ensure when we are no long colliding with npc, set name to blank so we cant talk to an npc far away
			if (otherNpc.size() == 0) {
				npcName = "";
				npcID = 0;
			}

			// Check collisions
			std::vector<Rectangle> others;
			// Set vector = the result of the checkTileCollisions function
			// CheckTile wants another rectangle to check against. So its going to check all the collisions rect
			// Against whatever we give it (player bounding box)
			// If it returns at least 1, handle Tile collision!
			if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
				// Player collided with atleast 1 tile
				this->_player.handleTileCollisions(others);
			}

			if ((others = this->_level.checkBreakableTileCollisions(this->_player.getBoundingBox())).size() > 0) {
				// Player collided with atleast 1 tile
				this->_player.setBreakableCollision(true);
				this->_player.handleTileCollisions(others);
			}
			else {
				this->_player.setBreakableCollision(false);
			}

			if ((others = this->_level.checkArenaCollisions(this->_player.getBoundingBox())).size() > 0) {
				// Player collided with atleast 1 tile
				this->_player.handleArenaCollisions(others);
			}

			if ((others = this->_level.checkLavaCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleLavaCollisions(others);
			}
			else {
				this->_player.setBurning(false);
			}

			if ((others = this->_level.checkPoisonCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handlePoisonCollisions(others);
			}
			if ((others = this->_level.checkWaterCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleWaterCollisions(others);
			}
			else {
				this->_player.setDrowning(false);
			}
			if ((others = this->_level.checkDeadzoneCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleDeadzoneCollisions(others);
			}
			if ((others = this->_level.checkLadderCollisions(this->_player.getBoundingBox())).size() > 0) {
				if (this->_player.handleLadderCollisions(others)) {
					isClimbing = true;
				}
				else {
					isClimbing = false;
					this->_player.setClimbing(false);
					if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
						// Player collided with atleast 1 tile
						this->_player.handleTileCollisions(others);
					}
				}
			}
			else {
				isClimbing = false;
				this->_player.setClimbing(false);
				if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
					// Player collided with atleast 1 tile
					this->_player.handleTileCollisions(others);
				}
			}

			if ((others = this->_level.checkSaveCollisions(this->_player.getBoundingBox())).size() > 0) {
				activeSave = true;
			}
			else
				activeSave = false;

			// Check slope
			std::vector<Slope> otherSlopes;
			if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleSlopeCollisions(otherSlopes);
			}

			/*this->_level.checkCollidingSlopes(this->_player);*/

			this->_level.checkEnemyTileCollision().size() > 0;
			this->_level.checkEnemyProjectileTileCollision().size() > 0;
			this->_level.checkProjectileCollisions(this->_player);
			this->_level.checkEnemyHP(this->_player, this->_graphics);
			if (this->_level.isEnemyDead()) {
				Mix_PlayChannel(-1, enHurt, 0);
				this->_level.setEnemyDead(false);
			}
			this->_level.checkProjectileBreakableLayer();
			this->_level.checkProjectileBounds(this->_player);
			this->_level.checkProjectileTileCollisions();

			if (pickUp == true) {
				this->_level.checkItemCollisions(this->_player, this->_player.getBoundingBox(), graphics, this->_inventory);
				pickUp = false;
			}

			// Check doors
			std::vector<Door> otherDoors;
			if ((otherDoors = this->_level.checkDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleDoorCollision(otherDoors, this->_level, this->_graphics, this->_inventory, this->_player);
			}
			std::vector<Door> lockedDoors;
			if ((lockedDoors = this->_level.checkLockedDoorCollisions(this->_player.getBoundingBox())).size() > 0) {
				this->_player.handleLockedDoorCollision(lockedDoors, this->_level, this->_graphics, this->_inventory, this->_player);
			}

			std::vector<Rectangle> cutScenes;
			if ((cutScenes = this->_level.checkCutsceneCollisions(this->_player.getBoundingBox())).size() > 0) {
				std::string cutSceneName = this->_level.getCutscene();
				if (this->_player.checkCutSceneCompleted(cutSceneName)) {
					this->_level.removeCutscene("");
				}
				else {
					this->loadCutscene(cutSceneName);
					sceneName = cutSceneName;
					sceneX = this->_player.getX();
					sceneY = this->_player.getY();
					activeCutscene = true;
				}
			}
			// Hud goes on top of everything
			this->_hud.update(elapsedTime, this->_player);
			// Update camera last once every object position has been updated to prevent screen shakes 
			this->_camera.Update(elapsedTime, this->_player);
		}
	}
}

void Game::updateCutscene(float elapsedTime, Graphics & graphics)
{
	this->_player.setPlayerDX(0);
	this->_player.setPlayerDY(0);
	this->_player.update(elapsedTime);
	if (sceneType == 1) {
		sceneTimer += elapsedTime;
		if (sceneTimer >= sceneMaxTime && sceneLineCounter <= sceneLines) {
			sceneLineChar++;
			sceneLineCounter++;
			sceneTimer = 0;
		}
		 if (sceneLineCounter > sceneLines) {
			this->_level.removeCutscene("");
			activeCutscene = false;
			sceneTimer = 0;
			sceneLineChar = 'a';
			sceneLineCounter = 1;
			sceneMaxTime = 0;
			this->_player.stopLookingDown();
			this->_player.addCutSceneTable(sceneName);
			return;
		}
	}

	else if (sceneType == 2) {
		if (stopScroll == false) {
			if (sceneX >= targetX + 300) {
				sceneX -= xMod;
				if (sceneX <= targetX + 300)
					stopScroll = true;
			}
			else if (sceneX <= targetX - 150) {
				sceneX += xMod;
				if (sceneX >= targetX - 150) {
					stopScroll = true;
				}
			}
			if (sceneY >= targetY)
				sceneY -= yMod;
			else if (sceneY <= targetY)
				sceneY += yMod;
		}
		this->_camera.sceneUpdate(sceneX, sceneY);
		this->_level.update(elapsedTime, this->_player);

		sceneTimer += elapsedTime;
		if (sceneTimer >= sceneMaxTime) {
			this->_level.removeCutscene("");
			activeCutscene = false;
			sceneTimer = 0;
			sceneLineChar = 'a';
			sceneLineCounter = 1;
			sceneMaxTime = 0;
			sceneX = 0;
			sceneY = 0;
			targetX = 0;
			targetY = 0;
			stopScroll = false;
			this->_player.stopLookingDown();
			this->_player.addCutSceneTable(sceneName);
			return;
		}
	}
	// Check collisions
	std::vector<Rectangle> others;
	// Set vector = the result of the checkTileCollisions function
	// CheckTile wants another rectangle to check against. So its going to check all the collisions rect
	// Against whatever we give it (player bounding box)
	// If it returns at least 1, handle Tile collision!
	if ((others = this->_level.checkTileCollisions(this->_player.getBoundingBox())).size() > 0) {
		// Player collided with atleast 1 title
		this->_player.handleTileCollisions(others);
	}

	this->_level.checkEnemyTileCollision().size() > 0;

	// Check slope
	std::vector<Slope> otherSlopes;
	if ((otherSlopes = this->_level.checkSlopeCollisions(this->_player.getBoundingBox())).size() > 0) {
		this->_player.handleSlopeCollisions(otherSlopes);
	}

	// this->_level.checkCollidingSlopes(this->_player);

	std::vector<Rectangle> cutScenes;
	if ((cutScenes = this->_level.checkCutsceneCollisions(this->_player.getBoundingBox())).size() > 0) {
		activeCutscene = true;
	}
}


