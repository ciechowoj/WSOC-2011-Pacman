#include "GameEditor.hpp"
namespace sparrow {

GameEditor::GameEditor(Framework* framework, Context2D* context2d)
	: _gameFont(context2d)
	, _spriteSet(context2d)
	, _context2d(context2d)
	, _framework(framework)
	, _chosenObject(LevelObjectPoint)
	, _buttonHint("<hint>")
	, _player1Button(0, 0, 48, 48)
	, _player2Button(48, 0, 96, 48)
	, _solidWallButton(96, 0, 144, 48)
	, _opaqueWallButton(144, 0, 192, 48)
	, _ghostButton(192, 0, 240, 48)
	, _pointButton(240, 0, 288, 48)
	, _createButton(584, 8, 616, 48)
	, _deleteButton(632, 8, 664, 48)
	, _renameButton(680, 8, 712, 48)
	, _moveButton(728, 8, 760, 48)
	, _insertButton(776, 8, 808, 48)
	, _nextButton(824, 8, 856, 48)
	, _prevButton(872, 8, 904, 48)
{
	_gameFont.deserialize(&FileStream("game.bin"));
	_spriteSet.deserialize(&FileStream("pacman.bin"));
	FileStream stream("level.bin");

	if(stream.opened())
		deserialize(_gameLevels, &stream);
	else
		_gameLevels.push_back_def();
}

GameEditor::~GameEditor()
{ }


void GameEditor::onCursorMove(s32x2 absolute, s32x2 relative)
{


}

void GameEditor::onCursorDrag(s32x2 absolute, s32x2 relative)
{

}

void GameEditor::onCursorDown(s32x2 absolute, CursorEvent cursorEvent)
{



}

void GameEditor::onCursorUp(s32x2 absolute, CursorEvent cursorEvent)
{



}

void GameEditor::onPageScroll(s32 delta)
{



}

void GameEditor::onKeyDown(u32 id)
{



}

void GameEditor::onKeyUp(u32 id)
{



}

void GameEditor::onCharacter(c16 charCode, bool repeated)
{


}

void GameEditor::onFramework(FrameworkEvent frameworkEvent)
{


}

void GameEditor::main()
{


}


void GameEditor::_redraw()
{
	SpriteDesc sprite_desc;
	sprite_desc.spriteSet = &_spriteSet;
	sprite_desc.zpos = 0.1f;
	sprite_desc.frame = 0;

	// draw player1 button
	sprite_desc.position = _player1Button.leftTop();
	sprite_desc.sprite = _spriteSet.getSpriteHandle("pacman");
	_context2d->drawSprite(sprite_desc);




	_context2d->flush();
	_context2d->render();
}

} // namespace sparrow
