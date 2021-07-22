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

class Graphics; //foward declare graphics so we can use it in .cpp
class Player;

class TextManager {
public:
	TextManager();
	TextManager(Graphics & graphics, Player & player);
	TextManager(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);
	~TextManager();
	void setTextStatus(bool status);
	bool isActiveText();

	void drawChatBox(Graphics & graphics, Player & player);
	void drawNpcText(Graphics & graphics, int x, int y, const std::string &str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });
	void drawSceneDialogue(Graphics & graphics, int posX, int posY, const std::string &str, SDL_Color color = { 255, 255, 255, 255 });
	void drawStats(Graphics & graphics, int posX, int posY, float hPoints, double dmgPoints, double defPoints, int available, SDL_Color color = { 255, 255, 255, 255 });
	void drawItemQuantity(Graphics & graphics, int x, int y, const std::string & str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });
	void drawCurrency(Graphics &graphics, int x, int y, const std::string &str, SDL_Color color = { 255, 255, 255, 255 });
	void drawDmg(Graphics & graphics, int x, int y, double dmg);
	void drawMapName(Graphics & graphics, std::string &mapName, int x, int y);
	void drawVersion(Graphics & graphics, int x, int y);
	void drawDeveloper(Graphics & graphics, int x, int y);
	void drawHPNumber(Graphics & graphics, int x, int y, float hp, SDL_Color color = { 255, 255, 255, 255 });
	void drawPercentNumber(Graphics & graphics, int x, int y, double exp, SDL_Color color = { 255, 255, 255, 255 });
	void drawNpcName(Graphics & graphics, int x, int y, const std::string &name, SDL_Color color = { 255, 255, 255, 255 });
	void drawQuestText(Graphics & graphics, int x, int y, const std::string &name, SDL_Color color = { 255, 255, 255 ,255 });
	void drawPlayerStatus(Graphics & graphics, int x, int y, const std::string &text, SDL_Color color = { 255, 255, 255, 255 });
	void drawEventMessages(Graphics &graphics, int x, int y, std::string &text, SDL_Color color = { 255, 255, 255, 255 });
	void drawBattleMessages(Graphics &graphics, int x, int y, std::string &text, SDL_Color color = { 255, 255, 255, 255 });
protected:
	SDL_Rect _textSourceRect; //where on the spritesheet to get the sprite from
	SDL_Rect _chatRect;
	SDL_Texture* _textSheet;
	Rectangle _TboundingBox; //rectangle around player is bounding box. Every sprite will get it.
	float _textX, _textY; //x,y pos of sprite
private:
	Sprite _chatBox;
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 20);
};
#endif
