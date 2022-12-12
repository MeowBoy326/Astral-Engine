#ifndef GLOBAL_H
#define GLOBAL_H

namespace globals {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

	const float SPRITE_SCALE = 2.0f; // will make it 2x
	const float PROJECTILE_SCALE = 1.2f; 
}

enum Surface {
	NOTHING,
	RECTANGLE,
	SLOPE
};

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	//helper function
	    inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM : //If side = TOP, then return BOTTOM ELSE
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Vector2 {
	int x, y; //no public or private, structures are by default public and we want public
	Vector2() : //default constructor
		x(0), y(0){}
	Vector2(int x, int y) : // Constructor we will be using most of the time
		x(x), y(y) {}
	Vector2 zero() { //helper function - return new vector2 intializes 0,0 same as constructor but makes more sense to call 0 then to call empty default
		// so we will use this function whenever we call
		return Vector2(0, 0);
	}

};

namespace {
	bool checkInRange(int low, int high, int x) {
		return ((x - high)*(x - low) <= 0);
	}
}


#endif