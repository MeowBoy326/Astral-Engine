#pragma once

#include "Global.h"
#include "Rectangle.h"

#include <algorithm>

class Slope {
public:
		Slope() {}
		Slope(Vector2 p1, Vector2 p2) :
			_p1(p1),
			_p2(p2) {
			// Calculate slope
			if (this->_p2.x - this->_p1.x != 0) {
				this->_slope = (fabs(this->_p2.y) - fabs(this->_p1.y)) / (fabs(this->_p2.x) - fabs(this->_p1.x));
			}
		}

		const inline float getSlope() const {
			return this->_slope;
		}

		const bool collidesWith(const Rectangle &other) {
			//rectangle we are passing in is the player or monster that is running on a slope
			if ((other.getRight() >= this->_p2.x &&
				other.getLeft() <= this->_p1.x &&
				other.getTop() <= this->_p2.y &&
				other.getBottom() >= this->_p1.y) ||
				(other.getRight() >= this->_p1.x &&
					other.getLeft() <= this->_p2.x &&
					other.getTop() <= this->_p1.y &&
					other.getBottom() >= this->_p2.y)) {
				if (isInsideSlope(other.getBottom(), other.getRight())) {
					return true;
				}
			}

			else if ((other.getLeft() <= this->_p1.x &&
				other.getRight() >= this->_p2.x &&
				other.getTop() <= this->_p1.y &&
				other.getBottom() >= this->_p2.y) ||
				(other.getLeft() <= this->_p2.x &&
					other.getRight() >= this->_p1.x &&
					other.getTop() <= this->_p2.y &&
					other.getBottom() >= this->_p1.y)) {
				if (isInsideSlope(other.getBottom(), other.getLeft())) {
					return true;
				}
			}
			// Checks every possible case to see if we are colliding with a rectangle and if any of these are true
			//that means we are colliding with a slope somewhere
			return false;
		}

		bool isInsideSlope(int rectY, int rectX) {
			float m = _slope;
			int b = (_p1.y - (_slope * fabs(_p1.x)));

			float slopeY = m * rectX + b;
			if (rectY >= slopeY) {
				return true;
			}
			else
				return false;
		}
		const inline Vector2 getP1() const { return this->_p1; }
		const inline Vector2 getP2() const { return this->_p2; }
private:
	Vector2 _p1, _p2; //point 1, 2 Every slope has 2 points.
	float _slope; //slopes have...SLOPES?!?! Madness...
};