#pragma once

#include <algorithm>

#include "Global.h"
#include <SDL.h>

struct SDL_Texture; // Foward declare
class Graphics; // Foward declare

class Parallax {
public:
	Parallax();
	Parallax(SDL_Texture* bgImg, float parallaxSpeed) :
		bgImg(bgImg),
		parallaxSpeed(parallaxSpeed),
		srcPosition({ 0, 0 }) {
		int imageWidth, imageHeight;
		SDL_QueryTexture(this->bgImg, nullptr, nullptr, &imageWidth, &imageHeight);

		int new_width = imageWidth * 1;
		this->srcPositionCopy = { -new_width, 0 };
	}
	void update(int elapsedTime, float dx, float dy);
	void draw(Graphics &graphics);
private:
	SDL_Texture* bgImg;
	float parallaxSpeed;
	Vector2 srcPosition; // Position of the background image
	Vector2 srcPositionCopy; // The copy background image's position.
	int timeToUpdate;
};