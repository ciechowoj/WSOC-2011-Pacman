#include "..\library\library.hpp"
#include "..\engine\Context2D.hpp"
#include "..\engine\Framework.hpp"
#include "GameObject.hpp"
#pragma comment(lib, "library")
#pragma comment(lib, "engine")
#pragma comment(lib, "OpenGL32")
using namespace sparrow;

int main(int argc, char** argv)
{
	sparrow::Framework framework(false, u32x2(IGameStage::StageWidth, IGameStage::StageHeight));
	glInitLibrary();
	Context2D context2d(&framework);

	GameObject gameObject(&framework, &context2d);
	gameObject.main();
		
	glQuitLibrary();

	return 0;
}
