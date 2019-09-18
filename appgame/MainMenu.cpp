#include "MainMenu.hpp"
#include "LevelStage.hpp"
#include "LoadGame.hpp"
#include "HighScores.hpp"
#include "Options.hpp"
namespace sparrow {

MainMenu::MainMenu()
	: _firstMenu("New Game\nLoad Game\nHigh Scores\nOptions\nExit")
	, _secondMenu("New Game\nLoad Game\nSave Game\nHigh Scores\nOptions\nResume\nExit")
	, _position(0)
	, _levelStage(nullptr)
	, _loadGame(nullptr)
	, _highScores(nullptr)
	, _options(nullptr)
{ 
	u32 index, diff;
	string::iterator itr, end;

	_firstSelect[index = 0].first = 0;
	for(itr = _firstMenu.begin(), end = _firstMenu.end(); itr < end; ++itr)
	{
		if(*itr == '\n')
		{
			diff = itr - _firstMenu.begin();
			_firstSelect[index].second = diff;
			_firstSelect[++index].first = diff + 1;
		}
	}
	diff = itr - _firstMenu.begin();
	_firstSelect[index].second = _firstMenu.size();

	_secondSelect[index = 0].first = 0;
	for(itr = _secondMenu.begin(), end = _secondMenu.end(); itr < end; ++itr)
	{
		if(*itr == '\n')
		{
			diff = itr - _secondMenu.begin();
			_secondSelect[index].second = diff;
			_secondSelect[++index].first = diff + 1;
		}
	}
	diff = itr - _secondMenu.begin();
	_secondSelect[index].second = _secondMenu.size();
}

MainMenu::~MainMenu()
{ }

IGameStage* MainMenu::onInputEvent(InputEvent input_event, c16 char_code)
{
	const u32 max_position = _levelStage->isActive() ? 6 : 4;
	IGameStage* next_stage = this;

	switch(input_event)
	{
	case InputEventLeft1:
	case InputEventLeft2:
		break;
	case InputEventUp1:
	case InputEventUp2:
		_position = _position == 0 ? max_position : --_position;
		break;
	case InputEventRight1:
	case InputEventRight2:
		break;
	case InputEventDown1:
	case InputEventDown2:
		_position = _position == max_position ? 0 : ++_position;
		break;
	case InputEventChar:
		break;
	case InputEventEnter:
		if(max_position == 4)
		{
			switch(_position)
			{
			case 0:
				_levelStage->loadGame();
				next_stage = _levelStage;
				break;
			case 1:
				_loadGame->setLoadState();
				next_stage = _loadGame;
				break;
			case 2:
				next_stage = _highScores;
				break;
			case 3:
				next_stage = _options;
				break;
			case 4:
				next_stage = nullptr;
				break;
			}
		}
		else
		{
			switch(_position)
			{
			case 0:
				_levelStage->loadGame();
				next_stage = _levelStage;
				break;
			case 1:
				_loadGame->setLoadState();
				next_stage = _loadGame;
				break;
			case 2:
				_loadGame->setSaveState();
				next_stage = _loadGame;
				break;
			case 3:
				next_stage = _highScores;
				break;
			case 4:
				next_stage = _options;
				break;
			case 5:
				next_stage = _levelStage;
				break;
			case 6:
				next_stage = nullptr; 
				break;
			}
		}
		break;
	case InputEventEscape:
		_position = max_position;
		break;
	}
	return next_stage;
}

void MainMenu::onUpdate(u32 delta_time, u32 current_time)
{ }

void MainMenu::onRedraw(Context2D* context2d)
{
	TextBoxDesc textBoxDesc;
	textBoxDesc.aabr = AABRs(0, 0, IGameStage::StageWidth, IGameStage::StageHeight);
	textBoxDesc.text = _levelStage->isActive() ? _secondMenu : _firstMenu;
	textBoxDesc.font = _menuFont;
	textBoxDesc.firstColor = u8x4(0xff, 0xd8, 0x00, 0xff);
	textBoxDesc.secondColor = u8x4(0x00, 0x13, 0x7f, 0xff);
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignCenter;
	textBoxDesc.textCursorMode = TextCursorModeHighlight;

	if(_levelStage->isActive())
	{
		textBoxDesc.selectionBegin = _secondSelect[_position].first;
		textBoxDesc.selectionEnd = _secondSelect[_position].second;
	}
	else
	{
		textBoxDesc.selectionBegin = _firstSelect[_position].first;
		textBoxDesc.selectionEnd = _firstSelect[_position].second;
	}

	context2d->drawTextBox(textBoxDesc);
}

void MainMenu::onPause()
{ }

void MainMenu::onResume()
{
	_position = _levelStage->isActive() ? 5 : 0;
}

void MainMenu::initStages(LevelStage* level_stage, LoadGame* load_game, HighScores* high_scores, Options* options)
{
	_levelStage = level_stage;
	_loadGame = load_game;
	_highScores = high_scores;
	_options = options;
}

void MainMenu::initResources(PixmapFont* menu_font, PixmapFont* small_font)
{
	_menuFont = menu_font;
	_smallFont = small_font;
}

} // namespace sparrow
