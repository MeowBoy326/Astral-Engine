#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>

#include "Rectangle.h"
#include "Global.h"
#include <vector>
#include <map>

/* Sprite Class
* Holds all information for individual sprites
*/

class Graphics; // Foward declare graphics so we can use it in .cpp

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	virtual void updateBullet(int bX, int bY, int bW, int bH);
	virtual void updateBoss(float scaleFactor);
	virtual void updateFBall();
	const Rectangle getProjectileBBox();
	void draw(Graphics &graphics, int x, int y);
	void drawScaled(Graphics &graphics, int x, int y, float scale);
	void drawBoundingbox(Graphics &graphics, int x, int y, SDL_Color color);
	void drawFBall(Graphics &graphics, int x, int y);
	void drawHPBar(Graphics &graphics, int x, int y);
	void drawVolumeBar(Graphics &graphics, int x, int y);
	void drawLevelBar(Graphics &graphics, int x, int y);
	void drawStatMenu(Graphics &graphics, int x, int y);
	void drawStatSelection(Graphics &graphics, int x, int y);
	void drawSaveMenu(Graphics &graphics, int x, int y);
	void drawSaveSelection(Graphics &graphics, int x, int y);
	void drawQuestSelection(Graphics &graphics, int x, int y);
	void drawiMenu(Graphics & graphics, int x, int y);
	void drawChat(Graphics & graphics, int x, int y);
	void drawTitle(Graphics & graphics, int x, int y);
	void drawSelectionBox(Graphics & graphics, int x, int y);
	void drawBullet(Graphics &graphics, int x, int y, int bulletID);

	void addProjectile(int x, int y, int width, int height, int bulletID);

	void drawProjectile(Graphics & graphics, SDL_Rect rect);

	const Rectangle getBoundingBox() const;
	void setBoundingBox(int x, int y, int width, int height);
	const sides::Side getCollisionSide(Rectangle &other) const; // Get collision side of bounding box and rectangle that gets passed in
	const sides::Side getArenaCollisionSide(Rectangle &other) const;

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }
	inline void setX(float newX) { this->_x = newX; }
	inline void setY(float newY) { this->_y = newY; }
	inline void addX(float newX) { this->_x += newX; }
	inline void addY(float newY) { this->_y += newY; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);

	const inline SDL_Rect getSourceRect() const { return this->_sourceRect; }

	void destroySprite();

	float _x, _y; // x,y pos of sprite
protected:
	SDL_Rect _sourceRect; // where on the spritesheet to get the sprite from
	SDL_Texture* _spriteSheet; // The sprite sheet that holds the specific sprite

	Rectangle _boundingBox; // Rectangle around player is bounding box. Every sprite will get it.
	Rectangle _projectileBBox;

	int _currentProjectile;
	int _bulletNum;
private:
	std::map<int, std::vector<SDL_Rect>> _projectiles;
	std::map<int, SDL_Rect> _bulletMap;
	std::vector<SDL_Rect> pRect;

	int charSprite = 0;
};
#endif
