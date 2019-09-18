#pragma once
#include "MainMenu.hpp"
#include "LevelStage.hpp"
#include "LoadGame.hpp"
#include "HighScores.hpp"
#include "Options.hpp"
namespace sparrow {

class GameObject : public IFrameworkEventHandler
{
public:
						GameObject(Framework* framework, Context2D* context2d);
	virtual				~GameObject();
	virtual void		onCursorMove(s32x2 absolute, s32x2 relative);
	virtual void		onCursorDrag(s32x2 absolute, s32x2 relative);
	virtual void		onCursorDown(s32x2 absolute, CursorEvent cursorEvent);
	virtual void		onCursorUp(s32x2 absolute, CursorEvent cursorEvent);

	virtual void		onPageScroll(s32 delta);
	virtual void		onKeyDown(u32 id);
	virtual void		onKeyUp(u32 id);
	virtual void		onCharacter(c16 charCode, bool repeated);
	virtual void		onFramework(FrameworkEvent frameworkEvent);

	void				main();
private:
	Framework*			_framework;
	Context2D*			_context2d;

	MainMenu			_mainMenu;
	LevelStage			_levelStage;
	LoadGame			_loadStage;
	HighScores			_highScores;
	Options				_options;

	IGameStage*			_thisStage;
	IGameStage*			_nextStage;

	SpriteSet			_spriteSet;
	PixmapFont			_menuFont;
	PixmapFont			_smallFont;
};

} // namespace sparrow
