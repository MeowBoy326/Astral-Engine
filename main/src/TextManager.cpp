#include "../headers/TextManager.h"
#include "../headers/Graphics.h"
#include "../headers/sprite.h"
#include "../headers/Global.h"

#include <iostream>
#include <string>

namespace textConstants {
	bool activeText = false;
	int count = 0;
}

TextManager::TextManager() {}

TextManager::TextManager(Graphics & graphics, Player & player)  {
	int posx = player.getX();
	int posy = player.getY();
	this->_chatBox = Sprite(graphics, "data\\npc\\npcTextBox.png", 0, 0, 243, 23, posx, posy);
	graphics.loadImage("data\\npc\\npcTextBox.png"); //loads sprite sheet in
	this->drawChatBox(graphics, player);
}

TextManager::TextManager(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) :
	//intializer
	_textX(posX),
	_textY(posY)

{	//set our sourceRectangle and spriteSheet
	this->_textSourceRect.x = sourceX;
	this->_textSourceRect.y = sourceY;
	this->_textSourceRect.w = width;
	this->_textSourceRect.h = height;
	this->_textSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath)); //take a render and surface will give us surface if not load then give us
	if (this->_textSheet == NULL) { //if spritesheet is null that means there was an error while loading image
		printf("\nError: Unable to load image \n");
	}
	this->_TboundingBox = Rectangle(this->_textX, this->_textY, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
}

TextManager::~TextManager()
{
}

void TextManager::setTextStatus(bool status) {
	textConstants::activeText = status;
}

bool TextManager::isActiveText() {
	return textConstants::activeText;
}

void TextManager::drawChatBox(Graphics & graphics, Player & player) {
	if (textConstants::activeText == true)
		this->_chatBox.drawChat(graphics, player.getX(), player.getY());
}

void TextManager::drawNpcText(Graphics &graphics, int x, int y, const std::string &str, int posX, int posY, SDL_Color color) { 
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	} 
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
	SDL_Surface *surface = TTF_RenderText_Solid(font, str.c_str(), color);;
	SDL_Rect destinationRectangle = { posX - 260 , posY + 105, surface->w, surface->h };;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);;

	std::string temp = "";
	int count = 0;
	for (int index = 0; index < str.length(); ++index) {
		if (str[index] == '$' && str[index + 1] == 'n') {
			count++;
			if (count == 1) {
				TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
				SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
				SDL_Rect destinationRectangle = { posX - 260 , posY + 105, surface->w, surface->h }; // where on screen we will be drawing
				SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
				graphics.blitSurface(tex, NULL, &destinationRectangle);
				SDL_FreeSurface(surface); //fixes crashing for access violation in loop
				TTF_CloseFont(font);
				SDL_DestroyTexture(tex);
			}
			else if (count == 2) {
				TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
				SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
				SDL_Rect destinationRectangle = { posX - 260 , posY + 135, surface->w, surface->h }; // where on screen we will be drawing
				SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
				graphics.blitSurface(tex, NULL, &destinationRectangle);
				SDL_FreeSurface(surface); //fixes crashing for access violation in loop
				TTF_CloseFont(font);
				SDL_DestroyTexture(tex);
			}
			temp = "";
			index += 2;
		}
		if (str[index] == '$' && str[index + 1] == 'e') {
			TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
			SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
			SDL_Rect destinationRectangle = { posX - 260 , posY + 165, surface->w, surface->h }; // where on screen we will be drawing
			SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
			graphics.blitSurface(tex, NULL, &destinationRectangle);
			SDL_FreeSurface(surface); //fixes crashing for access violation in loop
			TTF_CloseFont(font);
			SDL_DestroyTexture(tex);
		}
		temp += str[index];
	}
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}

void TextManager::drawSceneDialogue(Graphics & graphics, int posX, int posY, const std::string & str, SDL_Color color)
{
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
	SDL_Surface *surface = TTF_RenderText_Solid(font, str.c_str(), color);;
	SDL_Rect destinationRectangle = { posX - 320 , posY - 85, surface->w, surface->h };;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);;

	std::string temp = "";
	int count = 0;
	for (int index = 0; index < str.length(); ++index) {
		if (str[index] == '$' && str[index + 1] == 'n') {
			count++;
			if (count == 1) {
				TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 17);
				SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
				SDL_Rect destinationRectangle = { posX - 315 , posY - 85, surface->w, surface->h }; // where on screen we will be drawing
				SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
				graphics.blitSurface(tex, NULL, &destinationRectangle);
				SDL_FreeSurface(surface); //fixes crashing for access violation in loop
				TTF_CloseFont(font);
				SDL_DestroyTexture(tex);
			}
			else if (count == 2) {
				TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 17);
				SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
				SDL_Rect destinationRectangle = { posX - 315 , posY - 65, surface->w, surface->h }; // where on screen we will be drawing
				SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
				graphics.blitSurface(tex, NULL, &destinationRectangle);
				SDL_FreeSurface(surface); //fixes crashing for access violation in loop
				TTF_CloseFont(font);
				SDL_DestroyTexture(tex);
			}
			temp = "";
			index += 2;
		}
		if (str[index] == '$' && str[index + 1] == 'e') {
			TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 17);
			SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
			SDL_Rect destinationRectangle = { posX - 225 , posY - 45, surface->w, surface->h }; // where on screen we will be drawing
			SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
			graphics.blitSurface(tex, NULL, &destinationRectangle);
			SDL_FreeSurface(surface); //fixes crashing for access violation in loop
			TTF_CloseFont(font);
			SDL_DestroyTexture(tex);
		}
		else if (str[index] == '$' && str[index + 1] == 'h') {
			TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 32);
			SDL_Surface *surface = TTF_RenderText_Solid(font, temp.c_str(), color);
			SDL_Rect destinationRectangle = { posX - 265 , posY - 45, surface->w, surface->h }; // where on screen we will be drawing
			SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
			graphics.blitSurface(tex, NULL, &destinationRectangle);
			SDL_FreeSurface(surface); //fixes crashing for access violation in loop
			TTF_CloseFont(font);
			SDL_DestroyTexture(tex);
		}
		temp += str[index];
	}
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}

void TextManager::drawDmg(Graphics & graphics, int x, int y, double dmg)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *surface;
	SDL_Color color = { 255, 0, 0, 255 };
	int precisionVal = 2;
	std::string dmgNum = std::to_string(dmg).substr(0, std::to_string(dmg).find(".") + precisionVal + 1);
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
	surface = TTF_RenderText_Solid(iFont, dmgNum.c_str(), color);

	SDL_Rect destinationRectangle = { x, y - 30, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	//TTF_CloseFont(font);
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}

void TextManager::drawMapName(Graphics & graphics, std::string &mapName, int x, int y) {
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *backGround;
	SDL_Surface *surface;
	SDL_Color color = { 255, 255, 255, 255 };

	int fontSize = 52;
	if (mapName.length() > 8) {
		fontSize -= mapName.length() * 1.2;
	}
	std::transform(mapName.begin(), mapName.end(), mapName.begin(), ::toupper);
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\OptimusPrinceps.ttf", fontSize);

	//Surface = Text surface. backGround = semi-transparent filled rect where the text will be placed on
	surface = TTF_RenderText_Solid(iFont, mapName.c_str(), color);
	backGround = SDL_CreateRGBSurface(0, surface->w + 20, surface->h + 20, 32, 0, 0, 0, 0);
	
	SDL_FillRect(backGround, NULL, SDL_MapRGB(backGround->format, 41, 40, 40));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *bgTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), backGround);
	SDL_SetTextureAlphaMod(bgTex, 128); //50% opacity (255/2)
	SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);

	SDL_Rect destinationRectangle = { x - 120, y - 200, surface->w, surface->h }; // where on screen we will be drawing
	if (mapName.length() < 8) {
		destinationRectangle = { x - 50, y - 200, surface->w, surface->h };
	}

	graphics.blitSurface(bgTex, NULL, &destinationRectangle);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(backGround);
	TTF_CloseFont(iFont); //previously TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(bgTex);
}

void TextManager::drawVersion(Graphics & graphics, int x, int y)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *backGround;
	SDL_Surface *surface;
	SDL_Color color = { 255, 255, 255, 255 };

	// Get current time & version
	//std::time_t currentTime = std::time(0);
	//std::tm* localTime = std::localtime(&currentTime);
	////std::asctime(localTime)
	// Char time[40];
	//std::strftime(time, sizeof(time), "%B %d, %Y (%T)", localTime);
	//std::string timeToStr = time;
	//std::string version;
	//version += "Version " + std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR)
	//	+ "." + std::to_string(VERSION_PATCH) + "." + std::to_string(VERSION_BUILD) + " " + timeToStr;

	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\ClearSans-Light.ttf", 14);

	//Surface = Text surface. backGround = semi-transparent filled rect where the text will be placed on
	surface = TTF_RenderText_Solid(iFont, version.c_str(), color);
	backGround = SDL_CreateRGBSurface(0, surface->w + 20, surface->h + 20, 32, 0, 0, 0, 0);

	SDL_FillRect(backGround, NULL, SDL_MapRGB(backGround->format, 41, 40, 40));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *bgTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), backGround);
	SDL_SetTextureAlphaMod(bgTex, 128); //50% opacity (255/2)
	SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);

	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h }; // where on screen we will be drawing

	graphics.blitSurface(bgTex, NULL, &destinationRectangle);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(backGround);
	TTF_CloseFont(iFont); //previously TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(bgTex);
}

void TextManager::drawDeveloper(Graphics & graphics, int x, int y)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *backGround;
	SDL_Surface *surface;
	SDL_Color color = { 255, 255, 255, 255 };
	std::string version = "Developed By Arhum Z. Nayyar";
	int fontSize = 12;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", fontSize);

	//Surface = Text surface. backGround = semi-transparent filled rect where the text will be placed on
	surface = TTF_RenderText_Solid(iFont, version.c_str(), color);
	backGround = SDL_CreateRGBSurface(0, surface->w + 20, surface->h + 20, 32, 0, 0, 0, 0);

	SDL_FillRect(backGround, NULL, SDL_MapRGB(backGround->format, 41, 40, 40));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *bgTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), backGround);
	SDL_SetTextureAlphaMod(bgTex, 128); //50% opacity (255/2)
	SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);

	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h }; // where on screen we will be drawing

	graphics.blitSurface(bgTex, NULL, &destinationRectangle);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(backGround);
	TTF_CloseFont(iFont); //previously TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(bgTex);
}

void TextManager::drawHPNumber(Graphics & graphics, int x, int y, float hp, SDL_Color color)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *surface;
	int precisionVal = 2;
	std::string dmgNum = std::to_string(hp).substr(0, std::to_string(hp).find(".") + precisionVal + 1);
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 14);
	surface = TTF_RenderText_Solid(iFont, dmgNum.c_str(), color);

	SDL_Rect destinationRectangle = { x - 15, y, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}

void TextManager::drawPercentNumber(Graphics & graphics, int x, int y, double exp, SDL_Color color)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *surface;
	int precisionVal = 2;
	std::string percentNum = std::to_string(exp).substr(0, std::to_string(exp).find(".") + precisionVal + 1) +"%";
	//std::string expNum = std::to_string(exp) + "%";
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 14);
	surface = TTF_RenderText_Solid(iFont, percentNum.c_str(), color);

	SDL_Rect destinationRectangle = { x - 15, y, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}


void TextManager::drawNpcName(Graphics & graphics, int x, int y, const std::string & name, SDL_Color color)
{
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 14);
	SDL_Surface *surface = TTF_RenderText_Solid(font, name.c_str(), color);;
	SDL_Rect destinationRectangle = { x - 260 , y + 50, surface->w, surface->h };;
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);;

	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}

void TextManager::drawQuestText(Graphics & graphics, int x, int y, const std::string & text, SDL_Color color)
{
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Rect destinationRectangle = { x - 260 , y + 105, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
}

void TextManager::drawPlayerStatus(Graphics & graphics, int x, int y, const std::string & text, SDL_Color color)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Color effectColor = {255, 255, 255, 255}; //default white
	if (text == "POISONED") {
		effectColor = { 128, 0, 128, 255 }; //Purple
	}
	else if (text == "BURNING") {
		effectColor = { 255, 165, 0, 255 }; //Orange
	}
	TTF_Font *font = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 12);
	SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), effectColor);
	SDL_Surface *backGround = SDL_CreateRGBSurface(0, surface->w + 20, surface->h + 20, 32, 0, 0, 0, 0);
	SDL_FillRect(backGround, NULL, SDL_MapRGB(backGround->format, 41, 40, 40));
	SDL_Rect destinationRectangle = { x - 10, y - 15, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *bgTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), backGround);
	SDL_SetTextureAlphaMod(bgTex, 128); //50% opacity (255/2)
	SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);

	graphics.blitSurface(bgTex, NULL, &destinationRectangle);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(backGround);
	TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(bgTex);
}

void TextManager::drawEventMessages(Graphics & graphics, int x, int y, std::string &text, SDL_Color color)
{
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	SDL_Surface *backGround;
	SDL_Surface *surface;

	int fontSize = 52;
	if (text.length() > 8) {
		fontSize -= text.length() * 1.2;
		if (text.length() > 30)
			fontSize = 16;
		if (text.length() > 40)
			fontSize = 12;
	}
	std::transform(text.begin(), text.end(), text.begin(), ::toupper);
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", fontSize);

	//Surface = Text surface. backGround = semi-transparent filled rect where the text will be placed on
	surface = TTF_RenderText_Solid(iFont, text.c_str(), color);
	backGround = SDL_CreateRGBSurface(0, surface->w + 20, surface->h + 20, 32, 0, 0, 0, 0);

	SDL_FillRect(backGround, NULL, SDL_MapRGB(backGround->format, 41, 40, 40));
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *bgTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), backGround);
	SDL_SetTextureAlphaMod(bgTex, 128); //50% opacity (255/2)
	SDL_SetTextureBlendMode(bgTex, SDL_BLENDMODE_BLEND);

	SDL_Rect destinationRectangle = { x - 160, y - 100, surface->w, surface->h }; // where on screen we will be drawing
	if (text.length() < 8) {
		destinationRectangle = { x - 160, y - 100, surface->w, surface->h };
	}

	graphics.blitSurface(bgTex, NULL, &destinationRectangle);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(backGround);
	TTF_CloseFont(iFont); //previously TTF_CloseFont(font);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(bgTex);
}

void TextManager::drawBattleMessages(Graphics & graphics, int x, int y, std::string & text, SDL_Color color)
{
	SDL_Surface *surface;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 14);
	surface = TTF_RenderText_Solid(iFont, text.c_str(), color);
	// get the width and height of a string as it would be rendered in a loaded font
	int w, h;
	w = surface->w;
	h = surface->h;
	TTF_SizeText(iFont, text.c_str(), &w, &h);
	x -= w;
	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}

void TextManager::drawSettings(Graphics & graphics, int x, int y, std::string & text, int fontSize, SDL_Color color)
{
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	SDL_Surface *surface;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", fontSize);
	surface = TTF_RenderText_Solid(iFont, text.c_str(), color);
	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);

	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}

void TextManager::drawStats(Graphics & graphics, int posX, int posY, float hPoints, double dmgPoints, double defPoints, int available, SDL_Color color) {
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
	int precisionVal = 2;
	std::string pts = std::to_string(hPoints).substr(0, std::to_string(hPoints).find(".") + precisionVal + 1).c_str();
	std::string points = std::to_string(hPoints).substr(0, std::to_string(hPoints).find(".") + precisionVal + 1);
	SDL_Surface *surface, *dmgSurface, *defSurface, *availableSurface;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 24);
	surface = TTF_RenderText_Solid(iFont, std::to_string(hPoints).substr(0, std::to_string(hPoints).find(".") + precisionVal + 1).c_str(), color);
	dmgSurface = TTF_RenderText_Solid(iFont, std::to_string(dmgPoints).substr(0, std::to_string(dmgPoints).find(".") + precisionVal + 1).c_str(), color);
	defSurface = TTF_RenderText_Solid(iFont, std::to_string(defPoints).substr(0, std::to_string(defPoints).find(".") + precisionVal + 1).c_str(), color);
	availableSurface = TTF_RenderText_Solid(iFont, std::to_string(available).c_str(), color);
	SDL_Rect destinationRectangle = { posX - 10, posY, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	SDL_Texture *dTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), dmgSurface);
	SDL_Texture *defTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), defSurface);
	SDL_Texture *aTex = SDL_CreateTextureFromSurface(graphics.getRenderer(), availableSurface);

	graphics.blitSurface(tex, NULL, &destinationRectangle);
	destinationRectangle = { posX - 10, posY + 50, dmgSurface->w, dmgSurface->h };
	graphics.blitSurface(dTex, NULL, &destinationRectangle);
	destinationRectangle = { posX - 4, posY + 100, defSurface->w, defSurface->h };
	graphics.blitSurface(defTex, NULL, &destinationRectangle);
	destinationRectangle = { posX - 30, posY + 145, availableSurface->w, availableSurface->h };
	graphics.blitSurface(aTex, NULL, &destinationRectangle);

	SDL_FreeSurface(dmgSurface);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	SDL_FreeSurface(defSurface);
	SDL_FreeSurface(availableSurface);
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
	SDL_DestroyTexture(dTex);
	SDL_DestroyTexture(defTex);
	SDL_DestroyTexture(aTex);
}

void TextManager::drawItemQuantity(Graphics &graphics, int x, int y, const std::string &str, int posX, int posY, SDL_Color color) {
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	SDL_Surface *surface;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 12);
	surface = TTF_RenderText_Solid(iFont, str.c_str(), color);
	SDL_Rect destinationRectangle = { posX, posY, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	//graphics.blitSurface(tex, NULL, &dest);

	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}

void TextManager::drawCurrency(Graphics & graphics, int x, int y, const std::string & str, SDL_Color color)
{
	//TTF_Init();
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}

	SDL_Surface *surface;
	TTF_Font *iFont = TTF_OpenFont("data\\fonts\\Arcadia.ttf", 18);
	surface = TTF_RenderText_Solid(iFont, str.c_str(), color);
	SDL_Rect destinationRectangle = { x, y, surface->w, surface->h }; // where on screen we will be drawing
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);

	graphics.blitSurface(tex, NULL, &destinationRectangle);
	SDL_FreeSurface(surface); //fixes crashing for access violation in loop
	TTF_CloseFont(iFont);
	SDL_DestroyTexture(tex);
}
