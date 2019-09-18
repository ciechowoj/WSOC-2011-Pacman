#include "smart.hpp"
#ifdef WINDOWS32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif
#ifdef ANDROID
#error "Unimplemented!"
#endif
namespace sparrow {

void RefCtrMT::acquire() const
{
	InterlockedIncrement(&_refCtr);
}

bool RefCtrMT::release() const
{
	return InterlockedDecrement(&_refCtr) == 0;
}

} // namespace sparrow
