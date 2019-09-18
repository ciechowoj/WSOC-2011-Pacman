








#include "Ghost.hpp"
#include <cstdlib>
namespace sparrow {

const s32 GhostSpeed = 2;

Ghost::Ghost(const SpriteSet* sprite_set)
	: _currentDir(0) //left/top/right/bottom
{
	_sprite.spriteSet = sprite_set;
	_sprite.frame = 0;
	_sprite.zpos = 0.2f;

	_frameCount = sprite_set->getSpriteFrames("ghost_green");
	_frameCurrent = 0;
	_frameElapsed = 0;
	_frameSpeed = 8;
	_objectType = ObjectTypeGhost;

	_aabr = AABRi(IGameStage::TileWidth * 1,IGameStage::TileWidth * 1, IGameStage::TileWidth * 2, IGameStage::TileHeight * 2);
	srand(utimeu32());
	switch(rand() % 4)
	{
	case 0: 
		_speed = s32x2(GhostSpeed, 0);
		break;
	case 1:
		_speed = s32x2(-GhostSpeed, 0);
		break;
	case 2: 
		_speed = s32x2(0, GhostSpeed);
		break;
	case 3:
		_speed = s32x2(0, -GhostSpeed);
		break;
	}
}
					
Ghost::~Ghost()
{ }

bool Ghost::update()
{
	if(_aabr.left >= IGameStage::StageWidth)
	{
		_aabr.left = -IGameStage::TileWidth;
		_aabr.right = 0;
	}

	if(_aabr.right < 0)
	{
		_aabr.left = IGameStage::StageWidth;
		_aabr.right = _aabr.left + IGameStage::TileWidth;
	}

	if(_aabr.top >= IGameStage::StageHeight)
	{
		_aabr.top = 0;
		_aabr.bottom = IGameStage::TileHeight;
	}

	if(_aabr.bottom < IGameStage::TileHeight)
	{
		_aabr.top = IGameStage::StageHeight;
		_aabr.bottom = _aabr.top + IGameStage::TileHeight;
	}

	Object::update();
	return false;
}

void Ghost::resolve(const s32x2& translation, ObjectType object_type)
{
	u32 allowed_dir = _currentDir;
	switch(object_type)
	{
	case ObjectTypeWallLeft: 
		allowed_dir = 0;
		break;
	case ObjectTypeWallUp: 
		allowed_dir = 2;
		break;
	case ObjectTypeWallRight: 
		allowed_dir = 2;
		break;
	case ObjectTypeWallDown: 
		allowed_dir = 3;
		break;
	case ObjectTypeWallSolid: 
		allowed_dir = 4;
		break;
	}

	if(allowed_dir != _currentDir)
	{
		aabrTranslate(_aabr, translation);

		switch(rand() % 4)
		{
		case 0:
			if(_currentDir != 0)
				_speed = s32x2(-GhostSpeed, 0);
			else
				_speed = s32x2(GhostSpeed, 0);
			_currentDir = 0;
			break;
		case 1:
			if(_currentDir != 0)
				_speed = s32x2(0, -GhostSpeed);
			else
				_speed = s32x2(0, GhostSpeed);
			_currentDir = 1;
			break;
		case 2:
			if(_currentDir != 0)
				_speed = s32x2(GhostSpeed, 0);
			else
				_speed = s32x2(-GhostSpeed, 0);
			_currentDir = 2;
			break;
		case 3:
			if(_currentDir != 0)
				_speed = s32x2(0, GhostSpeed);
			else
				_speed = s32x2(0, -GhostSpeed);
			_currentDir = 3;
			break;
		}
	}
}

void Ghost::init(u32 type, const u32x2& location)
{
	switch(type)
	{
	case MapObjectGhostYellow:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("ghost_yellow");
		break;
	case MapObjectGhostGreen:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("ghost_green");
		break;
	case MapObjectGhostBlue:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("ghost_blue");
		break;
	case MapObjectGhostRed:
		_sprite.sprite = _sprite.spriteSet->getSpriteHandle("ghost_red");
		break;
	default:
		break;
	}

	_aabr = AABRi(location.x * IGameStage::TileWidth, (location.y + 1) * IGameStage::TileHeight, (location.x + 1) * IGameStage::TileWidth, (location.y + 2) * IGameStage::TileHeight);
}








} // namespace sparrow
