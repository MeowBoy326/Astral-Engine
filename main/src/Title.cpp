#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult;}
#endif

#include "../headers/Title.h"
#include "../headers/Input.h"
#include "../headers/tinyxml2.h"

#include <iostream>
#include <algorithm>
#include <filesystem>

using namespace tinyxml2;

namespace {
	const int FPS = 50;
	// remove *5
	const int MAX_FRAME_TIME = 5 * 1000 / FPS; //Max amount of time a frame is allowed to last
}

Title::Title() {}

Title::Title(Graphics &graphics, Input &input, SDL_Event &event) :
// Graphics, filePath, source x, source y on sprite sheet, width , height of sprite, x, y pos to start player out at (destinationRect), and timetoUpdate 100
AnimatedSprite(graphics, "data\\graphics\\dark_clouds.png", 0, 0, 640, 480, 0, 0, 140)
{
	graphics.loadImage("data\\graphics\\dark_clouds.png"); //loads sprite sheet in

	this->setupAnimations();
	this->playAnimation("zero");

	this->_title = Sprite(graphics, "data\\graphics\\title.png", 2, 0, 241, 68, 300, 100);
	//x, y, width, height, screen pos x, screen pos y
	std::cout << "sprite added" << std::endl;
	graphics.loadImage("data\\graphics\\title.png"); //loads sprite sheet in

	this->_startGame = Sprite(graphics, "data\\graphics\\startGame.png", 0, 84, 124, 23, 215, 270);
	graphics.loadImage("data\\graphics\\startGame.png");

	this->_loadGame = Sprite(graphics, "data\\graphics\\startGame.png", 0, 119, 130, 22, 215, 315);

	this->_settings = Sprite(graphics, "data\\graphics\\startGame.png", 0, 153, 95, 22, 215, 355);
	this->_settingsMenu = Sprite(graphics, "data\\graphics\\TextBox.png", 0, 195, 40, 42, 35, 70);
	this->_settingsVolume = Sprite(graphics, "data\\graphics\\TextBox.png", 93, 166, 82, 10, 320, 150);
	this->_settingsVolumePercent = Sprite(graphics, "data\\graphics\\TextBox.png", 95, 182, 64, 6, 335, 155);

	this->_exitMenu = Sprite(graphics, "data\\npc\\npcTextBox.png", 155, 51, 76, 26, 320, 300);

	this->_selectionBox = Sprite(graphics, "data\\graphics\\startGame.png", 0, 63, 19, 12, 185, 275);
	this->loadSettings();
}

Title::~Title()
{

}

bool Title::Start(Graphics &graphics, Input &input, SDL_Event &event)
{

	bool menuLoop = true;
	int LAST_UPDATE_TIME = SDL_GetTicks();

	while (menuLoop == true) {
		input.beginNewFrame(); //resets our released key pressed keys first time around doesnt matter already set to false but good to reset anyway

		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event); //if we are holding key start keydown event
				}
			}
			else if (event.type == SDL_KEYUP) { // if key was released
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				exit(0);
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_RETURN) == true) {
			if (menuChoice != 2 && !showSettings)
				menuLoop = false;
			else if (!showSettings) {
				this->loadSettings();
				showSettings = !showSettings;
				this->selectY = 155;
				this->selectX = 220;
				settingsChoice = 0;
			}
			else if (exitMenu && showSettings)
			{
				if (exitChoice == 0)
				{
					this->saveSettings();
					exitMenu = !exitMenu;
					this->selectY = this->_settings.getY() + 5;
					this->selectX = 185;
					showSettings = !showSettings;
					isSubmenu = !isSubmenu;
					// Restart application to load new settings
					std::system("astral-services.bat");
					std::exit(0);
				}
				else {
					// Undo any changes by loading the unsaved file
					this->loadSettings();
					exitMenu = !exitMenu;
					this->selectY = this->_settings.getY() + 5;
					this->selectX = 185;
					showSettings = !showSettings;
					isSubmenu = !isSubmenu;
				}
			}
			else {
				if (settingsChoice == 0) {
					changeVolume = !changeVolume;
					isSubmenu = !isSubmenu;
					this->selectY = 155;
					this->selectX = 220;
					settingsChoice = 0;
				}
				// Exit button -> Open confirmation (Save and Exit?)
				else {
					isSubmenu = !isSubmenu;
					exitMenu = !exitMenu;
					this->selectY = 325;
					this->selectX = 320;
					this->exitChoice = 0;
				}
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_DOWN) == true) {
			if (!showSettings && !isSubmenu) {
				if (menuChoice == 0) {
					this->selectY = this->_loadGame.getY() + 5;
					menuChoice++;
				}

				else if (menuChoice == 1) {
					this->selectY = this->_settings.getY() + 5;
					menuChoice++;
				}
			}
			else if (showSettings && !isSubmenu) {
				if (settingsChoice == 0) {
					this->selectY = 185;
					this->selectX = 220;
					settingsChoice++;
				}
			}

		}
		else if (input.wasKeyPressed(SDL_SCANCODE_UP) == true) {
			if (!showSettings) {
				if (menuChoice == 2) {
					this->selectY = this->_loadGame.getY() + 5;
					menuChoice--;
				}

				else if (menuChoice == 1) {
					this->selectY = this->_startGame.getY() + 5;
					menuChoice--;
				}
			}
			else if (showSettings && !isSubmenu) {
				if (settingsChoice == 1) {
					this->selectY = 155;
					this->selectX = 220;
					settingsChoice--;
				}
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_RIGHT) == true) {
			if (isSubmenu) {
				if (changeVolume && volumePercent < 100) {
					// Increase volume
					this->volumePercent += 5;
					float volNum = (float)volumePercent / 100;
					this->_settingsVolumePercent.setSourceRectW(std::floor(volNum * 64));
				}
				else if (exitMenu) {
					if (exitChoice == 0) {
						this->selectY = 325;
						this->selectX = 395;
						exitChoice++;
					}
				}
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_LEFT) == true) {
			if (isSubmenu) {
				if (changeVolume && volumePercent > 0) {
					this->volumePercent -= 5;
					float volNum = (float)volumePercent / 100;
					this->_settingsVolumePercent.setSourceRectW(std::floor(volNum * 64));
				}
				else if (exitMenu) {
					if (exitChoice == 1) {
						this->selectY = 325;
						this->selectX = 320;
						exitChoice--;
					}
				}
			}
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			exit(0);
		}

		this->Title::update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS; //loop will go again and current time - new last update will tell us how long next frame will take
		this->Title::draw(graphics);
	}
	return false;
}

int Title::getMenuChoice() {
	std::cout << "Menu Choice = " << this->menuChoice << std::endl;
	return this->menuChoice;
}

void Title::playNext(int num) {
	switch (num) {
	case 0:
		this->playAnimation("zero");
		break;
	case 1:
		this->playAnimation("one");
		break;
	case 2:
		this->playAnimation("two");
		break;
	case 3:
		this->playAnimation("three");
		break;
	case 4:
		this->playAnimation("four");
		break;
	}
}

void Title::animationDone(std::string currentAnimation) {}

void Title::setupAnimations() {
	this->addAnimation(4, 0, 0, "zero", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 483, "one", 640, 480, Vector2(0, 0)); // we are 16 pixels down now on sprite sheet
	this->addAnimation(1, 643, 1, "two", 640, 480, Vector2(0, 0)); //# of frames, x, y, name(RunLeft), height, width, offset (no so empty vector)
	this->addAnimation(1, 643, 483, "three", 640, 480, Vector2(0, 0));
	this->addAnimation(1, 1, 965, "four", 640, 480, Vector2(0, 0));
}

void Title::update(float elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Title::draw(Graphics &graphics) {
	graphics.clear(); // Clear any drawings MUST do

	AnimatedSprite::drawTitle(graphics, this->_x, this->_y);
	this->_title.drawTitle(graphics, 210, 50);
	this->_startGame.drawTitle(graphics, 215, 270);
	this->_loadGame.drawTitle(graphics, 215, 315);
	this->_settings.drawTitle(graphics, 215, 355);
	this->drawVersion(graphics, 0, 455);
	this->drawDeveloper(graphics, 0, 445);
	if (showSettings) {
		this->_settingsMenu.drawiMenu(graphics, 210, 125);
		this->_settingsVolume.drawVolumeBar(graphics, this->_settingsVolume.getX(), this->_settingsVolume.getY());
		this->_settingsVolumePercent.drawVolumeBar(graphics, this->_settingsVolumePercent.getX(), this->_settingsVolumePercent.getY());
		std::string label = "Volume: ";
		if (changeVolume)
			this->drawSettings(graphics, 240, 155, label, 14, {255,255,0,255});
		else
			this->drawSettings(graphics, 240, 155, label, 14);
		label = std::to_string(volumePercent) + "%";
		this->drawSettings(graphics, 375, 145, label, 10);
		label = "Exit";
		this->drawSettings(graphics, 240, 185, label, 14);
	}
	if (exitMenu) {
		this->_exitMenu.drawSaveMenu(graphics, this->_exitMenu.getX(), this->_exitMenu.getY());
	}
	this->_selectionBox.drawTitle(graphics, selectX, selectY);
	graphics.flip();
}

void Title::getSettings(int &volume) {
	// More settings will be added. Each value will be passed by reference
	volume = this->volumePercent;
}

int Title::saveSettings() {
	XMLDocument xml;
	XMLNode* root = xml.NewElement("Root");
	xml.InsertFirstChild(root);
	std::cout << "Creating the XML Document..." << std::endl;

	// Save each setting value
	XMLElement* element = xml.NewElement("Settings");
	element->SetAttribute("volumePercent", this->volumePercent);
	root->InsertEndChild(element);
	// Finalize save
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "profile";
	cwd.append("settings.xml");
	XMLError result = xml.SaveFile(cwd.string().c_str());
	XMLCheckResult(result)
	return result;
}

int Title::loadSettings() {
	XMLDocument xml;
	if (!std::filesystem::exists(std::filesystem::current_path() / "data" / "profile" / "settings.xml"))
	{
		std::cout << "File not found!" << std::endl;
		// Create a new save file using default settings
		this->saveSettings();
		return 0;
	}
	std::cout << "File was found!" << std::endl;
	

	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "profile";
	cwd.append("settings.xml");
	xml.LoadFile(cwd.string().c_str());

	XMLError result;
	XMLNode* root = xml.FirstChild();
	if (root == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	XMLElement* element = root->FirstChildElement("Settings");
	int x, y;
	result = element->QueryIntAttribute("volumePercent", &x);
	this->volumePercent = x;
	XMLCheckResult(result)
	return result;
}
