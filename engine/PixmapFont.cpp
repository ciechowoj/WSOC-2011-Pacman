#include "Context2D.hpp"
namespace sparrow {

PixmapFont::PixmapFont(Context2D* context)
	: _size(0)
	, _height(0)
	, _base(0)
	, _offset(0, 0)
	, _context(context)
{ }

PixmapFont::~PixmapFont()
{ 
	clear();
}

const string& PixmapFont::getFace() const
{
	return _face;
}

u32 PixmapFont::getSize() const
{
	return _size;
}

bool PixmapFont::isItalic() const
{
	return _feat[_Italic];
}

bool PixmapFont::isUnderlined() const
{
	return _feat[_Underlined];
}

bool PixmapFont::isBold() const
{
	return _feat[_Bold];
}

bool PixmapFont::isKerning() const
{
	return _kerning.size() != 0;
}

bool PixmapFont::isSmooth() const
{
	return _feat[_Smooth];
}

bool PixmapFont::isOutlined() const
{
	return _feat[_Outlined];
}

bool PixmapFont::isAvailable(c16 glyph) const
{
	return _glyphs.count(glyph) != 0;
}

/*void PixmapFont::serialize(DataStream* stream) const
{
	struct 
	{
		u16			size;
		u16			height;
		u16			base;
		u16			feat;
	} fields;
	
	if(valid())
	{
		fields.size = _size;
		fields.height = _height;
		fields.base = _base;
		fields.feat = static_cast<u16>(_feat.to_u32());

		string name = "sparrow::PixmapFont";
		name.serialize(stream);
		stream->write(&fields, sizeof fields); 
		_face.serialize(stream);
		_glyphs.serialize(stream);
		_kerning.serialize(stream);
	}
}*/

void PixmapFont::deserialize(const DataStream* stream)
{
	struct 
	{
		u16			size;
		u16			height;
		u16			base;
		u16			feat;
		u32			texsize;
	} fields;
	string name;

	sparrow::deserialize(name, stream);

	if(name == "sparrow::PixmapFont")
	{
		vector<u8> comp;
		vector<u8x4> data;
		u8x4 color = u8x4(0, 255, 0, 0);

		sparrow::deserialize(_name, stream);
		stream->read(&fields, sizeof fields); 
		sparrow::deserialize(_face, stream);
		sparrow::deserialize(_glyphs, stream);
		sparrow::deserialize(_kerning, stream);
		sparrow::deserialize(comp, stream);

		_size = fields.size;
		_height = fields.height;
		_base = fields.base;
		_texsize = fields.texsize;
		_feat = bitset<5>(static_cast<u32>(fields.feat));

		u32 count;
		u32 alpha;
		u32 index = 0;
		data.resize(fields.texsize * fields.texsize);
		for(u32 i = 0; i < comp.size(); ++i)
		{
			count = comp[i] & 0x0f; 
			alpha = comp[i] & 0xf0;
			for(u32 j = 0; j <= count; ++j)
				data[index++] = u8x4(255, 255, 255, alpha);
		}
		impl::TextureAtlas* atlas = &_context->_textureAtlas;
		atlas->acquireAtlasPage(this, _callback, data.data(), fields.texsize);
	}
}

void PixmapFont::clear()
{
	if(valid())
	{
		impl::TextureAtlas* atlas = &_context->_textureAtlas;
		atlas->releaseAtlasPage(this, _texsize);
	}

	_name = string();
	_face = string();
	_size = 0;
	_height = 0;
	_base = 0;
	_feat = bitset<5>();
	_offset = u16x2(0, 0);
	_glyphs.clear();
	_kerning.clear();
}

bool PixmapFont::valid() const
{
	return _face.size() != 0;
}

void PixmapFont::_updatePage(const u32x2& offset, const u32x2& size)
{
	s16x2 old_offset = -s16x2(_offset);
	s16x2 new_offset = s16x2(offset);
	if(offset != _offset)
	{
		vector_map<c16, _Glyph>::iterator itr = _glyphs.begin(), end = _glyphs.end();

		while(itr != end)
		{
			aabrTranslate(itr->source, old_offset);
			aabrTranslate(itr->source, new_offset);
			++itr;
		}
	}
	_offset = offset;
}

void PixmapFont::_callback(void* handler, const u32x2& offset, u32 size)
{
	static_cast<PixmapFont*>(handler)->_updatePage(offset, u32x2(size, size));
}

} //namespace sparrow
