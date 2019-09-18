#pragma once
#include <library\library.hpp>
namespace sparrow {

const u32 KeyIDArrowUp = 0x26;
const u32 KeyIDArrowDown = 0x28;
const u32 KeyIDArrowLeft = 0x25;
const u32 KeyIDArrowRight = 0x27;
const u32 KeyIDEnter = 0x0d;
const u32 KeyIDEscape = 0x1b;

enum CursorEvent
{
	CursorEventL = 0, //LMB or double tap
	CursorEventM = 1, //MMB or tap
	CursorEventR = 2, //RMB or hold tap
};

enum FrameworkEvent
{
	FrameworkEventCreate,
	FrameworkEventDestroy,
	FrameworkEventResume,
	FrameworkEventPause,
	FrameworkEventReleaseBuffer,
	FrameworkEventRestoreBuffer,
};

class IFrameworkEventHandler
{
public:
	virtual				~IFrameworkEventHandler() = 0;
	virtual void		onCursorMove(s32x2 absolute, s32x2 relative) = 0;
	virtual void		onCursorDrag(s32x2 absolute, s32x2 relative) = 0;
	virtual void		onCursorDown(s32x2 absolute, CursorEvent cursorEvent) = 0;
	virtual void		onCursorUp(s32x2 absolute, CursorEvent cursorEvent) = 0;

	virtual void		onPageScroll(s32 delta) = 0;
	virtual void		onKeyDown(u32 id) = 0;
	virtual void		onKeyUp(u32 id) = 0;
	virtual void		onCharacter(c16 charCode, bool repeated) = 0;
	virtual void		onFramework(FrameworkEvent frameworkEvent) = 0;
};

class Framework
{
public:
						Framework(bool fullscreen, const u32x2& size = u32x2(0, 0));
						~Framework();

	void				setCaption(const string& caption);
	const string&		getCaption() const;
	const s32x2&		getSize() const;
	const string&		getType() const;
	const string&		getSystem() const;
	void				showCursor(bool showed);
	void				lockCursor(bool locked);
	void				clipCursor(bool clipped);
	bool				isCursorVisible() const;
	bool				isCursorLocked() const;
	bool				isCursorClipped() const;

	bool				processEvents(IFrameworkEventHandler* frameworkEventHandler);
	void				swapBuffers();
private:
	u8					_pool[84];
};

inline IFrameworkEventHandler::~IFrameworkEventHandler() 
{ }

} //namespace sparrow
