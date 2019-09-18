#include "Context2D.hpp"
namespace sparrow {

SpriteSet::SpriteSet(Context2D* context)
	: _textureSize(0)
	, _offset(0, 0)
	, _context(context)
{ }



SpriteSet::~SpriteSet()
{ 
	clear();
}


void SpriteSet::deserialize(const DataStream* stream)
{
	struct Entry
	{
		u8	value;
		u8	count;
	};

	string name;
	sparrow::deserialize(name, stream);

	if(name == "sparrow::SpriteSet")
	{
		vector<Entry> red_channel;
		vector<Entry> green_channel;
		vector<Entry> blue_channel;
		vector<Entry> alpha_channel;
		vector<u8x4> data;
		
		sparrow::deserialize(_setName, stream);
		sparrow::deserialize(_sprites, stream);
		sparrow::deserialize(_frames, stream);
		sparrow::deserialize(_textureSize, stream);
		sparrow::deserialize(red_channel, stream);
		sparrow::deserialize(green_channel, stream);
		sparrow::deserialize(blue_channel, stream);
		sparrow::deserialize(alpha_channel, stream);

		data.resize(_textureSize * _textureSize);

		for(u32 i = 0, k = 0; i < red_channel.size(); ++i)
		{
			for(u32 j = 0; j <= red_channel[i].count; ++j)
				data[k++].x = red_channel[i].value;
		}

		for(u32 i = 0, k = 0; i < green_channel.size(); ++i)
		{
			for(u32 j = 0; j <= green_channel[i].count; ++j)
				data[k++].y = green_channel[i].value;
		}

		for(u32 i = 0, k = 0; i < blue_channel.size(); ++i)
		{
			for(u32 j = 0; j <= blue_channel[i].count; ++j)
				data[k++].z = blue_channel[i].value;
		}

		for(u32 i = 0, k = 0; i < alpha_channel.size(); ++i)
		{
			for(u32 j = 0; j <= alpha_channel[i].count; ++j)
				data[k++].w = alpha_channel[i].value;
		}

		impl::TextureAtlas* atlas = &_context->_textureAtlas;
		atlas->acquireAtlasPage(this, _callback, data.data(), _textureSize);
	}
}

void SpriteSet::clear()
{
	if(valid())
	{
		impl::TextureAtlas* atlas = &_context->_textureAtlas;
		atlas->releaseAtlasPage(this, _textureSize);
	}

	_setName = string();
	_textureSize = 0;
	_offset = s16x2(0, 0);
	_sprites.clear();
	_frames.clear();
}

bool SpriteSet::valid() const
{
	return _sprites.size() != 0;
}

void SpriteSet::_updatePage(const u32x2& offset, const u32x2& size)
{
	s16x2 old_offset = -s16x2(_offset);
	s16x2 new_offset = s16x2(offset);
	if(offset != _offset)
	{
		vector<AABRs>::iterator itr = _frames.begin(), end = _frames.end();

		while(itr != end)
		{
			aabrTranslate(*itr, old_offset);
			aabrTranslate(*itr, new_offset);
			++itr;
		}
	}
	_offset = offset;
}

void SpriteSet::_callback(void* handler, const u32x2& offset, u32 size)
{
	static_cast<SpriteSet*>(handler)->_updatePage(offset, u32x2(size, size));
}

} //namespace sparrow
