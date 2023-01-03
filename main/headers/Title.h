#pragma once

#include "Graphics.h"
#include "AnimatedSprite.h"
#include "TextManager.h"
#include "SDL.h"
#include "sprite.h"
#include "Input.h"

class Graphics;

class Title : public AnimatedSprite, public TextManager
{
protected:
	struct screenRes {
		int sWidth = 640;
		int sHeight = 480;
	};
public:
	Title();
	Title(Graphics &graphics, Input &input, SDL_Event &event);
	~Title();
	void playNext(int num);
	bool Start(Graphics &graphics, Input &input, SDL_Event &event);
	bool Pause(Graphics &graphics, Input &input, SDL_Event &event, Player &player);
	bool getReload() const { return this->reloadGame; }
	void animationDone(std::string currentAnimation);
	void setupAnimations();
	void update(float elapsedTime);
	void draw(Graphics & graphics);
	void drawPauseMenu(Graphics & graphics, Player &player);
	void getSettings(int &bgmVolume, int &sfxVolume, int &resWidth, int &resHeight);
	void setReload() { this->reloadGame = !reloadGame; }
	int saveSettings();
	int loadSettings();
	int getMenuChoice();

	screenRes& getResolution() { return res; }
private:
	Sprite _title;
	Sprite _startGame;
	Sprite _loadGame;
	Sprite _ExitToMenu;
	Sprite _settings;
	Sprite _settingsMenu;
	Sprite _settingsVolume;
	Sprite _settingsVolumePercent;
	Sprite _settingsSfxVolume;
	Sprite _settingsSfxVolumePercent;
	Sprite _settingsDisplayRes;
	Sprite _exitMenu;
	Sprite _selectionBox;

	screenRes res;
	std::vector<std::pair<int, int>> availableRes = { {640, 480}, {1024, 768}, {1280, 720}, {1920, 1080} };

	int selectX = 185;
	int selectY = 275;
	int menuChoice = 0;
	int settingsChoice = 0;
	int exitChoice = 0;
	int bgmVolumePercent = 100;
	int sfxVolumePercent = 100;
	int msgTimer = 0;
	int displayOption = 0;

	bool showSettings = false;
	bool changeBgmVolume = false;
	bool changeSfxVolume = false;
	bool changeDisplay = false;
	bool isSubmenu = false;
	bool exitMenu = false;
	bool showMsg = false;
	bool reloadGame = false;

	std::string systemMsg;
};

