#include "../headers/Parallax.h"
#include "../headers/Graphics.h"

#include <SDL.h>
Parallax::Parallax() {}

/* Update by players dx_ and dy_ for the srcPosition.x and srcPosition.y respectively */
void Parallax::update(int elapsedTime, float dx, float dy) {
	/* Check if Player is moving */
	if (dx != 0.0f) {
		this->timeToUpdate += elapsedTime;
		if (this->timeToUpdate >= 100) {

			/* Move in copy image as 1st image is ending and place original image at the start again */
			int imageWidth, imageHeight;
			SDL_QueryTexture(this->bgImg, nullptr, nullptr, &imageWidth, &imageHeight);

			if (dx > 0.0f) {
				this->srcPosition.x += (int)((dx + 1) * this->parallaxSpeed);
				this->srcPositionCopy.x += (int)((dx + 1) * this->parallaxSpeed);
				if (this->srcPosition.x >= imageWidth) {
					this->srcPosition.x = -globals::SCREEN_WIDTH; 
				}
				if (this->srcPositionCopy.x >= imageWidth) {
					this->srcPositionCopy.x = -globals::SCREEN_WIDTH;
				}
			}
			else {
				this->srcPosition.x -= (int) ( (abs(dx) + 1) * this->parallaxSpeed);
				this->srcPositionCopy.x -= (int)((abs(dx) + 1) * this->parallaxSpeed);
				if (this->srcPosition.x <= -globals::SCREEN_WIDTH) {
					this->srcPosition.x = globals::SCREEN_WIDTH;
				}
				if (this->srcPositionCopy.x <= -globals::SCREEN_WIDTH) {
					this->srcPositionCopy.x = globals::SCREEN_WIDTH;
				}
			}

			this->timeToUpdate = 0;
		}
	}
}

void Parallax::draw(Graphics& graphics) {
	int imageWidth, imageHeight;
	SDL_QueryTexture(this->bgImg, nullptr, nullptr, &imageWidth, &imageHeight);

	SDL_Rect destRect1 = { this->srcPosition.x, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT };
	graphics.windowBlitSurface(this->bgImg, NULL, &destRect1);

	SDL_Rect destRect2 = { this->srcPositionCopy.x, 0, globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT };
	graphics.windowBlitSurface(this->bgImg, NULL, &destRect2);
}