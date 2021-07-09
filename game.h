#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Level.h"
#include "HUD.h"
#include "Graphics.h"
#include "Camera.h"
#include "Projectile.h"
#include "Title.h"
#include "TextManager.h"
#include "Npc.h"
#include "Inventory.h"
#include "Items.h"
#include "GameOver.h"

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void drawCutscene(Graphics &graphics);
	void drawTitle(Graphics & graphics);
	void drawGameOver(Graphics & graphics);
	void updateTitle(float elapsedTime);
	void updateGameOver(float elapsedTime);
	void update(float elapsedTime, Graphics &graphics);
	void updateCutscene(float elapsedTime, Graphics &graphics);
	int loadCutscene(std::string name);
	int startCutscene(std::string name);
	int saveGame(Graphics &graphics);
	int loadGame(Graphics &graphics);

	Player _player;
	Level _level;
	HUD _hud;
	Camera _camera;
	Title _title;
	GameOver _gameOver;
	TextManager _chatBox;
	Inventory _inventory;
	Npc _npc;
	Graphics _graphics;
};
#endif

