#pragma once
#include "GameLevel.hpp"
namespace sparrow {

class GameEditor : public IFrameworkEventHandler
{
public:
						GameEditor(Framework* framework, Context2D* context2d);
						~GameEditor();

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
	void				_levelLMB(u32 x, u32 y);
	void				_levelRMB(u32 x, u32 y);

	void				_redraw();

	PixmapFont			_gameFont;
	SpriteSet			_spriteSet;
	Context2D*			_context2d;
	Framework*			_framework;
	vector<GameLevel>	_gameLevels;
	LevelObject			_chosenObject;
	u32					_currentLevel;
	string				_buttonHint;
	const AABRi			_player1Button;
	const AABRi			_player2Button;
	const AABRi			_solidWallButton;
	const AABRi			_opaqueWallButton;
	const AABRi			_ghostButton;
	const AABRi			_pointButton;
	const AABRi			_createButton;
	const AABRi			_deleteButton;
	const AABRi			_renameButton;
	const AABRi			_moveButton;
	const AABRi			_insertButton;
	const AABRi			_nextButton;
	const AABRi			_prevButton;
};

} // namespace sparrow
