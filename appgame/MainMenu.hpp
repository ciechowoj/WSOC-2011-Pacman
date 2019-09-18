#pragma once
#include "IGameStage.hpp"
namespace sparrow {

class LevelStage;
class LoadGame;
class HighScores;
class Options;

class MainMenu : public IGameStage
{
public:
						MainMenu();
	virtual				~MainMenu();

	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code);
	virtual void		onUpdate(u32 delta_time, u32 current_time);
	virtual void		onRedraw(Context2D* context2d);
	virtual void		onPause();
	virtual void		onResume();

	void				initStages(LevelStage* level_stage, LoadGame* load_game, HighScores* high_scores, Options* options);
	void				initResources(PixmapFont* menu_font, PixmapFont* small_font);
private:
	string				_firstMenu;
	string				_secondMenu;
	u32					_position;
	pair<u16, u16>		_firstSelect[5];
	pair<u16, u16>		_secondSelect[7];
	PixmapFont*			_menuFont;
	PixmapFont*			_smallFont;
	LevelStage*			_levelStage;
	LoadGame*			_loadGame;
	HighScores*			_highScores;
	Options*			_options;
};

} // namespace sparrow
