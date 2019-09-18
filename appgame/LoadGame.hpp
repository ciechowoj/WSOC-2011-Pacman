#pragma once
#include "IGameStage.hpp"
namespace sparrow
{

class MainMenu;
class LevelStage;

struct GameState
{
						GameState();

	void				serialize(DataStream* stream) const;
	void				deserialize(DataStream* stream);

	string				_player1Name;
	string				_player2Name;
	u32					_player1Score;
	u32					_player2Score;
	u32					_player1Lives;
	u32					_player2Lives;
	GameMode			_mode;
};

class LoadGame : public IGameStage
{
public:
						LoadGame();
	virtual				~LoadGame();

	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code);
	virtual void		onUpdate(u32 delta_time, u32 current_time);
	virtual void		onRedraw(Context2D* context2d);
	virtual void		onPause();
	virtual void		onResume();

	void				initStages(MainMenu* main_menu, LevelStage* level_stage);
	void				initResources(PixmapFont* menu_font, PixmapFont* small_font);
	void				setLoadState();
	void				setSaveState();
private:
	struct _Entry { string caption; GameState state; };
	string				_titleSave;
	string				_titleLoad;
	_Entry				_savedGames[10];
	u32					_position;
	bool				_stageMode;

	PixmapFont*			_menuFont;
	PixmapFont*			_smallFont;
	MainMenu*			_mainMenu;
	LevelStage*			_levelStage;
};

} // namespace sparrow
