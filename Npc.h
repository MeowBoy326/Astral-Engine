#ifndef NPC_H
#define NPC_H

#pragma once

#include "TextManager.h"
#include "AnimatedSprite.h"
//#include "Global.h"
//#include "Player.h"
//#include "Graphics.h"


//#include <string>
//#include <queue>
#include <fstream>
//#include <iostream>
//#include <SDL_ttf.h>

//class Graphics;
//class TextManager;

class Npc : public TextManager, public AnimatedSprite  {
public:
	Npc();
	Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate);

	virtual void update(int elapsedTime, Player &player);
	virtual void draw(Graphics &graphics);
	void drawTxt(Graphics &graphics, const std::string & str);
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void playNextScript(std::string name, Graphics & graphics, int posX, int posY, int currentLine);
	int getLineAmount();
	void setEmpty();
	void clearScripts(std::queue<std::string>& q);
	void runScript(std::string name, Graphics &graphics, int posX, int posY);

	const inline int getMaxHealth() const { return this->_maxHealth; }
	const inline int getCurrentHealth() const { return this->_currentHealth; }
	const inline std::string getName() const { return this->_npcName; }

	void say(Graphics & graphics, int posX, int posY);
	void rendScript(Graphics &graphics, const std::string &str, int posX, int posY, SDL_Color color = { 255, 255, 255, 255 });

	
protected:
	Direction _direction;

	int _maxHealth;
	int _currentHealth;

	std::string _npcName;

	Sprite _txtBox;

	std::string _script;
	std::queue<std::string> speech;
	//!< Circular queue of everything this Sprite says.
	//TTF_Font *fonts = TTF_OpenFont("ClearSans-Light.ttf", 20);
	std::string name;
	//!< Font the Sprite uses to 'speak'.
	
};

class Clock : public Npc {
public:
	Clock();
	Clock(Graphics &graphics, Vector2 spawPoint, std::string name);

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics);
	void touchPlayer(Player* player);
	void bulletHit();

	void animationDone(std::string currentAnimation);
	void setupScripts();
	void setupAnimations();
private:
	float _startingX, _startingY;
	bool _shouldMoveUp; //keep track of if Clock should move
};

#endif