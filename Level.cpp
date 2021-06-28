#include "Level.h"
#include "Graphics.h"
#include "Global.h"
#include "tinyxml2.h"
#include "utils.h"
#include "Enemy.h"
#include "Player.h"
#include "TextManager.h"
#include "Enemy.h"
#include "Npc.h"
#include "Items.h"
#include <SDL.h>
#include <sstream> //string stream
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>

using namespace tinyxml2; //all tinyxml2 is in a namespace because we will use so many features dont wanna write tinyxml:: all the time :D

template<typename T> Items * createInstance(Graphics& graphics, Vector2 spawnPoint) { return new T(graphics, spawnPoint); }

typedef std::map<std::string, Items* (*)(Graphics&, Vector2)> map_type;
map_type classMap;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics, Inventory &invent) :
	_mapName(mapName),
	_size(Vector2(0,0))
{
	this->loadMap(mapName, graphics, invent);
}

Level & Level::operator=(const Level & levelMap)
{
	//this->_enemies = levelMap._enemies;
	std::cout << "this size of enmy: " << this->_enemies.size()
		<< " | levelMap size: " << levelMap._enemies.size() << std::endl;
	this->_enemies.resize(levelMap._enemies.size());
	if (!levelMap._enemies.empty()) {
		for (int i = 0; i < levelMap._enemies.size(); i++) {
			this->_enemies[i] = levelMap._enemies[i];
		}
	}

	this->_items = levelMap._items;
	this->_npcs = levelMap._npcs;
	this->_animatedTileInfos = levelMap._animatedTileInfos;
	this->_animatedTileList = levelMap._animatedTileList;
	this->_backgroundTexture = levelMap._backgroundTexture;
	this->_collisionRects = levelMap._collisionRects;
	this->_doorList = levelMap._doorList;
	this->_lockDoor = levelMap._lockDoor;
	this->_mapName = levelMap._mapName;
	this->_size = levelMap._size;
	this->_slopes = levelMap._slopes;
	this->_spawnPoint = levelMap._spawnPoint;
	this->_tileList = levelMap._tileList;
	this->_tilesets = levelMap._tilesets;
	this->_tileSize = levelMap._tileSize;
	this->itemType = levelMap.itemType;
	this->mobDropList = levelMap.mobDropList;
	this->areaMap = levelMap.areaMap;
	// TODO: insert return statement here
	return *this;
}

Level::~Level() {
	std::cout << "Level Destructor called" << std::endl;
	//areaMap["ad"] = this;
	//if (!this->_enemies.empty()) {
	//	for (auto& ptr : this->_enemies)
	//		delete ptr;
	//}
}

void Level::loadMap(std::string mapName, Graphics &graphics, Inventory &invent) {
	//parse the .tmx file
	XMLDocument doc; //represents entire xml document
	std::stringstream ss;
	ss << mapName << ".tmx"; //ss << "content/maps" << mapName << ".tmx"; Pass in Map 1, we get content/maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());  //ss,.str convers stringstream into stream then string function called c_str converts string to c-string

	XMLElement* mapNode = doc.FirstChildElement("map"); //doc is the root of the xml doc, the first child element from the root called map we are selecting

	/* Save File function
	* getMap();
	* Returns the passed mapName value to game.cpp call
	* in order to save what map the player was on for 
	* the save file.
	*/


		//Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width); //20 will be in our temp variable
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height); //Now we have the size of our map

	//Get the width and the height of the Tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	//Load the tilesets. GOing to start everything with P
	XMLElement* pTileset = mapNode->FirstChildElement("tileset"); //if we have more then 1 tile set its a problem, so a work-around is linked list
	if (pTileset != NULL) { //just in case we dont have any tiles(?) so program wont crash
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source"); //returns a char* so thats why we set char* earlier
			char* path;
			std::stringstream ss;
			ss << source; //ss << "content/tilesets/" << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			//create a tilset variable and add it to our list of tilesets
			this->_tilesets.push_back(Tileset(tex, firstgid));
			//We now loaded our tileset! But now we need to load each tile :( Check below at loading the layers

			//Get all of the animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileID = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIDS.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					//printf("hi\n");
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset"); //will try to set our tileset to the next tileset (sibling) after first one process if there
			//isnt one it will move on to the next code
		}
	}
	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) { //loop through all layers
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) { //loop through tiles
						int tileCounter = 0; //add this because we need to know what tile we are on thru the loop
						while (pTile) {
							//build each individual tile here
							//if gid is 0, no tile should be drawn, continue loop.
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++; 
								if (pTile->NextSiblingElement("tile")) { //just because we dont wanna draw the tile ^ we cant just break we need to figure out if theres more gids
									pTile = pTile->NextSiblingElement("tile"); //we are doing this because if its a 0 it will never make it down below to move on
									continue; //because we are writing continue will go back to the while loop and go onto the next tile
								}
								else {
									break; //last tile, no more, break.
								}
								}
							//get the tileset for this specific gid. For our current map we only have 1 tileset however that wont always be case so lets add the logic
							int gid = pTile->IntAttribute("gid");
							int closest = 0;
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) { //the very first gid on a tileset, is the very lowest gid we can possibly have
									//so if its lower then the gid we are at, then its the one we want
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										//This is the tileset we want
										tls = this->_tilesets.at(i);
									}
								}
							}

							//if it cant find any tileset
							if (tls.FirstGid == -1) {
								//no tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							//Get the position of the tile in the level (confusing part)
							int xx = 0; //we need an x and y. We have a lot of variables so xx!
							int yy = 0;
							xx = tileCounter % width; //tile counter is which gid we are on(which tile we are on in the map) and mod(%) width
							//So to explain: first very first tile in the map is gid 34 So tile counter would be 0. Width of map is 20. So 0 mod(%) 20 = 0
							//Next tilecounter is 1 and 1 mod 20 is 1. 1 * 16 = 16 So our 2nd tile will be at an x of 16 (across) and that just works....
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							//tileHeight is 16. So we start at 0. Tilecounter is 0 / 20 = 0 * tileHeight (16) = 0 so our yy would be 0
							//Say our tile counter is at 20 (so 2nd row 1st tile for our map cave) 16 * (20/20[1]) = 16. 0 + 16 = 16
							//So it will draw the tile at a Y of 16. This algotrithm gets us to the right spot on the map
							Vector2 finalTilePosition = Vector2(xx, yy);

							//calculate the position of the tile in the tileset 
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);
							
							//Build the actual tile and add it to the level's tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < _animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileID == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIDS.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIDS.at(i), tileWidth, tileHeight));
								}
									AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
									this->_animatedTileList.push_back(tile);
							}
							else { //not animated tile, create static one
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							}
							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						//parse our x y
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height"); //these are taken as int, but tiles come out as floats (decimals) so we are going to round UP
						//round down would make collision too small perhaps and we fall through, bigger is always better in this case
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE, 
							std::ceil(y) * globals::SPRITE_SCALE, 
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));
						


						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//Other object groups go here with an else if (ss.str() == "whatever")
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points; //will hold all points
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y"))); //x,y already given to us in XML
						
						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' '); //static function: passing in string, vector, splitting up with space( )
							//Now we have each of the pairs. Loop through the list of pairs and split them into Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1)))); //stoi = string to int
							}
						}
						for (int i = 0; i < points.size(); i += 2) { //we are not using i++ because we want to skip every other point (not really)
							//because we will be using i - 1 to look at the last one and i + 1 to look at next one
							//need to figure out which point we are at, if we are at first point we dont wanna go anywhere, so add nothing to it.
							this->_slopes.push_back(Slope(Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE, 
																  (p1.y + points.at(i < 2 ? i : i - 1).y ) * globals::SPRITE_SCALE),
														  Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
																  (p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)
							)); 
							

						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			//SpawnPoint
			else if (ss.str() == "spawn points") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "player") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						//debug	
							this->_bulletPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "doors") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						float w = pObject->FloatAttribute("width");
						float h = pObject->FloatAttribute("height");
						Rectangle rect = Rectangle(x, y, w, h);

						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "destination") {
											const char* value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str());
											this->_doorList.push_back(door);
										}
										else if (ss.str() == "lockedDoor") {
											const char* value = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << value;
											Door lDoor = Door(rect, ss2.str());
											this->_lockDoor.push_back(lDoor);
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
							if (!mobDropList.empty()) {
								std::string mob = "bat";
								auto it = std::find_if(mobDropList.begin(), mobDropList.end(), [&mob](const auto& t) {return t.first == mob; });
								if (it == mobDropList.end()) {
									std::cout << "Not found & not empty" << std::endl;
									classMap["bat"] = &createInstance<SilverGem>;
									this->mobDropList.push_back(std::make_pair("bat", 30));
								}
							}
							else {
								classMap["bat"] = &createInstance<SilverGem>;
								this->mobDropList.push_back(std::make_pair("bat", 30));
							}
						}
						else if (ss.str() == "shade") {
							this->_enemies.push_back(new Shade(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE)));
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "npc") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "Luna") {
							//auto npObj = std::make_unique<Clock>(Clock(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
							//	std::floor(y) * globals::SPRITE_SCALE), ss.str()));
							//this->_npcs.push_back(std::move(npObj));
							this->_npcs.push_back(new Clock(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE), ss.str()));
							std::cout << "Luna added!" << std::endl;
							
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "item") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "HP") {
							if (!invent.isLooted(mapName, 0)) {
								this->_items.push_back(new HealthPotion(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
									std::floor(y) * globals::SPRITE_SCALE)));
								this->itemType.push_back(0);
								std::cout << "Item HP added!" << std::endl;
							}
						}
						else if (ss.str() == "permHP") {
							if (!invent.isLooted(mapName, 1)) {
								this->_items.push_back(new PermHP(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
								this->itemType.push_back(1);
								std::cout << "permHP added!" << std::endl;
							}
						}
						else if (ss.str() == "key") {
							if (!invent.isLooted(mapName, 3)) {
								this->_items.push_back(new Key(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
								this->itemType.push_back(3);
								std::cout << "Key item added to map" << std::endl;
							}
						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup"); //more then 1 obj group keep going
		}
	}
}

void Level::update(int elapsedTime, Player &player) {
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
	for (int i = 0; i < this->_npcs.size(); i++) {
		this->_npcs.at(i)->update(elapsedTime, player);
	}
	for (int i = 0; i < this->_items.size(); i++) {
		this->_items.at(i)->update(elapsedTime, player);
		if (this->_items.at(i)->isDroppedItem())
			this->checkItemFloorCollisions(this->_items.at(i));
	}

}

void Level::draw(Graphics &graphics, Player &player) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics); //loop thru all tiles and draw them
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}
	for (int i = 0; i < this->_npcs.size(); i++) {
		this->_npcs.at(i)->draw(graphics);
	}
	for (int i = 0; i < this->_items.size(); i++) {
		this->_items.at(i)->draw(graphics);
	}
	/* OLD code when maps/tile werent implemented
	//Draw the background
	//x = 0 , y = 0 because we start at top left corner of the bkBlue.png (64x64) width 64 / height 64
	SDL_Rect sourceRect = {0, 0, 64, 64}; //source rect will be entire bkBlue.png (map) Image is 64x64 we want entire thing
	SDL_Rect destRect; //where we are drawing to on the screen

	//Our screen size is obviously bigger than  64 so we need to loop this to draw multiple tiles in different spots
	//Nested for loop is a good thing to use here
	for (int x = 0; x < this->_size.x / 64; x++) { //how many pixels across / 64 size of background.
		//width of level divided by 64 is how many times we gotta draw this background (tile) going from left to right
		//Now we need to do it for top to bottom
		for (int y = 0; y < this->_size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE; //So it will draw it first at 0,0 64 pixels long then next time is x will be 1 starting at 64th pixels and going
			//all the way to the 128th pixel (another 64) and so on....
			//Do same for y
			destRect.y = y * 64 * globals::SPRITE_SCALE; //multiply by sprite scale so 2x size looks better!
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);

		}
	}
	*/
}

void Level::checkItemFloorCollisions(Items* obj)
{
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(obj->getBoundingBox())) {
			obj->addY(-0.8);
		}
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) { //Goes through all tiles and checks if they are colliding with other rectangle 
	//(player enemy or anything) and it will add it to a list and return that list of rectangles and return every rectangle we are colliding with might be 
	//more than 1
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) { //loop through our collision rects come from tiles
		if (this->_collisionRects.at(i).collidesWith(other)) { //Check if any of our collision rects collides with other
			others.push_back(this->_collisionRects.at(i)); //if it does add it to the others list
		}
	}
	return others; //return whatever collision rects we are colliding with

}

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for (int i = 0; i < this->_slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkDoorCollisions(const Rectangle &other) {
	std::vector<Door> others;
	for (int i = 0; i < this->_doorList.size(); i++) {
		if (this->_doorList.at(i).collidesWith(other)) {
			others.push_back(this->_doorList.at(i));
		}
	}
	return others;
}

std::vector<Door> Level::checkLockedDoorCollisions(const Rectangle & other)
{
	std::vector<Door> others;
	for (int i = 0; i < this->_lockDoor.size(); i++) {
		if (this->_lockDoor.at(i).collidesWith(other)) {
			others.push_back(this->_lockDoor.at(i));
		}
	}
	return others;
}

void Level::checkEnemyHP(Player & player, Graphics &graphics) {
	if (!this->_enemies.empty()) {
		for (int i = 0; i < this->_enemies.size(); i++) {
			if (this->_enemies.at(i)->getCurrentHealth() <= 0) {
				if (this->_enemies.at(i)->isRemoveable() == true) {
					bool dropRate = (rand() % 2) != 0; //50% chance
					this->_enemies.at(i)->setRemoveable();
					if (this->_enemies.at(i)->getCoinDropType() == "bronze") {
						if (dropRate) {
							this->_items.push_back(new BronzeCoin(graphics, Vector2(std::floor(this->_enemies.at(i)->getX()),
								std::floor(this->_enemies.at(i)->getY()))));
							this->itemType.push_back(2);
						}
					}
					else if (this->_enemies.at(i)->getCoinDropType() == "red") {
						this->_items.push_back(new RedCoin(graphics, Vector2(std::floor(this->_enemies.at(i)->getX()),
							std::floor(this->_enemies.at(i)->getY()))));
						this->itemType.push_back(2);
					}
					std::string mob = this->_enemies.at(i)->getName();
					auto it = std::find_if(mobDropList.begin(), mobDropList.end(), [&mob](const auto& t) {return t.first == mob; });
					auto distance = std::distance(this->mobDropList.begin(), it);
					if (it != mobDropList.end()) {
						std::random_device rd;  //Seed for the random number
						std::mt19937 gen(rd()); //Mersenne_twister_engine with rd seed
						std::uniform_int_distribution<> distrib(1, 100);
						if (distrib(gen) <= mobDropList[distance].second) {
							Items *b = classMap[mob](graphics, Vector2(std::floor(this->_enemies.at(i)->getX()), std::floor(this->_enemies.at(i)->getY())));
							this->_items.push_back(b);
							this->itemType.push_back(4);
						}

					}
					player.gainExp(this->_enemies.at(i)->enemyExpAmount());
					player.addKillCount(1);
					player.addKillTable(this->_enemies.at(i)->getName());
					//this->_enemies.erase(this->_enemies.begin() + i);
				}
			}
		}
	}
}

void Level::deallocateMemory()
{
	if (!this->_enemies.empty()) {
		std::cout << "enemy deletion. Size is: " << _enemies.size() << std::endl;
		for (auto& ptr : this->_enemies)
			delete ptr;
	}
	if (!this->_items.empty()) {
		std::cout << "item deletion. Size is: " << _items.size() << std::endl;
		for (auto& ptr : this->_items)
			delete ptr;
	}
	if (!this->_npcs.empty()) {
		std::cout << "NPC deletion. Size is: " << _npcs.size() << std::endl;
		for (auto& ptr : this->_npcs)
			delete ptr;
	}
	if (!classMap.empty())
		classMap.clear();
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
	std::vector<Enemy*> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other) && this->_enemies.at(i)->getCurrentHealth() >= 1) {
			others.push_back(this->_enemies.at(i));
		}
		/*
		if (this->_enemies.at(i)->getCurrentHealth() <= 0) {
			std::vector<Enemy*> death;
			death.push_back(_enemies.at(i))
			this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
				std::floor(y) * globals::SPRITE_SCALE)));
			_enemies.erase(_enemies.begin() + i);
			_enemies.at(i)->getX();
			_enemies.at(i)->playDeath();
			cout << "Enemy removed" << endl;
		}*/
	}
	return others;
}

std::vector<Enemy*> Level::checkBulletCollisions(const Rectangle &other) {
	std::vector<Enemy*> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}


std::vector<Npc*> Level::checkNpcCollisions(const Rectangle &other, Graphics &graphics) {
	std::vector<Npc*> others;
	for (int i = 0; i < this->_npcs.size(); i++) {
		if (this->_npcs.at(i)->getBoundingBox().collidesWith(other)) {
			//std::string tmp = this->_npcs.at(i)->getName();
			//this->_npcs.at(i)->runScript(tmp, graphics);
			others.push_back(this->_npcs.at(i));
			//cout << "debug: NPC touch" << endl;
		}
		else if (!this->_npcs.at(i)->getBoundingBox().collidesWith(other)) {
			//cout << "not colliding!" << endl;
			others.clear();
			return others;
		}
	}
	//cout << others.size() << endl;
	return others;

}

std::vector<Items*> Level::checkItemCollisions(Player & player, const Rectangle &other, Graphics &graphics, Inventory &invent) {
	std::vector<Items*> others;
	for (int i = 0; i < this->_items.size(); i++) {
		if (this->_items.at(i)->getBoundingBox().collidesWith(other)) {
			int type = itemType.at(i);
			std::cout << "type = " << type << std::endl;
			if (type == 1) //Permanent HP+1 item
				player.gainMaxHealth(5);
			else if (type == 2)
				player.gainCurrency(this->_items.at(i)->getAmount());
			else
				invent.storeItem(type);
			invent.addInstancedLoot(this->_mapName, type);
			others.push_back(this->_items.at(i));
			delete this->_items.at(i);
			this->_items.erase(_items.begin() + i);
			itemType.erase(itemType.begin() + i);
		}
	}
	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return 
		this->_spawnPoint;
}
//debug
const Vector2 Level::getBulletSpawnPoint() const {
	return
		this->_bulletPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	//calcualte the position of the tile in the tileset 
	int tilesetWidth, tilesetHeight;
	//pass in texture, format, access, width, height. We dont care about format or access so NULL and then reference to width/height
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = gid % (tilesetWidth / tileWidth) - 1;
	tsxx *= tileWidth; //we need to know where in our tileset where we are drawing from based on our gid alone

	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}