#include "Pacman.hpp"
namespace sparrow {

Pacman::Pacman(const SpriteSet* sprite_set)
	: _startPos(0, 0)
	, _spriteLeft(sprite_set->getSpriteHandle("pacman_left"))
	, _spriteRight(sprite_set->getSpriteHandle("pacman_right"))
	, _spriteUp(sprite_set->getSpriteHandle("pacman_up"))
	, _spriteDown(sprite_set->getSpriteHandle("pacman_down"))
	, _FrameSpeed(3)
	, _ObjectSpeed(2)
	, _lives(0)
	, _score(0)
	, _font(nullptr)
{
	_sprite.spriteSet = sprite_set;
	_sprite.frame = 2;
	_sprite.sprite = _spriteLeft;
	_sprite.zpos = 0.2f;

	_frameCount = sprite_set->getSpriteFrames("pacman_left");
	_frameCurrent = 2;
	_frameElapsed = 0;
	_frameSpeed = 100;
	_objectType = ObjectTypePacmanNormal;

	_aabr = AABRi(0, 0, IGameStage::TileWidth, IGameStage::TileHeight);
}

Pacman::~Pacman()
{ }

void Pacman::moveLeft()
{
	_speed = s32x2(-_ObjectSpeed, 0);
	_frameSpeed = _FrameSpeed;
	_sprite.sprite = _spriteLeft;
}

void Pacman::moveRight()
{
	_speed = s32x2(_ObjectSpeed, 0);
	_frameSpeed = _FrameSpeed;
	_sprite.sprite = _spriteRight;
}

void Pacman::moveUp()
{
	_speed = s32x2(0, -_ObjectSpeed);
	_frameSpeed = _FrameSpeed;
	_sprite.sprite = _spriteUp;
}

void Pacman::moveDown() 
{
	_speed = s32x2(0, _ObjectSpeed);
	_frameSpeed = _FrameSpeed;
	_sprite.sprite = _spriteDown;
}

bool Pacman::update()
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

void Pacman::redraw(Context2D* context2d)
{
	Object::redraw(context2d);

	TextBoxDesc textBoxDesc;
	textBoxDesc.aabr = AABRs(0, 0, IGameStage::StageWidth, IGameStage::StageHeight);
	textBoxDesc.text = "Score: " + u32tostr(_score) + "\nLives:" + u32tostr(_lives);
	textBoxDesc.font = _font;
	textBoxDesc.firstColor = u8x4(0xff, 0xd8, 0x00, 0xff);
	textBoxDesc.secondColor = u8x4(0x00, 0x13, 0x7f, 0xff);
	textBoxDesc.horizontalAlign = _playerNumber == 1 ? HorizontalAlignLeft : HorizontalAlignRight;
	textBoxDesc.zpos = 0.4f;

	context2d->drawTextBox(textBoxDesc);
}

void Pacman::resolve(const s32x2& translation, ObjectType object_type)
{
	switch(object_type)
	{
	case ObjectTypeWallSolid:
		aabrTranslate(_aabr, translation);
		_frameSpeed = 100;
		break;
	case ObjectTypeGhost:
		_lives -= 1;
		_aabr = AABRi(_startPos, _startPos + s32x2(IGameStage::TileWidth, IGameStage::TileHeight));
		_speed = s32x2(0, 0);
		_frameSpeed = 100;
		break;
	case ObjectTypePoint:
		_score += 10;
		break;
	}
}

void Pacman::init(u32 type, const u32x2& location, const PixmapFont* font, u32 lives, u32 score)
{
	_lives = lives;
	_score = score;
	_font = font;
	switch(type)
	{
	case MapObjectPacman1:
		_playerNumber = 1;
		break;
	case MapObjectPacman2:
		_playerNumber = 2;
		break;
	}

	_aabr = AABRi(location.x * IGameStage::TileWidth, (location.y + 1) * IGameStage::TileHeight, (location.x + 1) * IGameStage::TileWidth, (location.y + 2) * IGameStage::TileHeight);
	_startPos = _aabr.leftTop();
}

} // namespace sparrow
