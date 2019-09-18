#pragma once
#include "Framework.hpp"
namespace sparrow {

class Context2D;
class PixmapFont
{
public:
						PixmapFont(Context2D* context);
						~PixmapFont();

	const string&		getFace() const;
	u32					getSize() const;
	bool				isItalic() const;
	bool				isUnderlined() const;
	bool				isBold() const;
	bool				isKerning() const;
	bool				isSmooth() const;
	bool				isOutlined() const;
	bool				isAvailable(c16 glyph) const;
	
	void				deserialize(const DataStream* stream);
	void				clear();
	bool				valid() const;
private:
	enum { _Italic, _Underlined, _Bold, _Smooth, _Outlined };
	struct _Glyph { AABRs source, dest; s32 advance; };
	union _Kerning 
	{	
		struct				{ c16 st, nd; };
		u32					pair;
		bool				operator<(const _Kerning& that);
	};

	void				_updatePage(const u32x2& offset, const u32x2& size);
	static void			_callback(void* handler, const u32x2& offset, u32 size);

	string				_name;
	string				_face;
	u16					_size;
	u16					_height;
	u16					_base;
	bitset<5>			_feat;
	u16x2				_offset;
	u32					_texsize;					
	vector_map<c16, _Glyph>	_glyphs;
	vector_map<_Kerning, s32> _kerning;
	Context2D*			_context;
	friend class Context2D;
};
enum TextCursorMode
{
	TextCursorModeNone,
	TextCursorModeInsert,
	TextCursorModeOverwrite,
	TextCursorModeSelect,
	TextCursorModeHighlight,
};

struct TextBoxDesc
{

						TextBoxDesc()
		: aabr(0, 0, 1000, 1000)
		, font(nullptr)
		, zpos(0.f)
		, firstColor(0, 0, 0, 255)
		, secondColor(0, 0, 0, 255)
		, verticalAlign(VerticalAlignTop)
		, horizontalAlign(HorizontalAlignLeft)
		, textDirection(TextDirectionRight)
		, textCursorMode(TextCursorModeNone)
		, selectionBegin(0)
		, selectionEnd(0)
		, firstLine(0)
	{ }

	AABRs				aabr;
	string				text;
	const PixmapFont*	font;
	f32					zpos;
	u8x4				firstColor;
	u8x4				secondColor;
	VerticalAlign		verticalAlign;
	HorizontalAlign		horizontalAlign;
	TextDirection		textDirection;
	TextCursorMode		textCursorMode;
	u32					selectionBegin;
	u32					selectionEnd;
	u32					firstLine;
};

inline bool PixmapFont::_Kerning::operator<(const _Kerning& that)
{
	return pair < that.pair; 
}

inline bool operator<(const TextBoxDesc& a, const TextBoxDesc& b)
{
	return a.zpos < b.zpos;
}

inline bool operator>(const TextBoxDesc& a, const TextBoxDesc& b)
{
	return a.zpos > b.zpos;
}

} //namespace sparrow
