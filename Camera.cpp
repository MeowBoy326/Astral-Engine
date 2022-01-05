#include "Camera.h"
#include "Player.h"
#include <iostream>
#include <fstream>

static float previousY;
static SDL_Rect m_rect;

void Camera::Init()
{
    m_rect = {0, 0, 640, 480};
	m_rect.x = 0;
	m_rect.y = 0;
	m_rect.w = 640;
	m_rect.h = 480;
}


SDL_Rect Camera::GetRect()
{
	SDL_Rect GetRect = {m_rect.x,m_rect.y,640,480};
	return GetRect;
}

void Camera::Update(float elapsedTime, Player &player)
{
	previousY = m_rect.y;
	int newY = player.getY() - (globals::SCREEN_HEIGHT / 2);
	if (player._currentSurface == SLOPE) {
		// If there wasn't any significant change in Y then use the previous Y
		// to prevent the camera from flickering vertically.
		if (checkInRange(previousY - 3, previousY + 3, newY)) {
			newY = previousY;
		}
	}
	m_rect.x = player.getX() - (globals::SCREEN_WIDTH / 2);
	m_rect.y = newY;
}

void Camera::sceneUpdate(float x, float y)
{
	m_rect.x = x - (globals::SCREEN_WIDTH / 2);
	m_rect.y = y - (globals::SCREEN_HEIGHT / 2);
}


