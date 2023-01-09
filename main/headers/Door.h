#pragma once
#include "Rectangle.h";
#include "Global.h";

#include <string>

class Door : public Rectangle {
public:
	Door() {}
	Door(Rectangle r, std::string destination, int keyID) :
		Rectangle(r.getLeft() * globals::SPRITE_SCALE, r.getTop() * globals::SPRITE_SCALE,
			r.getWidth() * globals::SPRITE_SCALE, r.getHeight() * globals::SPRITE_SCALE),
		_destination(destination), keyID(keyID)
	{}

	const inline Rectangle getRectangle() const { return this->getRect(); }
	const inline std::string getDestination() const { return this->_destination; }
	const inline int getDoorKeyID() const { return this->keyID; }

private:
	std::string _destination;
	int keyID;
};
