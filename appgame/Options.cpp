#include "Options.hpp"
#include "MainMenu.hpp"
namespace sparrow {

Options::Options()
	: _position(0)
	, _player1Name("<unknown>")
	, _player2Name("<unknown>")
	, _volume(50)
	, _fullscreen(false)
	, _gameMode(GameModeSingle)
	, _title("Options")
	, _captions("Volume:\nFullscreen:\nPlayer 1 name:\nPlayer 2 name:\nGame mode:")
	, _single("Single Player")
	, _multi("MultiPlayer")
	, _tcpip("TCP/IP")
	, _yes("yes")
	, _no("no")
	, _menuFont(nullptr)
	, _smallFont(nullptr)
	, _mainMenu(nullptr)
{ 
	
}

Options::~Options()
{ }

IGameStage* Options::onInputEvent(InputEvent input_event, c16 char_code)
{
	const u32 max_position = 4;
	IGameStage* next_stage = this;

	switch(input_event)
	{
	case InputEventLeft1:
		switch(_position)
		{
		case 0:
			_volume = max(0, _volume - 1);
			break;
		case 1:
			_fullscreen = _fullscreen ? false : true;
			break;
		case 4:
			switch(_gameMode)
			{
			case GameModeSingle: _gameMode = GameModeMulti; break;
			case GameModeMulti: _gameMode = GameModeTCPIP; break;
			case GameModeTCPIP:_gameMode = GameModeSingle; break;
			}
			break;
		}
		break;
	case InputEventUp1:
		_position = _position == 0 ? max_position : --_position;
		break;
	case InputEventRight1:
		switch(_position)
		{
		case 0:
			_volume = min(100, _volume + 1);
			break;
		case 1:
			_fullscreen = _fullscreen ? false : true;
			break;
		case 4:
			switch(_gameMode)
			{
			case GameModeSingle: _gameMode = GameModeTCPIP; break;
			case GameModeMulti: _gameMode = GameModeSingle; break;
			case GameModeTCPIP:_gameMode = GameModeMulti; break;
			}
			break;
		}
		break;
	case InputEventDown1:
		_position = _position == max_position ? 0 : ++_position;
		break;
	case InputEventChar:
		if((_position == 2 || _position == 3) && (isalpha(char_code) || char_code == ' '))
		{
			string* dest = _position == 2 ? &_player1Name : &_player2Name;
			if(dest->size() < 20)
			{
				if(*dest == "<unknown>")
					*dest = string();
				wchar_t str[2] = { 0, 0 };
				str[0] = wchar_t(char_code);
				*dest += str;
			}
		}
		else if((_position == 2 || _position == 3) && char_code == 0x08)
		{
			string* dest = _position == 2 ? &_player1Name : &_player2Name;
			if(*dest == "<unknown>")
				*dest = string();
			if(dest->size() != 0)
				*dest = string(dest->begin(), dest->end() - 1);
		}
		break;
	case InputEventEscape:
		next_stage = _mainMenu;
		break;
	}
	return next_stage;
}

void Options::onUpdate(u32 delta_time, u32 current_time)
{ }

void Options::onRedraw(Context2D* context2d)
{
	TextBoxDesc textBoxDesc;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight * 2, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	textBoxDesc.font = _menuFont;
	textBoxDesc.firstColor = u8x4(0xff, 0xd8, 0x00, 0xff);
	textBoxDesc.secondColor = u8x4(0x00, 0x13, 0x7f, 0xff);

	string text;
	pair<u32, u32> offsets[5];
	u32 index, diff;
	string::iterator itr, end;

	offsets[index = 0].first = 0;
	for(itr = _captions.begin(), end = _captions.end(); itr < end; ++itr)
	{
		if(*itr == '\n')
		{
			diff = itr - _captions.begin();
			offsets[index].second = diff;
			offsets[++index].first = diff + 1;
		}
	}
	diff = itr - _captions.begin();
	offsets[index].second = _captions.size();

	textBoxDesc.text = _captions;
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignLeft;
	textBoxDesc.textCursorMode = TextCursorModeHighlight;
	textBoxDesc.selectionBegin = offsets[_position].first;
	textBoxDesc.selectionEnd = offsets[_position].second;
	context2d->drawTextBox(textBoxDesc);

	offsets[0].first = text.size();
	text += s32tostr(_volume) + "%\n";
	offsets[0].second = text.size();

	offsets[1].first = text.size();
	text += (_fullscreen ? _yes : _no) + "\n";
	offsets[1].second = text.size();

	offsets[2].first = text.size();
	text += _player1Name + "\n";
	offsets[2].second = text.size();

	offsets[3].first = text.size();
	text += _player2Name + "\n";
	offsets[3].second = text.size();

	offsets[4].first = text.size();
	switch(_gameMode)
	{
	case GameModeNone: 
		text += "<unknown>\n";
		break;
	case GameModeSingle: 
		text += _single + "\n";
		break;
	case GameModeMulti: 
		text += _multi + "\n";
		break;
	case GameModeTCPIP: 
		text +=  _tcpip + "\n";
		break;
	}
	offsets[4].second = text.size();

	textBoxDesc.text = text;
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignRight;
	textBoxDesc.selectionBegin = offsets[_position].first;
	textBoxDesc.selectionEnd = offsets[_position].second;
	context2d->drawTextBox(textBoxDesc);

	textBoxDesc.text = _title;
	textBoxDesc.verticalAlign = VerticalAlignTop;
	textBoxDesc.horizontalAlign = HorizontalAlignCenter;
	textBoxDesc.textCursorMode = TextCursorModeNone;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	context2d->drawTextBox(textBoxDesc);
}

void Options::onPause()
{ 
	if(_player1Name.size() == 0)
		_player1Name = "<unknown>";
	if(_player2Name.size() == 0)
		_player2Name = "<unknown>";

	FileStream stream(SaveGameFile, StreamModeBoth, FileModeOpenAlways);
	if(stream.opened())
	{
		u32 options_offset = 0;
		stream.read(&options_offset, sizeof options_offset);
		if(options_offset == 0)
		{
			options_offset = stream.size();
			stream.seek(sizeof u32 * 0);
			stream.write(&options_offset, sizeof options_offset);
		}
		stream.seek(options_offset);
	}
	else
	{
		u32 zero_offset = 0;
		u32 options_offset = sizeof u32 * 3;
		stream.open(SaveGameFile, StreamModeBoth, FileModeCreateAlways);
		stream.write(&options_offset, sizeof options_offset);
		stream.write(&zero_offset, sizeof zero_offset);
		stream.write(&zero_offset, sizeof zero_offset);
	}

	serialize(_player1Name, &stream);
	serialize(_player2Name, &stream);
	sparrow::serialize(_volume, &stream);
	sparrow::serialize(_fullscreen, &stream);
	stream.write(&_gameMode, sizeof _gameMode);
}

void Options::onResume()
{
	_position = 0;
}

void Options::initStages(MainMenu* main_menu)
{
	_mainMenu = main_menu;
}

void Options::initResources(PixmapFont* menu_font, PixmapFont* small_font)
{
	_menuFont = menu_font;
	_smallFont = small_font;
}

} // namespace sparrow
