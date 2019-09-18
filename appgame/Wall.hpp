#pragma once
#include "Object.hpp"
namespace sparrow {

class Wall : public Object
{
public:
						Wall(const SpriteSet* sprite_set);
						~Wall();

	/*virtual bool		update();
	virtual void		redraw(Context2D* context2d);
	virtual void		resolve(const u32x2& translation, ObjectType object_type);*/

	void				init(u32 type, const u32x2& location);
};

} // namespace sparrow
