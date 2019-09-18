#include "LevelStage.hpp"
#include "MainMenu.hpp"
#include "HighScores.hpp"
namespace sparrow {

LevelStage::LevelStage()
	: _menuFont(nullptr)
	, _smallFont(nullptr)
	, _spriteSet(nullptr)
	, _mainMenu(nullptr)
	, _highScores(nullptr)
	, _pacman1(nullptr)
	, _pacman2(nullptr)
{ }

LevelStage::~LevelStage()
{
	_clear();
}


IGameStage* LevelStage::onInputEvent(InputEvent input_event, c16 char_code)
{
	IGameStage* next_stage = this;

	switch(input_event)
	{
	case InputEventLeft1:
		if(_pacman1 != nullptr) 
			_pacman1->moveLeft();
		break;
	case InputEventUp1:
		if(_pacman1 != nullptr) 
			_pacman1->moveUp(); 
		break;
	case InputEventRight1:
		if(_pacman1 != nullptr) 
			_pacman1->moveRight();
		break;
	case InputEventDown1:
		if(_pacman1 != nullptr) 
			_pacman1->moveDown();
		break;
	case InputEventLeft2:
		
		break;
	case InputEventUp2:
		
		break;
	case InputEventRight2:
		
		break;
	case InputEventDown2:
		
		break;
	case InputEventEscape:
		next_stage = _mainMenu;
		break;
	}
	return next_stage;
}

void LevelStage::onUpdate(u32 delta_time, u32 current_time)
{
	vector<Object*>::iterator itr1, itr2, end1, end2;
	s32x2 translation;

	for(itr1 = _staticObjects.begin(), end1 = _staticObjects.end(); itr1 < end1; ++itr1)
	{
		if((*itr1)->update())
		{
			delete *itr1;
			itr1 = _staticObjects.erase(itr1);
			end1 = _staticObjects.end();
		}
	}

	for(itr1 = _dynamicObjects.begin(), end1 = _dynamicObjects.end(); itr1 < end1; ++itr1)
	{
		if((*itr1)->update())
		{
			delete *itr1;
			itr1 = _dynamicObjects.erase(itr1);
			end1 = _dynamicObjects.end();
		}
	}

	for(itr1 = _staticObjects.begin(), end1 = _staticObjects.end(); itr1 < end1; ++itr1)
	{
		for(itr2 = _dynamicObjects.begin(), end2 = _dynamicObjects.end(); itr2 < end2; ++itr2)
		{
			translation = (*itr2)->getSpeed();
			if(aabrTestCollisionNrm((*itr1)->getAABR(), (*itr2)->getAABR(), translation))
			{
				
				(*itr1)->resolve(translation, (*itr2)->getType());
				(*itr2)->resolve(-translation, (*itr1)->getType());
			}
		}
	}

	translation = s32x2(0, 0);
	for(itr1 = _dynamicObjects.begin(), end1 = end2 = _dynamicObjects.end(); itr1 < end1; ++itr1)
	{
		for(itr2 = itr1 + 1; itr2 < end2; ++itr2)
		{
			if(aabrTestCollision((*itr1)->getAABR(), (*itr2)->getAABR()))
			{
				(*itr1)->resolve(translation, (*itr2)->getType());
				(*itr2)->resolve(-translation, (*itr1)->getType());
			}
		}
	}
}

void LevelStage::onRedraw(Context2D* context2d)
{
	vector<Object*>::iterator itr1, end1;
	for(itr1 = _staticObjects.begin(), end1 = _staticObjects.end(); itr1 < end1; ++itr1)
		(*itr1)->redraw(context2d);
	for(itr1 = _dynamicObjects.begin(), end1 = _dynamicObjects.end(); itr1 < end1; ++itr1)
		(*itr1)->redraw(context2d);

}

void LevelStage::onPause()
{


}

void LevelStage::onResume()
{ }

bool LevelStage::isActive() const
{
	return !_dynamicObjects.empty();
}

void LevelStage::loadGame(const GameState* game_state)
{ 
	_clear();
	_pacman1 = new Pacman(_spriteSet);
	_pacman1->init(MapObjectPacman2, u32x2(4, 4), _smallFont);
	_dynamicObjects.push_back(_pacman1);
	for(u32 i = 0; i < IGameStage::LevelWidth; ++i)
	{
		Wall* wall = new Wall(_spriteSet);
		wall->init(MapObjectWallBlue, s32x2(i, 0));
		_staticObjects.push_back(wall);

		Point* point = new Point(_spriteSet);
		point->init(u32x2(i, 5));
		_dynamicObjects.push_back(point);

		wall = new Wall(_spriteSet);
		wall->init(MapObjectWallBlue, s32x2(i, IGameStage::LevelHeight - 1));
		_staticObjects.push_back(wall);
	}

	for(u32 i = 1; i < IGameStage::LevelHeight - 1; ++i)
	{
		Wall* wall = new Wall(_spriteSet);
		wall->init(MapObjectWallBlue, s32x2(0, i));
		_staticObjects.push_back(wall);

		wall = new Wall(_spriteSet);
		wall->init(MapObjectWallBlue, s32x2(IGameStage::LevelWidth - 1, i));
		_staticObjects.push_back(wall);
	}
	_dynamicObjects.push_back(new Ghost(_spriteSet));
	((Ghost*)(_dynamicObjects.back()))->init(MapObjectGhostGreen, u32x2(1, 1));
}

void LevelStage::saveGame(GameState* game_state)
{ }

void LevelStage::initStages(MainMenu* main_menu, HighScores* high_scores)
{
	_mainMenu = main_menu;
	_highScores = high_scores;
}

void LevelStage::initResources(PixmapFont* menu_font, PixmapFont* small_font, SpriteSet* sprite_set)
{
	_menuFont = menu_font;
	_smallFont = small_font;
	_spriteSet = sprite_set;
}

void LevelStage::_clear()
{
	vector<Object*>::iterator itr1, end1;
	for(itr1 = _staticObjects.begin(), end1 = _staticObjects.end(); itr1 < end1; ++itr1)
		delete *itr1;
	for(itr1 = _dynamicObjects.begin(), end1 = _dynamicObjects.end(); itr1 < end1; ++itr1)
		delete *itr1;

	_staticObjects.clear();
	_dynamicObjects.clear();
}

} // namespace sparrow
