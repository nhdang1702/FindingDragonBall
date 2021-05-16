#ifndef GAME_MAP_H
#define GAME_MAP_H

#include"CommonFunc.h"
#include"BaseObject.h"

#define MAX_TILES 300

class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }
	
	bool LoadMap(const char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
	Map getMap() const { return game_map_; }
	void SetMap(Map& map_data) { game_map_ = map_data; }
private:
	Map game_map_;          // chua toan bo thong tin ve ban do
	TileMat tile_mat[MAX_TILES];   // mang luu tru hinh anh
};

#endif // !GAME_MAP_H
