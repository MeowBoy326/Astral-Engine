//#pragma once we can use pragma, it is mostly preference since now most complilers and OS support it.
/* Graphics Class
* Holds all information dealing with graphics for the game
*
*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <map>

#include "Player.h"

struct SDL_Window; //
struct SDL_Renderer;

class Graphics {
public:
	Graphics();
	~Graphics();
	/* SDL_Surface* loadimage
	* Loads an image into the _spriteSheets map if it doesnt already exist
	* As a result each image will only ever be loaded once
	* Returns the image from the map regaurdless of wether or not it was just loaded
	*/
	SDL_Surface* loadImage(const std::string &filePath);
	//void blitzSurface will draw a texture to a certain part of the screen
	//source is what we are drawing to the screen, sourceRectangle is what part of the spritesheet we will draw
	//destinationRectangle is where on the screen we will draw it
	void blitSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	//Same as above (blitSurface) but will flip/rotate the image based on the flip parameter's value
	void blitFlippedSurface(SDL_Texture* source, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectalge, SDL_RendererFlip flip);
	//Drawings for the Title screen
	void windowBlitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle);
	//void flip: Renders everythign to the screen
	void flip();
	//void clear: clears the screen
	void clear();
	//return our render so we can use it elsewhere (sprite class)
	SDL_Renderer* getRenderer() const;
private:
	SDL_Window * _window;
	SDL_Renderer* _renderer;
	SDL_Renderer* _rendTwo;

	std::map<std::string, SDL_Surface*> _spriteSheets; //this map is going to hold in memory in our graphics object every spritesheet we loaded
};

#endif
