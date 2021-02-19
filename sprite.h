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

class Graphics; //foward declare graphics so we can use it in .cpp

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);

	virtual ~Sprite();
	virtual void update();
	virtual void updateBullet(int bX, int bY, int bW, int bH);
	const Rectangle getProjectileBBox() const;
	void draw(Graphics &graphics, int x, int y);
	void drawiMenu(Graphics & graphics, int x, int y);
	void drawChat(Graphics & graphics, int x, int y);
	void drawTitle(Graphics & graphics, int x, int y);
	void drawBullet(Graphics &graphics, int x, int y, int bulletID);

	void addProjectile(int x, int y, int width, int height, int bulletID);

	void drawProjectile(Graphics & graphics, SDL_Rect rect);

	const Rectangle getBoundingBox() const;
	const sides::Side getCollisionSide(Rectangle &other) const; //get collision side of bounding box and rectangle that gets passed in]

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);
protected:
	SDL_Rect _sourceRect; //where on the spritesheet to get the sprite from
	SDL_Texture* _spriteSheet; //the sprite sheet that holds the specific sprite

	Rectangle _boundingBox; //rectangle around player is bounding box. Every sprite will get it.
	Rectangle _projectileBBox;

	float _x, _y; //x,y pos of sprite

	int _currentProjectile;
	int _bulletNum;
private:
	std::map<int, std::vector<SDL_Rect>> _projectiles;
	std::map<int, SDL_Rect> _bulletMap;
	std::vector<SDL_Rect> pRect;
};



#endif
