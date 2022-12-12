#include "../headers/HUD.h"
#include "../headers/Graphics.h"

#include <iostream>

HUD::HUD() {}

HUD::HUD(Graphics &graphics, Player &player) {
	this->_player = player;
	// This->_healthBarSprite = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 40, 76, 8, 35, 70); // x, y, width, height, screen pos x, screen pos y
	this->_healthBarSprite = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 162, 76, 19, 35, 70);
	this->_currentHealthBar = Sprite(graphics, "data\\graphics\\TextBox.png", 19, 183, 56, 5, 83, 72);
	// This->_currentHealthBar = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 25, 40, 5, 83, 72);
	// This->_healthNumber1 = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 56, 8, 8, 66, 70);

	this->_lvWord = Sprite(graphics, "data\\graphics\\TextBox.png", 81, 81, 11, 7, 38, 55);
	this->_lvNumber = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 56, 8, 8, 66, 52);
	this->_expBarValue = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 31, 1, 8, 83, 52);
	// This->_slash = Sprite(graphics, "data\\graphics\\TextBox.png", 72, 48, 8, 8, 100, 36);
	// This->_dashes = Sprite(graphics, "data\\graphics\\TextBox.png", 81, 51, 15, 11, 132, 26);
	this->_expBar = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 144, 84, 8, 83, 52);
}

void HUD::update(int ElapsedTime, Player &player) {
	// This->_healthNumber1.setSourceRectX(8 * player.getCurrentHealth());

	// Calculate the width of the health bar
	//100% = 39px
	// Cast to float because int 0 division will happen and we will get 0 everytime.
	float hpNum = (float)player.getCurrentHealth() / player.getMaxHealth();
	this->_currentHealthBar.setSourceRectW(std::floor(hpNum * 56));

	// Exp & Level bars
	int expNum = (player.getCurrentExp() / player.getRequiredExp()) * 84;
	this->_expBarValue.setSourceRectW(expNum);
	this->_lvNumber.setSourceRectX(8 * player.getLevel());
}

void HUD::draw(Graphics &graphics, Player &player) {
	// This->_healthBarSprite.draw(graphics, this->_healthBarSprite.getX(), this->_healthBarSprite.getY());
	// This->_healthNumber1.draw(graphics, player.getX() - 285, player.getY() - 185);
	this->_healthBarSprite.drawHPBar(graphics, player.getX() - 317, player.getY() - 240);
	this->_currentHealthBar.drawHPBar(graphics, player.getX() - 260, player.getY() - 216);

	this->_lvWord.draw(graphics, player.getX() - 314, player.getY() - 179);
	this->_lvNumber.draw(graphics, player.getX() - 285, player.getY() - 179);
	this->_expBar.draw(graphics, player.getX() - 261, player.getY() - 201);
	this->_expBarValue.draw(graphics, player.getX() - 260, player.getY() - 202);

	// This->_slash.draw(graphics, player.getX() - 255, player.getY() - 220);
	// This->_dashes.draw(graphics, player.getX() - 230, player.getY() - 235);
}