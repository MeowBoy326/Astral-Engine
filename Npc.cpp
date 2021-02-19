#include "Npc.h"
#include "Input.h"
#include <limits>

//TextManager txt;
//Base Npc class
namespace npcConstants {
	bool endTalk = false;
	int numOfLines = 0;
}

Npc::Npc() {}

Npc::Npc(Graphics &graphics, std::string filePath, int sourceX, int sourceY, int width, int height, Vector2 spawnPoint, int timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate),
	_direction(LEFT),
	_maxHealth(10),
	_currentHealth(10)
	
	
{
	//font = TTF_OpenFont("Arcadia.ttf", 20);
	//cout << "font is: " << fonts << endl;
	this->_txtBox = Sprite(graphics, "npcTextBox.png", 0, 0, 76, 24, 505, 499);
	graphics.loadImage("npcTextBox.png"); //loads sprite sheet in
	this->_script = "clock.txt";
	if (!this->_script.empty()) {
		cout << "writing in script..." << endl;
		ifstream script(_script);
		string ln;
		while (script.good()) {
			getline(script, ln);
			speech.push(ln);
		}
	}
}

Input input;
SDL_Event event;
void eventQuit()
{
	SDL_Event *q = new SDL_Event();
	q->type = SDL_QUIT;
	SDL_FlushEvents(0, UINT32_MAX);
	SDL_PushEvent(q);
}  // void eventQuit()

SDL_Event pressAnyKey()
{
	SDL_Event e;
	SDL_FlushEvent(SDL_KEYDOWN);
	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
	SDL_WaitEvent(&e);
	while (e.type != SDL_KEYDOWN && e.type != SDL_MOUSEBUTTONDOWN && input.wasKeyPressed(SDL_SCANCODE_Q) == true)
		SDL_WaitEvent(&e);
	return e;
}  // void pressAnyKey()

void Npc::update(int elapsedTime, Player &player) {
	AnimatedSprite::update(elapsedTime);
}

void Npc::draw(Graphics &graphics) {
	AnimatedSprite::drawNpc(graphics, this->_x, this->_y);
}

void Npc::drawTxt(Graphics &graphics, const string &str) {
	//AnimatedSprite::drawTextBox(graphics, str, fonts);
}

void Npc::animationDone(std::string currentAnimation)
{
}


void Npc::setupAnimations() {

}

std::ifstream& GotoLine(std::ifstream& file, unsigned int num) {
	file.seekg(std::ios::beg);
	for (int i = 0; i < num - 1; ++i) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return file;
}

void Npc::playNextScript(std::string name, Graphics &graphics, int posX, int posY, int currentLine) {
	this->_script = name + ".txt";
	
	/*
	if (!this->_script.empty()) {
		ifstream script(_script);
		string dummyLine;
		string ln;
		GotoLine(script, npcConstants::numOfLines);
		string line1;
		script >> line1;
		//getline(script, ln);
		speech.push(line1);
		cout << line1 << endl;
	}*/

	std::string line;
	ifstream script(_script);
	for (int i = 0; i < currentLine; ++i) {
		getline(script, line);
	}
	getline(script, line);
	speech.push(line);

	speech.pop();
	std::string str = speech.front();
	//cout << "str = " << str << endl;
	//speech.pop();
	this->drawNpcText(graphics, 100, 100, str, posX, posY);
	
}

int Npc::getLineAmount() {
	return npcConstants::numOfLines;
}

void Npc::setEmpty() {
	this->clearScripts(speech);
}

void Npc::clearScripts(std::queue<std::string> &q) {
	std::queue<std::string> empty;
	std::swap(q, empty);
}

void Npc::runScript(std::string name, Graphics &graphics, int posX, int posY) {
	speech.empty();
	this->_script = name + ".txt";
	//cout << "runScript: script name is: " << _script;
	ifstream script(_script);
	if (!this->_script.empty()) {
		//cout << "runScript: writing in script..." << endl;
		
		string ln;
		
			getline(script, ln);
			speech.push(ln);

	}
	std::string str = speech.front();
	//speech.pop();

	//this->say(graphics, posX, posY);

	//cout << str << endl;
	this->drawNpcText(graphics, 100, 100, str, posX, posY);
	string lines;
	int count = 0;
	while (getline(script, lines)) {
		count++;
	}
	npcConstants::numOfLines = count + 1;

	//txt.drawNpcText(graphics, 100, 100, str);
	//TextManager::drawNpcText(graphics, 5, 5, str);
	//this->drawNpcText(graphics, 5, 5, str);

	//drawTxt(graphics, str);
	
	//this->_txtBox.draw(graphics, 505, 499);
}


void Npc::rendScript(Graphics &graphics, const std::string & str, int posX, int posY, SDL_Color color)
{
	std::string temp = str;

	while (!temp.empty() && npcConstants::endTalk == false)
	{
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
				return; //when the game ends or user exits
			}
		}
		this->renderTextbox(graphics, "", posX, posY);
		if (!name.empty())
			temp = this->renderTextbox(graphics, name + ": " + temp, posX, posY, color);
		else
			temp = this->renderTextbox(graphics, temp, posX, posY, color);
		SDL_RenderPresent(graphics.getRenderer());
		if (input.wasKeyPressed(SDL_SCANCODE_Q) == true) {
			npcConstants::endTalk = true;
		}
		//SDL_Event e = pressAnyKey();


	}
}

void Npc::say(Graphics &graphics, int posX, int posY)
{
	for (int i = 0; i < speech.size(); ++i) {
		cout << speech.front() << endl;
	}
	if (speech.empty()) {
		cout << "empty speech" << endl;
		rendScript(graphics, "I have nothing more to say.", posX, posY);
	}
	else
	{
		cout << "speech not empty..";
		std::string str = speech.front();
		speech.pop();
		rendScript(graphics, str, posX, posY);
		speech.push(str);
	}
} // void Sprite::say(SDL_Renderer *ren, string &str)*/

//Clock class
Clock::Clock() {}

Clock::Clock(Graphics &graphics, Vector2 spawnPoint, std::string name) :
	Npc(graphics, "female.png", 2, 33, 30, 30, spawnPoint, 140),
	_startingX(spawnPoint.x),
	_startingY(spawnPoint.y),
	_shouldMoveUp(false)
{
	_npcName = name;
	this->setupAnimations();
	this->setupScripts();
	this->playAnimation("clock");
	cout << "prep npc" << endl;
	//fonts = TTF_OpenFont("Arcadia.ttf", 20);
	//this->fonts = TTF_OpenFont("Arcadia.ttf", 20);
	this->_script = "clock.txt";
	if (!this->_script.empty()) {
		cout << "writing in script..." << endl;
		ifstream script(_script);
		string ln;
		while (script.good()) {
			getline(script, ln);
			speech.push(ln);
		}
	}
}

void Clock::update(int elapsedTime, Player &player) {
	/*
	this->_direction = player.getX() > this->_x ? RIGHT : LEFT;
	this->playAnimation(this->_direction == RIGHT ? "clock" : "clockD");


	//Move up or down
	this->_y += this->_shouldMoveUp ? -.06 : .06;
	if (this->_y > (this->_startingY + 30) || this->_y < this->_startingY - 30) {
		this->_shouldMoveUp = !this->_shouldMoveUp;
	}*/

	Npc::update(elapsedTime, player);
}

void Clock::draw(Graphics &graphics) {
	Npc::draw(graphics);
}

void Clock::animationDone(std::string currentAnimation) {

}

void Clock::setupScripts() {
	this->addScript(1, 0, 0, "say", 243, 23, Vector2(0, 0));
}

void Clock::setupAnimations() {
	this->addAnimation(1, 1, 1, "clock", 33, 31, Vector2(0, 0));
	this->addAnimation(1, 2, 33, "clockD", 33, 31, Vector2(0, 0));
}

void Clock::bulletHit() {
	this->_currentHealth += -1;
	cout << "hit! HP = " << this->_currentHealth << endl;
}

void Clock::touchPlayer(Player* player) {
	player->gainHealth(-1);
}




