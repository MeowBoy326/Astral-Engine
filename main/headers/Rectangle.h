#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Global.h"

class Rectangle {
public:
	Rectangle() {}

	Rectangle(int x, int y, int width, int height) :
		_x(x),
		_y(y),
		_width(width),
		_height(height) {

	}

	// Helper function to get certain parts of rectangle
	const int getCenterX() const {
		return this->_x + this->_width / 2; //take rectangle return x position and half the width, put us in the middle of rect
}
	const int getCenterY() const {return this->_y + this->_height / 2; }
	const int getLeft() const {return this->_x; }
	const int getRight() const {return this->_x + this->_width; }
	const int getTop() const { return this->_y; }
	const int getBottom() const { return this->_y + this->_height;  }

	const int getWidth() const { return this->_width; }
	const int getHeight() const { return this->_height; }

	const int getX() const { return this->_x; }
	const int getY() const { return this->_y; }

	const int getSide(const sides::Side side) const {
		return
			side == sides::LEFT ? this->getLeft() :
			side == sides::RIGHT ? this->getRight() :
			side == sides::TOP ? this->getTop() :
			side == sides::BOTTOM ? this->getBottom() :
			sides::NONE;
	}

	// Collision function
	//Takes in another rectangle and checks if the two are colliding
	const bool collidesWith(const Rectangle &other) const {
		return
			this->getRight() >= other.getLeft() &&
			this->getLeft() <= other.getRight() &&
			this->getTop() <= other.getBottom() &&
			this->getBottom() >= other.getTop(); //if all of these return true, return true! otherwise return false its not colliding
	}

	const bool projectileCollides(const Rectangle &other) const {
		return
			this->getRight() <= other.getLeft() &&
			this->getLeft() >= other.getRight() &&
			this->getTop() >= other.getBottom() &&
			this->getBottom() <= other.getTop();
	}

	const bool isValidRectangle() const {
		// Checks if rectangle is valid
		return (this->_x >= 0 && this->_y >= 0 && this->_width >= 0 && this->_height >= 0);
	}

	const inline Rectangle getRect() const { return *this; } //this = pointer to this instance of the class. In order to make it a regular rectangle and not a pointer
	// we use a * to derefrence it We wanna get this whole rectangle

private:
	int _x, _y, _width, _height;
};


#endif
