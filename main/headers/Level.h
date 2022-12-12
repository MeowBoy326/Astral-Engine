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

class TextManager;
class Graphics; //foward decalre
class Enemy;
class Effects;
class Projectile;
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
	Level& operator=(const Level& levelMap);
	Level(const Level&);
	~Level();

	void update(int elapsedTime, Player &player);
	void draw(Graphics &graphics, Player &player);
	void drawEnemyOutline(Graphics &graphics);
	void drawCollisionOutline(Graphics &graphics);
	void checkEnemyHP(Player & player, Graphics &graphics);
	void generateItems(Graphics &graphics);
	void generateMapItems(Graphics &graphics, std::string mapName, Inventory &invent);
	void generateEnemies(Graphics &graphics, std::string mapName, Player &player);
	void generateProjectile(Graphics &graphics, Player &player);
	void generateEffects(Graphics &graphics, Player &player);

	std::vector<Rectangle> checkTileCollisions(const Rectangle &other);
	std::vector<Rectangle> checkBreakableTileCollisions(const Rectangle &other);
	std::vector<Rectangle> checkEnemyTileCollision();
	std::vector<Rectangle> checkEnemyProjectileTileCollision();
	std::vector<Rectangle> checkCutsceneCollisions(const Rectangle &other);
	std::vector<Rectangle> checkLavaCollisions(const Rectangle &other);
	std::vector<Rectangle> checkPoisonCollisions(const Rectangle &other);
	std::vector<Rectangle> checkWaterCollisions(const Rectangle &other);
	std::vector<Rectangle> checkDeadzoneCollisions(const Rectangle &other);
	std::vector<Rectangle> checkLadderCollisions(const Rectangle &other);
	std::vector<Rectangle> checkSaveCollisions(const Rectangle &other);
	std::vector<Rectangle> checkArenaCollisions(const Rectangle &other);
	std::vector<Slope> checkSlopeCollisions(const Rectangle &other);
	std::vector<Door> checkDoorCollisions(const Rectangle &other);
	std::vector<Door> checkLockedDoorCollisions(const Rectangle &other);
	std::vector<Enemy*> checkEnemyCollisions(const Rectangle &other);
	std::vector<Enemy*> checkBulletCollisions(const Rectangle & other);
	std::vector<Npc*> checkNpcCollisions(const Rectangle & other, Graphics &graphics);
	void checkItemCollisions(Player &player, const Rectangle & other, Graphics & graphics, Inventory &invent);
	void checkItemFloorCollisions(Items* obj);
	void checkProjectileCollisions(Player &player);
	void checkProjectileTileCollisions();
	void checkProjectileBounds(Player &player);
	void checkProjectileBreakableLayer();

	std::string getMapBGM() const { return this->_mapBGM; }
	std::string getCutscene() const { return this->_cutsceneName; }
	float getSceneX(std::string name);
	float getSceneY(std::string name);
	void removeCutscene(std::string name);

	inline const std::string getMapName() const { return this->_mapName; }
	const Vector2 getPlayerSpawnPoint() const;

	bool isEnemyDead() const { return this->enemyDead; }
	void setEnemyDead(bool condition) { this->enemyDead = condition; }
	bool isArenaActive() const { return this->arenaActive; }

private:
	bool enemyDead = false;
	bool arenaActive = false;

	double bulletTimer = 0;
	double bulletDelay = 200;
	bool canShoot = true;

	std::string _mapName; 
	std::string _cutsceneName;
	std::string _arenaName;
	std::string _mapBGM;
	Vector2 _spawnPoint;
	Vector2 _bulletPoint;
	TextManager* _txt;

	Vector2 _size; // width height of entire map, not what we see but the entire map
	Vector2 _tileSize; //maps will all have same tile size so only 1 vector2
	Vector2 _mapSize;

	SDL_Texture* _backgroundTexture; // BkBlue.png

	std::vector<Tile> _tileList;
	std::vector<Tile> _breakTileList;
	std::vector<Tileset> _tilesets;
	std::vector<AnimatedTile> _animatedTileList;
	std::vector<AnimatedTileInfo> _animatedTileInfos;
	std::vector<Rectangle> _collisionRects; 
	std::vector<Rectangle> _cutsceneRects;
	std::vector<Rectangle> _lavaRects;
	std::vector<Rectangle> _poisonRects;
	std::vector<Rectangle> _waterRects;
	std::vector<Rectangle> _deadzoneRects;
	std::vector<Rectangle> _ladderRects;
	std::vector<Rectangle> _saveRects;
	std::vector<Rectangle> _arenaRects;
	std::vector<Slope> _slopes; //list of slopes
	std::vector<Door> _doorList;
	std::vector<Door> _lockDoor;
	std::vector<Enemy*> _enemies; //polymorphism to call the update and draw for the bat and not Enemy's functions, so needs to be a pointer for poly to work
	std::vector<Projectile*> _projectiles;
	std::vector<Effects*> _effects;
	std::vector<Npc*> _npcs;
	std::vector<Items*> _items;
	std::vector<int> itemType;
	std::vector<std::tuple<std::string, std::string, int>> levelDropTable; //mob name, item name, drop rate %
	std::vector<std::tuple<float, float, float, int>> dmgVector;

	//Private loads a map only call level within level class
	void loadMap(std::string mapName, Graphics &graphics, Inventory& invent); // wont need size or spawn because it will be parsed out of the xml within this function later
	//typedef std::map<std::string, Level* (*)(Graphics&, Vector2)> levelStorage;
	typedef std::map<std::string, Level (*)(std::string mapName, Graphics &graphics, Inventory &invent)> levelStorage;
	levelStorage areaMap;

	typedef std::map<std::string, Items* (*)(Graphics&, Vector2)> map_type;
	map_type classMap;

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
