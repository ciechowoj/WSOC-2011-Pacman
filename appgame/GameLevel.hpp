#pragma once
#include "IGameStage.hpp"
namespace sparrow {

enum LevelObject
{
	LevelObjectEmpty,
	LevelObjectPlayer1,
	LevelObjectPlayer2,
	LevelObjectWallR,
	LevelObjectWallG,
	LevelObjectWallB,
	LevelObjectWallY,
	LevelObjectWallRL,
	LevelObjectWallRR,
	LevelObjectWallRU,
	LevelObjectWallRD,
	LevelObjectWallGL,
	LevelObjectWallGR,
	LevelObjectWallGU,
	LevelObjectWallGD,
	LevelObjectWallBL,
	LevelObjectWallBR,
	LevelObjectWallBU,
	LevelObjectWallBD,
	LevelObjectWallYL,
	LevelObjectWallYR,
	LevelObjectWallYU,
	LevelObjectWallYD,
	LevelObjectGhostR,
	LevelObjectGhostG,
	LevelObjectGhostB,
	LevelObjectGhostY,
	LevelObjectPoint,
};

class GameLevel
{
public:
						GameLevel();
						~GameLevel();

	LevelObject			getObject(u32 x, u32 y);
	bool				getPoint(u32 x, u32 y);
	void				setObject(u32 x, u32 y, LevelObject object);

	friend void serialize(const GameLevel& level, DataStream* stream);
	friend void deserialize(GameLevel& level, const DataStream* stream);
private:
	vector<u8>			_objects;
	string				_name;
};

} // namespace sparrow
