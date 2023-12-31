#include "../headers/sprite.h"
#include "../headers/Graphics.h"
#include "../headers/Global.h"

#include <iostream>

Sprite::Sprite() {

}
Sprite::Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) :
	// Intializer
_x(posX), 
_y(posY)

{	// Set our sourceRectangle and spriteSheet
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath)); // Take a render and surface will give us surface if not load then give us
	if (this->_spriteSheet == NULL) { // If spritesheet is null that means there was an error while loading image
		printf("\nError: Unable to load image \n");
	}

	
	if (filePath == "data\\graphics\\Char.png") {
		this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE - 3, height * globals::SPRITE_SCALE);
		this->charSprite = 1;
	}
	else {
		this->_boundingBox = Rectangle(this->_x, this->_y, width * globals::SPRITE_SCALE, height * globals::SPRITE_SCALE);
	}
	
}

Sprite::~Sprite() {
}

void Sprite::draw(Graphics &graphics, int x, int y) { // This will do all the drawing which means we need to call blitSurface and to do that we need a destinationRectangle
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE }; // where on screen we will be drawing
	// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle); 
	}

void Sprite::drawScaled(Graphics& graphics, int x, int y, float scale) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w, this->_sourceRect.h };

	if (scale >= 0) {
		if (scale == 0)
			scale = 1;
		destinationRectangle.w *= scale;
		destinationRectangle.h *= scale;
	}

	else {
		scale *= -1;
		destinationRectangle.w /= scale;
		destinationRectangle.h /= scale;
	}

	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}


void Sprite::drawBoundingbox(Graphics & graphics, int x, int y, SDL_Color color)
{
	SDL_Rect bbRect = { x, y, this->_boundingBox.getWidth(), this->_boundingBox.getHeight() };
	graphics.blitBoundingbox(bbRect, color);
}

void Sprite::drawFBall(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 1.55, this->_sourceRect.h * 1.55 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawHPBar(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 3.0, this->_sourceRect.h * 3.0 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawVolumeBar(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 1.65, this->_sourceRect.h * 2.0 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawLevelBar(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 1.5, this->_sourceRect.h * 1.5 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawStatMenu(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 4, this->_sourceRect.h * 4 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawStatSelection(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x - 5, y, this->_sourceRect.w * 5.8, this->_sourceRect.h * 2 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawSaveMenu(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 2, this->_sourceRect.h * 2 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawSaveSelection(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x - 5, y, this->_sourceRect.w * 2, this->_sourceRect.h * 2 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawQuestSelection(Graphics & graphics, int x, int y)
{
	SDL_Rect destinationRectangle = { x - 5, y, this->_sourceRect.w * 11.8, this->_sourceRect.h * 2 }; // where on screen we will be drawing
// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawiMenu(Graphics &graphics, int x, int y) { // This will do all the drawing which means we need to call blitSurface and to do that we need a destinationRectangle
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 7, this->_sourceRect.h * 7 }; // where on screen we will be drawing
	// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawChat(Graphics &graphics, int x, int y) { // This will do all the drawing which means we need to call blitSurface and to do that we need a destinationRectangle
	SDL_Rect destinationRectangle = {x -300, y +55, this->_sourceRect.w * 4 * globals::SPRITE_SCALE, this->_sourceRect.h * 2 * globals::SPRITE_SCALE * 2 };																														

	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawSelectionBox(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * 5, this->_sourceRect.h * 4 }; // where on screen we will be drawing
	// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made

	graphics.windowBlitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawTitle(Graphics &graphics, int x, int y) { // This will do all the drawing which means we need to call blitSurface and to do that we need a destinationRectangle
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w, this->_sourceRect.h }; // where on screen we will be drawing
	// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made
	graphics.windowBlitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::addProjectile(int x, int y, int width, int height, int bulletID) {
	std::vector<SDL_Rect> rectangles; // Temp hold onto each of our new rectangle that will be put in our map

	SDL_Rect newRect = { x, y, width, height };
	rectangles.push_back(newRect);

	this->_bulletMap.insert({ bulletID, newRect });
	this->_projectiles.insert(std::pair<int, std::vector<SDL_Rect> >(bulletID, rectangles));
	// This->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void Sprite::drawProjectile(Graphics &graphics, SDL_Rect rect) {
	SDL_Rect destinationRectangle = {rect.x, rect.y, rect.w * globals::PROJECTILE_SCALE, rect.h * globals::PROJECTILE_SCALE};
	
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::drawBullet(Graphics &graphics, int x, int y, int bulletID) { 
	// BulletID--;
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::PROJECTILE_SCALE, this->_sourceRect.h * globals::PROJECTILE_SCALE };
	// SDL_Rect sourceRect = this->_projectiles.at[bulletID];
	// DestinationRectangle.x = this->pRect.at(bulletID).x;
	// Cout << this->pRect.at(bulletID).x << endl;
	// Cout << "debug: draw BulletID# " << bulletID << endl;
	for (int i = 0; i > _bulletMap.size(); ++i) {
		std::cout << "debug: for int i: " << _bulletMap[i].x << std::endl;
	}

	auto it = _bulletMap.find(bulletID);
	if (it != _bulletMap.end()) {
		std::cout << " debug auto it: " << it->second.x << std::endl;
		std::cout << it->first << std::endl;
		

		SDL_Rect sourceRect = { it->second.x, it->second.y, it->second.w, it->second.h };
		// Cout << sourceRect.x << ", " << sourceRect.y << ", " << sourceRect.w << ", " << sourceRect.h << endl;
		std::cout << " blitting" << std::endl;
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
	else {
		// Cout << "debug: error! cannot find bullet in Sprite::drawBullet in Sprite.cpp" << endl;
	}
	
	// Cout << this->pRect[bulletID].x << endl;
	// SDL_Rect sourceRect = { this->pRect.at(bulletID).x, this->pRect[bulletID].y, this->pRect[bulletID].w, this->pRect[bulletID].h, };
	// SDL_Rect sourceRect = this->_projectiles[bulletID][this->_bulletNum]; // Pull out correct rectangle
	


	/*
	// This will do all the drawing which means we need to call blitSurface and to do that we need a destinationRectangle
	// Cout << "\ns::drawBullet x,y,h,w = " << x << ", " << y << ", " << this->_sourceRect.h << ", " << this->_sourceRect.w << endl;
	// This->_sourceRect.h = 16;
	// This->_sourceRect.w = 16;
	SDL_Rect* bulletRect = new SDL_Rect;
	*bulletRect = { x, y, this->_sourceRect.w, this->_sourceRect.h };
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::PROJECTILE_SCALE, this->_sourceRect.h * globals::PROJECTILE_SCALE }; // where on screen we will be drawing
	// Cout << "\ns::drawBullet x,y,h,w = " << destinationRectangle.x << ", " << destinationRectangle.y << ", " << destinationRectangle.h << ", " << destinationRectangle.w << endl;																															// Does drawing- Needs spriteSheet, pointer to sourceRectangle and pointer to destination which we just made;
	// Graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, bulletRect);*/
}

void Sprite::update() {
	if (this->charSprite == 1) {
		this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE - 3, this->_sourceRect.h * globals::SPRITE_SCALE);
	}
	else {
		this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
	}
}

void Sprite::updateBoss(float scaleFactor) {
	this->_boundingBox = Rectangle(this->_x, this->_y - (this->_sourceRect.h * globals::SPRITE_SCALE), this->_sourceRect.w * globals::SPRITE_SCALE * scaleFactor, this->_sourceRect.h * globals::SPRITE_SCALE * scaleFactor);
}

void Sprite::updateFBall()
{
	this->_boundingBox = Rectangle(this->_x, this->_y, this->_sourceRect.w * 1.60, this->_sourceRect.h * 1.60);
}

void Sprite::updateBullet(int bX, int bY, int bW, int bH) {
	this->_projectileBBox = Rectangle(bX, bY, bW, bH);
	// Cout << "bounding box: " << this->_projectileBBox.getLeft() << "," << this->_projectileBBox.getRight() << " | " << this->_projectileBBox.getBottom() << "," << this->_projectileBBox.getTop() << endl;
}

const Rectangle Sprite::getProjectileBBox() {
	return
		this->_projectileBBox = Rectangle(this->_x, this->_y, 8, 8);
}

const Rectangle Sprite::getBoundingBox() const {
	return 
		this->_boundingBox;
}

void Sprite::setBoundingBox(int x, int y, int width, int height)
{
	this->_boundingBox = Rectangle(x, y, width, height);
}

// Side getCollisionSide
// Determine which side the collision happened on
const sides::Side Sprite::getCollisionSide(Rectangle &other) const {
	int amtRight, amtLeft, amtTop, amtBottom; // Lowest amount that its in (overlapping) is the side we are colliding on 
	amtRight = this->getBoundingBox().getRight() - other.getLeft();
	amtLeft = other.getRight() - this->getBoundingBox().getLeft();
	amtTop = other.getBottom() - this->getBoundingBox().getTop();
	amtBottom = this->getBoundingBox().getBottom() - other.getTop();
	// Create array of absoulte value of these and loop through array to figure out which one is lowest by overrriding the lowest variable
	// If next one in array is lower than the one thats current lowest replace that one with the lowest.
	int vals[4] = { abs(amtRight), abs(amtLeft), abs(amtTop), abs(amtBottom) };
	int lowest = vals[0];
	for (int i = 0; i < 4; i++) {
		if (vals[i] < lowest) {
			lowest = vals[i];
		}
	}
	return
		lowest == abs(amtRight) ? sides::RIGHT : // If lowest is right return right..
		lowest == abs(amtLeft) ? sides::LEFT :
		lowest == abs(amtTop) ? sides::TOP :
		lowest == abs(amtBottom) ? sides::BOTTOM :
		sides::NONE;
}

const sides::Side Sprite::getArenaCollisionSide(Rectangle & other) const
{
	int amtRight, amtLeft, amtTop, amtBottom;
	amtRight = this->getBoundingBox().getRight();
	amtLeft = this->getBoundingBox().getLeft();
	amtBottom = this->getBoundingBox().getBottom();
	amtTop = this->getBoundingBox().getTop();

	return other.getRight() == amtRight ? sides::RIGHT :
		other.getLeft() == amtLeft ? sides::LEFT :
		other.getBottom() == amtBottom ? sides::BOTTOM :
		other.getTop() == amtTop ? sides::TOP :
		sides::NONE;
}

void Sprite::setSourceRectX(int value) {
	this->_sourceRect.x = value;
}

void Sprite::setSourceRectY(int value) {
	this->_sourceRect.y = value;
}

void Sprite::setSourceRectW(int value) {
	this->_sourceRect.w = value;
}

void Sprite::setSourceRectH(int value) {
	this->_sourceRect.h = value;
}

void Sprite::destroySprite()
{
	SDL_DestroyTexture(this->_spriteSheet);
}
