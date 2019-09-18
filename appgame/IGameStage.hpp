#pragma once
#include "..\engine\Context2D.hpp"
namespace sparrow {

template <class T1, class T2> class pair 
{
public:
	typedef T1 first_type;
	typedef T2 second_type;
	
	T1 first;
	T2 second;

	pair()
		: first(T1())
		, second(T2())
	{ }

	pair(const T1& st, const T2& nd)
		: first(st)
		, second(nd)
	{ }

	template <class U, class V> pair(const pair<U, V>& p)
		: first(p.first)
		, second(p.second)
	{ }

	template <class U, class V> pair& operator=(const pair<U, V>& p)
	{
		first = p.first;
		second = p.second;
	}

	void swap(pair& p)
	{
		swap(first, p.first);
		swap(second, p.second);
	}
};

extern const char* SaveGameFile;
extern const char* SaveGameTemp;

enum GameMode
{
	GameModeNone,
	GameModeSingle,
	GameModeMulti,
	GameModeTCPIP
};

enum InputEvent
{
	InputEventLeft1,
	InputEventUp1,
	InputEventRight1,
	InputEventDown1,
	InputEventLeft2,
	InputEventUp2,
	InputEventRight2,
	InputEventDown2,
	InputEventChar,
	InputEventEnter,
	InputEventEscape,
};

class IGameStage
{
public:
	virtual				~IGameStage();
	virtual	IGameStage*	onInputEvent(InputEvent input_event, c16 char_code = 0) = 0;
	virtual void		onUpdate(u32 delta_time, u32 current_time) = 0;
	virtual void		onRedraw(Context2D* context2d) = 0;
	virtual void		onPause() = 0;
	virtual void		onResume() = 0;
	static const s32 TileWidth = 48;
	static const s32 TileHeight = 48;
	static const s32 LevelWidth = 19;
	static const s32 LevelHeight = 15;
	static const s32 StageWidth = LevelWidth * TileWidth; // 912
	static const s32 StageHeight = (LevelHeight + 1) * TileHeight; // 768
};

inline IGameStage::~IGameStage()
{ }

} // namespace sparrow
