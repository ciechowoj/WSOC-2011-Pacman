#pragma once
#include "Framework.hpp"
namespace sparrow {

class Context2D;

class FrameSet
{
public:
						FrameSet(Context2D* context);
						~FrameSet();

	const string&		getResourceName() const;
	void				getSprite(u32& out_first_frame, u32& out_frame_count, const string& in_sprite_name) const;
	u32x2				getFrameSize(u32 frame) const;
	bool				isAvailable(const string& sprite_name) const;
	
	void				deserialize(const DataStream* stream);
	void				clear();
private:
	struct _Sprite { u16 frame, count; };

	void				_updatePage(const u32x2& offset, const u32x2& size);
	static void			_callback(void* handler, const u32x2& offset, u32 size);

	string				_setName;
	u32					_textureSize;
	s16x2				_offset;
	vector_map<string, _Sprite> _sprites;
	vector<AABRs>		_frames;
	Context2D*			_context;
	friend class Context2D;
};

struct SpriteDesc
{

						SpriteDesc()
		: position(0, 0)
		, direction(0.f)
		, spriteSet(nullptr)
		, zpos(0.f)
		, sprite(0)
		, frame(0)
	{ }

	s16x2				position;
	f32					direction;
	const SpriteSet*	spriteSet;
	f32					zpos;
	u32					sprite;
	u32					frame;
};

inline const string& SpriteSet::getSetName() const
{
	return _setName;
}

inline u32 SpriteSet::getSpriteHandle(const string& sprite_name) const
{
	return _sprites[sprite_name].frame;
}

inline u32 SpriteSet::getSpriteFrames(const string& sprite_name) const
{
	return _sprites[sprite_name].count;
}

inline u32x2 SpriteSet::getSpriteSize(const string& sprite_name, u32 frame) const
{
	return aabrSize(_frames[_sprites[sprite_name].frame + frame]);
}

inline bool SpriteSet::isAvailable(const string& sprite_name) const
{
	return _sprites.find(sprite_name) != _sprites.end();
}

inline bool operator<(const SpriteDesc& a, const SpriteDesc& b)
{
	return a.zpos < b.zpos;
}

inline bool operator>(const SpriteDesc& a, const SpriteDesc& b)
{
	return a.zpos > b.zpos;
}

} //namespace sparrow
