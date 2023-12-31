/*
* Input class
* Keeps track of Keyboard state
*/

#include "../headers/Input.h"

SDL_Keysym stateQuit;

void loadKey() {
#ifdef __APPLE__
	stateQuit.sym = SDLK_q;
	stateQuit.mod = KMOD_GUI;
#elif WIN32
	stateQuit.sym = SDLK_F4;
	stateQuit.mod = KMOD_ALT;
#else
	stateQuit.sym = SDLK_c;
	stateQuit.mod = KMOD_CTRL;
#endif
}


// This gets called at the beginning of each frame to reset the keys that are no longer relevant
void Input::beginNewFrame() {
	this->_pressedKeys.clear();
	this->_releasedKeys.clear();
}

// This gets called when a key has been pressed
void Input::keyDownEvent(const SDL_Event& event) {
	this->_pressedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = true;
}

// This gets called when a key gets released
void Input::keyUpEvent(const SDL_Event& event) {
	this->_releasedKeys[event.key.keysym.scancode] = true;
	this->_heldKeys[event.key.keysym.scancode] = false; // Because it was already set true when it was pressed and now we set false since we stopped holding
}

// Checks if a certain key was pressed during the current frame
bool Input::wasKeyPressed(SDL_Scancode key) {
	return this->_pressedKeys[key]; // will return a bool if it was pressed
}

// Checks if a certain key was released during the current frame
bool Input::wasKeyReleased(SDL_Scancode key) {
	return this->_releasedKeys[key];
}

// Checks if a certain key is currently being held
bool Input::isKeyHeld(SDL_Scancode key) {
	return this->_heldKeys[key];
}