/*
* Astral - A 2D Platform Game.
* Copyright(C) 2019 Arhum Zeena Nayyar
*
* This program is free software : you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <https://www.gnu.org/licenses/>.
*
* Email: arhumnayyar@gmail.com
*/

/**************************Main.cpp**************************
* Project: Astral
* Developed By: Arhum Z. Nayyar
* Main.cpp: Entry point of the game
*************************************************************/
#include <iostream>
#include "../headers/game.h"


/* Multi-threading (Not necessary, used as a demonstration) */

//// This function will be run on the update thread
//int UpdateThreadFunction(void* data) {
//	Game* game = static_cast<Game*>(data);
//
//	// Set up a timer to control the update loop
//	int LAST_UPDATE_TIME = SDL_GetTicks();
//	const int FPS = 50;
//	const int MAX_FRAME_TIME = 5 * 1000 / FPS; // Max amount of time a frame is allowed to last
//
//	while (true) {
//		// Calculate the elapsed time since the last update
//
//		const int CURRENT_TIME_MS = SDL_GetTicks();
//		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
//
//		// Update the game
//		game->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), game->getGraphics());
//
//		LAST_UPDATE_TIME = CURRENT_TIME_MS;
//	}
//
//	return 0;
//}

int main(int argc, char * argv[]) {
	Game game;

	/* Multi-threading (Not necessary, used as a demonstration) */

	//SDL_Thread* update_thread =
	//	SDL_CreateThread(UpdateThreadFunction, "UpdateThread", &game);

	//game.gameLoop();

	//SDL_WaitThread(update_thread, NULL);

	return 0;
}