#include "Point.hpp"
namespace sparrow {

Point::Point(const SpriteSet* sprite_set)
	: _delete(false)
{
	_sprite.spriteSet = sprite_set;
	_sprite.frame = 0;
	_sprite.sprite = sprite_set->getSpriteHandle("dot");
	_sprite.zpos = 0.1f;

	_frameCount = 1;
	_frameCurrent = 0;
	_frameElapsed = 0;
	_frameSpeed = 100;
	_objectType = ObjectTypePoint;
}

Point::~Point()
{ }

bool Point::update()
{
	return _delete;
}

void Point::resolve(const s32x2& translation, ObjectType object_type)
{
	switch(object_type)
	{
	case ObjectTypePacmanNormal:
		_delete = true;
		break;
	case ObjectTypePacmanRush:
		_delete = true;
		break;
	}
}

void Point::init(const u32x2& location)
{
	s32x2 size = _sprite.spriteSet->getSpriteSize("dot");
	s32x2 offset = (s32x2(IGameStage::TileWidth, IGameStage::TileHeight) - size) / 2;
	_aabr.left =  location.x * IGameStage::TileWidth + offset.x;
	_aabr.top = (location.y + 1) * IGameStage::TileHeight + offset.y;
	_aabr.right = (location.x + 1) * IGameStage::TileWidth - offset.x;
	_aabr.bottom = (location.y + 2) * IGameStage::TileHeight - offset.y;
}

} // namespace sparrow
