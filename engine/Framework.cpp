#ifdef TARGET_WINDOWS
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include <Windowsx.h>
#undef max
#undef min

#include "Framework.hpp"
extern "C" {

#ifndef WGL_ARB_extensions_string
#endif

#ifndef WGL_ARB_pixel_format
#define WGL_NUMBER_PIXEL_FORMATS_ARB   0x2000
#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_DRAW_TO_BITMAP_ARB         0x2002
#define WGL_ACCELERATION_ARB           0x2003
#define WGL_NEED_PALETTE_ARB           0x2004
#define WGL_NEED_SYSTEM_PALETTE_ARB    0x2005
#define WGL_SWAP_LAYER_BUFFERS_ARB     0x2006
#define WGL_SWAP_METHOD_ARB            0x2007
#define WGL_NUMBER_OVERLAYS_ARB        0x2008
#define WGL_NUMBER_UNDERLAYS_ARB       0x2009
#define WGL_TRANSPARENT_ARB            0x200A
#define WGL_TRANSPARENT_RED_VALUE_ARB  0x2037
#define WGL_TRANSPARENT_GREEN_VALUE_ARB 0x2038
#define WGL_TRANSPARENT_BLUE_VALUE_ARB 0x2039
#define WGL_TRANSPARENT_ALPHA_VALUE_ARB 0x203A
#define WGL_TRANSPARENT_INDEX_VALUE_ARB 0x203B
#define WGL_SHARE_DEPTH_ARB            0x200C
#define WGL_SHARE_STENCIL_ARB          0x200D
#define WGL_SHARE_ACCUM_ARB            0x200E
#define WGL_SUPPORT_GDI_ARB            0x200F
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_STEREO_ARB                 0x2012
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_RED_BITS_ARB               0x2015
#define WGL_RED_SHIFT_ARB              0x2016
#define WGL_GREEN_BITS_ARB             0x2017
#define WGL_GREEN_SHIFT_ARB            0x2018
#define WGL_BLUE_BITS_ARB              0x2019
#define WGL_BLUE_SHIFT_ARB             0x201A
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_ALPHA_SHIFT_ARB            0x201C
#define WGL_ACCUM_BITS_ARB             0x201D
#define WGL_ACCUM_RED_BITS_ARB         0x201E
#define WGL_ACCUM_GREEN_BITS_ARB       0x201F
#define WGL_ACCUM_BLUE_BITS_ARB        0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB       0x2021
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_AUX_BUFFERS_ARB            0x2024
#define WGL_NO_ACCELERATION_ARB        0x2025
#define WGL_GENERIC_ACCELERATION_ARB   0x2026
#define WGL_FULL_ACCELERATION_ARB      0x2027
#define WGL_SWAP_EXCHANGE_ARB          0x2028
#define WGL_SWAP_COPY_ARB              0x2029
#define WGL_SWAP_UNDEFINED_ARB         0x202A
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_TYPE_COLORINDEX_ARB        0x202C
#endif

#ifndef WGL_ARB_create_context
#define WGL_CONTEXT_DEBUG_BIT_ARB      0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define WGL_CONTEXT_LAYER_PLANE_ARB    0x2093
#define WGL_CONTEXT_FLAGS_ARB          0x2094
#define ERROR_INVALID_VERSION_ARB      0x2095
#endif

#ifndef WGL_ARB_extensions_string
#define WGL_ARB_extensions_string 1
#ifdef WGL_WGLEXT_PROTOTYPES
extern const char * WINAPI wglGetExtensionsStringARB (HDC hdc);
#endif /* WGL_WGLEXT_PROTOTYPES */
typedef const char * (WINAPI * PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
#endif

#ifndef WGL_ARB_pixel_format
#define WGL_ARB_pixel_format 1
#ifdef WGL_WGLEXT_PROTOTYPES
extern BOOL WINAPI wglGetPixelFormatAttribivARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
extern BOOL WINAPI wglGetPixelFormatAttribfvARB (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
extern BOOL WINAPI wglChoosePixelFormatARB (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
#endif /* WGL_WGLEXT_PROTOTYPES */
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
typedef BOOL (WINAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, FLOAT *pfValues);
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
#endif

#ifndef WGL_ARB_create_context
#define WGL_ARB_create_context 1
#ifdef WGL_WGLEXT_PROTOTYPES
extern HGLRC WINAPI wglCreateContextAttribsARB (HDC hDC, HGLRC hShareContext, const int *attribList);
#endif /* WGL_WGLEXT_PROTOTYPES */
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
#endif

} // extern "C"

#endif //TARGET_WINDOWS

namespace sparrow {

class _FrameworkImpl
{
public:
						_FrameworkImpl();
						~_FrameworkImpl();
	// window related
	string				windowCaption;
	string				windowType;
	s32x2				windowSize;
	s32x2				windowPosition;

	// input related
	s32x2				cursorCoords;
	bool				cursorShown;
	bool				cursorLocked;
	bool				cursorClipped;

	// platform specific
#ifdef TARGET_WINDOWS
	s32x2				windowCenter;
	s32x2				screenCenter;
	HWND				windowHandle;
	HGLRC				openGLContext;
	HDC					deviceContext;
	bool				cursorDrag;

	bool				registerWindowClass();
	bool				unregisterWindowClass();
	bool				createWindow(bool fullscreen, const u32x2& size);
	bool				destroyWindow();
	bool				createOpenGLContext();
	bool				destroyOpenGLContext();
#endif //TARGET_WINDOWS
};

inline _FrameworkImpl* _FrameworkImpl_cast(void* impl) { return reinterpret_cast<_FrameworkImpl*>(impl); }
inline const _FrameworkImpl* _FrameworkImpl_cast(const void* impl) { return reinterpret_cast<const _FrameworkImpl*>(impl); }
inline _FrameworkImpl* get_FrameworkImpl(Framework* window) { return reinterpret_cast<_FrameworkImpl*>(window); }

#ifdef TARGET_WINDOWS
HWND getHWND(Framework* framework) { return get_FrameworkImpl(framework)->windowHandle; }
const wchar_t* WindowClassName = L"SparrowClass";
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif //TARGET_WINDOWS

Framework::Framework(bool fullscreen, const u32x2& size)
{
	// create _WindowImpl
	static_assert(sizeof(_FrameworkImpl) == sizeof(_pool), "Correct _pool size!");
	_FrameworkImpl* _this = new (_pool) _FrameworkImpl;

#ifdef TARGET_WINDOWS
	_this->registerWindowClass();
	_this->createWindow(fullscreen, size);
	_this->createOpenGLContext();
#endif //TARGET_WINDOWS
}

Framework::~Framework()
{
	_FrameworkImpl* _this = _FrameworkImpl_cast(_pool);

#ifdef TARGET_WINDOWS
	_this->destroyOpenGLContext();
	_this->destroyWindow();
	_this->unregisterWindowClass();
#endif //TARGET_WINDOWS	
}

void Framework::setCaption(const string& caption)
{
	_FrameworkImpl* _this = _FrameworkImpl_cast(_pool);
	_this->windowCaption = caption;
	SetWindowTextW(_this->windowHandle, _this->windowCaption.c_wstr());
}

const string& Framework::getCaption() const
{
	return _FrameworkImpl_cast(_pool)->windowCaption;
}

const s32x2& Framework::getSize() const
{
	return _FrameworkImpl_cast(_pool)->windowSize;
}

const string& Framework::getType() const
{
	return _FrameworkImpl_cast(_pool)->windowType;
}

void Framework::showCursor(bool state)
{
	if(state && !_FrameworkImpl_cast(_pool)->cursorShown)
	{
		ShowCursor(TRUE);
		ShowCursor(TRUE);
		_FrameworkImpl_cast(_pool)->cursorShown = true;
	}
	else if(_FrameworkImpl_cast(_pool)->cursorShown)
	{
		ShowCursor(FALSE);
		ShowCursor(FALSE);
		_FrameworkImpl_cast(_pool)->cursorShown = false;
	}
}

void Framework::lockCursor(bool locked)
{
	_FrameworkImpl_cast(_pool)->cursorLocked = locked;
}

void Framework::clipCursor(bool clipped)
{
	_FrameworkImpl_cast(_pool)->cursorClipped = clipped;
}

bool Framework::isCursorVisible() const
{
	return _FrameworkImpl_cast(_pool)->cursorShown;
}

bool Framework::isCursorLocked() const
{
	return _FrameworkImpl_cast(_pool)->cursorLocked;
}

bool Framework::isCursorClipped() const
{
	return _FrameworkImpl_cast(_pool)->cursorClipped;
}

bool Framework::processEvents(IFrameworkEventHandler* frameworkEventHandler)
{	
	_FrameworkImpl* _this = _FrameworkImpl_cast(_pool);
	MSG message;
	if(_this->cursorClipped)
	{
		RECT window, client;
		GetWindowRect(_this->windowHandle, &window);
		GetClientRect(_this->windowHandle, &client);
		s32 b = (window.right - window.left - client.right) / 2;
		window.left += b;
		window.right -= b;
		window.bottom -= b;
		window.top = window.bottom - client.bottom;
		ClipCursor(&window);
	}

	SetWindowLongPtrW(_this->windowHandle, 0, reinterpret_cast<LONG_PTR>(frameworkEventHandler));
	SetWindowLongPtrW(_this->windowHandle, sizeof(LONG_PTR), reinterpret_cast<LONG_PTR>(_this));
	SetWindowPos(_this->windowHandle, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

	if(PeekMessageW(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessageW(&message);
		return false;
	}

	SetWindowLongPtrW(_this->windowHandle, 0, reinterpret_cast<LONG_PTR>(nullptr));
	SetWindowLongPtrW(_this->windowHandle, sizeof(LONG_PTR), reinterpret_cast<LONG_PTR>(nullptr));

	return true;
}

void Framework::swapBuffers()
{
	_FrameworkImpl* _this = _FrameworkImpl_cast(_pool);
	SwapBuffers(_this->deviceContext);
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const u32 PreviousState = 1u << 30;
	const u32 ExtendedKey = 1u << 24;

	if(hwnd != 0)
	{
		IFrameworkEventHandler* handler = absolute_cast<IFrameworkEventHandler*>(GetWindowLongPtr(hwnd, 0));
		_FrameworkImpl* _this = reinterpret_cast<_FrameworkImpl*>(GetWindowLongPtr(hwnd, sizeof(LONG_PTR)));

		if(_this != nullptr && handler != nullptr)
		{
			s32x2 cursorCoords, cursorDelta;
			switch(uMsg)
			{
			case WM_MOUSEMOVE:
				cursorCoords = s32x2(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				cursorDelta = s32x2(0, 0);
				if(_this->cursorLocked)
				{
					if(cursorCoords != _this->windowCenter)
					{
						cursorDelta = cursorCoords - _this->windowCenter;
						_this->cursorCoords += cursorDelta;

						if(_this->cursorCoords.x < 0)
							_this->cursorCoords.x = 0;
						if(_this->cursorCoords.x > _this->windowSize.x)
							_this->cursorCoords.x = _this->windowSize.x;
						if(_this->cursorCoords.y < 0)
							_this->cursorCoords.y = 0;
						if(_this->cursorCoords.y > _this->windowSize.y)
							_this->cursorCoords.y = _this->windowSize.y;
						
						SetCursorPos(_this->screenCenter.x, _this->screenCenter.y);
					}
				}
				else
				{
					cursorDelta = cursorCoords - _this->cursorCoords;
					_this->cursorCoords = cursorCoords;
				}

				if(cursorDelta != s32x2(0, 0))
				{
					handler->onCursorMove(_this->cursorCoords, cursorDelta);
					if(_this->cursorDrag)
						handler->onCursorDrag(_this->cursorCoords, cursorDelta);
				}
				return 0;
			case WM_LBUTTONDOWN:
				_this->cursorDrag = true;
				handler->onCursorDown(_this->cursorCoords, CursorEventL);
				return 0;
			case WM_LBUTTONUP: 
				_this->cursorDrag = false;
				handler->onCursorUp(_this->cursorCoords, CursorEventL);
				return 0;
			case WM_RBUTTONDOWN:
				handler->onCursorDown(_this->cursorCoords, CursorEventR);
				return 0;
			case WM_RBUTTONUP:
				handler->onCursorUp(_this->cursorCoords, CursorEventR);
				return 0;
			case WM_MBUTTONDOWN:
				handler->onCursorDown(_this->cursorCoords, CursorEventM);
				return 0;
			case WM_MBUTTONUP:
				handler->onCursorUp(_this->cursorCoords, CursorEventM);
				return 0;
			case WM_KEYDOWN:
				if((lParam & PreviousState) == 0)
					handler->onKeyDown(static_cast<u32>(wParam));
				return 0;
			case WM_KEYUP:
				handler->onKeyUp(static_cast<u32>(wParam));
				return 0;
			case WM_CHAR:
				handler->onCharacter(static_cast<c16>(wParam), (lParam & PreviousState) != 0);
				return 0;
			case WM_MOUSEWHEEL:
				handler->onPageScroll(static_cast<s32>(GET_WHEEL_DELTA_WPARAM(wParam)));
				return 0;
			case WM_CLOSE:
				handler->onFramework(FrameworkEventDestroy);
				return 0;
			case WM_SIZE:
				if(wParam == SIZE_RESTORED)
				{
					_this->windowSize.x = LOWORD(lParam);
					_this->windowSize.y = HIWORD(lParam);

					handler->onFramework(FrameworkEventReleaseBuffer);
					// change size there
					handler->onFramework(FrameworkEventRestoreBuffer);
				}
				return 0;
			case WM_MOVE:
				_this->windowCenter.x = _this->windowSize.x / 2;
				_this->windowCenter.y = _this->windowSize.y / 2;
				_this->screenCenter = _this->windowCenter;
				ClientToScreen(_this->windowHandle, (LPPOINT)&_this->screenCenter);
				return 0;
			}
		}
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

_FrameworkImpl::_FrameworkImpl()
	: windowSize(0, 0)
	, windowPosition(0, 0)
	, cursorCoords(0, 0)
	, cursorShown(false)
	, cursorLocked(false)
	, cursorClipped(false)
#ifdef TARGET_WINDOWS
	, windowCenter(0, 0)
	, screenCenter(0, 0)
	, windowHandle(0)
	, cursorDrag(false)
#endif //TARGET_WINDOWS
{ }

_FrameworkImpl::~_FrameworkImpl()
{ }

#ifdef TARGET_WINDOWS
bool _FrameworkImpl::registerWindowClass()
{
	WNDCLASSEXW wcex;
	wcex.cbSize			= sizeof(WNDCLASSEXW);
	wcex.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wcex.lpfnWndProc    = WindowProcedure;
	wcex.cbClsExtra     = 0;
	wcex.cbWndExtra     = 2 * sizeof(LONG_PTR);
	wcex.hInstance      = (HINSTANCE)GetModuleHandleW(nullptr);
	wcex.hIcon          = 0;
	wcex.hCursor        = 0;
	wcex.hbrBackground  = 0;
	wcex.lpszMenuName   = nullptr;
	wcex.lpszClassName  = L"SparrowClass";
	wcex.hIconSm        = 0;

	return RegisterClassExW(&wcex) != 0;
}

bool _FrameworkImpl::unregisterWindowClass()
{
	return UnregisterClassW(L"SparrowClass", (HINSTANCE)GetModuleHandleW(nullptr)) != 0;
}

bool _FrameworkImpl::createWindow(bool fullscreen, const u32x2& size)
{
	DWORD dwStyle = 0;
	DEVMODEW mode;
	EnumDisplaySettingsW(nullptr, ENUM_REGISTRY_SETTINGS, &mode);

	if(fullscreen)
	{
		windowSize.x = mode.dmPelsWidth;
		windowSize.y = mode.dmPelsHeight;
		windowType = "fullscreen";
		dwStyle = WS_POPUP;


		/*memset(&mode, 0, sizeof(mode));
		mode.dmSize=sizeof(mode);	
		mode.dmPelsWidth = windowSize.x;
		mode.dmPelsHeight = windowSize.y;
		mode.dmBitsPerPel = 32;
		mode.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
	
		ChangeDisplaySettingsW(&mode, CDS_FULLSCREEN);*/
	}
	else
	{
		if(size.x == 0 || size.y == 0)
		{
			windowSize.x = mode.dmPelsWidth / 2;
			windowSize.y = mode.dmPelsHeight / 2;
		}
		else
		{
			windowSize = size;
		}
		windowType = "window";
		dwStyle = WS_POPUP; //WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_BORDER;
	}
	
	RECT windowRect = { 0, 0, windowSize.x, windowSize.y };
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, WS_EX_APPWINDOW);
	u32x2 offset = u32x2(mode.dmPelsWidth - windowRect.right + windowRect.left, mode.dmPelsHeight - windowRect.bottom + windowRect.top);
	offset = fullscreen ? u32x2(0, 0) : offset = offset / 2;
	windowHandle = CreateWindowExW(WS_EX_APPWINDOW, L"SparrowClass", windowCaption.c_wstr(), dwStyle, offset.x, offset.y, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top, NULL, NULL, (HINSTANCE)GetModuleHandleW(nullptr), NULL);
	ShowWindow(windowHandle, SW_SHOW);
	return windowHandle != 0;
}

bool _FrameworkImpl::destroyWindow()
{
	return DestroyWindow(windowHandle) != 0;
}

bool _FrameworkImpl::createOpenGLContext()
{
	// create false context
	HWND hAuxWnd = CreateWindowExW(0, L"SprrowClass", L"", WS_POPUP, 0, 0, 0, 0, NULL, NULL, (HINSTANCE)GetModuleHandleW(nullptr), NULL);
	PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 24, 8, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
	HDC hAuxDC = GetDC(hAuxWnd);
	int format = ChoosePixelFormat(hAuxDC, &pfd);
	SetPixelFormat(hAuxDC, format, &pfd);
	HGLRC hAuxGLRC;
	hAuxGLRC = wglCreateContext(hAuxDC);
	wglMakeCurrent(hAuxDC, hAuxGLRC);

	// get extension ptr's
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	PFNWGLGETPIXELFORMATATTRIBIVARBPROC wglGetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribivARB");
	PFNWGLGETPIXELFORMATATTRIBFVARBPROC wglGetPixelFormatAttribfvARB = (PFNWGLGETPIXELFORMATATTRIBFVARBPROC)wglGetProcAddress("wglGetPixelFormatAttribfvARB");
	PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	// create true context
	const int formatAttribs[] = { WGL_DRAW_TO_WINDOW_ARB, 1, WGL_SUPPORT_OPENGL_ARB, 1, WGL_DOUBLE_BUFFER_ARB, 1, WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB, WGL_COLOR_BITS_ARB, 32, WGL_DEPTH_BITS_ARB, 24, WGL_STENCIL_BITS_ARB, 8, 0 };
	int contextAttribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 2, 0 };
	deviceContext = GetDC(windowHandle);
	u32 number = 0;
	//wglChoosePixelFormatARB(hDC, formatAttribs, NULL, 1, &format, &number);
	SetPixelFormat(deviceContext, format, &pfd);
	openGLContext = wglCreateContextAttribsARB(deviceContext, 0, contextAttribs);
	wglMakeCurrent(deviceContext, openGLContext);

	// destroy false context
	DestroyWindow(hAuxWnd);
	wglDeleteContext(hAuxGLRC);
	return true;
}

bool _FrameworkImpl::destroyOpenGLContext()
{
	HDC hDC = GetDC(windowHandle);
	wglMakeCurrent(hDC, 0);
	wglDeleteContext(openGLContext);
	return true;
}
#endif //TARGET_WINDOWS

} //namespace sparrow

