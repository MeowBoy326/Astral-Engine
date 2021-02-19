#include "Camera.h"
#include "Player.h"
#include <iostream>
#include <fstream>

using namespace std;

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
	m_rect.x = player.getX() - (globals::SCREEN_WIDTH / 2);
	m_rect.y = player.getY() - (globals::SCREEN_HEIGHT / 2);

	GetRect();
}


