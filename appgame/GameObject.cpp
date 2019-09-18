#include "GameObject.hpp"
namespace sparrow {

const char* SaveGameFile = "svg.bin";
const char* SaveGameTemp = "svg.tmp";

GameObject::GameObject(Framework* framework, Context2D* context2d)
	: _framework(framework)
	, _context2d(context2d)
	, _spriteSet(context2d)
	, _menuFont(context2d)
	, _smallFont(context2d)
{ 
	_thisStage = &_mainMenu;
	_nextStage = &_mainMenu;
	_spriteSet.deserialize(&FileStream("pacman.bin", StreamModeRead, FileModeOpenAlways));
	_menuFont.deserialize(&FileStream("menu.bin", StreamModeRead, FileModeOpenAlways));
	_smallFont.deserialize(&FileStream("game.bin", StreamModeRead, FileModeOpenAlways));

	_mainMenu.initStages(&_levelStage, &_loadStage, &_highScores, &_options);
	_mainMenu.initResources(&_menuFont, &_smallFont);

	_levelStage.initStages(&_mainMenu, &_highScores);
	_levelStage.initResources(&_menuFont, &_smallFont, &_spriteSet);

	_highScores.initStages(&_mainMenu);
	_highScores.initResources(&_menuFont, &_smallFont);

	_loadStage.initStages(&_mainMenu, &_levelStage);
	_loadStage.initResources(&_menuFont, &_smallFont);

	_options.initStages(&_mainMenu);
	_options.initResources(&_menuFont, &_smallFont);
}

GameObject::~GameObject()
{ }

void GameObject::onCursorMove(s32x2 absolute, s32x2 relative)
{ }

void GameObject::onCursorDrag(s32x2 absolute, s32x2 relative)
{ }

void GameObject::onCursorDown(s32x2 absolute, CursorEvent cursorEvent)
{ }

void GameObject::onCursorUp(s32x2 absolute, CursorEvent cursorEvent)
{ }

void GameObject::onPageScroll(s32 delta)
{ }

void GameObject::onKeyDown(u32 id)
{ 
	switch(id)
	{
	case 'W':
		_nextStage = _thisStage->onInputEvent(InputEventUp2);
		break;
	case 'A':
		_nextStage = _thisStage->onInputEvent(InputEventLeft2);
		break;
	case 'S':
		_nextStage = _thisStage->onInputEvent(InputEventDown2);
		break;
	case 'D':
		_nextStage = _thisStage->onInputEvent(InputEventRight2);
		break;
	case KeyIDArrowUp:
		_nextStage = _thisStage->onInputEvent(InputEventUp1);
		break;
	case KeyIDArrowLeft:
		_nextStage = _thisStage->onInputEvent(InputEventLeft1);
		break;
	case KeyIDArrowDown:
		_nextStage = _thisStage->onInputEvent(InputEventDown1);
		break;
	case KeyIDArrowRight:
		_nextStage = _thisStage->onInputEvent(InputEventRight1);
		break;
	case KeyIDEnter:
		_nextStage = _thisStage->onInputEvent(InputEventEnter);
		break;
	case KeyIDEscape:
		_nextStage = _thisStage->onInputEvent(InputEventEscape);
	}
}

void GameObject::onKeyUp(u32 id)
{ }

void GameObject::onCharacter(c16 charCode, bool repeated)
{ 
	_nextStage = _thisStage->onInputEvent(InputEventChar, charCode);
}

void GameObject::onFramework(FrameworkEvent frameworkEvent)
{ }

void GameObject::main()
{
	glViewport(0, 0, _framework->getSize().x, _framework->getSize().y);
	glClearColor(0.f, 0.f, 0.f, 0.f);
	f64 delta_time = 0, old_time = timef64(), current_time = timef64();
	const f64 step_time = 0.0166666666;
	bool work = true;
	while(work)
	{
		_framework->processEvents(this);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		
		if(_thisStage != _nextStage)
		{
			_thisStage->onPause();
			_thisStage->onResume();
			_thisStage = _nextStage;
		}

		if(_thisStage != nullptr)
		{ 
			current_time = timef64();
			delta_time += current_time - old_time;
			while(delta_time > step_time)
			{
				_thisStage->onUpdate(u32(step_time * 1000), u32(current_time * 1000));
				delta_time -= step_time;
			}
			old_time = current_time;
			_thisStage->onRedraw(_context2d);
		}
		else
		{
			work = false;
		}

		_context2d->flush();
		_context2d->render();

		glDisable(GL_TEXTURE_2D);
		glFlush();
		_framework->swapBuffers();
	}

}

} // namespace sparrow
