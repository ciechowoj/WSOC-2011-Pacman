#include "GameLevel.hpp"
namespace sparrow {

GameLevel::GameLevel()
{
	for(u32 i = 0; i < _objects.size(); ++i)
		_objects[i] = 0;
}

GameLevel::~GameLevel()
{ }

LevelObject GameLevel::getObject(u32 x, u32 y)
{
	return (LevelObject)(_objects[y * IGameStage::LevelWidth + x] & 0x7f);
}

bool GameLevel::getPoint(u32 x, u32 y)
{
	return (_objects[y * IGameStage::LevelWidth + x] & 0x80) != 0;
}

void GameLevel::setObject(u32 x, u32 y, LevelObject object)
{
	if(object == LevelObjectPoint)
		_objects[y * IGameStage::LevelWidth + x] |= 0x80;
	else if(object == LevelObjectEmpty)
		_objects[y * IGameStage::LevelWidth + x] = 0;
	else
	{
		u8 a = (_objects[y * IGameStage::LevelWidth + x] & 0x80) | u8(object);
		_objects[y * IGameStage::LevelWidth + x] = a;
	}
}

void serialize(const GameLevel& level, DataStream* stream)
{
	serialize(level._name, stream);
	serialize(level._objects, stream);
}

void deserialize(GameLevel& level, const DataStream* stream)
{
	deserialize(level._name, stream);
	deserialize(level._objects, stream);
}

} // namespace sparrow
