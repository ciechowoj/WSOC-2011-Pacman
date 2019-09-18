#include "Context2D.hpp"
namespace sparrow {
namespace impl {

TextureAtlas::TextureAtlas(Framework* framework, u32 start_size, u32 min_size)
{
	_atlasExp = msbindex(start_size);
	_deltaExp = msbindex(min_size);
	_freePages.resize(_atlasExp - _deltaExp + 1);
	_usedPages.resize(_atlasExp - _deltaExp + 1);
	_freePages.back().insert(0);

	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, start_size, start_size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

TextureAtlas::~TextureAtlas()
{
	glDeleteTextures(1, &_texture);
}

void TextureAtlas::acquireAtlasPage(void* handler, PageCallback callback, void* data, u32 size)
{
	u32 index = _acquire(msbindex(size), handler, callback);
	u32x2 offset;
	_getOffByIndex(index, &offset);

	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, offset.x, offset.y, size, size, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	callback(handler, offset, 1 << _atlasExp);
}

void TextureAtlas::releaseAtlasPage(void* handler, u32 size)
{
	u32 withDelta = msbindex(size) - _deltaExp;
	UsedPages::iterator itr = _usedPages[withDelta].begin(), end = _usedPages[withDelta].end();
	while(itr < end && itr->_handler != handler)
		++itr;
	if(itr != end)
		_release(_usedPages[withDelta].key_at(itr));
}

void TextureAtlas::bindTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
}

u32 TextureAtlas::getDimension() const
{
	return 1u << _atlasExp;
}

TextureAtlas::_Page::_Page()
	: _handler(nullptr)
	, _callback(nullptr)
{ }

TextureAtlas::_Page::_Page(void* handler, PageCallback callback)
	: _handler(handler)
	, _callback(callback)
{ }

u32 TextureAtlas::_getExpByIndex(u32 index)
{
	u32 end = 0;
	u32 exp = _atlasExp;
	while(end < index)
	{
		end += 1;
		end *= 4;
		--exp;
	}
	return exp;
}

void TextureAtlas::_getOffByIndex(u32 index, u32x2* offset)
{
	u32 size = _getExpByIndex(index);
	u32 mod0, mod1;
	offset->x = 0;
	offset->y = 0;
	while(index != 0)
	{
		index -= 1;

		mod0 = index;
		index >>= 2;
		mod0 -= index << 2;

		mod1 = mod0;
		mod0 >>= 1;
		mod1 -= mod0 << 1;

		offset->x += mod1 << size;
		offset->y += mod0 << size;

		size += 1;
	}
}

u32 TextureAtlas::_acquire(u32 page_exp, void* handler, PageCallback callback)
{
	// if page is bigger than available capacity
	if(_atlasExp < page_exp)
		_enlarge(max(_atlasExp, page_exp) + 1);

	// update pageExp value
	u32 withDelta = page_exp - _deltaExp;
	u32 result = 0;

	// look for free page
	if(_freePages[withDelta].empty())
	{
		// split
		u32 index = _acquire(page_exp + 1) * 4;
		result = ++index;
		_usedPages[withDelta].insert(result, _Page(handler, callback));
		_freePages[withDelta].insert(++index);
		_freePages[withDelta].insert(++index);
		_freePages[withDelta].insert(++index);
	}
	else
	{
		// get first
		result = _freePages[withDelta].front();
		_usedPages[withDelta].insert(result, _Page(handler, callback));
		_freePages[withDelta].erase(_freePages[withDelta].begin());
	}

	// return found page
	return result;
}

void TextureAtlas::_release(u32 index)
{
	u32 withDelta = _getExpByIndex(index) - _deltaExp;

	_freePages[withDelta].insert(index);
	_usedPages[withDelta].erase(index);

	// join pages
	// find first page
	u32 parent = (index - 1) >> 2;
	u32 base = (parent << 2) + 1;

	// check if there are 4 free pages
	FreePages::iterator begin = _freePages[withDelta].find(base);
	bool release = false;
	if(begin + 4 <= _freePages[withDelta].end())
	{
		release |= *begin == base; ++begin; ++base;
		release &= *begin == base; ++begin; ++base;
		release &= *begin == base; ++begin; ++base;
		release &= *begin == base; ++begin;
	}

	// if there are, then erase it and release parent
	if(release)
	{
		_freePages[withDelta].erase(begin - 4, begin);
		_release(parent);
	}
}

void TextureAtlas::_enlarge(u32 atlas_exp)
{
	u32 diff = atlas_exp - _atlasExp;
	_atlasExp = atlas_exp;
	vector<UsedPages> usedPages;
	usedPages.swap(_usedPages);
	_usedPages.resize(usedPages.size() + diff);
	_freePages.clear();
	_freePages.resize(usedPages.size() + diff);
	_freePages.back().insert(0);
	
	for(u32 i = 0; i < _freePages.size(); ++i)
	{
		for(UsedPages::iterator itr = _usedPages[i].begin(); itr < _usedPages[i].end(); ++itr)
			_acquire(i + _deltaExp, itr->_handler, itr->_callback);
	}

	u32 texture, proxy;
	u32 size = 1 << atlas_exp;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glGenBuffers(1, &proxy);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, proxy);
	glBufferData(GL_PIXEL_PACK_BUFFER, size * size * sizeof(u8x4), nullptr, GL_STATIC_COPY);

	glBindTexture(GL_TEXTURE_2D, _texture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, proxy);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size, size, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glDeleteBuffers(1, &proxy);

	glDeleteTextures(1, &_texture);
	_texture = texture;
	glEnable(GL_TEXTURE_2D);
}

QuadBuffer::QuadBuffer(Framework* framework, u32 start_size)
	: _bufsize(0)
{
	reserve(start_size);
	glGenBuffers(1, &_vbuffer);
	glGenBuffers(1, &_ibuffer);

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D), (void*)8);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)16);
	glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex2D), (void*)20);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibuffer);
}

QuadBuffer::~QuadBuffer()
{
	glDeleteBuffers(1, &_vbuffer);
	glDeleteBuffers(1, &_ibuffer);
	glDeleteVertexArrays(1, &_vao);
}

void QuadBuffer::bindBuffer()
{
	if(_bufsize < capacity())
	{
		reserve(capacity());
		_enlarge();
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size() * sizeof Vertex2D, data());
	}

	glBindVertexArray(_vao);
}

void QuadBuffer::_enlarge()
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbuffer);
	glBufferData(GL_ARRAY_BUFFER, capacity() * sizeof Vertex2D, data(), GL_STREAM_DRAW);

	u32 quad_count = capacity() / 4;
	u32 index_count = quad_count * 5;
	u16* index_data = new u16[index_count];
	for(u32 i = 0; i < quad_count; ++i)
	{
		for(u32 j = 0; j < 4; ++j)
			index_data[i * 5 + j] = i * 4 + j;
		index_data[i * 5 + 4] = 0xffff;
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof u16, index_data, GL_STATIC_DRAW);

	delete[] index_data;
}

} // namespace impl

Context2D::Context2D(Framework* framework)
	: _textureAtlas(framework, 1024, 32)
	, _quadBuffer(framework, 1024)
	, _dimension(framework->getSize())
	, _atlasDest(0, 0, 0, 0)
{
	const char* vertex_source =
		"#version 150\n"
		"in vec2 in_position;\n"
		"in vec2 in_texcoord;\n"
		"in vec4 in_color_add;\n"
		"in vec4 in_color_mul;\n"
		"out vec2 texcoord;\n"
		"out vec4 color_add;\n"
		"out vec4 color_mul;\n"
		"uniform vec2 pos_factor;\n"
		"uniform vec2 tex_factor;\n"
		"void main(void)\n"
		"{\n"
		"	gl_Position = vec4(in_position.x * pos_factor.x - 1.f, 1.f - in_position.y * pos_factor.y, 0.f, 1.f);\n"
		"	texcoord = in_texcoord * tex_factor;\n"
		"	color_add = in_color_add;\n"
		"	color_mul = in_color_mul;\n"
		"}\n";

	const char* fragment_source =
		"#version 150\n"
		"uniform sampler2D texture1;\n"
		"in vec2 texcoord;\n"
		"in vec4 color_add;\n"
		"in vec4 color_mul;\n"
		"out vec4 target0;\n"
		"void main(void)\n"
		"{\n"
		"	target0 = texture(texture1, texcoord) * color_mul + color_add;\n"
		"}\n";

	s32 is_compiled = false;
	s32 log_length = 0;
	u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	u32 fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);

	glCompileShader(vertex_shader);
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == false)
	{
		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH, &log_length);
 
		/* The maxLength includes the NULL character */
		char* log = new char[log_length];
 
		glGetShaderInfoLog(vertex_shader, log_length, &log_length, log);
 
		TOGGLE_BREAKPOINT
		delete [] log;
	}

	glCompileShader(fragment_shader);
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &is_compiled);
	if(is_compiled == false)
	{
		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &log_length);
 
		/* The maxLength includes the NULL character */
		char* log = new char[log_length];
 
		glGetShaderInfoLog(fragment_shader, log_length, &log_length, log);
 
		TOGGLE_BREAKPOINT
		delete [] log;
	}


	_program = glCreateProgram();
	glAttachShader(_program, vertex_shader);
	glAttachShader(_program, fragment_shader);
	glBindAttribLocation(_program, 0, "in_position");
	glBindAttribLocation(_program, 1, "in_texcoord");
	glBindAttribLocation(_program, 2, "in_color_mul");
	glBindAttribLocation(_program, 3, "in_color_add");
	glLinkProgram(_program);
	glUseProgram(_program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	_texFactorIndex = glGetUniformLocation(_program, "tex_factor");
	_posFactorIndex = glGetUniformLocation(_program, "pos_factor");
	_samplerIndex = glGetUniformLocation(_program, "texture1");	
}

Context2D::~Context2D()
{
	glUseProgram(0);
	glDeleteProgram(_program);



}

void Context2D::flush()
{
	_quadBuffer.clear();

	vector_multiset<TextBoxDesc>::iterator text_box_itr = _textBoxBuffer.begin(), text_box_end = _textBoxBuffer.end();
	vector_multiset<SpriteDesc>::iterator sprite_itr = _spriteBuffer.begin(), sprite_end = _spriteBuffer.end();
	f32 current_zpos;

	while(text_box_itr < text_box_end && sprite_itr < sprite_end)
	{
		if(text_box_itr->zpos < sprite_itr->zpos)
		{
			current_zpos = text_box_itr->zpos;
			while(text_box_itr < text_box_end && text_box_itr->zpos == current_zpos)
			{
				_drawTextBox(*text_box_itr);
				++text_box_itr;
			}
		}
		else
		{
			current_zpos = sprite_itr->zpos;
			while(sprite_itr < sprite_end && sprite_itr->zpos == current_zpos)
			{
				_drawSprite(sprite_itr);
				++sprite_itr;
			}
		}
	}

	while(text_box_itr < text_box_end)
	{
		_drawTextBox(*text_box_itr);
		++text_box_itr;
	}

	while(sprite_itr < sprite_end)
	{
		_drawSprite(sprite_itr);
		++sprite_itr;
	}

	if(aabrArea(_atlasDest) != 0)
	{
		u32 atlas_begin = _quadBuffer.size();
		_quadBuffer.push_back_def(4);
		_quadBuffer[atlas_begin + 0].position = _atlasDest.leftTop();
		_quadBuffer[atlas_begin + 0].texcoord = f32x2(0, 0);
		_quadBuffer[atlas_begin + 0].color_mul = u8x4(255, 255, 255, 0);
		_quadBuffer[atlas_begin + 0].color_add = u8x4(0, 0, 0, 255);
		_quadBuffer[atlas_begin + 1].position = _atlasDest.leftBottom();
		_quadBuffer[atlas_begin + 1].texcoord = f32x2(0, (f32)_textureAtlas.getDimension());
		_quadBuffer[atlas_begin + 1].color_mul = u8x4(255, 255, 255, 0);
		_quadBuffer[atlas_begin + 1].color_add = u8x4(0, 0, 0, 255);
		_quadBuffer[atlas_begin + 2].position = _atlasDest.rightTop();
		_quadBuffer[atlas_begin + 2].texcoord = f32x2((f32)_textureAtlas.getDimension(), 0);
		_quadBuffer[atlas_begin + 2].color_mul = u8x4(255, 255, 255, 0);
		_quadBuffer[atlas_begin + 2].color_add = u8x4(0, 0, 0, 255);
		_quadBuffer[atlas_begin + 3].position = _atlasDest.rightBottom();
		_quadBuffer[atlas_begin + 3].texcoord = f32x2((f32)_textureAtlas.getDimension(), (f32)_textureAtlas.getDimension());
		_quadBuffer[atlas_begin + 3].color_mul = u8x4(255, 255, 255, 0);
		_quadBuffer[atlas_begin + 3].color_add = u8x4(0, 0, 0, 255);
	}

	_textBoxBuffer.clear();
	_spriteBuffer.clear();
}

void Context2D::render()
{
	f32 tex_factor = 1.f / (f32)_textureAtlas.getDimension();
	f32x2 pos_factor = f32x2(2.f / _dimension.x, 2.f / _dimension.y);

	glUseProgram(_program);
	glUniform1i(_samplerIndex, 0);
	glUniform2f(_texFactorIndex, tex_factor, tex_factor);
	glUniform2f(_posFactorIndex, pos_factor.x, pos_factor.y);
	_textureAtlas.bindTexture();
	_quadBuffer.bindBuffer();
	glPrimitiveRestartIndex(0xffff);
	glEnable(GL_PRIMITIVE_RESTART);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	u32 element_count = _quadBuffer.size() / 4 * 5;
	glDrawElements(GL_TRIANGLE_STRIP, element_count, GL_UNSIGNED_SHORT, 0);
	glDisable(GL_BLEND);
	glDisable(GL_PRIMITIVE_RESTART);
}

void Context2D::_drawTextBox(const TextBoxDesc& desc)
{
	const u8x4 color = u8x4(desc.firstColor.x, desc.firstColor.y, desc.firstColor.z, 255);
	
	u32 char_index = 0;
	u32 word_index = 0;
	u32 line_start_index = 0;
	u32 text_start_vertex = _quadBuffer.size();
	u32 line_start_vertex = _quadBuffer.size();
	string::iterator itr = desc.text.begin();
	string::iterator end = desc.text.end();

	const PixmapFont::_Glyph* this_glyph = nullptr;
	c16 this_char = 0;

	s32x2 current_position = desc.aabr.leftTop();
	AABRs line_aabr;
	s32 offset;

	while(itr < end)
	{
		this_char = *itr;
		this_glyph = &(*desc.font->_glyphs.find(this_char));

		if(current_position.x + this_glyph->advance >= desc.aabr.right || this_char == '\n')
		{
			if(this_char != '\n')
			{
				/*u32 line_start_index = (line_start_vertex - text_start_vertex) / 4;*/
				u32 delta = char_index - word_index;
				if(word_index > line_start_index)
				{
					itr -= delta;
					char_index -= delta;
					_quadBuffer.pop_back(delta * 4);
				}
			}
			else
			{
				++itr;
				++char_index;
			}

			line_aabr.leftTop(_quadBuffer[line_start_vertex].position);
			line_aabr.rightBottom(_quadBuffer.back().position);

			switch(desc.horizontalAlign)
			{
			case HorizontalAlignCenter:
				offset = (desc.aabr.right - line_aabr.right) / 2;
				for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
					_quadBuffer[i].position.x += offset;
				break;
			case HorizontalAlignRight:
				offset = desc.aabr.right - line_aabr.right;
				for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
					_quadBuffer[i].position.x += offset;
				break;
			}

			current_position.x = desc.aabr.left;
			current_position.y += desc.font->_height;
			line_start_vertex = _quadBuffer.size();
			line_start_index = char_index;

			if(current_position.y >= desc.aabr.bottom)
				break;
		}
		else
		{
			u8x4 local_color = color;
			if(desc.textCursorMode == TextCursorModeHighlight && char_index >= desc.selectionBegin && char_index < desc.selectionEnd)
				local_color = u8x4(desc.secondColor.x, desc.secondColor.y, desc.secondColor.z, 255);
			else
				local_color = color;
			
			if(iswhite(this_char))
				word_index = char_index + 1;

			u32 glyph_begin = _quadBuffer.size();
			_quadBuffer.push_back_def(4);
			_quadBuffer[glyph_begin + 0].position = current_position + this_glyph->dest.leftTop();
			_quadBuffer[glyph_begin + 0].texcoord = this_glyph->source.leftTop();
			_quadBuffer[glyph_begin + 0].color_mul = local_color;
			_quadBuffer[glyph_begin + 0].color_add = u8x4(0, 0, 0, 0);
			_quadBuffer[glyph_begin + 1].position = current_position + this_glyph->dest.leftBottom();
			_quadBuffer[glyph_begin + 1].texcoord = this_glyph->source.leftBottom();
			_quadBuffer[glyph_begin + 1].color_mul = local_color;
			_quadBuffer[glyph_begin + 1].color_add = u8x4(0, 0, 0, 0);
			_quadBuffer[glyph_begin + 2].position = current_position + this_glyph->dest.rightTop();
			_quadBuffer[glyph_begin + 2].texcoord = this_glyph->source.rightTop();
			_quadBuffer[glyph_begin + 2].color_mul = local_color;
			_quadBuffer[glyph_begin + 2].color_add = u8x4(0, 0, 0, 0);
			_quadBuffer[glyph_begin + 3].position = current_position + this_glyph->dest.rightBottom();
			_quadBuffer[glyph_begin + 3].texcoord = this_glyph->source.rightBottom();
			_quadBuffer[glyph_begin + 3].color_mul = local_color;
			_quadBuffer[glyph_begin + 3].color_add = u8x4(0, 0, 0, 0);

			++itr; ++char_index;
			current_position.x += this_glyph->advance;
		}
	}

	switch(desc.horizontalAlign)
	{
	case HorizontalAlignCenter:
		offset = (desc.aabr.right - (s32)_quadBuffer.back().position.x) / 2;
		for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
			_quadBuffer[i].position.x += offset;
		break;
	case HorizontalAlignRight:
		offset = desc.aabr.right - (s32)_quadBuffer.back().position.x;
		for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
			_quadBuffer[i].position.x += offset;
		break;
	}

	switch(desc.verticalAlign) 
	{
	case VerticalAlignCenter:
		offset = (desc.aabr.bottom - (s32)_quadBuffer.back().position.y) / 2;
		for(u32 i = text_start_vertex; i <  _quadBuffer.size(); ++i)
			_quadBuffer[i].position.y += offset;
		break;
	case VerticalAlignBottom:
		offset = desc.aabr.bottom - (s32)_quadBuffer.back().position.y;
		for(u32 i = text_start_vertex; i <  _quadBuffer.size(); ++i)
			_quadBuffer[i].position.y += offset;
		break;
	}
}

void Context2D::_drawSprite(const SpriteDesc* desc)
{
	const u8x4 color_mul = u8x4(255, 255, 255, 255);
	const u8x4 color_add = u8x4(0, 0, 0, 0);
	u32 index = desc->sprite + desc->frame;
	
	AABRf src = desc->spriteSet->_frames[index];
	AABRf dest = AABRf(desc->position.x, desc->position.y, desc->position.x + aabrWidth(src), desc->position.y + aabrHeight(src));

	u32 sprite_begin = _quadBuffer.size();
	_quadBuffer.push_back_def(4);
	_quadBuffer[sprite_begin + 0].position = dest.leftTop();
	_quadBuffer[sprite_begin + 0].texcoord = src.leftTop();
	_quadBuffer[sprite_begin + 0].color_mul = color_mul;
	_quadBuffer[sprite_begin + 0].color_add = color_add;
	_quadBuffer[sprite_begin + 1].position = dest.leftBottom();
	_quadBuffer[sprite_begin + 1].texcoord = src.leftBottom();
	_quadBuffer[sprite_begin + 1].color_mul = color_mul;
	_quadBuffer[sprite_begin + 1].color_add = color_add;
	_quadBuffer[sprite_begin + 2].position = dest.rightTop();
	_quadBuffer[sprite_begin + 2].texcoord = src.rightTop();
	_quadBuffer[sprite_begin + 2].color_mul = color_mul;
	_quadBuffer[sprite_begin + 2].color_add = color_add;
	_quadBuffer[sprite_begin + 3].position = dest.rightBottom();
	_quadBuffer[sprite_begin + 3].texcoord = src.rightBottom();
	_quadBuffer[sprite_begin + 3].color_mul = color_mul;
	_quadBuffer[sprite_begin + 3].color_add = color_add;
}

/*void Context2D::_drawTextBox(const TextBoxDesc& desc)
{
	// set color
	const u8x4 color = u8x4(desc.color.x, desc.color.y, desc.color.z, 255);
	// units are quads/glyphs (1 glyph == 1 quad)
	u32 char_index = 0;
	u32 line_index = 0;
	u32 text_start_vertex = _quadBuffer.size();
	u32 line_start_vertex = _quadBuffer.size();
	u32 word_length = 0;

	string::iterator itr = desc.text.begin();
	string::iterator end = desc.text.end();
	string::iterator line_begin = itr;
	string::iterator temp = itr;

	const PixmapFont::_Glyph* this_glyph = nullptr;
	c16 this_char = 0;

	s32x2 current_position = desc.aabr.getLeftTop();
	AABRs line_aabr;
	s32 offset;

	while(itr < end)
	{
		this_char = *itr;
		this_glyph = &(*desc.font->_glyphs.find(this_char));

		u32 glyph_begin = _quadBuffer.size();
		_quadBuffer.push_back_def(4);
		_quadBuffer[glyph_begin + 0].position = current_position + this_glyph->dest.getLeftTop();
		_quadBuffer[glyph_begin + 0].texcoord = this_glyph->source.getLeftTop();
		_quadBuffer[glyph_begin + 0].color = color;
		_quadBuffer[glyph_begin + 1].position = current_position + this_glyph->dest.getLeftBottom();
		_quadBuffer[glyph_begin + 1].texcoord = this_glyph->source.getLeftBottom();
		_quadBuffer[glyph_begin + 1].color = color;
		_quadBuffer[glyph_begin + 2].position = current_position + this_glyph->dest.getRightTop();
		_quadBuffer[glyph_begin + 2].texcoord = this_glyph->source.getRightTop();
		_quadBuffer[glyph_begin + 2].color = color;
		_quadBuffer[glyph_begin + 3].position = current_position + this_glyph->dest.getRightBottom();
		_quadBuffer[glyph_begin + 3].texcoord = this_glyph->source.getRightBottom();
		_quadBuffer[glyph_begin + 3].color = color;
		++word_length;
	
		if(isWhitespace(this_char))
			word_length = 0;

		++itr;

		current_position.x += this_glyph->advance;
		if(current_position.x + this_glyph->advance > desc.aabr.right || this_char == '\n')
		{
			line_aabr.setLeftTop(_quadBuffer[line_start_vertex].position);
			line_aabr.setRightBottom(_quadBuffer.back().position);
			
			if(this_char != '\n')
			{
				temp = itr;
				itr -= word_length;
				if(itr > line_begin || word_length == 1)
					_quadBuffer.pop_back(word_length * 4);
				else
					itr = temp;
				word_length = 0;
				line_aabr.right = _quadBuffer.empty() ? line_aabr.left : _quadBuffer.back().position.x;
			}
			
			switch(desc.horizontalAlign)
			{
			case HorizontalAlignCenter:
				offset = (desc.aabr.right - line_aabr.right) / 2;
				for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
					_quadBuffer[i].position.x += offset;
				break;
			case HorizontalAlignRight:
				offset = desc.aabr.right - line_aabr.right;
				for(u32 i = line_start_vertex; i <  _quadBuffer.size(); ++i)
					_quadBuffer[i].position.x += offset;
				break;
			}

			line_start_vertex = _quadBuffer.size();
			line_begin = itr;
			current_position.x = desc.aabr.left;
			current_position.y += desc.font->_height;
		}

		if(current_position.y > desc.aabr.bottom)
			break;
	}

	

}*/

} // namespace sparrow
