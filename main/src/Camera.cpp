#include "../headers/Camera.h"
#include "../headers/Player.h"

#include <iostream>
#include <fstream>

static float previousY;
static SDL_Rect m_rect;

void Camera::Init() {}


SDL_Rect Camera::GetRect()
{
	SDL_Rect GetRect = {m_rect.x,m_rect.y,globals::SCREEN_WIDTH,globals::SCREEN_HEIGHT };
	return GetRect;
}

void Camera::Update(float elapsedTime, Player &player)
{
	if (globals::SCALED_MODE) {

		/* Works in all resolution cases since we just need to change the float value to integer */
		int scale_x = 1;
		int scale_y = 1;

		/* Inverse of the scale factor to calculate player position */
		int player_x = player.getX() / scale_x;
		int player_y = player.getY() / scale_y;

		int camera_x = player_x - (640 / 2);
		int camera_y = player_y - (480 / 2);

		m_rect.x = camera_x;
		m_rect.y = camera_y;
	}
	else {
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
}

void Camera::sceneUpdate(float x, float y)
{
	m_rect.x = x - (globals::SCREEN_WIDTH / 2);
	m_rect.y = y - (globals::SCREEN_HEIGHT / 2);
}


