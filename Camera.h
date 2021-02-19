#pragma once
#include <SDL.h>
#include "Player.h"

class Player;

class Camera {
public:
	//int x, y, w, h;
	static void Init(); // Use this instead of a constructor
	static SDL_Rect GetRect(); // FloatRect is SFML. Use SDL_Rect or something like that for SDL2.
	static void Update(float elapsedTime, Player &player);
	//static int valueX, valueY;
};