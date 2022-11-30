#include "../headers/Tile.h"
#include "../headers/Graphics.h"
#include "../headers/Camera.h"

//#include "Tile.h"
//#include "Graphics.h"
//#include "Camera.h"

#include <SDL.h>

Tile::Tile() {}

Tile::Tile(SDL_Texture* tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position, SDL_RendererFlip tileFlip) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x * globals::SPRITE_SCALE, position.y * globals::SPRITE_SCALE)),
	_tileFlip(tileFlip)
{}

void Tile::update(int elapsedTime) {}

void Tile::draw(Graphics &graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y, this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };
	//SDL_Rect destRect = { Camera::GetRect().x, Camera::GetRect().y, this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };
	//SDL_Rect tmp = {destRect.x - Camera::GetRect().x, destRect.y - Camera::GetRect().y, destRect.w, destRect.h };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };


	if (this->_tileFlip == SDL_FLIP_NONE) {
		graphics.blitSurface(this->_tileset, &sourceRect, &destRect);
	}
	else {
		graphics.blitFlippedSurface(this->_tileset, &sourceRect, &destRect, this->_tileFlip);
	}
}