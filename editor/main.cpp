#include "Resource.hpp"
#include <IL\il.h>
using namespace sparrow;

#pragma comment(lib, "library")
#pragma comment(lib, "engine")
#pragma comment(lib, "OpenGL32")
#pragma comment(lib, "DevIL")

int main()
{
	ilInit();

	PixmapFontDesc fontDesc;
	fontDesc.name = "menu";
	fontDesc.face = "Arial";
	fontDesc.size = 48;
	fontDesc.italic = false;
	fontDesc.bold = false;
	fontDesc.outline = true;
	fontDesc.smooth = true;
	fontDesc.create(nullptr);

	fontDesc.name = "game";
	fontDesc.face = "Arial";
	fontDesc.size = 24;
	fontDesc.italic = false;
	fontDesc.bold = false;
	fontDesc.outline = true;
	fontDesc.smooth = true;
	fontDesc.create(nullptr);

	SpriteSetDesc spriteDesc;
	spriteDesc.name = "pacman";
	spriteDesc.path = "sprite_desc.txt";
	spriteDesc.create(nullptr);
	
	ilShutDown();
	return 0;
}