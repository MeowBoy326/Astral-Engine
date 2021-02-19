#pragma once

#include "SDL.h"
#include "sprite.h"
#include "Graphics.h"
#include "Player.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

//using namespace QuickSDL;

class Projectile : public Sprite
{
public:
	Projectile();
	Projectile(Graphics &graphics, Player &player);
	~Projectile();

	void shootBullet(Graphics &graphics, Player &player);
	void shootUp(Graphics & graphics, Player & player);
	void shootDown(Graphics & graphics, Player & player);
	void shootLeft(Graphics & graphics, Player & player);
	void updateUp(float ElapsedTime, Player & player);
	void updateDown(float ElapsedTime, Player & player);
	void updateLeft(float ElapsedTime, Player & player);
	void update(float ElapsedTime, Player &player);
	void drawUp(Graphics & graphics, Player & player);
	void drawDown(Graphics & graphics, Player & player);
	void drawLeft(Graphics & graphics, Player & player);
	int checkBullets(vector<int> bVec);
	void draw(Graphics &graphics, Player &player);
	bool isActive();

	void handleProjectileCollisions(std::vector<Enemy*>& others);

	const float getX() const;
	const float getY() const; //getting variables not changes const make sure it doesnt
	
private:
	Sprite _bullet;
	float _shootDx, _shootDy; //(dx (delta x) is change in x postition during frame)
	vector<int> bulletList;
	vector<SDL_Rect> bulletVec; //right
	vector<SDL_Rect> bulletUp;
	vector<SDL_Rect> bulletDown;
	vector<SDL_Rect> bulletLeft;
	std::map<int, vector<SDL_Rect>> _bulletMap;
	//std::map<std::string, std::vector<SDL_Rect>> _animation;
	std::map<int, int> _bulletTime;
};

