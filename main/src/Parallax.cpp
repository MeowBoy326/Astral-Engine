#include "../headers/Parallax.h"
#include "../headers/Graphics.h"

#include <iostream>
#include <SDL.h>
Parallax::Parallax() {}

Parallax::~Parallax() {
	SDL_DestroyTexture(this->bgImg);
}
/* Update by players dx_ and dy_ for the srcPosition.x and srcPosition.y respectively */
void Parallax::update(float elapsedTime, float dx, float dy) {
	/* Check if Player is moving */
	if (dx != 0.0f) {
		this->timeToUpdate += elapsedTime;
		if (this->timeToUpdate >= 30) {

			/* Move in copy image as 1st image is ending and place original image at the start again */
			int imageWidth, imageHeight, reposition;
			SDL_QueryTexture(this->bgImg, nullptr, nullptr, &imageWidth, &imageHeight);

			int new_width = imageWidth * 1;
			//if (imageWidth < globals::SCREEN_WIDTH) {
			//	new_width = globals::SCREEN_WIDTH; //imageWidth * 2;
			//}
			reposition = 1;
			if (imageWidth > globals::SCREEN_WIDTH) {
				reposition = -1;
			}

			if (dx > 0.0f) {
				this->srcPosition.x -= (int)((abs(dx) + 1) * this->parallaxSpeed);
				this->srcPositionCopy.x -= (int)((abs(dx) + 1) * this->parallaxSpeed);
				if (this->srcPosition.x <= -new_width) { //-globals::SCREEN_WIDTH) {
					this->srcPosition.x = new_width + reposition;
				}
				else if (this->srcPositionCopy.x <= -new_width) { //-globals::SCREEN_WIDTH) {
					this->srcPositionCopy.x = new_width + reposition;
				}
			}
			else {
				this->srcPosition.x += (int)((dx + 1) * this->parallaxSpeed);
				this->srcPositionCopy.x += (int)((dx + 1) * this->parallaxSpeed);
				if (this->srcPosition.x >= new_width) {
					this->srcPosition.x = -new_width;
				}
				if (this->srcPositionCopy.x >= new_width) {
					this->srcPositionCopy.x = -new_width;
				}
			}

			this->timeToUpdate = 0;
		}
	}
}

void Parallax::draw(Graphics& graphics) {
	int imageWidth, imageHeight;
	SDL_QueryTexture(this->bgImg, nullptr, nullptr, &imageWidth, &imageHeight);

	int new_width, new_height;

	new_height = imageHeight * 1;
	if (imageHeight <= globals::SCREEN_HEIGHT) {
		new_height = 480; //globals::SCREEN_HEIGHT; //imageHeight * 2;
	}
	//if (imageHeight <= globals::SCREEN_HEIGHT) {
	//	new_height = imageHeight * 1; //globals::SCREEN_HEIGHT; //imageHeight * 2;
	//	std::cout << "height = " << imageHeight << ", newHeight = " << new_height << std::endl;
	//	if (new_height <= 400) {
	//		new_height = imageHeight * 2;
	//	}
	//}
	
	new_width = imageWidth * 1;
	if (imageWidth < globals::SCREEN_WIDTH) {
		new_width = globals::SCREEN_WIDTH; //imageWidth * 2;
	}

	SDL_Rect srcRect1 = { 0, 0, new_width, new_height };
	SDL_Rect destRect1 = { this->srcPosition.x, 0, new_width, new_height };
	graphics.windowBlitSurface(this->bgImg, &srcRect1, &destRect1);

	SDL_Rect srcRect2 = { 0, 0, new_width, new_height };
	SDL_Rect destRect2 = { this->srcPositionCopy.x, 0, new_width, new_height };
	graphics.windowBlitSurface(this->bgImg, &srcRect2, &destRect2);
}