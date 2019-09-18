#include "Object.hpp"
namespace sparrow {

Object::Object()
	: _aabr(0, 0, 0, 0)
	, _speed(0, 0)
	, _frameCount(0)
	, _frameCurrent(0)
	, _frameElapsed(0)
	, _objectType(ObjectTypeUnknown)
{ }

Object::~Object()
{ }

bool Object::update()
{
	_frameElapsed += _frameSpeed < 100 ? 1 : 0;
	if(_frameElapsed > _frameSpeed)
	{
		++_frameCurrent;
		if(_frameCurrent >= _frameCount)
			_frameCurrent = 0;
		_frameElapsed = 0;
	}

	aabrTranslate(_aabr, _speed);
	return false;
}

void Object::redraw(Context2D* context2d)
{
	_sprite.frame = _frameCurrent;
	_sprite.position = _aabr.leftTop();
	context2d->drawSprite(_sprite);
}

void Object::resolve(const s32x2& translation, ObjectType object_type)
{ }

} // namespace sparrow
