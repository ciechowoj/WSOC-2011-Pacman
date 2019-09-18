#pragma once
#include "library\library.hpp"
namespace sparrow {

class PixmapFontDesc
{
public:
	sparrow::string		name;
	sparrow::string		face;
	sparrow::u32		size;
	bool				bold;
	bool				italic;
	bool				outline;
	bool				smooth;
	bool create(DataStream* stream) const;
};

class SpriteSetDesc
{
public:
	string				name;
	string				path;
	bool create(DataStream* out_stream) const;
};

} //namespace tool
