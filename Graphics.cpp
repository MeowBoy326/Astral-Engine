/* Graphics Class
* Holds all information dealing with graphics for the game
*
*/
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "Global.h"
#include "Camera.h"
#include <iostream>
#include <fstream>


Graphics::Graphics() {
	SDL_CreateWindowAndRenderer(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT, 0, &this->_window, &this->_renderer);
	SDL_SetWindowTitle(this->_window, "Astral");
	SDL_Surface* icon = IMG_Load("icon.png");
	SDL_SetWindowIcon(this->_window, icon);
}

Graphics::~Graphics() {
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer); //avoid memory leaks from having a pointer to an SDL rendering memory dont let it hang, destroy when done.
}

SDL_Surface* Graphics::loadImage(const std::string &filePath) {
	//sprite hasnt been loaded yet
	//map count takes in a key for the map (string/filepath) and tells if any exist with that key and if its 0
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str()); //will use SDL to load image, it uses a c-string
	}
	//cout << "graphics.cpp ::loadImage - loaded..." << endl;
	return this->_spriteSheets[filePath]; //regaurdless we will return sprite if its loaded already or needs to be
	
}

	void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
		SDL_Rect tmp = { destinationRectangle->x - Camera::GetRect().x, destinationRectangle->y - Camera::GetRect().y, destinationRectangle->w, destinationRectangle->h };
		SDL_RenderCopy(this->_renderer, texture, sourceRectangle, &tmp);
		/*int valueX = tmp.x;
		std::ofstream file;
		file.open("graph.txt", std::ios_base::app);
		file << "\n " << valueX;
		file.close();*/
		//SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
	}

	void Graphics::windowBlitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
		SDL_Rect tmp = { destinationRectangle->x, destinationRectangle->y, destinationRectangle->w, destinationRectangle->h };
		SDL_RenderCopy(this->_renderer, texture, sourceRectangle, &tmp);
	}

	void Graphics::blitTwo(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
		SDL_Rect tmp = { destinationRectangle->x - Camera::GetRect().x, destinationRectangle->y - Camera::GetRect().y, destinationRectangle->w, destinationRectangle->h };
		SDL_RenderCopy(this->_rendTwo, texture, sourceRectangle, &tmp);
	}

	void Graphics::flipTwo() {
		SDL_RenderPresent(this->_rendTwo);
	}

	void Graphics::flip() {
		SDL_RenderPresent(this->_renderer);
	}

	void Graphics::clear() {
		SDL_RenderClear(this->_renderer);
	}

	void Graphics::clearTwo() {
		SDL_RenderClear(this->_rendTwo);
	}

	SDL_Renderer* Graphics::getRendTwo() const {
		return this->_rendTwo;
	}

	SDL_Renderer* Graphics::getRenderer() const {
		return this->_renderer;
	}

	/*
	SDL_Texture* renderText(Graphics &graphics, TTF_Font *font,
		const string &str, SDL_Color color)
	{
		SDL_Surface *surf = TTF_RenderText_Blended(font, str.c_str(), color);
		if (!surf)
			//quit("TTF_RenderText");
			cout << "debug: renderText graphics.cpp !surf" << endl;
		SDL_Texture* tex(SDL_CreateTextureFromSurface(graphics.getRenderer(), surf));
		if (!tex)
			//quit("CreateTexture");
			cout << "debug: renderText graphics.cpp !tex" << endl;
		SDL_FreeSurface(surf);
		return tex;
	}  // SDL_Texture* renderText()

	string Graphics::renderTextboxLine(Graphics &graphics, TTF_Font *font,
SDL_Color color, string &line, string &word, int &lineNum, SDL_Rect &src, SDL_Rect &dest)
	{
		SDL_Texture* text;
		//font = TTF_OpenFont("ClearSans-Light.ttf", 20);
		//int lineH = TTF_FontLineSkip(font);
		int boxH = globals::SCREEN_HEIGHT / 5;  // magic number: textbox 1/5 of screen tall.
		string excess;
		excess.clear();

		while (!line.empty() && line[0] == ' ')
			line.erase(line.begin());
		if (line.empty())
			return "";
		text = renderText(graphics, font, line, color);

		graphics.blitSurface(text, &src, &dest);
		//renderTexture(text, ren, 10, (SCREEN_HEIGHT - boxH + lineNum * lineH + 10));
		// magic number 10 for top margin of text box.
		SDL_DestroyTexture(text);
		line.clear();
		lineNum++;
		if (boxH < (lineNum * 10 + 20))  // margin of 10 at top and bottom.
			excess += word += ' ';
		return excess;
	}  // string renderTextboxLine()


	string Graphics::renderTextbox(Graphics &graphics, TTF_Font *font, const std::string &str, Player &player,
		SDL_Color color)
	{
		cout << "render font is : " << &font << endl;
		SDL_Texture* textbox = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage("npcTextBox.png"));
		int boxH = globals::SCREEN_HEIGHT / 5;  // magic number: textbox 1/5 of screen tall.
		unsigned int charsInLine = (globals::SCREEN_WIDTH - 20) / 9;
		// left and right margins of 10, 9 pixels per character.
		int lineNum = 0;
		string line, word, excess;
		line.clear();
		word.clear();
		excess.clear();
		SDL_Rect src = {0, 0, 243, 23};
		SDL_Rect dest = { player.getX(), player.getY(), 100, 100 };
		graphics.blitSurface(textbox, &src, &dest);
		//renderTexture(textbox, graphics, 0, (globals::SCREEN_HEIGHT - boxH), globals::SCREEN_WIDTH, boxH);
		for (const char c : str)
		{
			if (!excess.empty())
				excess += c;
			else if (c == ' ' || c == '\n')  // if word completed
			{
				if ((line.length() + word.length()) < charsInLine)
					line += ' ';  // line with word added is within length allowed
				else  // line is finished. print line and clear line.
					excess += graphics.renderTextboxLine(graphics, font, color, line, word, lineNum, src, dest);
				line += word;
				word.clear();
			}
			else
				word += c;
			if (c == '\n')
				excess += graphics.renderTextboxLine(graphics, font, color, line, word, lineNum, src, dest);
		}
		if (excess.empty())
		{
			if ((line.length() + word.length()) < charsInLine)
				line = line + ' ' + word;
			else
				excess += word;
			graphics.renderTextboxLine(graphics, font, color, line, word, lineNum, src, dest);
		}
		SDL_DestroyTexture(textbox);
		return excess;
	}  // void renderTextbox() */
