#include "TextBox.hpp"
namespace sparrow {

void TextBox::getCharHit(const s32x2& in_corrds, u32& out_char_index) const
{
	if(!_hitCacheUpdated)
		_buildHitCache();

	// TODO
}

void TextBox::setSource(const s32x2& in_source)
{
	_source = in_source;
}

void TextBox::setDestination(const AABRi& in_destination)
{
	s32 old_width = aabrWidth(_destination);
	s32 new_width = aabrWidth(in_destination);
	if(old_width != new_width)
	{
		_hitCacheUpdated = false;
		_drawCacheUpdated = false;
	}
}

void TextBox::setColor(const f32x4& in_color)
{
	colorConvertFloatToByte(_color, in_color);
}

void TextBox::setAlign(HorizontalAlign in_horizontal_align, VerticalAlign in_vertical_align)
{
	_halign = in_horizontal_align;
	_valign = in_vertical_align;
}

void TextBox::setDirection(const TextDirection& in_direction)
{
	_direction = in_direction;
}

void TextBox::setFont(const PixmapFont* in_pixmap_font)
{
	// in_pixmap_font->acquire();
	_font = in_pixmap_font;
}

void TextBox::setSelection(u32 in_begin, u32 in_end, const f32x4& in_color)
{
	_selectBegin = in_begin;
	_selectEnd = in_end;
	colorConvertFloatToByte(_selectColor, in_color);
	_selectionUpdated = false;
}

void TextBox::setCursor(u32 in_position, TextCursor in_type)
{
	_cursorPos = in_position;
	_cursorType = in_type;
	_cursorUpdated = false;
}

void TextBox::setString(const string& in_string)
{
	_string = in_string;
	_drawCacheUpdated = false;
}

void TextBox::setZLayer(f32 in_layer)
{
	_zorder = in_layer;
}

void TextBox::acquire() const
{
	++_refcount;
}

void TextBox::release() const
{
	if(--_refcount == 0)
		delete this;
}

void TextBox::_buildDrawCache() const
{
}

void TextBox::_buildHitCache() const
{
}

} // namespace sparrow
