#include "AnimatedSprite.h"
#include "Graphics.h"
#include "sprite.h"


/* Animated Sprite class
* Animates our sprites
*/

AnimatedSprite::AnimatedSprite() {} //default constructor

AnimatedSprite::AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY,
	int width, int height, float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0),
	_timeElapsed(0),
	_timeToUpdate(timeToUpdate),
	_visible(true),
	_currentAnimationOnce(false),
	_currentAnimation(""),
	_currentScript(""),
	_currentScriptOnce(false)
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles; //temp hold onto each of our new rectangle that will be put in our map
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	std::map<std::string, std::vector<SDL_Rect>>::iterator it;
	it = this->_animation.find(name);
	if (it == this->_animation.end()) {
		this->_animation.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
		this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
	}
}

void AnimatedSprite::addSpecialAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset)
{
	std::vector<SDL_Rect> rectangles; //temp hold onto each of our new rectangle that will be put in our map
	int nWidth = width;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (nWidth + x), y, width, height };
		nWidth += width;
		rectangles.push_back(newRect);
	}
	this->_animation.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}



void AnimatedSprite::addScript(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles; //temp hold onto each of our new rectangle that will be put in our map
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}
	this->_scripts.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
	this->_scriptOffsets.insert(std::pair<std::string, Vector2>(name, offset));
}

/*void AnimatedSprite::addBulletAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset, int id) {
	std::vector<SDL_Rect> rectangles; //temp hold onto each of our new rectangle that will be put in our map
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { (i + x) * width, y, width, height };
		rectangles.push_back(newRect);
	}

	this->_bulletAnimation.insert(std::tuple<std::string, std::vector<SDL_Rect>, float>(name, rectangles, id));
	this->_bulletAnimation.insert(int, std::pair<std::string, std::vector<SDL_Rect>>(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
} */

void AnimatedSprite::resetAnimations() {
	this->_animation.clear();
	this->_offsets.clear();
}

void AnimatedSprite::resetScripts() {
	this->_scripts.clear();
	this->_scriptOffsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimationOnce = once; //all this is to play animation
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::playScript(std::string animation, bool once) {
	this->_currentScriptOnce = once; //all this is to play animation
	if (this->_currentScript != animation) {
		this->_currentScript = animation;
		this->_scriptIndex = 0;
	}
}

void AnimatedSprite::playBulletAnimation(std::string animation, int id) {
	//this->_currentAnimationOnce = once; //all this is to play animation
	//if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
		this->_bulletID = id;
	//}
}

void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}

void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0; //no matter what
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::stopScript() {
	this->_scriptIndex = 0; //no matter what
	this->animationDone(this->_currentScript);
}

void AnimatedSprite::update(int elapsedTime) { //timer checks when to go to next frame in animation
	Sprite::update(); //since we extend we can call like this

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ change frame
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animation[this->_currentAnimation].size() - 1) {
			this->_frameIndex++; //increment by 1
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::updateBoss(int elapsedTime, int y) {
	Sprite::updateBoss(y); //since we extend we can call like this

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ change frame
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animation[this->_currentAnimation].size() - 1) {
			this->_frameIndex++; //increment by 1
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::updateFBall(int elapsedTime, int y)
{
	Sprite::updateFBall(); //since we extend we can call like this

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ change frame
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animation[this->_currentAnimation].size() - 1) {
			this->_frameIndex++; //increment by 1
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->stopAnimation();
		}
	}
}

void AnimatedSprite::updateScript(int elapsedTime) { //timer checks when to go to next frame in animation
	Sprite::update(); //since we extend we can call like this

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ change frame
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_scriptIndex < this->_scripts[this->_currentScript].size() - 1) {
			this->_scriptIndex++; //increment by 1
		}
		else {
			if (this->_currentScriptOnce == true) {
				this->setVisible(false);
			}
			this->stopScript();
		}
	}
}



void AnimatedSprite::draw(Graphics &graphics, int x, int y) {
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x; 
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

void AnimatedSprite::drawBoss(Graphics &graphics, int x, int y, float scaleFactor) {
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::SPRITE_SCALE * scaleFactor;
		destinationRectangle.h = this->_sourceRect.h * globals::SPRITE_SCALE * scaleFactor;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

/*
void AnimatedSprite::drawTextBox(Graphics &graphics, std::string speech, TTF_Font *font) {
	SDL_Color color = { 255, 255, 255, 255 };

	SDL_Surface *surface;

	surface = TTF_RenderText_Solid(font, "hello", color);
	SDL_Rect src = { 0, 0, 100, 100 };
	SDL_Rect dest = { 200, 200, 100, 100 };
	SDL_Texture *tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), surface);
	graphics.blitSurface(tex, NULL, &dest);
}*/

void AnimatedSprite::drawNpc(Graphics &graphics, int x, int y) {
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * 1.22;
		destinationRectangle.h = this->_sourceRect.h * 1.22;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}



void AnimatedSprite::drawTitle(Graphics &graphics, int x, int y) {
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x;
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y;
		destinationRectangle.w = this->_sourceRect.w;
		destinationRectangle.h = this->_sourceRect.h;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.windowBlitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
		//graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

void AnimatedSprite::drawBullet(Graphics &graphics, int x, int y) {
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * globals::PROJECTILE_SCALE;
		destinationRectangle.h = this->_sourceRect.h * globals::PROJECTILE_SCALE;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}
void AnimatedSprite::drawLoot(Graphics & graphics, int x, int y)
{
	if (this->_visible) { //only draw when visible
		SDL_Rect destinationRectangle; //temp where we draw on screen
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x;
		//this will push it over to whatever we set from offset when we draw (doesnt change postion) but drawn in different position with offset
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y;
		destinationRectangle.w = this->_sourceRect.w * 1.2;
		destinationRectangle.h = this->_sourceRect.h * 1.2;

		SDL_Rect sourceRect = this->_animation[this->_currentAnimation][this->_frameIndex]; //pull out correct rectangle
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}
/*
void AnimatedSprite::updateBullet(int elapsedTime) { //timer checks when to go to next frame in animation
	Sprite::updateBullet(); //since we extend we can call like this

	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ change frame
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animation[this->_currentAnimation].size() - 1) {
			this->_frameIndex++; //increment by 1
		}
		else {
			//if (this->_currentAnimationOnce == true) {
			//	this->setVisible(false);
			//}
			//this->stopAnimation();
		}
	}
}*/

