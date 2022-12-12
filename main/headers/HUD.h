#pragma once

#include "sprite.h"
#include "Player.h"

class Graphics;
// Class Player; Can't foward declare player here because Player_player is not a pointer

class HUD {
public:
	HUD();
	HUD(Graphics &graphics, Player &player);
	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics, Player &player);

private:
	Player _player;

	// Health Sprites
	Sprite _healthBarSprite;
	Sprite _healthNumber1;
	Sprite _currentHealthBar;

	// Exp Sprites
	Sprite _lvWord;
	Sprite _lvNumber;
	Sprite _expBarValue;
	Sprite _expBar;

	// Gun Sprite
	Sprite _slash;
	Sprite _dashes;
};