#pragma once
#include "Object.hpp"
namespace sparrow {

class Pacman : public Object
{
public:
						Pacman(const SpriteSet* sprite_set);
						~Pacman();


	void				moveLeft();
	void				moveRight();
	void				moveUp();
	void				moveDown();

	virtual bool		update();
	virtual void		redraw(Context2D* context2d);
	virtual void		resolve(const s32x2& translation, ObjectType object_type);
	void				init(u32 type, const u32x2& location, const PixmapFont* font, u32 lives = 3, u32 score = 0);
private:
	s32x2				_startPos;
	u32					_spriteLeft;
	u32					_spriteRight;
	u32					_spriteUp;
	u32					_spriteDown;
	const s32			_FrameSpeed;
	const s32			_ObjectSpeed;
	u32					_playerNumber;
	u32					_lives;
	u32					_score;
	const PixmapFont*	_font;
};

} // namespace sparrow
