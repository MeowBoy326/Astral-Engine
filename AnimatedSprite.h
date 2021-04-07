#ifndef ANIMATED_SPRITE_H
#define ANIMATED_SPRITE_H

#pragma once

#include "sprite.h"
#include "Global.h"

#include <vector>
#include <map>
#include <string>
#include <queue>


class Graphics;

/* AnimatedSprite class
* Holds logic for animating sprites
*/

class AnimatedSprite : public Sprite { //inherit from sprite class
public:
	AnimatedSprite();
	AnimatedSprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY, float timeToUpdate);
	//timetoupdate time until go to next frame in animation

	/* void playAnimation
	* Plays the animation provided if its not already playing
	*/

	void playAnimation(std::string animation, bool once = false); //this is how we will start playing different animations by storing animations by name 
	void playScript(std::string animation, bool once);
	void playBulletAnimation(std::string animation, int id);
	/* void update
	* Updates the animated sprite with a timer thats going to keep track when to move to next frame
	*/

	void update(int elapsedTime);
	void updateBoss(int elapstedTime, int y);
	void updateScript(int elapsedTime);
	//void updateBullet(int elapsedTime);

	/*void draw
	Draws the sprite to the screen
	*/
	void draw(Graphics &graphics, int x, int y);
	void drawBoss(Graphics &graphics, int x, int y);
	//void drawTextBox(Graphics & graphics, std::string speech, TTF_Font *font);
	void drawNpc(Graphics & graphics, int x, int y);
	void drawTitle(Graphics & graphics, int x, int y);
	void drawBullet(Graphics &graphics, int x, int y);

	

protected:
	double _timeToUpdate;
	bool _currentAnimationOnce;
	bool _currentScriptOnce;
	std::string _currentAnimation;
	std::string _currentScript;

	/* void addAnimation
	* Adds an animation to the map of animations for the sprite
	*/
	void addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset); //add animation to our list for sprites
	void addScript(int frames, int x, int y, std::string name, int width, int height, Vector2 offset);
	void addBulletAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset, int id);
	/* resetAnimations();
	* Resets all animations associated with this sprite
	*/
	void resetAnimations();

	void resetScripts();

	/* void stopAnimation
	* Stops the current animation
	*/
	void stopAnimation();

	void stopScript();

	/* void setVisible
	* Changes the visibility of the animated sprite
	*/
	void setVisible(bool visible);

	/* void animationDone
	*Logic that happens when an animation ends
	*/
	virtual void animationDone(std::string currentAnimation) = 0;

	/* void setupAnimations
	* A required (virtual) function that sets up all animations for a sprite
	* Any class that is animated sprite you must implement this, if you dont, there wont be any animations to go through
	*/

	virtual void setupAnimations() = 0; //virtual because we are not actually implementing here in animated sprite, eventually pure virtual until we make player class

private:
	std::map<std::string, std::vector<SDL_Rect>> _animation;
	std::map<std::string, std::vector<SDL_Rect>> _scripts;
	std::map<std::string, Vector2> _scriptOffsets;
	std::map<int, std::string, std::vector<SDL_Rect>> _bulletAnimation;
	std::map<std::string, Vector2> _offsets; //use this for example maybe not exactly 16px per frame or move where the sprite will be drawn, give an offset by x amount pixels

	int _frameIndex; //which frame in the animation we are at
	int _scriptIndex;
	int _bulletID;
	double _timeElapsed; //for timer
	bool _visible; //is animation visible?
	std::queue<std::string> speech;

};



#endif
