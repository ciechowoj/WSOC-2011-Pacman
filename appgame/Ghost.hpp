#pragma once
#include "Object.hpp"
namespace sparrow {

class Ghost : public Object
{
public:
						Ghost(const SpriteSet* sprite_set);
						~Ghost();

	virtual bool		update();
	virtual void		resolve(const s32x2& translation, ObjectType object_type);
	void				init(u32 type, const u32x2& location);
private:
	u32					_currentDir;
};

} // namespace sparrow
