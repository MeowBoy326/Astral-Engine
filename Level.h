#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <queue>

#include "Global.h"
#include "Tile.h"
#include "Rectangle.h"
#include "Slope.h"
#include "AnimatedTile.h"
#include "Door.h"

class Graphics; //foward decalre
class Enemy;
class Npc;
class Player;
class Inventory;
class Items;
struct SDL_Texture; //foward declare
struct SDL_Rect;
struct Tileset;

class Level {
public:
	Level();
	Level(std::string mapName, Graphics &graphics, Inventory &invent);
	~Level();

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics, Player &player);
	void checkEnemyHP(Player & player, Graphics &graphics);
	void deallocateMemory();

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
	std::vector<Door> checkDoorCollisions(const Rectangle &other);
	std::vector<Door> checkLockedDoorCollisions(const Rectangle &other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);
	std::vector<Enemy*> checkBulletCollisions(const Rectangle & other);
	std::vector<Npc*> checkNpcCollisions(const Rectangle & other, Graphics &graphics);
	std::vector<Items*> checkItemCollisions(Player &player, const Rectangle & other, Graphics & graphics, Inventory &invent);
	void checkItemFloorCollisions(Items* obj);

	inline const std::string getMapName() const { return this->_mapName; }
	const Vector2 getPlayerSpawnPoint() const;
	//debug
	const Vector2 getBulletSpawnPoint() const;

private:
	std::string _mapName; 
	Vector2 _spawnPoint;
	Vector2 _bulletPoint;

	Vector2 _size; //width height of entire map, not what we see but the entire map
	Vector2 _tileSize; //maps will all have same tile size so only 1 vector2

	SDL_Texture* _backgroundTexture; //bkBlue.png

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;
	std::vector<Rectangle> _collisionRects; 
	std::vector<Slope> _slopes; //list of slopes
	std::vector<Door> _doorList;
	std::vector<Door> _lockDoor;
	std::vector<Enemy*> _enemies; //polymorphism to call the update and draw for the bat and not Enemy's functions, so needs to be a pointer for poly to work
	std::vector<Npc*> _npcs;
	std::vector<Items*> _items;
	std::vector<int> itemType;
	std::vector<std::pair<std::string, int>> mobDropList;

	//Private loads a map only call level within level class
	void loadMap(std::string mapName, Graphics &graphics, Inventory& invent); //wont need size or spawn because it will be parsed out of the xml within this function later

	Vector2 getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight);
};

//Tileset structure. Struct by default is public
struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;

	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};

#endif
