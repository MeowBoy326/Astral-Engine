#pragma once

#include <sstream>

#ifndef GLOBAL_H
#define GLOBAL_H

namespace globals {
	/*
	 * Original: 640x480
	 * New available sizes:
	 * 1024x768
	 * 1280x720
	 * 1920x1080
	 */
	extern int SCREEN_WIDTH;
	extern int SCREEN_HEIGHT;

	const float SPRITE_SCALE = 2.0f; // will make it 2x
	const float PLAYER_SCALE = 2.5f;
	const float PROJECTILE_SCALE = 1.2f;

	extern bool SCALED_MODE;

	template <typename T>
	std::string to_string_with_precision(const T a_value, const int n = 6) {
		std::ostringstream out;
		out.precision(n);
		out << std::fixed << a_value;
		return std::move(out).str();
	}
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

	// Helper function
	    inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM : // If side = TOP, then return BOTTOM ELSE
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
	int x, y; // No public or private, structures are by default public and we want public
	Vector2() : // Default constructor
		x(0), y(0){}
	Vector2(int x, int y) : // Constructor we will be using most of the time
		x(x), y(y) {}
	Vector2 zero() { // Helper function - return new vector2 intializes 0,0 same as constructor but makes more sense to call 0 then to call empty default
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