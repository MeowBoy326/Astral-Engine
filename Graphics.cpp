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
}

void Graphics::blitFlippedSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle, SDL_RendererFlip flip)
{
	SDL_Rect tmp = { destinationRectangle->x - Camera::GetRect().x, destinationRectangle->y - Camera::GetRect().y, destinationRectangle->w, destinationRectangle->h };
	//RenderCopyEx will flip the texture depending on the value of the flip parameter
	SDL_RenderCopyEx(this->_renderer, texture, sourceRectangle, &tmp, 0, 0, flip);
}

void Graphics::windowBlitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_Rect tmp = { destinationRectangle->x, destinationRectangle->y, destinationRectangle->w, destinationRectangle->h };
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, &tmp);
}

void Graphics::blitBoundingbox(SDL_Rect &bbRect, SDL_Color color)
{
	// Offset x/y by camera x/y
	bbRect.x -= Camera::GetRect().x;
	bbRect.y -= Camera::GetRect().y;

	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(this->_renderer, &bbRect);
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
}

void Graphics::blitSlopeOutline(SDL_Point & startPoint, SDL_Point & endPoint, SDL_Color color)
{
	// Offset x/y by camera x/y
	startPoint.x -= Camera::GetRect().x;
	startPoint.y -= Camera::GetRect().y;
	endPoint.x -= Camera::GetRect().x;
	endPoint.y -= Camera::GetRect().y;

	SDL_SetRenderDrawColor(this->_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawLine(this->_renderer, startPoint.x, startPoint.y, endPoint.x, endPoint.y);
	SDL_SetRenderDrawColor(this->_renderer, 0, 0, 0, 0);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->_renderer;
}