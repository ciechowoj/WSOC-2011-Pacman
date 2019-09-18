#pragma once
#include "PixmapFont.hpp"
#include "SpriteSet.hpp"
namespace sparrow {
namespace impl {

class TextureAtlas
{
public:
						TextureAtlas(Framework* framework, u32 start_size, u32 min_size);
						~TextureAtlas();

	typedef void (*PageCallback)(void* handler, const u32x2& offset, u32 size);
	void				acquireAtlasPage(void* handler, PageCallback callback, void* data, u32 size);
	void				releaseAtlasPage(void* handler, u32 size);
	void				bindTexture();
	u32					getDimension() const;
private:
	class _Page
	{
	public:
							_Page();
							_Page(void* handler, PageCallback callback);
							
		void*				_handler;
		PageCallback		_callback;
	};
	
	typedef vector_set<u32> FreePages;
	typedef vector_map<u32, _Page, greater<u32>> UsedPages;

	u32					_getExpByIndex(u32 index);
	void				_getOffByIndex(u32 index, u32x2* offset);
	u32					_acquire(u32 page_exp, void* handler = nullptr, PageCallback callback = nullptr);
	void				_release(u32 index);
	void				_enlarge(u32 atlas_exp);
	
	u32					_atlasExp;
	u32					_deltaExp;
	u32					_texture;

	vector<FreePages>	_freePages;
	vector<UsedPages>	_usedPages;
};

struct Vertex2D { f32x2 position, texcoord; u8x4 color_mul, color_add; };

class QuadBuffer : public vector<Vertex2D>
{
public:
						QuadBuffer(Framework* framework, u32 start_size);
						~QuadBuffer();

	void				bindBuffer();
private:
	void				_enlarge();
	u32					_vbuffer;
	u32					_ibuffer;
	u32					_bufsize;
	u32					_vao;
};

} // namespace impl

class Context2D
{
public:
						Context2D(Framework* framework);
						~Context2D();
	void				drawTextBox(const TextBoxDesc& desc);
	void				drawSprite(const SpriteDesc& desc);
	//void				drawControl(const ControlDesc& desc);
	void				drawAtlas(const AABRs& dest);
	void				setCoordSystem(const f32x2& dimensions);
	const f32x2&		getCoordSystem() const;
	void				flush();
	void				render();
private:
	void				_drawTextBox(const TextBoxDesc& desc);
	void				_drawSprite(const SpriteDesc* desc);
	impl::TextureAtlas	_textureAtlas;
	impl::QuadBuffer	_quadBuffer;
	u32					_program;
	s32					_texFactorIndex;
	s32					_posFactorIndex;
	s32					_samplerIndex;
	f32x2				_dimension;
	Framework*			_framework;
	AABRs				_atlasDest;

	vector_multiset<TextBoxDesc> _textBoxBuffer;
	vector_multiset<SpriteDesc> _spriteBuffer;
	friend class PixmapFont;
	friend class SpriteSet;
};

inline void Context2D::drawTextBox(const TextBoxDesc& desc)
{
	_textBoxBuffer.insert(desc);
}

inline void Context2D::drawSprite(const SpriteDesc& desc)
{
	_spriteBuffer.insert(desc);
}

//inline void Context2D::drawControl(const ControlDesc& desc) { }

inline void Context2D::drawAtlas(const AABRs& dest)
{
	_atlasDest = dest;
}

} // namespace sparrow
