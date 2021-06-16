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

// No longer can foward declare due to using it now class Graphics;

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics &graphics);
	void drawBullet(Graphics &graphics);
	void drawTitle(Graphics & graphics);
	void updateTitle(float elapsedTime);
	void updateGameOver(float elapsedTime);
	void drawGameOver(Graphics & graphics);
	void update(float elapsedTime, Graphics &graphics);
	void updateBullet(float elapsedTime);
	int saveGame(Graphics &graphics);
	int loadGame(Graphics &graphics);

	Player _player;

	Level _level;

	HUD _hud;

	Camera _camera;

	Projectile _bullet;

	Title _title;

	GameOver _gameOver;

	TextManager _chatBox;

	Inventory _inventory;

	Npc _npc;

	Clock _clock;

	Graphics _graphics; //store graphics object
};

#endif

