#pragma once
#include "IGameStage.hpp"
#include "Pacman.hpp"
#include "Wall.hpp"
#include "Ghost.hpp"
#include "Point.hpp"
namespace sparrow {

struct GameState;
class MainMenu;
class HighScores;

class LevelStage : public IGameStage
{
public:
						LevelStage();
	virtual				~LevelStage();

	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code);
	virtual	void		onUpdate(u32 delta_time, u32 current_time);
	virtual void		onRedraw(Context2D* context2d);
	virtual void		onPause();
	virtual void		onResume();

	bool				isActive() const;
	void				loadGame(const GameState* game_state = nullptr);
	void				saveGame(GameState* game_state);
	void				initStages(MainMenu* main_menu, HighScores* high_scores);
	void				initResources(PixmapFont* menu_font, PixmapFont* small_font, SpriteSet* sprite_set);
private:
	void				_clear();

	vector<Object*>		_staticObjects;
	vector<Object*>		_dynamicObjects;
	Pacman*				_pacman1;
	Pacman*				_pacman2;

	PixmapFont*			_menuFont;
	PixmapFont*			_smallFont;
	SpriteSet*			_spriteSet;
	MainMenu*			_mainMenu;
	HighScores*			_highScores;
};


} // namespace sparrow
