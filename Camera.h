#pragma once
#include <SDL.h>
#include "Player.h"

class Player;

class Camera {
public:
	static void Init();
	static SDL_Rect GetRect();
	static void Update(float elapsedTime, Player &player);
	static void sceneUpdate(float x, float y);
};