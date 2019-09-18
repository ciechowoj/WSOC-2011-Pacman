#pragma once
#include "IGameStage.hpp"
namespace sparrow {

class MainMenu;

class HighScores : public IGameStage
{
public:
						HighScores();
	virtual				~HighScores();

	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code);
	virtual void		onUpdate(u32 delta_time, u32 current_time);
	virtual void		onRedraw(Context2D* context2d);
	virtual void		onPause();
	virtual void		onResume();

	void				initStages(MainMenu* main_menu);
	void				initResources(PixmapFont* menu_font, PixmapFont* small_font);
	void				addScore(const string& name, u32 value);
private:
	string				_title;
	struct _Score
	{
						_Score();
						_Score(const string& n, u32 v);

		string			name;
		u32				value;

		bool operator<(const HighScores::_Score& that) const;
		friend void	serialize(const _Score& object, DataStream* stream);
		friend void deserialize(_Score& object, const DataStream* stream);
	};

	vector_set<_Score>	_scores;

	PixmapFont*			_menuFont;
	PixmapFont*			_smallFont;
	MainMenu*			_mainMenu;
	friend void	serialize(const _Score& object, DataStream* stream);
	friend void deserialize(_Score& object, const DataStream* stream);
};

inline HighScores::_Score::_Score()
	: value(0)
{ }

inline HighScores::_Score::_Score(const string& n, u32 v)
	: name(n)
	, value(v)
{ }

inline bool HighScores::_Score::operator<(const HighScores::_Score& that) const
{
	return that.value < value;
}

inline void serialize(const HighScores::_Score& object, DataStream* stream)
{
	sparrow::serialize(object.name, stream);
	sparrow::serialize(object.value, stream);
}

inline void deserialize(HighScores::_Score& object, const DataStream* stream)
{
	sparrow::deserialize(object.name, stream);
	sparrow::deserialize(object.value, stream);
}


} // namespace sparrow
