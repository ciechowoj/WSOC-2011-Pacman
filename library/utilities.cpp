#include "utilities.hpp"
#include <Windows.h>
namespace sparrow {

void utf8count(u32& size, u32& byte, const u16* data)
{
	const u16* ptr = data;
	byte = size = 0;

	while(*ptr != 0)
	{
		if(*ptr > 0x7f)
			byte += *ptr > 0x7FF ? 2 : 1;
		++ptr;
	}

	size = ptr - data;
	byte += size;
}

void utf8count(u32& size, u32& byte, const u16* begin, const u16* end)
{
	const u16* ptr = begin;
	size = byte = 0;

	while(ptr != end)
	{
		if(*ptr > 0x7f)
			byte += *ptr > 0x7FF ? 2 : 1;
		++ptr;
	}

	size = ptr - begin;
	byte += size;
}

void utf8copy(u8* dest, const u16* begin, const u16* end)
{
	const u8 Sequence = 0x8 << 4, Sequence1 = 0xC << 4, Sequence2 = 0xE << 4;
	const u16 Mask = 0x3f, Mask1 = 0x1f, Mask2 = 0x0f;

	const u16* pSrc = begin, *pEnd = end;
	u8* pDest = dest;

	while(pSrc != pEnd)
	{
		if(*pSrc <= 0x007fu)
		{
			*pDest = static_cast<u8>(*pSrc);
		}
		else if(*pSrc <= 0x07ffu)
		{
			*pDest = Sequence1 | (*pSrc >> 6 & Mask1); ++pDest;
			*pDest = Sequence | (*pSrc & Mask);
		}
		else
		{
			*pDest = Sequence2 | (*pSrc >> 12 & Mask2); ++pDest;
			*pDest = Sequence | (*pSrc >> 6 & Mask); ++pDest;
			*pDest = Sequence | (*pSrc & Mask);
		}
		++pDest;
		++pSrc;
	}
}

vector<string> split(const string& text, c16 separator)
{
	vector<string> result;
	string::iterator first = text.begin();
	string::iterator last = text.begin();
	while(last < text.end())
	{
		if(*last == separator)
		{
			result.push_back_def();
			result.back() = string(first, last);
			first = last + 1;
		}
		++last;
	}
	if(last != first)
	{
		result.push_back_def();
		result.back() = string(first, last);
	}
	return result;
}

void changeEndiannes16(u16* buffer, u32 size)
{
	for(u32 i = 0; i < size; ++i)
		buffer[i] = buffer[i] >> 8 | buffer[i] << 8;
}

u32 bitcount(u32 value)
{
	const u8 BitsetTable16[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
	u32 c = 0;
	for(u32 i = 0; i < 8; ++i)
		c += BitsetTable16[value << i * 4 >> 28];
	return c;
}

u32 bitcount(u64 value)
{
	const u8 BitsetTable16[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
	u32 c = 0;
	for(u32 i = 0; i < 16; ++i)
		c += BitsetTable16[value << i * 4 >> 60];
	return c;
}

void leftshift(u32* vec, u32 size, u32 shift)
{
	runtime_assert(vec != nullptr && size != 0 && shift <= size * 32, "invalidarg");

	u32 hShift = shift >> 5u;
	u32 lShift = shift - (hShift << 5u);
	s32 term = hShift;
	u32 mask = lShift ? 0xffffffff : 0x00000000;

	for(s32 i = size - 1; i > term; --i)
		vec[i] = (vec[i - hShift] << lShift) | (vec[i - hShift - 1] >> (32 - lShift) & mask);
	vec[term] = (vec[term - hShift] << lShift);
	for(s32 i = static_cast<u32>(term - 1); i >= 0; --i)
		vec[i] = 0;
}

void rightshift(u32* vec, u32 size, u32 shift)
{
	runtime_assert(vec != nullptr && size != 0 && shift <= size * 32, "invalidarg");

	u32 hShift = shift >> 5u;
	u32 lShift = shift - (hShift << 5u);
	s32 term = size - hShift - 1;
	u32 mask = lShift ? 0xffffffff : 0x00000000;

	for(s32 i = 0; i < term; ++i)
		vec[i] = (vec[i + hShift] >> lShift) | (vec[i + hShift + 1] << (32 - lShift) & mask);
	vec[term] = (vec[term + hShift] >> lShift);
	for(u32 i = static_cast<u32>(term + 1); i < size; ++i)
		vec[i] = 0;
}

u32 msbindex(u32 value)
{
	u32 result = 0;
	while(value != 1 && result < 32)
	{
		value >>= 1;
		++result;
	}
	return result;
}

u32 msbindex(u64 value)
{
	u32 result = 0;
	while(value != 1 && result < 64)
	{
		value >>= 1;
		++result;
	}
	return result;
}

inline void _getQPCFrequency(u64& frequency)
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency));
}

inline void _getQPCCounter(u64& counter) 
{ 
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&counter));
}

f64 timef64()
{
	u64 counter, frequency;
	_getQPCCounter(counter);
	_getQPCFrequency(frequency);
	return static_cast<double>(counter) / static_cast<double>(frequency);
}

f32 timef32() 
{ 
	return static_cast<f32>(GetTickCount()) / 1000.f; 
}

u64 ntimeu64()
{
	u64 counter, frequency;
	_getQPCCounter(counter);
	_getQPCFrequency(frequency);
	return counter * 1000000000 / frequency;
}

u32 utimeu32() 
{ 
	return GetTickCount(); 
}

void sleep(u32 ms)
{
	Sleep(ms);
}

void deleteFile(const string& path)
{
	DeleteFileW(path.c_wstr());
}

} // namespace sparrow
