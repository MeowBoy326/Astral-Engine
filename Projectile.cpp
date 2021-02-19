#include "Projectile.h"
#include <iostream>

using namespace std;

namespace projectileConstants {
	const float BULLET_VELOCITY = 0.6f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	bool active = false;

	int count = 0;
}

Projectile::Projectile()
{
}

Projectile::Projectile(Graphics & graphics, Player & player)
{
	int posx = player.getX();
	int posy = player.getY();
	this->_bullet = Sprite(graphics, "Bullet.png", 0, 6, 8, 8, posx, posy);
	//	this->_currentHealthBar = Sprite(graphics, "Textbox.png", 0, 25, 39, 5, 83, 72);
	cout << "sprite added" << endl;
	graphics.loadImage("Bullet.png"); //loads sprite sheet in
	cout << "sprite loaded" << endl;
}

Projectile::~Projectile()
{
}

void Projectile::shootBullet(Graphics & graphics, Player & player)
{
	projectileConstants::active = true;
	/*
	int dir = 3;
	if (player.lookingUp == true) {
		dir = 1;
	}
	else if (player.lookingDown == true) {
		dir = 2;
	}
	std::vector<SDL_Rect> rectangles; //temp hold onto each of our new rectangle that will be put in our map
	rectangles.push_back(newRect);
	this->_bulletMap.insert(std::pair<int, std::vector<SDL_Rect>> (dir, rectangles));

	*/

	SDL_Rect newRect = { player.getX(), player.getY(), 16, 16 };
	bulletVec.push_back(newRect);

	
	
	this->draw(graphics, player);
}

void Projectile::shootUp(Graphics &graphics, Player &player) {
	projectileConstants::active = true;

	SDL_Rect newRect = { player.getX(), player.getY(), 16, 16 };
	bulletUp.push_back(newRect);

	this->drawUp(graphics, player);
}

void Projectile::shootDown(Graphics &graphics, Player &player) {
	projectileConstants::active = true;

	SDL_Rect newRect = { player.getX(), player.getY(), 16, 16 };
	bulletDown.push_back(newRect);

	this->drawDown(graphics, player);
}

void Projectile::shootLeft(Graphics &graphics, Player &player) {
	projectileConstants::active = true;

	SDL_Rect newRect = { player.getX(), player.getY(), 16, 16 };
	bulletLeft.push_back(newRect);

	this->drawLeft(graphics, player);
}

void Projectile::updateUp(float ElapsedTime, Player &player) {
	if (projectileConstants::active == true && bulletUp.size() != 0) {
		for (int i = 0; i < bulletUp.size(); ++i) {
			bulletUp[i].x += 0 * ElapsedTime;
			bulletUp[i].y -= projectileConstants::BULLET_VELOCITY * ElapsedTime;

			//this->_bullet.update()
			//this->_bullet.updateBullet(bulletUp[i].x, bulletUp[i].y);
			this->updateBullet(bulletUp[i].x, bulletUp[i].y, bulletUp[i].w, bulletUp[i].h);
		}

		for (int i = 0; i < bulletUp.size(); ++i) {
			if (bulletUp[i].y <= player.getY() / 3) {
				bulletUp.erase(bulletUp.begin() + i);
				//cout << "debug: bullet "  << " removed!" << endl;
			}
		}
	}
}

void Projectile::updateDown(float ElapsedTime, Player &player) {
	if (projectileConstants::active == true && bulletDown.size() != 0) {
		for (int i = 0; i < bulletDown.size(); ++i) {
			bulletDown[i].x += 0 * ElapsedTime;
			bulletDown[i].y += projectileConstants::BULLET_VELOCITY * ElapsedTime;

			//this->_x = bulletVec[i].x;
			this->updateBullet(bulletDown[i].x, bulletDown[i].y, bulletDown[i].w, bulletDown[i].h);
		}

		for (int i = 0; i < bulletDown.size(); ++i) {
			if (bulletDown[i].y >= player.getY() * 2) {
				bulletDown.erase(bulletDown.begin() + i);
				//cout << "debug: bullet "  << " removed!" << endl;
			}
		}
	}
}

void Projectile::updateLeft(float ElapsedTime, Player &player) {
	if (projectileConstants::active == true && bulletLeft.size() != 0) {
		for (int i = 0; i < bulletLeft.size(); ++i) {
			bulletLeft[i].x -= projectileConstants::BULLET_VELOCITY * ElapsedTime;
			bulletLeft[i].y += 0 * ElapsedTime;

			//this->_x = bulletVec[i].x;
			//bulletLeft[i].h;
			
			this->updateBullet(bulletLeft[i].x, bulletLeft[i].y, bulletLeft[i].w, bulletLeft[i].h);
		}

		for (int i = 0; i < bulletLeft.size(); ++i) {
			if (bulletLeft[i].x <= player.getX() / 3) {
				bulletLeft.erase(bulletLeft.begin() + i);
				//cout << "debug: bullet "  << " removed!" << endl;
			}
		}
	}
}

void Projectile::update(float ElapsedTime, Player &player)
{
	if (projectileConstants::active == true && bulletVec.size() != 0) {
		for (int i = 0; i < bulletVec.size(); ++i) {
			bulletVec[i].x += projectileConstants::BULLET_VELOCITY * ElapsedTime;
			bulletVec[i].y += 0 * ElapsedTime;
			//this->_x = bulletVec[i].x;
			this->updateBullet(bulletVec[i].x, bulletVec[i].y, bulletVec[i].w, bulletVec[i].h);
			//cout << "bullet: " << bulletVec[i].x << "," << bulletVec[i].y << endl;
		}

		for (int i = 0; i < bulletVec.size(); ++i) {
			if (bulletVec[i].x >= player.getX() * 2) {
				bulletVec.erase(bulletVec.begin() + i);
				//cout << "debug: bullet "  << " removed!" << endl;
			}
		}
	}
}


void Projectile::drawUp(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletUp.size() != 0) {
		for (int i = 0; i < bulletUp.size(); ++i) {
			SDL_Rect src = { bulletUp[i].x, bulletUp[i].y - 30, bulletUp[i].w, bulletUp[i].h };
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::drawDown(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletDown.size() != 0) {
		for (int i = 0; i < bulletDown.size(); ++i) {
			SDL_Rect src = { bulletDown[i].x, bulletDown[i].y + 30, bulletDown[i].w, bulletDown[i].h };
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::drawLeft(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletLeft.size() != 0) {
		for (int i = 0; i < bulletLeft.size(); ++i) {
			SDL_Rect src = { bulletLeft[i].x - 30, bulletLeft[i].y, bulletLeft[i].w, bulletLeft[i].h };
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::draw(Graphics & graphics, Player & player)
{
	if (projectileConstants::active == true && bulletVec.size() != 0) {
	for (int i = 0; i < bulletVec.size(); ++i) {
		SDL_Rect src = { bulletVec[i].x, bulletVec[i].y, bulletVec[i].w, bulletVec[i].h };
		this->_bullet.drawProjectile(graphics, src);
	}

	/*
	if (projectileConstants::active == true && bulletVec.size() != 0) {
		int i = 0;
		map<int, std::vector<SDL_Rect>>::iterator itr;
		for (itr = _bulletMap.begin(); itr != _bulletMap.end(); ++itr) {
			
			//get Dimensions for src
			if (itr->first == 1) {
				itr->second.at(i).y -= 10
			}
			SDL_Rect src = { itr->second.at(i).x, itr->second.at(i).y, itr->second.at(i).w, itr->second.at(i).h};
			this->_bullet.drawProjectile(graphics, src);
		}*/



		/*
		for (int i = 0; i < _bulletMap.size(); ++i) {
			auto it = _bulletMap.find(i);
			if (it != _bulletMap.end()) {
				SDL_Rect src = { it->second.x, it->second.y, it->second.w, it->second.h };
				this->_bullet.drawProjectile(graphics, src);
			}
		}

		for (int i = 0; i <= bulletList.size(); ++i) {
			auto it = _bulletMap.find(i);
			if (it != _bulletMap.end()) {
				SDL_Rect src = { it->second.x, it->second.y, it->second.w, it->second.h };
				this->_bullet.drawProjectile(graphics, src);
			}
			//this->_bullet.drawBullet(graphics, this->_x, this->_y, bulletID);
		} */
	}
}

bool Projectile::isActive()
{
	if (projectileConstants::active == true) {
		return true;
	}
	else if (projectileConstants::active == false) {
		return false;
	}
}

void Projectile::handleProjectileCollisions(std::vector<Enemy*> &others) {
	for (int i = 0; i < others.size(); i++) {
		others.at(i)->bulletHit();
		if (bulletUp.size() > 0) {
			bulletUp[i].x = 0;
			bulletUp[i].y = 0;
			bulletUp[i].w = 0;
			bulletUp[i].h = 0;
			//this->_bullet.update()
			//this->_bullet.updateBullet(bulletUp[i].x, bulletUp[i].y);
			this->updateBullet(bulletUp[i].x, bulletUp[i].y, bulletUp[i].w, bulletUp[i].h);
		bulletUp.erase(bulletUp.begin() + i);
		}
		if (bulletVec.size() > 0) {
			bulletVec[i].x = 0;
			bulletVec[i].y = 0;
			bulletVec[i].w = 0;
			bulletVec[i].h = 0;
			//this->_bullet.update()
			//this->_bullet.updateBullet(bulletUp[i].x, bulletUp[i].y);
			this->updateBullet(bulletVec[i].x, bulletVec[i].y, bulletVec[i].w, bulletVec[i].h);
			bulletVec.erase(bulletVec.begin() + i);
		}
		if (bulletDown.size() > 0) {
			bulletDown[i].x = 0;
			bulletDown[i].y = 0;
			bulletDown[i].w = 0;
			bulletDown[i].h = 0;
			//this->_bullet.update()
			//this->_bullet.updateBullet(bulletUp[i].x, bulletUp[i].y);
			this->updateBullet(bulletDown[i].x, bulletDown[i].y, bulletDown[i].w, bulletDown[i].h);
			bulletDown.erase(bulletDown.begin() + i);
		}
		if (bulletLeft.size() > 0) {
			bulletLeft[i].x = 0;
			bulletLeft[i].y = 0;
			bulletLeft[i].w = 0;
			bulletLeft[i].h = 0;
			//this->_bullet.update()
			//this->_bullet.updateBullet(bulletUp[i].x, bulletUp[i].y);
			this->updateBullet(bulletLeft[i].x, bulletLeft[i].y, bulletLeft[i].w, bulletLeft[i].h);
			bulletLeft.erase(bulletLeft.begin() + i);
		}

	}
}

const float Projectile::getX() const
{
	return 0.0f;
}

const float Projectile::getY() const
{
	return 0.0f;
}

