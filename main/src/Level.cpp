#include "../headers/Level.h"
#include "../headers/Graphics.h"
#include "../headers/Global.h"
#include "../headers/tinyxml2.h"
#include "../headers/utils.h"
#include "../headers/Enemy.h"
#include "../headers/Player.h"
#include "../headers/TextManager.h"
#include "../headers/Effects.h"
#include "../headers/Projectile.h"
#include "../headers/Npc.h"
#include "../headers/Items.h"

#include <SDL.h>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <set>
#include <filesystem>

#include "../headers/Inventory.h"

using namespace tinyxml2; // All tinyxml2 is in a namespace because we will use so many features dont wanna write tinyxml:: all the time :D

template<typename T> Items * createInstance(Graphics& graphics, Vector2 spawnPoint) { return new T(graphics, spawnPoint); }

// The bits at the end of the 32-bit tileset GID that determine the tile's flip position
const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;

Level::Level() {}

Level::Level(std::string mapName, Graphics &graphics, Inventory &invent) :
	_mapName(mapName),
	_size(Vector2(0,0))
{
	this->loadMap(mapName, graphics, invent);
}

Level & Level::operator=(const Level & levelMap)
{
	for (auto& p : this->_enemies) {
		if (p != nullptr) {
			delete p;
			p = NULL;
		}
	}
	for (auto& p : this->_items) {
		if (p != nullptr) {
			delete p;
			p = NULL;
		}
	}
	for (auto & p : this->_projectiles) {
		if (p != nullptr) {
			delete p;
			p = NULL;
		}
	}
	for (auto & p : this->_effects) {
		if (p != nullptr) {
			delete p;
			p = NULL;
		}
	}
	for (auto & p : this->_parallaxList) {
		if (p.second != nullptr) {
			delete p.second;
			p.second = NULL;
		}
	}
	this->_effects.clear();
	this->_projectiles.clear();
	this->_items.clear();
	this->_enemies.clear();
	this->_parallaxList.clear();
	//this->itemType = levelMap.itemType;
	this->_npcs = levelMap._npcs;
	this->_animatedTileInfos = levelMap._animatedTileInfos;
	this->_animatedTileList = levelMap._animatedTileList;
	this->_backgroundTexture = levelMap._backgroundTexture;
	this->_collisionRects = levelMap._collisionRects;
	this->_cutsceneRects = levelMap._cutsceneRects;
	this->_cutsceneName = levelMap._cutsceneName;
	this->_lavaRects = levelMap._lavaRects;
	this->_poisonRects = levelMap._poisonRects;
	this->_waterRects = levelMap._waterRects;
	this->_deadzoneRects = levelMap._deadzoneRects;
	this->_ladderRects = levelMap._ladderRects;
	this->_saveRects = levelMap._saveRects;
	this->_doorList = levelMap._doorList;
	this->_lockDoor = levelMap._lockDoor;
	this->_mapName = levelMap._mapName;
	this->_size = levelMap._size;
	this->_mapSize = levelMap._mapSize;
	this->_slopes = levelMap._slopes;
	this->_spawnPoint = levelMap._spawnPoint;
	this->_tileList = levelMap._tileList;
	this->_tilesets = levelMap._tilesets;
	this->_tileSize = levelMap._tileSize;
	this->_mapBGM = levelMap._mapBGM;
	this->_breakTileList = levelMap._breakTileList;
	this->_arenaRects = levelMap._arenaRects;
	this->_arenaName = levelMap._arenaName;
	this->arenaActive = levelMap.arenaActive;
	this->_txt = levelMap._txt;

	this->classMap.insert(levelMap.classMap.begin(), levelMap.classMap.end());
	std::set<std::string> values;
	for (auto it = classMap.begin(); it != classMap.end(); it++) {
		if (values.find(it->first) != values.end())
			classMap.erase(it--);
		else
			values.insert(it->first);
	}
	return *this;
}

Level::Level(const Level &)
{
}

Level::~Level() {
}

void Level::loadMap(std::string mapName, Graphics &graphics, Inventory &invent) {
	// Parse the .tmx file
	XMLDocument doc; // Represents entire xml document
	std::stringstream ss;
	ss << mapName << ".tmx"; // Ss << "content/maps" << mapName << ".tmx"; Pass in Map 1, we get content/maps/Map 1.tmx
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "maps";
	cwd.append(mapName + ".tmx");
	doc.LoadFile(cwd.string().c_str());  // Ss,.str convers stringstream into stream then string function called c_str converts string to c-string
	XMLElement* mapNode = doc.FirstChildElement("map"); // Doc is the root of the xml doc, the first child element from the root called map we are selecting

	/* Save File function
	* getMap();
	* Returns the passed mapName value to game.cpp call
	* in order to save what map the player was on for 
	* the save file.
	*/


		// Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width); //20 will be in our temp variable
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height); // Now we have the size of our map

	// Get the width and the height of the Tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);

	this->_mapSize = Vector2(this->_size.x * this->_tileSize.x, this->_size.y * this->_tileSize.y);

	// Load BGM
	XMLElement* propsNode = mapNode->FirstChildElement("properties");
	if (propsNode != NULL) {
		XMLElement* prop = propsNode->FirstChildElement("property");
		while (prop) {
			const char* propsName = prop->Attribute("name");
			std::stringstream ssProp;
			ssProp << propsName;
			if (ssProp.str() == "BGM") {
				const char* getBGMName = prop->Attribute("value");
				this->_mapBGM = getBGMName;
			}
			//else if (ssProp.str() == "PXLayer1") {
			//	const char* bgValue = prop->Attribute("value");
			//	std::stringstream bgFileName;
			//	bgFileName << "data/maps/" << bgValue << ".png";
			//	SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
			//	Parallax* pxLayer = new Parallax(tex, 1.0f);
			//	this->_parallaxList.push_back(std::make_pair(1, pxLayer));
			//}
			//else if (ssProp.str() == "PXLayer2") {
			//	const char* bgValue = prop->Attribute("value");
			//	std::stringstream bgFileName;
			//	bgFileName << "data/maps/" << bgValue << ".png";
			//	SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
			//	Parallax* pxLayer = new Parallax(tex, 1.0f);
			//	this->_parallaxList.push_back(std::make_pair(2, pxLayer));
			//}
			//else if (ssProp.str() == "PXLayer3") {
			//	const char* bgValue = prop->Attribute("value");
			//	std::stringstream bgFileName;
			//	bgFileName << "data/maps/" << bgValue << ".png";
			//	SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
			//	Parallax* pxLayer = new Parallax(tex, 1.4f);
			//	this->_parallaxList.push_back(std::make_pair(3, pxLayer));
			//}
			//else if (ssProp.str() == "PXLayer4") {
			//	const char* bgValue = prop->Attribute("value");
			//	std::stringstream bgFileName;
			//	bgFileName << "data/maps/" << bgValue << ".png";
			//	SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
			//	Parallax* pxLayer = new Parallax(tex, 2.0f);
			//	this->_parallaxList.push_back(std::make_pair(4, pxLayer));
			//}
			prop = prop->NextSiblingElement("property");
		}
	}
	//std::sort(this->_parallaxList.begin(), this->_parallaxList.end());

	// Load the tilesets.
	XMLElement* pTileset = mapNode->FirstChildElement("tileset"); // If we have more then 1 tile set its a problem, so a work-around is linked list
	if (pTileset != NULL) { // Just in case we dont have any tiles(?) so program wont crash
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source"); // Returns a char* so thats why we set char* earlier
			std::stringstream ss;
			// Ss << source; // Ss << "content/tilesets/" << source;
			ss << "data/maps/" << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			// Create a tilset variable and add it to our list of tilesets
			this->_tilesets.push_back(Tileset(tex, firstgid));
			// we now loaded our tileset! But now we need to load each tile :( Check below at loading the layers

			// Get all of the animations for that tileset
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
					// Printf("hi\n");
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset"); // will try to set our tileset to the next tileset (sibling) after first one process if there
			// Isnt one it will move on to the next code
		}
	}
	// Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) { // Loop through all layers
			// Loading the data element
				const char* lyrName = pLayer->Attribute("name");
				std::stringstream ssProp;
				ssProp << lyrName;
				if (ssProp.str() == "breakLayer") {
					XMLElement* pData = pLayer->FirstChildElement("data");
					if (pData != NULL) {
						while (pData) {
							// Loading the tile element
							XMLElement* pTile = pData->FirstChildElement("tile");
							if (pTile != NULL) { // Loop through tiles
								int tileCounter = 0; // Add this because we need to know what tile we are on thru the loop
								while (pTile) {
									// Build each individual tile here
									// If gid is 0, no tile should be drawn, continue loop.
									if (pTile->IntAttribute("gid") == 0) {
										tileCounter++;
										if (pTile->NextSiblingElement("tile")) { // Just because we dont wanna draw the tile ^ we cant just break we need to figure out if theres more gids
											pTile = pTile->NextSiblingElement("tile"); // we are doing this because if its a 0 it will never make it down below to move on
											continue; // Because we are writing continue will go back to the while loop and go onto the next tile
										}
										else {
											break; // Last tile, no more, break.
										}
									}
									// Get the tileset for this specific gid. For our current map we only have 1 tileset however that wont always be case so lets add the logic
									int gid = pTile->IntAttribute("gid");
									int closest = 0;
									Tileset tls;
									for (int i = 0; i < this->_tilesets.size(); i++) {
										if (this->_tilesets[i].FirstGid <= gid) { // The very first gid on a tileset, is the very lowest gid we can possibly have
											// So if its lower then the gid we are at, then its the one we want
											if (this->_tilesets[i].FirstGid > closest) {
												closest = this->_tilesets[i].FirstGid;
												// This is the tileset we want
												tls = this->_tilesets.at(i);
											}
										}
									}

									// If it cant find any tileset
									if (tls.FirstGid == -1) {
										// No tileset was found for this gid
										tileCounter++;
										if (pTile->NextSiblingElement("tile")) {
											pTile = pTile->NextSiblingElement("tile");
											continue;
										}
										else {
											break;
										}
									}
									// Get the position of the tile in the level (confusing part)
									int xx = 0; // we need an x and y. We have a lot of variables so xx!
									int yy = 0;
									xx = tileCounter % width; // Tile counter is which gid we are on(which tile we are on in the map) and mod(%) width
									// So to explain: first very first tile in the map is gid 34 So tile counter would be 0. Width of map is 20. So 0 mod(%) 20 = 0
									// Next tilecounter is 1 and 1 mod 20 is 1. 1 * 16 = 16 So our 2nd tile will be at an x of 16 (across) and that just works....
									xx *= tileWidth;
									yy += tileHeight * (tileCounter / width);
									// TileHeight is 16. So we start at 0. Tilecounter is 0 / 20 = 0 * tileHeight (16) = 0 so our yy would be 0
									// Say our tile counter is at 20 (so 2nd row 1st tile for our map cave) 16 * (20/20[1]) = 16. 0 + 16 = 16
									// So it will draw the tile at a Y of 16. This algotrithm gets us to the right spot on the map
									Vector2 finalTilePosition = Vector2(xx, yy);

									// Calculate the position of the tile in the tileset 
									Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

									// Build the actual tile and add it to the level's tile list
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
									else { // Not animated tile, create static one
										Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition, SDL_FLIP_NONE);
										this->_breakTileList.push_back(tile);
									}
									tileCounter++;
									pTile = pTile->NextSiblingElement("tile");
								}
							}
							break;
							// PData = pData->NextSiblingElement("data");
						}
					}
					break;
					// PLayer = pLayer->NextSiblingElement("layer");
				}

			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					// Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) { // Loop through tiles
						int tileCounter = 0; // Add this because we need to know what tile we are on thru the loop
						while (pTile) {
							// Build each individual tile here
							// If gid is 0, no tile should be drawn, continue loop.
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++; 
								if (pTile->NextSiblingElement("tile")) { // Just because we dont wanna draw the tile ^ we cant just break we need to figure out if theres more gids
									pTile = pTile->NextSiblingElement("tile"); // we are doing this because if its a 0 it will never make it down below to move on
									continue; // Because we are writing continue will go back to the while loop and go onto the next tile
								}
								else {
									break; // Last tile, no more, break.
								}
								}
							// Get the tileset for this specific gid. For our current map we only have 1 tileset however that wont always be case so lets add the logic
							int gid = pTile->IntAttribute("gid");
							// Read out the flags for flipped tiles
							SDL_RendererFlip tileFlip = SDL_FLIP_NONE;
							bool flipped_horizontally = (gid & FLIPPED_HORIZONTALLY_FLAG);
							bool flipped_vertically = (gid & FLIPPED_VERTICALLY_FLAG);
							bool flipped_diagonally = (gid & FLIPPED_DIAGONALLY_FLAG);
							if (flipped_horizontally) {
								tileFlip = SDL_FLIP_HORIZONTAL;
							}
							else if (flipped_vertically) {
								tileFlip = SDL_FLIP_VERTICAL;
							}
							else if (flipped_diagonally) {
								// TODO: Diagonal flips/rotations
							}
							// Clear the flag bits only
							gid &= ~(FLIPPED_HORIZONTALLY_FLAG |
								FLIPPED_VERTICALLY_FLAG |
								FLIPPED_DIAGONALLY_FLAG);
							int closest = 0;
							Tileset tls;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) { // The very first gid on a tileset, is the very lowest gid we can possibly have
									// So if its lower then the gid we are at, then its the one we want
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										// This is the tileset we want
										tls = this->_tilesets.at(i);
									}
								}
							}

							// If it cant find any tileset
							if (tls.FirstGid == -1) {
								// No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else {
									break;
								}
							}
							// Get the position of the tile in the level (confusing part)
							int xx = 0; // we need an x and y. We have a lot of variables so xx!
							int yy = 0;
							xx = tileCounter % width; // Tile counter is which gid we are on(which tile we are on in the map) and mod(%) width
							// So to explain: first very first tile in the map is gid 34 So tile counter would be 0. Width of map is 20. So 0 mod(%) 20 = 0
							// Next tilecounter is 1 and 1 mod 20 is 1. 1 * 16 = 16 So our 2nd tile will be at an x of 16 (across) and that just works....
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width);
							// TileHeight is 16. So we start at 0. Tilecounter is 0 / 20 = 0 * tileHeight (16) = 0 so our yy would be 0
							// Say our tile counter is at 20 (so 2nd row 1st tile for our map cave) 16 * (20/20[1]) = 16. 0 + 16 = 16
							// So it will draw the tile at a Y of 16. This algotrithm gets us to the right spot on the map
							Vector2 finalTilePosition = Vector2(xx, yy);

							// Calculate the position of the tile in the tileset 
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);
							
							// Build the actual tile and add it to the level's tile list
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
							else { // Not animated tile, create static one
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition, tileFlip);
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

	// Parse out the collisions
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
						// Parse our x y
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height"); // These are taken as int, but tiles come out as floats (decimals) so we are going to round UP
						// Round down would make collision too small perhaps and we fall through, bigger is always better in this case
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE, 
							std::ceil(y) * globals::SPRITE_SCALE, 
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));
						


						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "cutscenes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_cutsceneRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));

						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						this->_cutsceneName = ss.str();

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "lava") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_lavaRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "poison") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_poisonRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "water") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_waterRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "deadzone") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_deadzoneRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "ladder") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject != NULL) {
				while (pObject) {
					float x, y, width, height;
					x = pObject->FloatAttribute("x");
					y = pObject->FloatAttribute("y");
					width = pObject->FloatAttribute("width");
					height = pObject->FloatAttribute("height");
					this->_ladderRects.push_back(Rectangle(
						std::ceil(x) * globals::SPRITE_SCALE,
						std::ceil(y) * globals::SPRITE_SCALE,
						std::ceil(width) * globals::SPRITE_SCALE,
						std::ceil(height) * globals::SPRITE_SCALE));

					pObject = pObject->NextSiblingElement("object");
				}
			  }
			}
			else if (ss.str() == "saves") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject != NULL) {
				while (pObject) {
					float x, y, width, height;
					x = pObject->FloatAttribute("x");
					y = pObject->FloatAttribute("y");
					width = pObject->FloatAttribute("width");
					height = pObject->FloatAttribute("height");
					this->_saveRects.push_back(Rectangle(
						std::ceil(x) * globals::SPRITE_SCALE,
						std::ceil(y) * globals::SPRITE_SCALE,
						std::ceil(width) * globals::SPRITE_SCALE,
						std::ceil(height) * globals::SPRITE_SCALE));

					pObject = pObject->NextSiblingElement("object");
				}
			  }
			}
			else if (ss.str() == "arena") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");
			if (pObject != NULL) {
				while (pObject) {
					float x, y, width, height;
					x = pObject->FloatAttribute("x");
					y = pObject->FloatAttribute("y");
					width = pObject->FloatAttribute("width");
					height = pObject->FloatAttribute("height");
					this->_arenaRects.push_back(Rectangle(
						std::ceil(x) * globals::SPRITE_SCALE,
						std::ceil(y) * globals::SPRITE_SCALE,
						std::ceil(width) * globals::SPRITE_SCALE,
						std::ceil(height) * globals::SPRITE_SCALE));

					const char* name = pObject->Attribute("name");
					std::stringstream ss;
					ss << name;
					this->_arenaName = ss.str();
					this->arenaActive = true;

					pObject = pObject->NextSiblingElement("object");
				}
			}
			}
			// Other object groups go here with an else if (ss.str() == "whatever")
			else if (ss.str() == "slopes") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						std::vector<Vector2> points; // will hold all points
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y"))); // x,y already given to us in XML
						
						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' '); // Static function: passing in string, vector, splitting up with space( )
							// Now we have each of the pairs. Loop through the list of pairs and split them into Vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1)))); // Stoi = string to int
							}
						}
						for (int i = 0; i < points.size(); i += 2) { // we are not using i++ because we want to skip every other point (not really)
							// Because we will be using i - 1 to look at the last one and i + 1 to look at next one
							// Need to figure out which point we are at, if we are at first point we dont wanna go anywhere, so add nothing to it.
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
			// SpawnPoint
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
						// Debug	
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
									int keyID = 0;
									bool isLocked = false;
									std::stringstream ss2;
									while (pProperty) {
										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "destination") {
											const char* value = pProperty->Attribute("value");
											//std::stringstream ss2;
											ss2 << value;
											Door door = Door(rect, ss2.str(), 0);
											this->_doorList.push_back(door);
										}
										else if (ss.str() == "lockedDoor") {
											const char* value = pProperty->Attribute("value");
											//std::stringstream ss2;
											ss2 << value;
											//Door lDoor = Door(rect, ss2.str());
											//this->_lockDoor.push_back(lDoor);
										}
										else if (ss.str() == "keyID") {
											int value = pProperty->IntAttribute("value");
											keyID = value;
											isLocked = true;
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
									if (isLocked)
									{
										Door lDoor = Door(rect, ss2.str(), keyID);
										this->_lockDoor.push_back(lDoor);
									}
									
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}

						}

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "npc") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				int npcID = 0;
				// Get Custom Properties first
				XMLElement* propsNode = pObject->FirstChildElement("properties");
				if (propsNode != NULL) {
					const char* propsName = propsNode->FirstChildElement("property")->Attribute("name");
					std::stringstream ssProp;
					ssProp << propsName;
					if (ssProp.str() == "NpcID") {
						const int getNpcId = propsNode->FirstChildElement("property")->IntAttribute("value");
						npcID = getNpcId;
					}
				}
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "Luna") {
							this->_npcs.push_back(new Luna(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
								std::floor(y) * globals::SPRITE_SCALE), ss.str(), npcID));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup"); // More then 1 obj group keep going
		}
	}
}

void Level::update(float elapsedTime, Player &player) {
	// Allowing the timer to elapse without checking if canShoot == false 
	// Actually provides a more stable DPS. If using a check, it creates
	// A wider variety towards DPS. So, keep it like this.
	this->bulletTimer += elapsedTime;
	if (this->bulletTimer >= this->bulletDelay) {
		this->canShoot = true;
		this->bulletTimer = 0;
	}
	for (int i = 0; i < this->_parallaxList.size(); i++) {
		this->_parallaxList.at(i).second->update(elapsedTime, player.getPlayerDX(), player.getPlayerDY());
	}
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
	if (this->arenaActive) {
		for (int i = 0; i < this->_effects.size(); i++) {
			this->_effects.at(i)->update(elapsedTime, player);
		}
	}
	for (int i = 0; i < this->_projectiles.size(); i++) {
		this->_projectiles.at(i)->update(elapsedTime, player);
	}
	for (int i = 0; i < this->dmgVector.size(); i++) {
		std::get<3>(this->dmgVector[i]) += elapsedTime;
		std::get<1>(this->dmgVector[i]) -= 0.09;
		if (this->dmgVector.size() >= 2)
			std::get<1>(this->dmgVector[i]) -= 0.18;
		if (std::get<3>(this->dmgVector[i]) >= 600) {
			this->dmgVector.erase(this->dmgVector.begin() + i);
		}
	}
}

void Level::draw(Graphics &graphics, Player &player) {
	for (int i = 0; i < this->_parallaxList.size(); i++) {
		this->_parallaxList.at(i).second->draw(graphics);
	}
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics); // Loop thru all tiles and draw them
	}
	for (int i = 0; i < this->_breakTileList.size(); i++) {
		this->_breakTileList.at(i).draw(graphics);
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
	if (this->arenaActive) {
		for (int i = 0; i < this->_effects.size(); i++) {
			this->_effects.at(i)->draw(graphics);
		}
	}
	for (int i = 0; i < this->_projectiles.size(); i++) {
		this->_projectiles.at(i)->draw(graphics);
	}
	for (int i = 0; i < this->dmgVector.size(); i++) {
		_txt->drawDmg(graphics, std::get<0>(this->dmgVector[i]), std::get<1>(this->dmgVector[i]),
			std::get<2>(this->dmgVector[i]));
	}

	/* OLD code when maps/tile werent implemented
	// Draw the background
	// x = 0 , y = 0 because we start at top left corner of the bkBlue.png (64x64) width 64 / height 64
	SDL_Rect sourceRect = {0, 0, 64, 64}; // Source rect will be entire bkBlue.png (map) Image is 64x64 we want entire thing
	SDL_Rect destRect; // where we are drawing to on the screen

	// Our screen size is obviously bigger than  64 so we need to loop this to draw multiple tiles in different spots
	// Nested for loop is a good thing to use here
	for (int x = 0; x < this->_size.x / 64; x++) { // How many pixels across / 64 size of background.
		// width of level divided by 64 is how many times we gotta draw this background (tile) going from left to right
		// Now we need to do it for top to bottom
		for (int y = 0; y < this->_size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE; // So it will draw it first at 0,0 64 pixels long then next time is x will be 1 starting at 64th pixels and going
			// All the way to the 128th pixel (another 64) and so on....
			// Do same for y
			destRect.y = y * 64 * globals::SPRITE_SCALE; // Multiply by sprite scale so 2x size looks better!
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);

		}
	}
	*/
}

void Level::drawEnemyOutline(Graphics & graphics)
{
	SDL_Color color = { 245, 66, 224, 255 };
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->drawBoundingbox(graphics, this->_enemies.at(i)->getBoundingBox().getX(),
			this->_enemies.at(i)->getBoundingBox().getY(), color);
	}
}

void Level::drawCollisionOutline(Graphics & graphics)
{
	SDL_Color color = { 52, 235, 58, 255 };
	SDL_Rect collRect = { 0, 0, 0, 0 };

	// Rectanglular collision
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		collRect.x = this->_collisionRects.at(i).getX();
		collRect.y = this->_collisionRects.at(i).getY();
		collRect.w = this->_collisionRects.at(i).getWidth();
		collRect.h = this->_collisionRects.at(i).getHeight();
		graphics.blitBoundingbox(collRect, color);
	}

	// Slope (polygon) collision
	color = { 232, 232, 30, 255 };
	SDL_Point startPoint = { 0, 0 };
	SDL_Point endPoint = { 0, 0 };
	for (int i = 0; i < this->_slopes.size(); i++) {
		startPoint.x = this->_slopes.at(i).getP1().x;
		startPoint.y = this->_slopes.at(i).getP1().y;
		endPoint.x = this->_slopes.at(i).getP2().x;
		endPoint.y = this->_slopes.at(i).getP2().y;
		graphics.blitSlopeOutline(startPoint, endPoint, color);
	}

	// Ladder collision
	color = { 232, 134, 30, 255 };
	for (int i = 0; i < this->_ladderRects.size(); i++) {
		collRect.x = this->_ladderRects.at(i).getX();
		collRect.y = this->_ladderRects.at(i).getY();
		collRect.w = this->_ladderRects.at(i).getWidth();
		collRect.h = this->_ladderRects.at(i).getHeight();
		graphics.blitBoundingbox(collRect, color);
	}

	// Player projectile collision
	color = { 28, 119, 237, 255 };
	for (int i = 0; i < this->_projectiles.size(); i++) {
		collRect.x = this->_projectiles.at(i)->getX();
		collRect.y = this->_projectiles.at(i)->getY();
		collRect.w = this->_projectiles.at(i)->getBoundingBox().getWidth();
		collRect.h = this->_projectiles.at(i)->getBoundingBox().getHeight();
		graphics.blitBoundingbox(collRect, color);
	}
}

void Level::checkItemFloorCollisions(Items* obj)
{
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(obj->getBoundingBox())) {
			obj->addY(-0.8f);
		}
	}
}

void Level::checkProjectileCollisions(Player & player) {
	for (int i = 0; i < this->_enemies.size(); ++i) {
		for (int j = 0; j < this->_projectiles.size(); ++j) {
			if (this->_enemies.at(i)->getBoundingBox().collidesWith(this->_projectiles.at(j)->getBoundingBox()) &&
				this->_enemies.at(i)->getCurrentHealth() > 0) {
				double damage = player.getDmgMod();
				if (this->_enemies.at(i)->getEnemyLevel() > player.getLevel()) {
					if (this->_enemies.at(i)->getEnemyLevel() - player.getLevel() >= 2)
						damage = damage * 0.66;
					else
						damage = damage * 0.88;
				}
				double damageReduction = player.getDmgReduction();
				damage -= damageReduction;
				this->_enemies.at(i)->bulletHit(damage);
				this->dmgVector.push_back(std::make_tuple(this->_enemies.at(i)->getX(), this->_enemies.at(i)->getY(),
					damage, 0));
				player.handleRestoreableHealth(damage);
				if (player.hasLifeStealActive()) {
					if (this->_enemies.at(i)->isBoss() || this->_enemies.at(i)->isMiniBoss()) {
						player.gainHealth(static_cast<float>(this->_enemies.at(i)->getMaxHealth() / 4 + damage) * player.getLifeSteal());
					}
					else {
						player.gainHealth(static_cast<float>(this->_enemies.at(i)->getMaxHealth() + damage) * player.getLifeSteal());
					}
				}
				delete this->_projectiles.at(j);
				this->_projectiles.erase(this->_projectiles.begin() + j);
			}
		}
	}
}

void Level::checkProjectileTileCollisions()
{
	for (int i = 0; i < this->_collisionRects.size(); ++i) {
		for (int j = 0; j < this->_projectiles.size(); ++j) {
			if (this->_collisionRects.at(i).collidesWith(this->_projectiles.at(j)->getProjectileBBox())) {
				delete this->_projectiles.at(j);
				this->_projectiles.erase(this->_projectiles.begin() + j);
			}
		}
	}
}

void Level::checkProjectileBounds(Player & player)
{
	for (int i = 0; i < this->_projectiles.size(); ++i) {
		if (this->_projectiles[i]->getProjectileDirection() == LEFT && 
			this->_projectiles[i]->getX() <= this->_projectiles[i]->getProjectileStartingX() - 250) {
			delete this->_projectiles[i];
			this->_projectiles.erase(this->_projectiles.begin() + i);
		}
		else if (this->_projectiles[i]->getProjectileDirection() == RIGHT &&
			this->_projectiles[i]->getX() >= this->_projectiles[i]->getProjectileStartingX() + 250) {
			delete this->_projectiles[i];
			this->_projectiles.erase(this->_projectiles.begin() + i);
		}
		else if (this->_projectiles[i]->getProjectileDirection() == UP &&
			this->_projectiles[i]->getY() <= this->_projectiles[i]->getProjectileStartingY() - 250) {
			delete this->_projectiles[i];
			this->_projectiles.erase(this->_projectiles.begin() + i);
		}
		else if (this->_projectiles[i]->getProjectileDirection() == DOWN &&
			this->_projectiles[i]->getY() >= this->_projectiles[i]->getProjectileStartingY() + 250) {
			delete this->_projectiles[i];
			this->_projectiles.erase(this->_projectiles.begin() + i);
		}
	}
}

void Level::checkProjectileBreakableLayer()
{
	for (int i = 0; i < this->_projectiles.size(); i++) {
		for (int j = 0; j < this->_breakTileList.size(); j++) {
			Rectangle brTile = { this->_breakTileList.at(j).getTilePosition().x, this->_breakTileList.at(j).getTilePosition().y,
			16, 16 };
			if (brTile.collidesWith(this->_projectiles.at(i)->getBoundingBox())) {
				this->_breakTileList.erase(this->_breakTileList.begin() + j);
			}
		}
	}
}

float Level::getSceneX(std::string name)
{
	for (int i = 0; i < this->_enemies.size(); ++i) {
		if (this->_enemies.at(i)->getName() == name)
			return this->_enemies.at(i)->getX();
	}
	return 0.0f;
}

float Level::getSceneY(std::string name)
{
	for (int i = 0; i < this->_enemies.size(); ++i) {
		if (this->_enemies.at(i)->getName() == name)
			return this->_enemies.at(i)->getY();
	}
	return 0.0f;
}

void Level::removeCutscene(std::string name)
{
	for (int i = 0; i < this->_cutsceneRects.size(); i++) { // Loop through our collision rects come from tiles
		this->_cutsceneRects.erase(this->_cutsceneRects.begin() + i);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) { // Goes through all tiles and checks if they are colliding with other rectangle 
	//(player enemy or anything) and it will add it to a list and return that list of rectangles and return every rectangle we are colliding with might be 
	// More than 1
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) { // Loop through our collision rects come from tiles
		if (this->_collisionRects.at(i).collidesWith(other)) { // Check if any of our collision rects collides with other
			others.push_back(this->_collisionRects.at(i)); // If it does add it to the others list
		}
	}
	return others; // Return whatever collision rects we are colliding with
}

std::vector<Rectangle> Level::checkBreakableTileCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_breakTileList.size(); i++) {
		Rectangle brTile = { this->_breakTileList.at(i).getTilePosition().x, this->_breakTileList.at(i).getTilePosition().y,
		16, 16 };
		if (other.collidesWith(brTile)) {
			others.push_back(brTile);
		}
	}
	return others;
}

std::vector<Rectangle> Level::checkEnemyTileCollision()
{
	std::vector<Rectangle> others; 
	for (int i = 0; i < this->_enemies.size(); i++) {
		for (int j = 0; j < this->_collisionRects.size(); j++) {
			if (this->_collisionRects.at(j).collidesWith(this->_enemies.at(i)->getBoundingBox())) {
				others.push_back(this->_collisionRects.at(j));
				this->_enemies.at(i)->handleEnemyTileCollision(others);
			}
		}
	}
	/*for (int x = 0; x < this->_enemies.size(); x++) {
		this->_enemies.at(x)->handleEnemyTileCollision(others);
	}*/
	return others;
}

std::vector<Rectangle> Level::checkEnemyProjectileTileCollision()
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		for (int j = 0; j < this->_collisionRects.size(); j++) {
			if (this->_collisionRects.at(j).collidesWith(this->_enemies.at(i)->getEnemyProjectileBoundingBox())) {
				others.push_back(this->_collisionRects.at(j));
				this->_enemies.at(i)->handleEnemyProjectileTileCollision();
			}
		}
	}
	return others;
}

std::vector<Rectangle> Level::checkCutsceneCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_cutsceneRects.size(); i++) { // Loop through our collision rects come from tiles
		if (this->_cutsceneRects.at(i).collidesWith(other)) { // Check if any of our collision rects collides with other
			others.push_back(this->_cutsceneRects.at(i)); // If it does add it to the others list
		}
	}
	return others; // Return whatever collision rects we are colliding with
}

std::vector<Rectangle> Level::checkLavaCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_lavaRects.size(); i++) { 
		if (this->_lavaRects.at(i).collidesWith(other))
			others.push_back(this->_lavaRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkPoisonCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_poisonRects.size(); i++) {
		if (this->_poisonRects.at(i).collidesWith(other))
			others.push_back(this->_poisonRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkWaterCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_waterRects.size(); i++) {
		if (this->_waterRects.at(i).collidesWith(other))
			others.push_back(this->_waterRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkDeadzoneCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_deadzoneRects.size(); i++) {
		if (this->_deadzoneRects.at(i).collidesWith(other))
			others.push_back(this->_deadzoneRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkLadderCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_ladderRects.size(); i++) {
		if (this->_ladderRects.at(i).collidesWith(other))
			others.push_back(this->_ladderRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkSaveCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_saveRects.size(); i++) {
		if (this->_saveRects.at(i).collidesWith(other))
			others.push_back(this->_saveRects.at(i));
	}
	return others;
}

std::vector<Rectangle> Level::checkArenaCollisions(const Rectangle & other)
{
	std::vector<Rectangle> others;
	if (!arenaActive)
		return others;
	for (int i = 0; i < this->_arenaRects.size(); i++) { // Loop through our collision rects come from tiles
		if (this->_arenaRects.at(i).collidesWith(other)) { // Check if any of our collision rects collides with other
			others.push_back(this->_arenaRects.at(i)); // If it does add it to the others list
		}
	}
	return others; // Return whatever collision rects we are colliding with
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

//bool Level::checkCollidingSlopes(Player& player) {
//	for (int i = 0; i < this->_slopes.size(); i++) {
//		if (this->_slopes.at(i).checkSlopeCollision(player.getX(), player.getY(), this->_slopes.at(i))) {
//			player.handlePlayerSlopeCollision(this->_slopes.at(i));
//			std::cout << "Slope collisions (checkCS in Level)" << std::endl;
//			return true;
//		}
//	}
//	std::cout << "Return false." << std::endl;
//	return false;
//}

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
					if (dropRate && this->_enemies.at(i)->getEnemyLevel() <= 10 && !this->_enemies.at(i)->isMiniBoss() && !this->_enemies.at(i)->isBoss()) {
						this->_items.push_back(new BronzeCoin(graphics, Vector2(this->_enemies.at(i)->getX(), this->_enemies.at(i)->getY())));
						//this->itemType.push_back(2);
					}
					else if (this->_enemies.at(i)->isMiniBoss() || this->_enemies.at(i)->isBoss()) {
							this->_items.push_back(new RedCoin(graphics, Vector2(this->_enemies.at(i)->getX(), this->_enemies.at(i)->getY())));
							//this->itemType.push_back(2);
							player.completeBossTable(this->_enemies.at(i)->getName(), this->_mapName, this->_enemies.at(i)->getStartingX(), this->_enemies.at(i)->getStartingY());
					}
					std::string mob = this->_enemies.at(i)->getName();
					auto cMapIt = std::find_if(this->levelDropTable.begin(), this->levelDropTable.end(), [&mob](const auto& t) { return std::get<0>(t) == mob; });
					auto cDistance = std::distance(this->levelDropTable.begin(), cMapIt);
					if (cMapIt != this->levelDropTable.end()) {
						std::random_device rd; // Seed for the random number (for drop rate check)
						std::mt19937 gen(rd()); // Mersenne twister engine with rd seed ^
						std::uniform_int_distribution<> luckyNumber(1, 100);
						if (luckyNumber(gen) <= std::get<2>(this->levelDropTable[cDistance])) {
							Items *b = classMap[std::get<1>(this->levelDropTable[cDistance])](graphics, Vector2(this->_enemies.at(i)->getX(),
								this->_enemies.at(i)->getY()));
							this->_items.push_back(b);
							//this->itemType.push_back(4);
						}
					}
					player.gainExpFromEnemy(this->_enemies.at(i)->getEnemyLevel(), this->_enemies.at(i)->enemyExpAmount());
					player.addKillCount(1);
					player.addKillTable(this->_enemies.at(i)->getName());
					this->enemyDead = true;
					if (this->_enemies.at(i)->isBoss() || this->_enemies.at(i)->isMiniBoss()) {
						if (arenaActive) {
							if (this->_enemies.at(i)->getName() == this->_arenaName) {
								arenaActive = false;
								this->_arenaName.clear();
							}
						}
					}
					if (!this->_enemies.at(i)->isMiniBoss() || !this->_enemies.at(i)->isBoss()) {
						/* It is a regular enemy. So, do not allow for respawn until the game is saved, which saving respawns enemies. */
						player.addDespawnTable(this->_enemies.at(i)->getName(), this->_mapName, this->_enemies.at(i)->getStartingX(),
							this->_enemies.at(i)->getStartingY());
					}
					delete this->_enemies.at(i);
					this->_enemies.erase(this->_enemies.begin() + i);
				}
			}
		}
	}
}

void Level::generateItems(Graphics &graphics)
{
	// Create all items that are droppable once.
	// Note that they do not initialze / expend memory until passed through the &createInstance() function
	std::cout << "Generating Items" << std::endl;
	classMap["SilverGem"] = &createInstance<SilverGem>;
	// More items below
}

void Level::generateMapItems(Graphics & graphics, std::string mapName, Inventory &invent)
{
	XMLDocument doc; // Represents entire xml document
	std::stringstream ss;
	ss << mapName << ".tmx"; // Ss << "content/maps" << mapName << ".tmx"; Pass in Map 1, we get content/maps/Map 1.tmx
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "maps";
	cwd.append(mapName + ".tmx");
	doc.LoadFile(cwd.string().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "item") {
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
							}
						}
						else if (ss.str() == "permHP") {
							if (!invent.isLooted(mapName, 1)) {
								this->_items.push_back(new PermHP(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
								//this->itemType.push_back(1);
							}
						}
						else if (ss.str() == "key") {
							if (!invent.isLooted(mapName, 1001)) {
								this->_items.push_back(new Key(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
								//this->itemType.push_back(3);
							}
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup"); // More then 1 obj group keep going
		}
	}
}

void Level::generateEnemies(Graphics & graphics, std::string mapName, Player &player)
{
	XMLDocument doc; // Represents entire xml document
	std::stringstream ss;
	ss << mapName << ".tmx"; // Ss << "content/maps" << mapName << ".tmx"; Pass in Map 1, we get content/maps/Map 1.tmx
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "maps";
	cwd.append(mapName + ".tmx");
	doc.LoadFile(cwd.string().c_str());
	XMLElement* mapNode = doc.FirstChildElement("map");

	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			 if (ss.str() == "enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						XMLElement* pProperties = pObject->FirstChildElement("properties");
						if (pProperties != NULL) {
							while (pProperties) {
								XMLElement* pProperty = pProperties->FirstChildElement("property");
								if (pProperty != NULL) {
									while (pProperty) {
										const char* dropValue = "SilverGem";
										const char* mobValue = "NA";

										const char* name = pProperty->Attribute("name");
										std::stringstream ss;
										ss << name;
										if (ss.str() == "drop") {
											dropValue = pProperty->Attribute("value");
											std::stringstream ss2;
											ss2 << dropValue;
										}
										pProperty = pProperty->NextSiblingElement("property");
										const char* mName = pProperty->Attribute("name");
										std::stringstream mobName;
										mobName << mName;
										if (mobName.str() == "mobName") {
											mobValue = pProperty->Attribute("value");
											std::stringstream mobName2;
											mobName2 << mobValue;
									
										}
										pProperty = pProperty->NextSiblingElement("property");
										const char* rateName = pProperty->Attribute("name");
										std::stringstream rate;
										int rateValue = 10; // Arbitrary number
										rate << rateName;
										if (rate.str() == "rate") {
											pProperty->QueryIntAttribute("value", &rateValue);
										}
										auto cMapIt = std::find_if(this->levelDropTable.begin(), this->levelDropTable.end(), [&mobValue](const auto& t) {return std::get<0>(t) == mobValue; });
										if (cMapIt == levelDropTable.end()) {
											std::cout << "Sucessfully pushed: " << mobValue << " , " << dropValue << " , " << rateValue << std::endl;
											this->levelDropTable.push_back(std::make_tuple(mobValue, dropValue, rateValue));
										}
										pProperty = pProperty->NextSiblingElement("property");
									}
								}
								pProperties = pProperties->NextSiblingElement("properties");
							}
						}
						// Load enemies now that drops are done
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							std::string mobName = "bat";
							if (!player.checkEnemyDespawn(mobName, mapName, std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE))
								this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
									std::floor(y) * globals::SPRITE_SCALE)));
						}
						else if (ss.str() == "JellyFish") {
							std::string mobName = "JellyFish";
							if (!player.checkEnemyDespawn(mobName, mapName, std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE))
								this->_enemies.push_back(new JellyFish(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
									std::floor(y) * globals::SPRITE_SCALE)));
						}
						else if (ss.str() == "Ghost") {
							std::string mobName = "Ghost";
							if (!player.checkEnemyDespawn(mobName, mapName, std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE))
								this->_enemies.push_back(new Ghost(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
									std::floor(y) * globals::SPRITE_SCALE)));
						}
						else if (ss.str() == "shade") {
							if (!player.checkBossCompleted(ss.str(), mapName, std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)) {
								this->_enemies.push_back(new Shade(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE,
									std::floor(y) * globals::SPRITE_SCALE)));
								player.addBossTable(ss.str(), mapName, std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE);
							}
							else { // If the boss was defeated already make sure that the boss arena is set to false
								if (this->_arenaName == ss.str()) {
									this->_arenaName.clear();
									this->arenaActive = false;
								}
							}
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
		pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup"); // More then 1 obj group keep going
		}
	}
}

void Level::generateProjectile(Graphics & graphics, Player & player)
{
	if (this->canShoot) {
		int pX = player.getX(), pY = player.getY();
		Direction pDir = player.facingDir();
		if (pDir == RIGHT && player.lookingDown() == false && player.lookingUp() == false) {
			pX += 35;
			pY += 5;
		}
		else if (pDir == LEFT && player.lookingDown() == false && player.lookingUp() == false) {
			pX -= 15;
			pY += 5;
		}
		if (player.lookingDown() == true) {
			pDir = DOWN;
			if (player.facingDir() == RIGHT) {
				pX += 12;
				pY += 10;
			}
			else {
				pX -= 15;
				pY += 10;
			}
		}
		else if (player.lookingUp() == true) {
			pDir = UP;
			if (player.facingDir() == RIGHT) {
				pX += 12;
				pY -= 10;
			}
			else {
				pX -= 15;
				pY -= 10;
			}
		}
		this->_projectiles.push_back(new SilverBullet(graphics, Vector2(pX, pY), pDir));
		this->canShoot = false;
	}
}

void Level::generateEffects(Graphics & graphics, Player & player)
{
	if (arenaActive) {
		this->_effects.push_back(new ArenaEffect(graphics, Vector2(std::floor(this->_arenaRects.at(0).getLeft()),
			std::floor(this->_arenaRects.at(0).getTop()))));
		this->_effects.push_back(new ArenaEffect(graphics, Vector2(std::floor(this->_arenaRects.at(0).getRight()),
			std::floor(this->_arenaRects.at(0).getTop()))));
	}
}

void Level::generateParallax(Graphics& graphics, std::string mapName, Player& player) {
	// Parse the .tmx file
	XMLDocument doc; // Represents entire xml document
	std::stringstream ss;
	ss << mapName << ".tmx"; // Ss << "content/maps" << mapName << ".tmx"; Pass in Map 1, we get content/maps/Map 1.tmx
	std::filesystem::path cwd = std::filesystem::current_path() / "data" / "maps";
	cwd.append(mapName + ".tmx");
	doc.LoadFile(cwd.string().c_str());  // Ss,.str convers stringstream into stream then string function called c_str converts string to c-string
	XMLElement* mapNode = doc.FirstChildElement("map"); // Doc is the root of the xml doc, the first child element from the root called map we are selecting


	XMLElement* propsNode = mapNode->FirstChildElement("properties");
	if (propsNode != NULL) {
		XMLElement* prop = propsNode->FirstChildElement("property");
		while (prop) {
			const char* propsName = prop->Attribute("name");
			std::stringstream ssProp;
			ssProp << propsName;
			if (ssProp.str() == "PXLayer1") {
				const char* bgValue = prop->Attribute("value");
				std::stringstream bgFileName;
				bgFileName << "data/maps/" << bgValue << ".png";
				SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
				Parallax* pxLayer = new Parallax(tex, 1.0f);
				this->_parallaxList.push_back(std::make_pair(1, pxLayer));
			}
			else if (ssProp.str() == "PXLayer2") {
				const char* bgValue = prop->Attribute("value");
				std::stringstream bgFileName;
				bgFileName << "data/maps/" << bgValue << ".png";
				SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
				Parallax* pxLayer = new Parallax(tex, 1.0f);
				this->_parallaxList.push_back(std::make_pair(2, pxLayer));
			}
			else if (ssProp.str() == "PXLayer3") {
				const char* bgValue = prop->Attribute("value");
				std::stringstream bgFileName;
				bgFileName << "data/maps/" << bgValue << ".png";
				SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
				Parallax* pxLayer = new Parallax(tex, 1.4f);
				this->_parallaxList.push_back(std::make_pair(3, pxLayer));
			}
			else if (ssProp.str() == "PXLayer4") {
				const char* bgValue = prop->Attribute("value");
				std::stringstream bgFileName;
				bgFileName << "data/maps/" << bgValue << ".png";
				SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(bgFileName.str()));
				Parallax* pxLayer = new Parallax(tex, 2.0f);
				this->_parallaxList.push_back(std::make_pair(4, pxLayer));
			}
			prop = prop->NextSiblingElement("property");
		}
	}
	std::sort(this->_parallaxList.begin(), this->_parallaxList.end());
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
			// Others.push_back(this->_enemies.at(i));
		}
	}
	return others;
}


std::vector<Npc*> Level::checkNpcCollisions(const Rectangle &other, Graphics &graphics) {
	std::vector<Npc*> others;
	for (int i = 0; i < this->_npcs.size(); i++) {
		if (this->_npcs.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_npcs.at(i));
		}
		else if (!this->_npcs.at(i)->getBoundingBox().collidesWith(other)) {
			others.clear();
			return others;
		}
	}
	return others;

}

void Level::checkItemCollisions(Player & player, const Rectangle &other, Graphics &graphics, Inventory &invent) {
	for (int i = 0; i < this->_items.size(); i++) {
		if (this->_items.at(i)->getBoundingBox().collidesWith(other)) {
			//int type = itemType.at(i);
			
			if (this->_items.at(i)->getID() == 1) {
				player.gainMaxHealth(5);
				//invent.storeItem(1);
			}
			else if (this->_items.at(i)->isCurrency()) {
				player.gainCurrency(this->_items.at(i)->getAmount());
			}
			else {
				invent.addItem(this->_items.at(i)->getID(), 1, player);
			}

			if (!this->_items.at(i)->isCurrency()){
				invent.addInstancedLoot(this->_mapName, this->_items.at(i)->getID());
			}

			//if (type == 1) // Permanent HP+1 item
			//	player.gainMaxHealth(5);
			//else if (type == 2)
			//	player.gainCurrency(this->_items.at(i)->getAmount());
			//else
			//	invent.storeItem(type);
			//if (type != 2)
			//	invent.addInstancedLoot(this->_mapName, type);

			delete this->_items.at(i);
			this->_items.erase(_items.begin() + i);
			//itemType.erase(itemType.begin() + i);
		}
	}
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return 
		this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	// Calcualte the position of the tile in the tileset 
	int tilesetWidth, tilesetHeight;
	// Pass in texture, format, access, width, height. We dont care about format or access so NULL and then reference to width/height
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	// Get the tileset X position Subtract the gid - first gid of the tileset
	// Then modulo (TS width / T width)
	int tsxx = (gid - tls.FirstGid) % (tilesetWidth / tileWidth);
	tsxx *= tileWidth;
	// Same as above but for the tileset Y position
	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}