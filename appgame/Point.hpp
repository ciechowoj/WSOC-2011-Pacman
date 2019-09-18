#pragma once
#include "Object.hpp"
namespace sparrow {

class Point : public Object
{
public:
						Point(const SpriteSet* sprite_set);
						~Point();

	virtual bool		update();
	virtual void		resolve(const s32x2& translation, ObjectType object_type);
	void				init(const u32x2& location);
private:
	bool				_delete;
};

} // namespace sparrow
