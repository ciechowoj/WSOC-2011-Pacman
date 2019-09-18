#pragma once
#include "PixmapFont.hpp"
namespace sparrow {
	
enum VerticalAlign
{
	VerticalAlignTop,
	VerticalAlignCenter,
	VerticalAlignBottom,
};

enum HorizontalAlign
{
	HorizontalAlignLeft,
	HorizontalAlignCenter,
	HorizontalAlignRight,
};

enum TextDirection
{
	TextDirectionRight,	// 0
	TextDirectionDown,	// 90
	TextDirectionLeft,	// 180
	TextDirectionUp,	// 270
};

enum TextCursor 
{ 
	TextCursorNoCursor, 
	TextCursorInsert, 
	TextCursorOverwrite 
};

class TextBox
{
public:
	void				getSource(s32x2& out_source) const;
	void				getDestination(AABRi& out_destination) const;
	void				getColor(f32x4& out_color) const;
	void				getAlign(HorizontalAlign& out_horizontal_align, VerticalAlign& out_vertical_align) const;
	void				getDirection(TextDirection& out_direction) const;
	void				getFont(PixmapFont* out_pixmap_font) const;
	void				getSelection(u32& out_begin, u32& out_end, f32x4& out_color) const;
	void				getCursor(u32& out_position, TextCursor& out_cursor_type) const;
	void				getString(string& out_string) const;
	void				getZLayer(f32& out_zorder) const;
	void				getCharHit(const s32x2& in_corrds, u32& out_char_index) const;

	void				setSource(const s32x2& in_source);
	void				setDestination(const AABRi& in_destination);
	void				setColor(const f32x4& in_color);
	void				setAlign(HorizontalAlign in_horizontal_align, VerticalAlign in_vertical_align);
	void				setDirection(const TextDirection& in_direction);
	void				setFont(const PixmapFont* in_pixmap_font);
	void				setSelection(u32 in_begin, u32 in_end, const f32x4& in_color);
	void				setCursor(u32 in_position, TextCursor in_type);
	void				setString(const string& in_string);
	void				setZLayer(f32 in_layer);

	void				acquire() const;
	void				release() const;
private:
	void				_buildDrawCache() const;
	void				_buildHitCache() const;
	void				_requestCache() const;

	class				_HitCache
	{
	public:



		vector<AABRi>	



	};

	class				_DrawCache
	{
	public:
							
	private:
		struct Entry
		{
			s32x2			position;
			s16x2			texcoord;
			s16x2			size;
			s32				xoffset;
			u32				line;
		};

	};

	mutable struct _Cache
	{
		struct Entry
		{
			s32x2				position;
			s16x2				texcoord;
			s8x2				size;
			s16					xoffset;
			u16					line;
		};

		u32					height;
		vector<Entry>		glyphs;
		vector<AABRs>		selection;
		AABRs				cursor;
	} _drawCache;

	mutable vector<AABRs>	_hitCache;

	f32					_zlayer;
	RefCtrMT			_refCtr;
	AABRi				_destination;
	s32x2				_source;
	string				_string;
	const PixmapFont*	_font;
	u32					_color;
	HorizontalAlign		_halign;
	VerticalAlign		_valign;
	TextDirection		_direction;
	TextCursor			_cursorType;
	u32					_cursorPos;
	u32					_selectBegin;
	u32					_selectEnd;
	u32					_selectColor;
	bitset<4>			_cacheState;


						TextBox();
						TextBox(const TextBox& that);
						~TextBox();
	TextBox&			operator=(const TextBox& that);
};

inline void TextBox::getSource(s32x2& out_source) const
{
	out_source = _source;
}

inline void TextBox::getDestination(AABRi& out_destination) const
{
	out_destination = _destination;
}

inline void TextBox::getColor(f32x4& out_color) const
{
	colorConvertByteToFloat(out_color, _color);
}

inline void TextBox::getAlign(HorizontalAlign& out_horizontal_align, VerticalAlign& out_vertical_align) const
{
	out_horizontal_align = _halign;
	out_vertical_align = _valign;
}

inline void TextBox::getDirection(TextDirection& out_direction) const
{
	out_direction = _direction;
}

inline void TextBox::getFont(PixmapFont* out_pixmap_font) const
{
	out_pixmap_font = _font;
}

inline void TextBox::getSelection(u32& out_begin, u32& out_end, f32x4& out_color) const
{
	out_begin = _selectBegin;
	out_end = _selectEnd;
	colorConvertByteToFloat(out_color, _selectColor);
}

inline void TextBox::getCursor(u32& out_position, TextCursor& out_cursor_type) const
{
	out_position = _cursorPos;
	out_cursor_type = _cursor;
}

inline void TextBox::getString(string& out_string) const
{
	out_string = _string;
}

inline void TextBox::getZLayer(f32& out_zorder) const
{
	out_zorder = _zorder;
}

} // namespace sparrow
