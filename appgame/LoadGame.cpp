#include "LoadGame.hpp"
#include "MainMenu.hpp"
#include "LevelStage.hpp"
namespace sparrow {

GameState::GameState()
	: _player1Score(0)
	, _player2Score(0)
	, _player1Lives(0)
	, _player2Lives(0)
	, _mode(GameModeNone)
{ }

void GameState::serialize(DataStream* stream) const
{
	sparrow::serialize(_player1Name, stream);
	sparrow::serialize(_player2Name, stream);
	sparrow::serialize(_player1Score, stream);
	sparrow::serialize(_player2Score, stream);
	sparrow::serialize(_player1Lives, stream);
	sparrow::serialize(_player2Lives, stream);
	stream->write(&_mode, sizeof _mode);
}

void GameState::deserialize(DataStream* stream)
{
	sparrow::deserialize(_player1Name, stream);
	sparrow::deserialize(_player2Name, stream);
	sparrow::deserialize(_player1Score, stream);
	sparrow::deserialize(_player2Score, stream);
	sparrow::deserialize(_player1Lives, stream);
	sparrow::deserialize(_player2Lives, stream);
	stream->read(&_mode, sizeof _mode);
}

LoadGame::LoadGame()
	: _titleSave("Save Game")
	, _titleLoad("Load Game")
	, _position(0)
	, _stageMode(true) // true == load
{
	const string empty_slot = "<empty slot>";
	for(u32 i = 0; i < 10; ++i)
		_savedGames[i].caption = empty_slot;
}

LoadGame::~LoadGame()
{ }

IGameStage* LoadGame::onInputEvent(InputEvent input_event, c16 char_code)
{
	const u32 max_position = 10;
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
		if(_stageMode) //load game
		{
			_levelStage->loadGame(&_savedGames[_position].state);
			next_stage = _levelStage;
		}
		else
		{
			_levelStage->saveGame(&_savedGames[_position].state);
			next_stage = _levelStage;
		}
		break;
	case InputEventEscape:
		next_stage = _mainMenu;
		break;
	}
	return next_stage;
}

void LoadGame::onUpdate(u32 delta_time, u32 current_time)
{ }

void LoadGame::onRedraw(Context2D* context2d)
{ 
	TextBoxDesc textBoxDesc;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight * 2, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	textBoxDesc.font = _menuFont;
	textBoxDesc.firstColor = u8x4(0xff, 0xd8, 0x00, 0xff);
	textBoxDesc.secondColor = u8x4(0x00, 0x13, 0x7f, 0xff);

	_Entry *itr, *end;
	pair<u32, u32> offsets[10];
	u32 index;
	string text;

	for(itr = _savedGames, end = _savedGames + 10, index = 0; itr < end; ++itr, ++index)
	{
		offsets[index].first = text.size();
		text += itr->caption + "\n";
		offsets[index].second = text.size();
	}

	textBoxDesc.text = text;
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignLeft;
	textBoxDesc.textCursorMode = TextCursorModeHighlight;
	textBoxDesc.selectionBegin = offsets[_position].first;
	textBoxDesc.selectionEnd = offsets[_position].second;
	context2d->drawTextBox(textBoxDesc);

	textBoxDesc.text = _stageMode ? _titleLoad : _titleSave;
	textBoxDesc.verticalAlign = VerticalAlignTop;
	textBoxDesc.horizontalAlign = HorizontalAlignCenter;
	textBoxDesc.textCursorMode = TextCursorModeNone;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	context2d->drawTextBox(textBoxDesc);
}

void LoadGame::onPause()
{ 
	if(!_stageMode)
	{
		FileStream stream(SaveGameFile, StreamModeBoth, FileModeOpenAlways);
		if(stream.opened())
		{
			u32 save_offset = 0;
			stream.seek(sizeof u32 * 2);
			stream.read(&save_offset, sizeof save_offset);
			if(save_offset == 0)
			{
				save_offset = stream.size();
				stream.seek(sizeof u32 * 2);
				stream.write(&save_offset, sizeof save_offset);
			}
			stream.seek(save_offset);
		}
		else
		{
			u32 zero_offset = 0;
			u32 save_offset = sizeof u32 * 3;
			stream.open(SaveGameFile, StreamModeBoth, FileModeCreateAlways);
			stream.write(&zero_offset, sizeof zero_offset);
			stream.write(&zero_offset, sizeof zero_offset);
			stream.write(&save_offset, sizeof save_offset);
		}

		for(u32 i = 0; i < 10; ++i)
		{
			serialize(_savedGames[i].caption, &stream);
			_savedGames[i].state.serialize(&stream);
		}
	}
}

void LoadGame::onResume()
{ 
	_position = 0;
}

void LoadGame::initStages(MainMenu* main_menu, LevelStage* level_stage)
{
	_mainMenu = main_menu;
	_levelStage = level_stage;
}

void LoadGame::initResources(PixmapFont* menu_font, PixmapFont* small_font)
{
	_menuFont = menu_font;
	_smallFont = small_font;
}

void LoadGame::setLoadState()
{
	_stageMode = true;
}

void LoadGame::setSaveState()
{
	_stageMode = false;
}

} // namespace sparrow
