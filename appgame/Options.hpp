#pragma once
#include "IGameStage.hpp"
namespace sparrow {

class MainMenu;

class Options : public IGameStage
{
public:
						Options();
	virtual				~Options();

	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code);
	virtual void		onUpdate(u32 delta_time, u32 current_time);
	virtual void		onRedraw(Context2D* context2d);
	virtual void		onPause();
	virtual void		onResume();

	void				initStages(MainMenu* main_menu);
	void				initResources(PixmapFont* menu_font, PixmapFont* small_font);

	const string&		getPlayer1Name() const;
	const string&		getPlayer2Name() const;
	u32					getVolume() const;
	bool				getFullscreen() const;
	GameMode			getGameMode() const;
private:
	u32					_position;
	string				_player1Name;
	string				_player2Name;
	s32					_volume;
	bool				_fullscreen;
	GameMode			_gameMode;

	string				_title;
	string				_captions;
	string				_single;
	string				_multi;
	string				_tcpip;
	string				_yes;
	string				_no;

	PixmapFont*			_menuFont;
	PixmapFont*			_smallFont;
	MainMenu*			_mainMenu;

};

inline const string& Options::getPlayer1Name() const
{
	return _player1Name;
}

inline const string& Options::getPlayer2Name() const
{
	return _player2Name;
}

inline u32 Options::getVolume() const
{
	return _volume;
}

inline bool Options::getFullscreen() const
{
	return _fullscreen;
}

inline GameMode Options::getGameMode() const
{
	return _gameMode;
}

} // namespace sparrow
