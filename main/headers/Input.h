#ifndef INPUT_H
#define INPUT_H

#include <SDL.h>
#include <map>
#include <algorithm>
#include <chrono>
#include <cstddef>  // implicitly added on non-windows machines
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <random>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
extern SDL_Keysym stateQuit;

class Input {
public:
	//!< Returns true if the two keys are the same.
	void loadKeys();

	void beginNewFrame(); // Called every new frame
	void keyUpEvent(const SDL_Event& event);
	void keyDownEvent(const SDL_Event& event);

	bool wasKeyPressed(SDL_Scancode key);
	bool wasKeyReleased(SDL_Scancode key);
	bool isKeyHeld(SDL_Scancode key);
private:
	std::map<SDL_Scancode, bool> _heldKeys;
	std::map<SDL_Scancode, bool> _pressedKeys;
	std::map<SDL_Scancode, bool> _releasedKeys;
};

#endif