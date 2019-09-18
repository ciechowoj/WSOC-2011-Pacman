#include "Resource.hpp"
#include <stdlib.h>
#include <IL\il.h>
namespace sparrow {

bool PixmapFontDesc::create(DataStream* stream) const
{
	//	font format
	//	1. font id "sparrow::GUIFont"
	//	2. font name <string>
	//	3. font size <u16>
	//	4. font height <u16>
	//	5. font base <u16>
	//	6. font feat <bitset 0/1/2/3/4 italic/underlined/bold/smooth/outlined : u16>
	//	7. font texsize <u32> 
	//	8. font face <string>
	//	9. glyphs <vector_map>
	//	10. kerning pairs <vector_map>
	//	11. texture with glyphs <vector<ubyte> of RLE compressed pixels>

	//	glyph format
	//	1. source <AABRs>
	//	2. offset <s16>
	//	3. code <u16>
	//	kerning pair format
	//	1. first u16
	//	2. second u16
	//	3. adjust s16

	// auxiliary structures
	struct Glyph 
	{ 
		AABRs			source;
		AABRs			dest;
		s32				advance;
	};

	union Kerning 
	{ 
						Kerning()
			: pair(0)
		{ }

						Kerning(c16 first, c16 second)
			: st(first)
			, nd(second)
		{ }

		struct			{ c16 st, nd; };
		u32				pair;

		bool operator<(const Kerning& that) const
		{
			return pair < that.pair;
		}
	};

	struct BMF
	{
		struct Header
		{
			c8		magic[4];
		};

		struct Info
		{
			u16		fontSize;
			u8		bitField;
			u8		charset;
			u16		stretchH;
			u8		aa;
			u8		paddingUp;
			u8		paddingRight;
			u8		paddingDown;
			u8		paddingLeft;
			u8		spacingHoriz;
			u8		spacingVert;
			u8		outline;
			char	fontName;
		};

		struct Common
		{
			u16		lineHeight;
			u16		base;
			u16		scaleW;
			u16		scaleH;
			u16		pages;
			u16		bitField;
			u8		alphaChnl;
			u8		redChnl;
			u8		greenChnl;
			u8		blueChnl;
		};

		struct Glyph
		{
			u32		id;
			u16		x;
			u16		y;
			u16		width;
			u16		height;
			s16		xoffset;
			s16		yoffset;
			s16		xadvance;
			u8		page;
			u8		chnl;
		};

		struct Kerning
		{
			u32		first;
			u32		second;
			s16		adjust;
		};
	};

	// font properties
	u8* data = nullptr;
	BMF::Header* header = nullptr;
	u8 *ptr = nullptr, *end = nullptr;
	string texfile;
	u32 texsize = ceil2(size) * 8;
	s32 baseline = 0;
	s32 height = 0;
	vector_map<c16, Glyph> glyphs;
	vector_map<Kerning, s32> kernings;
	vector<u8> pxData;
	bool result = false;

	// prepare bmfc file
	string config;

	config += "# AngelCode Bitmap Font Generator configuration file\n";
	config += "fileVersion=1\n\n";

	config += "# font settings\n";
	config += "fontName=" + face + "\n";
	config += "charSet=186\n";
	config += "fontSize=" + u32tostr(size) + "\n";
	config += "aa=1\n";
	config += "scaleH=100\n";
	config += "useSmoothing=" + u32tostr(static_cast<u32>(smooth)) + "\n";
	config += "isBold=" + u32tostr(static_cast<u32>(bold)) + "\n";
	config += "isItalic=" + u32tostr(static_cast<u32>(italic)) + "\n";
	config += "useUnicode=1\n";
	config += "disableBoxChars=1\n";
	config += "outputInvalidCharGlyph=0\n\n";

	config += "# character alignment\n";
	config += "paddingDown=0\n";
	config += "paddingUp=0\n";
	config += "paddingRight=0\n";
	config += "paddingLeft=0\n";
	config += "spacingHoriz=0\n";
	config += "spacingVert=0\n\n";

	config += "# output file\n";
	config += "outWidth=" + u32tostr(texsize) + "\n";
	config += "outHeight=" + u32tostr(texsize) + "\n";
	config += "outBitDepth=32\n";
	config += "fontDescFormat=2\n";
	config += "fourChnlPacked=0\n";
	config += "textureFormat=dds\n";
	config += "textureCompression=0\n";
	config += "alphaChnl=0\n";
	config += "redChnl=3\n";
	config += "greenChnl=3\n";
	config += "blueChnl=3\n";
	config += "invA=0\n";
	config += "invR=0\n";
	config += "invG=0\n";
	config += "invB=0\n\n";

	config += "# outline\n";
	config += "outlineThickness=" + u32tostr(static_cast<u32>(outline)) + "\n\n";
	
	config += "# selected chars\n";
	config += "chars=32-126,160-190,211,243,260-263,280-281,321-324,346-347,377-380\n\n"; // polish charset

	config += "# imported icon images\n\n";

	// create config file
	FileStream file("tempin.bmfc", StreamModeWrite, FileModeCreateAlways);
	save(config, &file, false);
	file.close();

	// invoke Bitmap font generator
	system("bmfont.exe -c tempin.bmfc -o tempout.fnt");

	// load file description
	deleteFile("tempin.bmfc"); // delete temporary file
	file.open("tempout.fnt", StreamModeRead, FileModeOpenExisting);
	result = file.opened();

	if(result)
	{
		data = new u8[file.size()];
		file.read(data, file.size());
		ptr = data;
		end = data + file.size();
		file.close();
		deleteFile("tempout.fnt");
		// load magic number
		header = reinterpret_cast<BMF::Header*>(ptr);
		result = header->magic[0] == 'B' && header->magic[1] == 'M' && header->magic[2] == 'F' && header->magic[3] == 3;
		ptr += 4;
	}

	// load info block
	if(result)
	{
		if(*ptr == 1) // check block type
		{
			++ptr;
			u32 block_size = *reinterpret_cast<u32*>(ptr);
			ptr += 4;
			BMF::Info* info = reinterpret_cast<BMF::Info*>(ptr);
			ptr += block_size;
			result = result && face == &info->fontName;
			result = result && size == info->fontSize;
			result = result && italic == ((info->bitField & 0x20) != 0);
			result = result && bold == ((info->bitField & 0x10) != 0);
			result = result && smooth == ((info->bitField & 0x80) != 0);
			result = result && outline == (info->outline > 0);
		}
		else
		{
			result = false;
		}
	}

	// load common block
	if(result)
	{
		if(*ptr == 2) // check block type
		{
			++ptr;
			u32 block_size = *reinterpret_cast<u32*>(ptr);
			ptr += 4;
			BMF::Common* common = reinterpret_cast<BMF::Common*>(ptr);
			ptr += block_size; // block size

			height = common->lineHeight;
			baseline = common->base;
			texsize = common->scaleW;
			result = result && texsize == common->scaleH;
			result = result && common->pages == 1;
		}
		else
		{
			result = false;
		}
	}

	// load page block
	if(result)
	{
		if(*ptr == 3) // check block type
		{
			++ptr; // block type
			u32 block_size = *reinterpret_cast<u32*>(ptr);
			ptr += 4; // block size
			texfile = reinterpret_cast<char*>(ptr);
			ptr += block_size; // block size
		}
		else
		{
			result = false;
		}
	}

	// load glyphs
	if(result)
	{
		if(*ptr == 4) // check block type
		{
			++ptr; // block type
			u32 block_size = *reinterpret_cast<u32*>(ptr);
			ptr += 4; // block size

			u32 count = block_size / 20;
			BMF::Glyph* glyph = nullptr;
			glyphs.reserve(count);
			vector_map<c16, Glyph>::iterator itr = glyphs.end();

			for(u32 i = 0; i < count; ++i)
			{
				glyph = reinterpret_cast<BMF::Glyph*>(ptr);
				ptr += 20; // one glyph

				itr = glyphs.insert_def(glyph->id, itr);

				itr->source.left = glyph->x;
				itr->source.top = glyph->y;
				itr->source.right = glyph->x + glyph->width;
				itr->source.bottom = glyph->y + glyph->height;

				itr->dest.left = glyph->xoffset;
				itr->dest.top = glyph->yoffset;
				itr->dest.right = glyph->xoffset + glyph->width;
				itr->dest.bottom = glyph->yoffset + glyph->height;

				itr->advance = glyph->xadvance;
			}
		}
		else
		{
			result = false;
		}
	}

	// load kerning pairs (optional)
	if(result && ptr < (end - sizeof BMF::Kerning))
	{
		if(*ptr == 5) // check block type
		{
			++ptr; // block type
			u32 block_size = *reinterpret_cast<u32*>(ptr);
			ptr += 4; // block size
			u32 count = block_size / 10;
			BMF::Kerning* kerning = nullptr;
			kernings.reserve(count);

			for(u32 i = 0; i < count; ++i)
			{
				kerning = reinterpret_cast<BMF::Kerning*>(ptr);
				ptr += 10; // one pair
				kernings.insert(Kerning(kerning->first, kerning->second), kerning->adjust);
			}
		}
		else
		{
			result = false;
		}
	}

	delete[] data;

	// load texture
	if(result)
	{
		struct {
			u32			dwSize;
			u32			dwFlags;
			u32			dwHeight;
			u32			dwWidth;
			u32			dwPitchOrLinearSize;
			u32			dwDepth;
			u32			dwMipMapCount;
			u32			dwReserved1[11];
			struct {
				u32		dwSize;
				u32		dwFlags;
				u32		dwFourCC;
				u32		dwRGBBitCount;
				u32		dwRBitMask;
				u32		dwGBitMask;
				u32		dwBBitMask;
				u32		dwABitMask;
			}			ddspf;
			u32			dwCaps;
			u32			dwCaps2;
			u32			dwCaps3;
			u32			dwCaps4;
			u32			dwReserved2;
		} ddsheader;
		//	texture  format
		//	1. width <u16>
		//	2. height <u16>
		//	3. data size <u32>
		//	4. RLE compressed alpha

		// open dds surface
		file.open(texfile, StreamModeRead, FileModeOpenExisting);

		// load magic
		u32 magic = 0;
		file.read(&magic, sizeof magic);
		runtime_assert(magic == 0x20534444, "Given file isn't dds!");
		
		// load header
		file.read(&ddsheader, sizeof ddsheader);
		runtime_assert(ddsheader.dwWidth == texsize, "Invalid texture width!");
		runtime_assert(ddsheader.dwHeight == texsize, "Invalid teture height!");
		runtime_assert(ddsheader.ddspf.dwABitMask == 0xff000000, "Invalid alpha bitmask!");
		runtime_assert(ddsheader.ddspf.dwFlags == 0x00000041, "Invalid alpha bitmask!");
		runtime_assert(ddsheader.ddspf.dwRGBBitCount == 0x00000020, "Invalid alpha bitmask!");
		u32 pitch = ddsheader.dwPitchOrLinearSize;

		// load data
		u32 pix_size = texsize * texsize;
		vector<u32> pixels;
		pixels.resize(pix_size);
		file.read(pixels.data(), pix_size * sizeof u32);
		file.close();

		// compress
		// RLE 0xKL
		// K - value
		// L - count of occurences

		u32 current = 0;
		u32 count = 0;
		u32 prev = pixels.front() >> 28u << 4u;
		u32 size_one = pix_size - 1;

		for(u32 i = 0; i < size_one; ++i)
		{
			current = pixels[i] >> 28u;
			++count;

			if(current != (pixels[i + 1] >> 28u) || count == 16)
			{
				pxData.push_back((current << 4u) | (count - 1));
				count = 0;
			}
		}

		current = pixels[size_one] >> 28u;
		pxData.push_back((current << 4u) | count);

		// delete file
		deleteFile(texfile);
	}

	if(result)
	{
		struct 
		{
			u16			size;
			u16			height;
			u16			base;
			u16			feat;
			u32			texsize;
		} fields;
		
		// save all to stream/file
		if(stream == nullptr)
		{
			file.open(name + ".bin", StreamModeWrite, FileModeCreateAlways);
			stream = &file;
		}

		if(stream->opened())
		{
			//	1. font id "sparrow::GUIFont"
			serialize(string("sparrow::PixmapFont"), stream);
			//	2. font name <string>
			serialize(name, stream);
			//	3. font size <u16>
			//	4. font height <u16>
			//	5. font base <u16>
			//	6. font feat <bitset 0/1/2/3 bold/italic/smooth/outlined : u16>
			//	7. font texsize <u32>
			fields.size = size;
			fields.height = height;
			fields.base = baseline;
			fields.feat = static_cast<u32>(italic) | static_cast<u32>(0) << 1 | static_cast<u32>(bold) << 2 | static_cast<u32>(smooth) << 3 | static_cast<u32>(outline) << 4;
			fields.texsize = texsize;
			stream->write(&fields, sizeof fields);
			//	8. font face <string>
			serialize(face, stream);
			//	9. glyphs <vector_map>
			serialize(glyphs, stream);
			//	10. kerning pairs <vector_map>
			serialize(kernings, stream);
			//	11. texture with glyphs <vector<ubyte> of RLE compressed pixels>
			serialize(pxData, stream);
		}
		else
		{
			result = false;
		}
	}
	return result;
}


bool SpriteSetDesc::create(DataStream* out_stream) const
{
	//	sprite set format
	//	1. set id "sparrow::GUIFont"
	//	2. set name <string>
	//	3. sprites <vector_map>
	//	4. frames <vector>
	//	5. texture <teture_size/RLE compressed channels>

	// sprite format
	//	1. name <string> : key
	//	2. frame <u16> : value
	//	3. count <u16> : value

	struct Sprite
	{
		Sprite(u16 f, u16 c)
			: frame(f)
			, count(c)
		{ }

		Sprite()
			: frame(0)
			, count(0)
		{ }

		u16 frame;
		u16 count;
	};

	struct Entry
	{
		u8	value;
		u8	count;
	};

	vector<AABRs> frames;
	vector_map<string, Sprite> sprites;
	vector_map<string, Sprite>::iterator itr;
	vector<Entry> red_channel;
	vector<Entry> green_channel;
	vector<Entry> blue_channel;
	vector<Entry> alpha_channel;
	u32 texture_size;

	bool result = true;
	StringLexer lexer;
	Lexem lexem;
	string strlex;
	string texture_path;
	string script_source;
	FileStream file_stream;
	file_stream.open(path, StreamModeRead, FileModeOpenExisting);
	
	if(file_stream.readable())
	{
		load(script_source, &file_stream);
		lexer.setSource(script_source);

		lexem = lexer.getLexem(strlex);

		if(lexem == LexemLabel && "texture" == strlex)
		{
			lexem = lexer.getLexem(strlex);

			if(lexem == LexemString)
			{
				texture_path = string(strlex);

				while(true)
				{
					lexem = lexer.getLexem(strlex);

					if(lexem == LexemLabel)
					{
						if("frame" == string(strlex))
						{
							frames.push_back_def();

							if((lexem = lexer.getLexem(strlex)) == LexemInteger)
								frames.back().left = dectos32(strlex);
							else
								break;

							if((lexem = lexer.getLexem(strlex)) == LexemInteger)
								frames.back().top = dectos32(strlex);
							else
								break;

							if((lexem = lexer.getLexem(strlex)) == LexemInteger)
								frames.back().right = dectos32(strlex);
							else
								break;

							if((lexem = lexer.getLexem(strlex)) == LexemInteger)
								frames.back().bottom = dectos32(strlex);
							else
								break;

							++itr->count;
						}
						else if("sprite" == string(strlex))
						{
							if((lexem = lexer.getLexem(strlex)) == LexemString)
								itr = sprites.insert(string(strlex), Sprite(frames.size(), 0));
							else
								break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
	}
	else
	{
		setLastError(OperationFailed);
		result = false;
	}

	if(lexem != LexemFinished)
	{
		setLastError(OperationFailed);
		result = false;
	}
	else
	{
		u32 texture = ilGenImage();
		ilBindImage(texture);
		ILboolean e = ilLoadImage(texture_path.c_str());
		if(e != IL_FALSE)
		{
			u32 width = ilGetInteger(IL_IMAGE_WIDTH);
			u32 height = ilGetInteger(IL_IMAGE_HEIGHT);
			u32 bytes_per_pixel = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
			texture_size = width;
			if(width == height)
			{
				if(bytes_per_pixel == 4)
				{
					u8x4* pixels = (u8x4*)ilGetData();
					for(u32 i = 0, s = width * height; i < s; ++i)
					{
						if(red_channel.size() == 0 || red_channel.back().count == 255)
						{
							red_channel.push_back_def();
							red_channel.back().count = 0;
							red_channel.back().value = pixels[i].x;
						}
						else
						{
							if(red_channel.back().value == pixels[i].x)
								++red_channel.back().count;
							else
							{
								red_channel.push_back_def();
								red_channel.back().count = 0;
								red_channel.back().value = pixels[i].x;
							}
						}

						if(green_channel.size() == 0 || green_channel.back().count == 255)
						{
							green_channel.push_back_def();
							green_channel.back().count = 0;
							green_channel.back().value = pixels[i].y;
						}
						else
						{
							if(green_channel.back().value == pixels[i].y)
								++green_channel.back().count;
							else
							{
								green_channel.push_back_def();
								green_channel.back().count = 0;
								green_channel.back().value = pixels[i].y;
							}
						}

						if(blue_channel.size() == 0 || blue_channel.back().count == 255)
						{
							blue_channel.push_back_def();
							blue_channel.back().count = 0;
							blue_channel.back().value = pixels[i].z;
						}
						else
						{
							if(blue_channel.back().value == pixels[i].z)
								++blue_channel.back().count;
							else
							{
								blue_channel.push_back_def();
								blue_channel.back().count = 0;
								blue_channel.back().value = pixels[i].z;
							}
						}

						if(alpha_channel.size() == 0 || alpha_channel.back().count == 255)
						{
							alpha_channel.push_back_def();
							alpha_channel.back().count = 0;
							alpha_channel.back().value = pixels[i].w;
						}
						else
						{
							if(alpha_channel.back().value == pixels[i].w)
								++alpha_channel.back().count;
							else
							{
								alpha_channel.push_back_def();
								alpha_channel.back().count = 0;
								alpha_channel.back().value = pixels[i].w;
							}
						}
					}
				}
				else
				{
					setLastError(OperationFailed);
					result = false;
				}
			}
			else
			{
				setLastError(OperationFailed);
				result = false;
			}
		}
		else
		{
			setLastError(OperationFailed);
			result = false;
		}
		ilDeleteImage(texture);
	}

	if(result)
	{
		if(out_stream == nullptr)
		{
			file_stream.open(name + ".bin", StreamModeWrite, FileModeCreateAlways);
			out_stream = &file_stream;
		}

		if(out_stream->writeable())
		{
			//	1. set id "sparrow::SpriteSet"
			serialize(string("sparrow::SpriteSet"), out_stream);
			//	2. set name <string>
			serialize(name, out_stream);
			//	3. sprites <vector_map>
			serialize(sprites, out_stream); 
			//	4. frames <vector>
			serialize(frames, out_stream); 
			//	5. texture <teture_size/RLE compressed channels>
			serialize(texture_size, out_stream); 
			serialize(red_channel, out_stream); 
			serialize(green_channel, out_stream); 
			serialize(blue_channel, out_stream); 
			serialize(alpha_channel, out_stream);
		}
		else
		{
			setLastError(OperationFailed);
			result = false;
		}
	}

	return result;
}

} //namespace tool
