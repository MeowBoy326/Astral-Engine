#include "Projectile.h"
#include <iostream>
#include "TextManager.h" 

namespace projectileConstants {
	const float BULLET_VELOCITY = 0.6f;
	const float JUMP_DISTANCE = 0.7f;

	const float GRAVITY = 0.002f;
	const float GRAVITY_CAP = 0.8f;

	double BULLET_DMG = 1.0;

	bool active = false;

	int count = 0;

	float enemyX = 0;
	float enemyY = 0;
}

Projectile::Projectile()
{
}

Projectile::Projectile(Graphics & graphics, Player & player)
{
	int posx = player.getX();
	int posy = player.getY();
	this->_bullet = Sprite(graphics, "Bullet.png", 66, 0, 12, 11, posx, posy);
	//	this->_currentHealthBar = Sprite(graphics, "Textbox.png", 0, 25, 39, 5, 83, 72);
	std::cout << "sprite added" << std::endl;
	graphics.loadImage("Bullet.png"); //loads sprite sheet in
	std::cout << "sprite loaded" << std::endl;

	this->_Gun = Sprite(graphics, "Arms.png", 52, 10, 10, 5, (posx - 10), (posy + 10));
	graphics.loadImage("Arms.png");
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

	SDL_Rect newRect = { player.getX() + 20, player.getY() + 14, 16, 16 };
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

	SDL_Rect newRect = { player.getX(), player.getY() + 10, 16, 16 };
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
	if (player.getSoulLevel() >= 1) {
		projectileConstants::BULLET_DMG = player.getDmgMod();
	}
}


void Projectile::drawUp(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletUp.size() != 0) {
		for (int i = 0; i < bulletUp.size(); ++i) {
			SDL_Rect src = { bulletUp[i].x, bulletUp[i].y - 30, bulletUp[i].w, bulletUp[i].h };
			this->_bullet.setSourceRectX(86);
			this->_bullet.setSourceRectW(6);
			this->_bullet.setSourceRectH(15);
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::drawDown(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletDown.size() != 0) {
		for (int i = 0; i < bulletDown.size(); ++i) {
			SDL_Rect src = { bulletDown[i].x, bulletDown[i].y + 30, bulletDown[i].w, bulletDown[i].h };
			this->_bullet.setSourceRectX(117);
			this->_bullet.setSourceRectW(6);
			this->_bullet.setSourceRectH(15);
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::drawLeft(Graphics & graphics, Player & player) {
	if (projectileConstants::active == true && bulletLeft.size() != 0) {
		for (int i = 0; i < bulletLeft.size(); ++i) {
			SDL_Rect src = { bulletLeft[i].x - 30, bulletLeft[i].y, bulletLeft[i].w, bulletLeft[i].h };
			this->_bullet.setSourceRectX(66);
			this->_bullet.setSourceRectW(12);
			this->_bullet.setSourceRectH(11);
			this->_bullet.drawProjectile(graphics, src);
		}
	}
}

void Projectile::draw(Graphics & graphics, Player & player)
{
	if (projectileConstants::active == true && bulletVec.size() != 0) {
	for (int i = 0; i < bulletVec.size(); ++i) {
		SDL_Rect src = { bulletVec[i].x, bulletVec[i].y, bulletVec[i].w, bulletVec[i].h };
		this->_bullet.setSourceRectX(97);
		this->_bullet.setSourceRectW(12);
		this->_bullet.setSourceRectH(11);
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

void Projectile::drawGun(Graphics & graphics, Player & player) {

	if (player.lookingLeft() && !player.lookingUp() && !player.lookingDown()) {
		_Gun.setSourceRectX(52);
		_Gun.setSourceRectY(10);
		this->_Gun.draw(graphics, player.getX() - 8, player.getY() + 14);
	}
	else if (player.lookingRight() && !player.lookingUp() && !player.lookingDown()) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(26);
		_Gun.setSourceRectH(6);
		this->_Gun.draw(graphics, player.getX() + 19, player.getY() + 16);
	}
	else if ((player.lookingUp() == true) && (player._facing == RIGHT)) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(34);
		this->_Gun.draw(graphics, player.getX() + 19, player.getY() + 16);
	}
	else if ((player.lookingUp() == true) && (player._facing == LEFT)) {
		_Gun.setSourceRectX(57);
		_Gun.setSourceRectY(50);
		this->_Gun.draw(graphics, player.getX() - 1, player.getY() + 14);
	}
	else if ((player.lookingDown() == true) && (player._facing == LEFT)) {
		_Gun.setSourceRectX(59);
		_Gun.setSourceRectY(70);
		_Gun.setSourceRectH(10);
		this->_Gun.draw(graphics, player.getX() - 4, player.getY() + 14);
	}
	else if ((player.lookingDown() == true) && (player._facing == RIGHT)) {
		_Gun.setSourceRectX(54);
		_Gun.setSourceRectY(86);
		_Gun.setSourceRectH(10);
		this->_Gun.draw(graphics, player.getX() + 20, player.getY() + 14);
	}	
}

void Projectile::drawDmgText(Graphics & graphics)
{
	if (projectileConstants::active == true && (projectileConstants::enemyY && projectileConstants::enemyX != 0)) {
		this->_txt->drawDmg(graphics, projectileConstants::enemyX, projectileConstants::enemyY, projectileConstants::BULLET_DMG);
	}
	
}

void Projectile::updateDmgText(float ElapsedTime) {
	this->_timeElapsed += ElapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) { //is it time to update/ remove text?
		this->_timeElapsed -= this->_timeToUpdate; 
		projectileConstants::enemyX = 0;
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

void Projectile::handleProjectileCollisions(std::vector<Enemy*> &others, Graphics &graphics) {
	for (int i = 0; i < others.size(); i++) { 
		others.at(i)->bulletHit(projectileConstants::BULLET_DMG);

		projectileConstants::enemyX = others.at(i)->getX();
		projectileConstants::enemyY = others.at(i)->getY();
		//this->drawDmgText(graphics, enemyX, enemyY, projectileConstants::BULLET_DMG);
		//this->drawDmgText(graphics);

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

void Projectile::setDmg(double dmg) {
	projectileConstants::BULLET_DMG = dmg;
	std::cout << "Dmg = " << projectileConstants::BULLET_DMG << std::endl;
}

double Projectile::checkDmg() {
	return projectileConstants::BULLET_DMG;
}

const float Projectile::getX() const
{
	return 0.0f;
}

const float Projectile::getY() const
{
	return 0.0f;
}

