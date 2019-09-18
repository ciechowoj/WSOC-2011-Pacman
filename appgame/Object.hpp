#pragma once
#include "IGameStage.hpp"
namespace sparrow {

struct GameState;
class MainMenu;
class HighScores;

enum ObjectType
{
	ObjectTypeUnknown,
	ObjectTypePacmanNormal,
	ObjectTypePacmanNoEat,
	ObjectTypePacmanRush,
	ObjectTypeGhost,
	ObjectTypePoint,
	ObjectTypeWallSolid,
	ObjectTypeWallLeft,
	ObjectTypeWallRight,
	ObjectTypeWallUp,
	ObjectTypeWallDown,
	ObjectTypeApple,
	ObjectTypeKiwi,
	ObjectTypeLemon,
	ObjectTypePineapple,
	ObjectTypePear,
	ObjectTypeGrapes,
	ObjectTypeWatermelon,
	ObjectTypeStrawberry,
	ObjectTypeCherries,
	ObjectTypeOrange,
	ObjectTypeBanana,
	ObjectTypePlums,
};

const u32 MapObjectNone = 0;
const u32 MapObjectPacman1 = 1;
const u32 MapObjectPacman2 = 2;
const u32 MapObjectGhostYellow = 3;
const u32 MapObjectGhostGreen = 4;
const u32 MapObjectGhostBlue = 5;
const u32 MapObjectGhostRed = 6;
const u32 MapObjectWallYellow = 7;
const u32 MapObjectWallGreen = 8;
const u32 MapObjectWallBlue = 9;
const u32 MapObjectWallRed = 10;
const u32 MapObjectWallYellowDown = 11;
const u32 MapObjectWallGreenDown = 12;
const u32 MapObjectWallBlueDown = 13;
const u32 MapObjectWallRedDown = 14;
const u32 MapObjectWallYellowRight = 15;
const u32 MapObjectWallGreenRight = 16;
const u32 MapObjectWallBlueRight = 17;
const u32 MapObjectWallRedRight = 18;
const u32 MapObjectWallYellowLeft = 19;
const u32 MapObjectWallGreenLeft = 20;
const u32 MapObjectWallBlueLeft = 21;
const u32 MapObjectWallRedLeft = 22;
const u32 MapObjectWallYellowUp = 23;
const u32 MapObjectWallGreenUp = 24;
const u32 MapObjectWallBlueUp = 25;
const u32 MapObjectWallRedUp = 26;
const u32 MapObjectApple = 27;
const u32 MapObjectKiwi = 28;
const u32 MapObjectLemon = 29;
const u32 MapObjectPineapple = 30;
const u32 MapObjectPear = 31;
const u32 MapObjectGrapes = 32;
const u32 MapObjectWatermelon = 33;
const u32 MapObjectStrawberry = 34;
const u32 MapObjectCherries = 35;
const u32 MapObjectOrange = 36;
const u32 MapObjectBanana = 37;
const u32 MapObjectPlums = 38;
const u32 MapObjectPoint = 39;

class Object
{
public:
						Object();
	virtual				~Object();

	virtual bool		update();
	virtual void		redraw(Context2D* context2d);
	virtual void		resolve(const s32x2& translation, ObjectType object_type);

	const AABRi&		getAABR() const;
	ObjectType			getType() const;
	const s32x2&		getSpeed() const;
protected:
	AABRi				_aabr;
	s32x2				_speed;
	SpriteDesc			_sprite;
	u8					_frameCount;
	u8					_frameCurrent;
	u8					_frameElapsed;
	u8					_frameSpeed;
	ObjectType			_objectType;
};

inline const AABRi& Object::getAABR() const
{
	return _aabr;
}

inline ObjectType Object::getType() const
{
	return _objectType;
}

inline const s32x2& Object::getSpeed() const
{
	return _speed;
}

} // namespace sparrow
