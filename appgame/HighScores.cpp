#include "HighScores.hpp"
#include "MainMenu.hpp"
namespace sparrow {

HighScores::HighScores()
	: _title("High Scores")
	, _menuFont(nullptr)
	, _smallFont(nullptr)
	, _mainMenu(nullptr)
{
	if(_scores.empty())
	{
		_scores.insert(_Score("Pepe", 100));
		_scores.insert(_Score("Mullins", 400));
		_scores.insert(_Score("Dagoth", 700));
		_scores.insert(_Score("Lara", 1300));
		_scores.insert(_Score("Denton", 2500));
		_scores.insert(_Score("Sam&Max", 3900));
		_scores.insert(_Score("Unnamed", 8400));
		_scores.insert(_Score("Ezio", 16100));
		_scores.insert(_Score("Geralt", 30900));
		_scores.insert(_Score("Sirius", 59300));
	}
}

HighScores::~HighScores()
{ }

IGameStage*	HighScores::onInputEvent(InputEvent input_event, c16 char_code)
{
	IGameStage* next_stage = this;

	switch(input_event)
	{
	case InputEventEscape:
		next_stage = _mainMenu;
		break;
	}

	return next_stage;
}

void HighScores::onUpdate(u32 delta_time, u32 current_time)
{ }

void HighScores::onRedraw(Context2D* context2d)
{
	runtime_assert(_scores.size() == 10);

	TextBoxDesc textBoxDesc;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight * 2, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	textBoxDesc.font = _menuFont;
	textBoxDesc.firstColor = u8x4(0xff, 0xd8, 0x00, 0xff);
	textBoxDesc.secondColor = u8x4(0x00, 0x13, 0x7f, 0xff);

	vector_set<_Score>::iterator itr, end;
	string text;

	for(itr = _scores.begin(), end = _scores.end(); itr < end; ++itr)
		text += itr->name + "\n";

	textBoxDesc.text = text;
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignLeft;
	context2d->drawTextBox(textBoxDesc);

	text = string();
	for(itr = _scores.begin(), end = _scores.end(); itr < end; ++itr)
		text += u32tostr(itr->value) + "\n";

	textBoxDesc.text = text;
	textBoxDesc.verticalAlign = VerticalAlignCenter;
	textBoxDesc.horizontalAlign = HorizontalAlignRight;
	context2d->drawTextBox(textBoxDesc);

	textBoxDesc.text = _title;
	textBoxDesc.verticalAlign = VerticalAlignTop;
	textBoxDesc.horizontalAlign = HorizontalAlignCenter;
	textBoxDesc.aabr = AABRs(IGameStage::TileWidth, IGameStage::TileHeight, IGameStage::StageWidth - IGameStage::TileWidth, IGameStage::StageHeight - IGameStage::TileHeight * 2);
	context2d->drawTextBox(textBoxDesc);
}

void HighScores::onPause()
{ 
	FileStream stream(SaveGameFile, StreamModeRead, FileModeOpenAlways);
	if(stream.opened())
	{
		u32 score_offset = 0;
		stream.seek(sizeof u32 * 1);
		stream.read(&score_offset, sizeof score_offset);
		if(score_offset == 0)
		{
			score_offset = stream.size();
			stream.seek(sizeof u32 * 1);
			stream.write(&score_offset, sizeof score_offset);
		}
		stream.seek(score_offset);
	}
	else
	{
		u32 zero_offset = 0;
		u32 score_offset = sizeof u32 * 3;
		stream.open(SaveGameFile, StreamModeWrite, FileModeCreateAlways);
		stream.write(&zero_offset, sizeof zero_offset);
		stream.write(&score_offset, sizeof score_offset);
		stream.write(&zero_offset, sizeof zero_offset);
	}

	serialize(_scores, &stream);
}

void HighScores::onResume()
{ }

void HighScores::initStages(MainMenu* main_menu)
{
	_mainMenu = main_menu;
}

void HighScores::initResources(PixmapFont* menu_font, PixmapFont* small_font)
{
	_menuFont = menu_font;
	_smallFont = small_font;
}

void HighScores::addScore(const string& name, u32 value)
{
	_scores.insert(_Score(name, value));
	_scores.erase(_scores.end());
}

} // namespace sparrow
