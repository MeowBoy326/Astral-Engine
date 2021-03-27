#include "HUD.h"
#include "Graphics.h"
#include <iostream>

HUD::HUD() {}

HUD::HUD(Graphics &graphics, Player &player) {
	this->_player = player;
	this->_healthBarSprite = Sprite(graphics, "TextBox.png", 0, 40, 64, 8, 35, 70); //x, y, width, height, screen pos x, screen pos y
	this->_healthNumber1 = Sprite(graphics, "TextBox.png", 0, 56, 8, 8, 66, 70);
	this->_currentHealthBar = Sprite(graphics, "Textbox.png", 0, 25, 40, 5, 83, 72);
	this->_lvWord = Sprite(graphics, "TextBox.png", 81, 81, 11, 7, 38, 55);
	this->_lvNumber = Sprite(graphics, "TextBox.png", 0, 56, 8, 8, 66, 52);
	this->_expBarValue = Sprite(graphics, "TextBox.png", 0, 31, 1, 8, 83, 52);
	this->_slash = Sprite(graphics, "TextBox.png", 72, 48, 8, 8, 100, 36);
	this->_dashes = Sprite(graphics, "TextBox.png", 81, 51, 15, 11, 132, 26);
	this->_expBar = Sprite(graphics, "TextBox.png", 0, 144, 40, 8, 83, 52);
}

void HUD::update(int ElapsedTime, Player &player) {
	this->_healthNumber1.setSourceRectX(8 * player.getCurrentHealth());

	//Calculate the width of the health bar
	//100% = 39px
	//Cast to float because int 0 division will happen and we will get 0 everytime.
	float hpNum = (float)player.getCurrentHealth() / player.getMaxHealth();
	this->_currentHealthBar.setSourceRectW(std::floor(hpNum * 40));

	//Exp & Level bars
	int expNum = (player.getCurrentExp() / player.getRequiredExp()) * 40;
	this->_expBarValue.setSourceRectW(expNum);
	this->_lvNumber.setSourceRectX(8 * player.getLevel());
}

void HUD::draw(Graphics &graphics, Player &player) {
	//this->_healthBarSprite.draw(graphics, this->_healthBarSprite.getX(), this->_healthBarSprite.getY());
	//had to calculate this manually relative to player pos :(
	this->_expBar.draw(graphics, player.getX() - 269, player.getY() - 201);
	this->_healthBarSprite.draw(graphics, player.getX() - 317, player.getY() - 185);
	this->_healthNumber1.draw(graphics, player.getX() - 285, player.getY() - 185);
	this->_currentHealthBar.draw(graphics, player.getX() - 270, player.getY() - 183);
	this->_lvWord.draw(graphics, player.getX() - 314, player.getY() - 200);
	this->_lvNumber.draw(graphics, player.getX() - 285, player.getY() - 201);
	this->_expBarValue.draw(graphics, player.getX() - 269, player.getY() - 202);
	this->_slash.draw(graphics, player.getX() - 255, player.getY() - 220);
	this->_dashes.draw(graphics, player.getX() - 230, player.getY() - 235);
}