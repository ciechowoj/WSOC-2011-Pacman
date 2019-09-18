#include "Wall.hpp"
namespace sparrow {

Wall::Wall(const SpriteSet* sprite_set)
{
	_sprite.spriteSet = sprite_set;
	_sprite.frame = 0;
	_sprite.sprite = 0;
	_sprite.zpos = 0.3f;

	_frameCount = 1;
	_frameCurrent = 0;
	_frameElapsed = 0;
	_frameSpeed = 100;
	_objectType = ObjectTypeWallSolid;
}

Wall::~Wall()
{ }

void Wall::init(u32 type, const u32x2& location)
{
	switch(type)
	{
	case MapObjectWallYellow:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_yellow");
		_objectType = ObjectTypeWallSolid;
		break;
	case MapObjectWallGreen:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_green");
		_objectType = ObjectTypeWallSolid;
		break;
	case MapObjectWallBlue:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_blue");
		_objectType = ObjectTypeWallSolid;
		break;
	case MapObjectWallRed:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_red");
		_objectType = ObjectTypeWallSolid;
		break;
	case MapObjectWallYellowDown:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_yellow_down");
		_objectType = ObjectTypeWallDown;
		break;
	case MapObjectWallGreenDown:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_green_down");
		_objectType = ObjectTypeWallDown;
		break;
	case MapObjectWallBlueDown:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_blue_down");
		_objectType = ObjectTypeWallDown;
		break;
	case MapObjectWallRedDown:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_red_down");
		_objectType = ObjectTypeWallDown;
		break;
	case MapObjectWallYellowRight:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_yellow_right");
		_objectType = ObjectTypeWallRight;
		break;
	case MapObjectWallGreenRight:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_green_right");
		_objectType = ObjectTypeWallRight;
		break;
	case MapObjectWallBlueRight:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_blue_right");
		_objectType = ObjectTypeWallRight;
		break;
	case MapObjectWallRedRight:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_red_right");
		_objectType = ObjectTypeWallRight;
		break;
	case MapObjectWallYellowLeft:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_yellow_left");
		_objectType = ObjectTypeWallLeft;
		break;
	case MapObjectWallGreenLeft:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_green_left");
		_objectType = ObjectTypeWallLeft;
		break;
	case MapObjectWallBlueLeft:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_blue_left");
		_objectType = ObjectTypeWallLeft;
		break;
	case MapObjectWallRedLeft:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_red_left");
		_objectType = ObjectTypeWallLeft;
		break;
	case MapObjectWallYellowUp:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_yellow_up");
		_objectType = ObjectTypeWallUp;
		break;
	case MapObjectWallGreenUp:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_green_up");
		_objectType = ObjectTypeWallUp;
		break;
	case MapObjectWallBlueUp:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_blue_up");
		_objectType = ObjectTypeWallUp;
		break;
	case MapObjectWallRedUp:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("wall_red_up");
		_objectType = ObjectTypeWallUp;
		break;
	default:
		break;
	}

	_aabr = AABRi(location.x * IGameStage::TileWidth, (location.y + 1) * IGameStage::TileHeight, (location.x + 1) * IGameStage::TileWidth, (location.y + 2) * IGameStage::TileHeight);
}

} // namespace sparrow
