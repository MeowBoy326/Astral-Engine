#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#pragma once

#include <SDL.h>
#include "Rectangle.h"
#include <string>
#include <iostream>
#include "Global.h"
#include <SDL_ttf.h>
#include "sprite.h"
#include "Graphics.h"
//#include <vector>
//#include <map>
//#include "Player.h"

class Graphics; //foward declare graphics so we can use it in .cpp
class Player;

class TextManager {
public:
	TextManager();
	TextManager(Graphics & graphics, Player & player);
	TextManager(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);


	void setTextStatus(bool status);

	bool isActiveText();

	void drawChatBox(Graphics & graphics, Player & player);
	void drawTest(Graphics & graphics);
	void drawNpcText(Graphics & graphics, int x, int y, const std::string &str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });
	void drawStats(Graphics & graphics, int posX, int posY, float hPoints, double dmgPoints, double defPoints, int available, SDL_Color color = { 255, 255, 255, 255 });
	void drawItemQuantity(Graphics & graphics, int x, int y, const std::string & str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });
	void drawDmg(Graphics & graphics, int x, int y, double dmg);
	void drawMapName(Graphics & graphics, std::string &mapName, int x, int y);
	void drawVersion(Graphics & graphics, int x, int y);
	void drawDeveloper(Graphics & graphics, int x, int y);
	void drawHPNumber(Graphics & graphics, int x, int y, float hp, SDL_Color color = { 255, 255, 255, 255 });

	SDL_Texture* renderText(Graphics &graphics, TTF_Font *font,
		const std::string &str, SDL_Color color = { 255, 255, 255, 255 });
	//!< Renders text into a picture. Text is white by default.
	std::string renderTextbox(Graphics &graphics, const std::string &str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });
	//!< Draws a textbox with text on the screen. Text is white by default.
	std::string renderTextboxLine(Graphics &graphics, SDL_Color color, std::string &line, std::string &word, int &lineNum, int posX, int posY);

	//virtual void update();
	//virtual void updateBullet(int bX, int bY);
	const Rectangle getTextBoundingBox() const;
	//void draw(Graphics &graphics, int x, int y);
	void setTextXY(Player & player);
	//void drawTitle(Graphics & graphics, int x, int y);
	//void drawBullet(Graphics &graphics, int x, int y, int bulletID);

	//void addProjectile(int x, int y, int width, int height, int bulletID);

	//void drawProjectile(Graphics & graphics, SDL_Rect rect);

	//const Rectangle getBoundingBox() const;
	const sides::Side getTextBoxCollisionSide(Rectangle &other) const; //get collision side of bounding box and rectangle that gets passed in]

	//const inline float getX() const { return this->_x; }
	//const inline float getY() const { return this->_y; }

	void setTextSourceRectX(int value);
	void setTextSourceRectY(int value);
	void setTextSourceRectW(int value);
	void setTextSourceRectH(int value);
protected:
	SDL_Rect _textSourceRect; //where on the spritesheet to get the sprite from
	SDL_Rect _chatRect;
	//SDL_Texture* _spriteSheet; //the sprite sheet that holds the specific sprite
	SDL_Texture* _textSheet;

	Rectangle _TboundingBox; //rectangle around player is bounding box. Every sprite will get it.

	float _textX, _textY; //x,y pos of sprite

private:
	//std::map<int, std::vector<SDL_Rect>> _projectiles;
	//std::map<int, SDL_Rect> _bulletMap;
	//std::vector<SDL_Rect> pRect;
	Sprite _chatBox;
	TTF_Font *font = TTF_OpenFont("Arcadia.ttf", 20);
};



#endif
