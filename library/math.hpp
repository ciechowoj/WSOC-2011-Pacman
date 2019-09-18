#pragma once
#include "graphic.hpp"
namespace sparrow {

// Scalar functions -----------------------------------------------------------------------------------------------------------

template <class T> T clamp(const T& x, const T& min, const T& max);
template <class T> T clamps(const T& x);
template <class T> T clampu(const T& x);
template <class T> T degtorad(const T& deg);
template <class T> const T& max(const T& a, const T& b);
template <class T> const T& max(const T& a, const T& b, const T& c);
template <class T> const T& min(const T& a, const T& b);
template <class T> const T& min(const T& a, const T& b, const T& c);
template <class T> void swap(T& a, T& b);
template <class T> T abs(const T& a);

float floor(float x);
float ceil(float x);
float round(float x);
s32 roundi(f32 x);
s8 sign(s8 a);
s16 sign(s16 a);
s32 sign(s32 a);
f32 sign(f32 a);
u32 floor2(u32 x);
u32 ceil2(u32 x);

// Vector functions -----------------------------------------------------------------------------------------------------------
f32x3& vectorCross(f32x3& out, const f32x3& a, const f32x3& b);
float& vectorDot(float& out, const f32x4& a, const f32x4& b);
float& vectorDot(float& out, const f32x3& a, const f32x3& b);
float& vectorDot(float& out, const f32x2& a, const f32x2& b);
float vectorLengthSq(const f32x4& a);
float vectorLengthSq(const f32x3& a);
float vectorLengthSq(const f32x2& a);
float vectorLength(const f32x4& a);
float vectorLength(const f32x3& a);
float vectorLength(const f32x2& a);
f32x4& vectorFloor(f32x4& out, const f32x4& a);
f32x3& vectorFloor(f32x3& out, const f32x3& a);
f32x2& vectorFloor(f32x2& out, const f32x2& a);
f32x4& vectorCeil(f32x4& out, const f32x4& a);
f32x3& vectorCeil(f32x3& out, const f32x3& a);
f32x2& vectorCeil(f32x2& out, const f32x2& a);
f32x4& vectorRound(f32x4& out, const f32x4& a);
f32x3& vectorRound(f32x3& out, const f32x3& a);
f32x2& vectorRound(f32x2& out, const f32x2& a);
s32x4& vectorFloor(s32x4& out, const f32x4& a);
s32x3& vectorFloor(s32x3& out, const f32x3& a);
s32x2& vectorFloor(s32x2& out, const f32x2& a);
s32x4& vectorCeil(s32x4& out, const f32x4& a);
s32x3& vectorCeil(s32x3& out, const f32x3& a);
s32x2& vectorCeil(s32x2& out, const f32x2& a);
s32x4& vectorRound(s32x4& out, const f32x4& a);
s32x3& vectorRound(s32x3& out, const f32x3& a);
s32x2& vectorRound(s32x2& out, const f32x2& a);

f32x4& vectorNormalize(f32x4& out, const f32x4& a);
f32x3& vectorNormalize(f32x3& out, const f32x3& a);
f32x2& vectorNormalize(f32x2& out, const f32x2& a);

template <class T> __vec2<T>& vectorIdentity(__vec2<T>& out);
template <class T> __vec3<T>& vectorIdentity(__vec3<T>& out);
template <class T> __vec4<T>& vectorIdentity(__vec4<T>& out);
template <class T> __vec2<T>& vectorClear(__vec2<T>& out);
template <class T> __vec3<T>& vectorClear(__vec3<T>& out);
template <class T> __vec4<T>& vectorClear(__vec4<T>& out);

//- Matrix functions ----------------------------------------------------------------------------------------------------------
f32x4x4& matrixMultiply(f32x4x4& out, const f32x4x4& a, const f32x4x4& b);
f32x4x4& matrixInverse(f32x4x4& out, float det, const f32x4x4& a);
f32x4x4& matrixInverse(f32x4x4& out, const f32x4x4& a);
f32x4x4& matrixInverse(f32x4x4& out);
f32x4x4& matrixTranspose(f32x4x4& out, const f32x4x4& a);
f32x4x4& matrixTranspose(f32x4x4& out);
float matrixDeterminant(const f32x4x4& a);
float matrixCofactor(const f32x4x4& a, s32 row, s32 col);
float matrixMinor(const f32x4x4& a, s32 row, s32 col);
f32x4x4& matrixIdentity(f32x4x4& out);
f32x4x4& matrixClear(f32x4x4& out);

//- color functions -----------------------------------------------------------------------------------------------------------

void colorConvertFloatToByte(u32& out, const f32x4& in);
void colorConvertByteToFloat(f32x4& out, u32 in);

//- AABR functions ------------------------------------------------------------------------------------------------------------

template <class T> const __vec2<T> aabrSize(const __AABR<T>& a);
template <class T> const __vec2<T>& aabrPosition(const __AABR<T>& a);
template <class T> T aabrWidth(const __AABR<T>& in);
template <class T> T aabrHeight(const __AABR<T>& in);
template <class T> __AABR<T>& aabrTranslate(__AABR<T>& out, const __AABR<T>& in, const __vec2<T>& v);
template <class T> __AABR<T>& aabrTranslate(__AABR<T>& inout, const __vec2<T>& v);
template <class T> __AABR<T>& aabrUnion(__AABR<T>& out, const __AABR<T>& in_a, const __AABR<T>& in_b);

f32 aabrArea(const AABRf& r);
s32 aabrArea(const AABRi& r);
s32 aabrArea(const AABRs& r);
template <class T> bool aabrTestCollisionInv(const __AABR<T>& in_a, const __AABR<T>& in_b, __vec2<T>& inout_v);
template <class T> bool aabrTestCollisionNrm(const __AABR<T>& in_a, const __AABR<T>& in_b, __vec2<T>& inout_v);
template <class T> bool aabrTestCollision(const __AABR<T>& in_a, const __AABR<T>& in_b);

//- Inilne implementations ----------------------------------------------------------------------------------------------------
template <class T> inline T clamp(const T& x, const T& min, const T& max) 
{ 
	if(x < min) return min; 
	if(x > max) return max; 
	return x; 
}

template <class T> inline T clamps(const T& x) 
{ 
	if(x < static_cast<T>(-1)) 
		return static_cast<T>(-1); 
	if(x > static_cast<T>(1)) 
		return static_cast<T>(1); 
	return x; 
}

template <class T> inline T clampu(const T& x) 
{ 
	if(x < static_cast<T>(0)) 
		return static_cast<T>(0); 
	if(x > static_cast<T>(1)) 
		return static_cast<T>(1); 
	return x; 
}

template <class T> inline T degtorad(const T& deg) 
{ 
	static const float factor = (1.f / 180.f) * Pi; 
	return deg * factor; 
}

template <class T> inline const T& max(const T& a, const T& b) 
{ 
	return a < b ? b : a; 
}

template <class T> inline const T& max(const T& a, const T& b, const T& c)
{
	return a < b ? (b < c ? c : b) : (a < c ? c : a);
}

template <class T> inline const T& min(const T& a, const T& b) 
{ 
	return a > b ? b : a; 
}

template <class T> inline const T& min(const T& a, const T& b, const T& c)
{
	return a > b ? (b > c ? c : b) : (a > c ? c : a);
}

template <class T> inline void swap(T& a, T& b) 
{ 
	T t = a; 
	a = b; 
	b = t; 
}

template <class T> inline T abs(const T& a)
{
	return a < T() ? -a : a;
}



inline s32 roundi(f32 x)
{
	return static_cast<s32>(round(x));
}

inline s8 sign(s8 a)
{
	return -(s8)(u8(a) >> 7) * 2 + 1;
}

inline s16 sign(s16 a)
{
	return -(s16)(u16(a) >> 15) * 2 + 1;
}

inline s32 sign(s32 a)
{
	return -(s32)(u32(a) >> 31) * 2 + 1;
}

inline f32 sign(f32 a)
{
	return a < 0.f ? -1.f : 1.f;
}

inline u32 floor2(u32 x)
{
	u32 i = 1;
	while(x != 0)
	{
		i <<= 1;
		x >>= 1;
	}
	return i >> 1;
}

inline u32 ceil2(u32 x)
{
	u32 i = 1; x -= 1;
	while(x != 0)
	{
		i <<= 1;
		x >>= 1;
	}
	return i;
}

inline f32x3& vectorCross(f32x3& out, const f32x3& a, const f32x3& b) 
{ 
	out = f32x3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); 
	return out; 
}

inline float& vectorDot(float& out, const f32x4& a, const f32x4& b) 
{
	out = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; 
	return out; 
}

inline float& vectorDot(float& out, const f32x3& a, const f32x3& b) 
{ 
	out = a.x * b.x + a.y * b.y + a.z * b.z; 
	return out; 
}

inline float& vectorDot(float& out, const f32x2& a, const f32x2& b) 
{ 
	out = a.x * b.x + a.y * b.y; 
	return out; 
}

inline float vectorLengthSq(const f32x4& a) 
{ 
	return a.x * a.x + a.y * a.y + a.z * a.z + a.w * a.w; 
}

inline float vectorLengthSq(const f32x3& a) 
{ 
	return a.x * a.x + a.y * a.y + a.z * a.z; 
}

inline float vectorLengthSq(const f32x2& a) 
{ 
	return a.x * a.x + a.y * a.y; 
}

inline f32x4& vectorFloor(f32x4& out, const f32x4& a)
{
	out.x = floor(a.x);
	out.y = floor(a.y);
	out.z = floor(a.z);
	out.w = floor(a.w);
	return out;
}

inline f32x3& vectorFloor(f32x3& out, const f32x3& a)
{
	out.x = floor(a.x);
	out.y = floor(a.y);
	out.z = floor(a.z);
	return out;
}

inline f32x2& vectorFloor(f32x2& out, const f32x2& a)
{
	out.x = floor(a.x);
	out.y = floor(a.y);
	return out;
}

/*inline f32x1& vectorFloor(f32x1& out, const f32x1& a)
{
	out.x = floor(a.x);
	return out;
}*/

inline f32x4& vectorCeil(f32x4& out, const f32x4& a)
{
	out.x = ceil(a.x);
	out.y = ceil(a.y);
	out.z = ceil(a.z);
	out.w = ceil(a.w);
	return out;
}

inline f32x3& vectorCeil(f32x3& out, const f32x3& a)
{
	out.x = ceil(a.x);
	out.y = ceil(a.y);
	out.z = ceil(a.z);
	return out;
}

inline f32x2& vectorCeil(f32x2& out, const f32x2& a)
{
	out.x = ceil(a.x);
	out.y = ceil(a.y);
	return out;
}

inline f32x4& vectorRound(f32x4& out, const f32x4& a)
{
	out.x = round(a.x);
	out.y = round(a.y);
	out.z = round(a.z);
	out.w = round(a.w);
	return out;
}

inline f32x3& vectorRound(f32x3& out, const f32x3& a)
{
	out.x = round(a.x);
	out.y = round(a.y);
	out.z = round(a.z);
	return out;
}

inline f32x2& vectorRound(f32x2& out, const f32x2& a)
{
	out.x = round(a.x);
	out.y = round(a.y);
	return out;
}

/*inline f32x1& vectorRound(f32x1& out, const f32x1& a)
{
	out.x = round(a.x);
	return out;
}*/

inline s32x4& vectorFloor(s32x4& out, const f32x4& a)
{
	out.x = static_cast<s32>(floor(a.x));
	out.y = static_cast<s32>(floor(a.y));
	out.z = static_cast<s32>(floor(a.z));
	out.w = static_cast<s32>(floor(a.w));
	return out;
}

inline s32x3& vectorFloor(s32x3& out, const f32x3& a)
{
	out.x = static_cast<s32>(floor(a.x));
	out.y = static_cast<s32>(floor(a.y));
	out.z = static_cast<s32>(floor(a.z));
	return out;
}

inline s32x2& vectorFloor(s32x2& out, const f32x2& a)
{
	out.x = static_cast<s32>(floor(a.x));
	out.y = static_cast<s32>(floor(a.y));
	return out;
}

/*inline s32x1& vectorFloor(s32x1& out, const f32x1& a)
{
	out.x = static_cast<s32>(floor(a.x));
	return out;
}*/

inline s32x4& vectorCeil(s32x4& out, const f32x4& a)
{
	out.x = static_cast<s32>(ceil(a.x));
	out.y = static_cast<s32>(ceil(a.y));
	out.z = static_cast<s32>(ceil(a.z));
	out.w = static_cast<s32>(ceil(a.w));
	return out;
}

inline s32x3& vectorCeil(s32x3& out, const f32x3& a)
{
	out.x = static_cast<s32>(ceil(a.x));
	out.y = static_cast<s32>(ceil(a.y));
	out.z = static_cast<s32>(ceil(a.z));
	return out;
}

inline s32x2& vectorCeil(s32x2& out, const f32x2& a)
{
	out.x = static_cast<s32>(ceil(a.x));
	out.y = static_cast<s32>(ceil(a.y));
	return out;
}

/*inline s32x1& vectorCeil(s32x1& out, const f32x1& a)
{
	out.x = static_cast<s32>(ceil(a.x));
	return out;
}*/

inline s32x4& vectorRound(s32x4& out, const f32x4& a)
{
	out.x = static_cast<s32>(round(a.x));
	out.y = static_cast<s32>(round(a.y));
	out.z = static_cast<s32>(round(a.z));
	out.w = static_cast<s32>(round(a.w));
	return out;
}

inline s32x3& vectorRound(s32x3& out, const f32x3& a)
{
	out.x = static_cast<s32>(round(a.x));
	out.y = static_cast<s32>(round(a.y));
	out.z = static_cast<s32>(round(a.z));
	return out;
}

inline s32x2& vectorRound(s32x2& out, const f32x2& a)
{
	out.x = static_cast<s32>(round(a.x));
	out.y = static_cast<s32>(round(a.y));
	return out;
}

/*inline s32x1& vectorRound(s32x1& out, const f32x1& a)
{
	out.x = static_cast<s32>(round(a.x));
	return out;
}*/

inline f32x4& vectorNormalize(f32x4& out, const f32x4& a) 
{ 
	float t = 1.f / vectorLength(a); 
	out[0] = a[0] * t; 
	out[1] = a[1] * t; 
	out[2] = a[2] * t; 
	out[3] = a[3] * t; 
	return out; 
}

inline f32x3& vectorNormalize(f32x3& out, const f32x3& a) 
{ 
	float t = 1.f / vectorLength(a); 
	out[0] = a[0] * t; 
	out[1] = a[1] * t; 
	out[2] = a[2] * t; 
	return out;
}

inline f32x2& vectorNormalize(f32x2& out, const f32x2& a) 
{ 
	float t = 1.f / vectorLength(a); 
	out[0] = a[0] * t; 
	out[1] = a[1] * t; 
	return out;
}

template <class T> inline __vec2<T>& vectorIdentity(__vec2<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(1);
	return out;
}

template <class T> inline __vec3<T>& vectorIdentity(__vec3<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(0);
	out.z = static_cast<T>(1);
	return out;
}

template <class T> inline __vec4<T>& vectorIdentity(__vec4<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(0);
	out.z = static_cast<T>(0);
	out.w = static_cast<T>(1);
	return out;
}

template <class T> inline __vec2<T>& vectorClear(__vec2<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(0);
	return out;
}

template <class T> inline __vec3<T>& vectorClear(__vec3<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(0);
	out.z = static_cast<T>(0);
	return out;
}

template <class T> inline __vec4<T>& vectorClear(__vec4<T>& out)
{
	out.x = static_cast<T>(0);
	out.y = static_cast<T>(0);
	out.z = static_cast<T>(0);
	out.w = static_cast<T>(0);
	return out;
}

inline void colorConvertFloatToByte(u32& out, const f32x4& in)
{
	static const u32 ChannelMask = 0x000000ff;
	u32 R, G, B, A;
	R = (u32)(255.f * in.x) & ChannelMask;
	G = (u32)(255.f * in.y) & ChannelMask;
	B = (u32)(255.f * in.z) & ChannelMask;
	A = (u32)(255.f * in.w) & ChannelMask;
	out = R << 24 | G << 16 | B << 8 | A;
}

inline void colorConvertByteToFloat(f32x4& out, u32 in)
{
	static const f32 Factor = 1.f / 255.f;
	u32 R, G, B, A;
	R = in >> 24;
	G = in << 8 >> 24;
	B = in << 16 >> 24;
	A = in << 24 >> 24;
	out.x = (f32)R * Factor;
	out.y = (f32)G * Factor;
	out.z = (f32)B * Factor;
	out.w = (f32)A * Factor;
}


template <class T> inline const __vec2<T> aabrSize(const __AABR<T>& a)
{
	return __vec2<T>(a.right - a.left, a.bottom - a.top);
}

template <class T> inline const __vec2<T>& aabrPosition(const __AABR<T>& a)
{
	return (__vec2<T>*)(a)[0];
}

template <class T> inline T aabrWidth(const __AABR<T>& in)
{
	return in.right - in.left;
}

template <class T> inline T aabrHeight(const __AABR<T>& in)
{
	return in.bottom - in.top;
}

template <class T> inline __AABR<T>& aabrTranslate(__AABR<T>& out, const __AABR<T>& in, const __vec2<T>& v)
{
	out.left = in.left + v.x;
	out.top = in.top + v.y;
	out.right = in.right + v.x;
	out.bottom = in.bottom + v.y;
	return out;
}

template <class T> inline __AABR<T>& aabrTranslate(__AABR<T>& inout, const __vec2<T>& v)
{
	inout.left += v.x;
	inout.top += v.y;
	inout.right += v.x;
	inout.bottom += v.y;
	return inout;
}

template <class T> inline __AABR<T>& aabrUnion(__AABR<T>& out, const __AABR<T>& in_a, const __AABR<T>& in_b)
{
	if(in_a.left <= in_b.left && in_b.left < in_a.right)
	{
		out.left = in_b.left;
		out.right = in_b.left < in_a.right && in_a.right <= in_b.right ? in_a.right : in_b.right;
	}

	if(in_b.left <= in_a.left && in_a.left < in_b.right)
	{
		out.left = in_a.left;
		out.right = in_a.left < in_b.right && in_b.right <= in_a.right ? in_b.right : in_a.right;
	}

	if(in_a.top <= in_b.top && in_b.top < in_a.bottom)
	{
		out.top = in_b.top;
		out.bottom = in_b.top < in_a.bottom && in_a.bottom <= in_b.bottom ? in_a.bottom : in_b.bottom;
	}

	if(in_b.top <= in_a.top && in_a.top < in_b.bottom)
	{
		out.top = in_a.top;
		out.bottom = in_a.top < in_b.bottom && in_b.bottom <= in_a.bottom ? in_b.bottom : in_a.bottom;
	}
	return out;
}

inline f32 aabrArea(const AABRf& r)
{
	f32 w = r.right - r.left;
	f32 h = r.bottom - r.top;
	return w * h;
}

inline s32 aabrArea(const AABRi& r)
{
	s32 w = r.right - r.left;
	s32 h = r.bottom - r.top;
	return w * h;
}

inline s32 aabrArea(const AABRs& r)
{
	s32 w = r.right - r.left;
	s32 h = r.bottom - r.top;
	return w * h;
}

template <class T> inline bool aabrTestCollisionInv(const __AABR<T>& in_a, const __AABR<T>& in_b, __vec2<T>& inout_v)
{
	__AABR<T> aabr(0, 0, 0, 0);
	aabrUnion(aabr, in_a, in_b);
	
	T w = aabrWidth(aabr) * sign(inout_v.x);
	T h = aabrHeight(aabr) * sign(inout_v.y);
	bool result = w * h != 0;

	if(result && (inout_v.x != 0 || inout_v.y != 0))
	{
		if(abs(inout_v.x) > abs(inout_v.y))
		{
			h = inout_v.x;
			inout_v.x = -min(w, inout_v.x);
			inout_v.y = -w * inout_v.y / h;
		}
		else
		{
			w = inout_v.y;
			inout_v.y = -min(h, inout_v.y);
			inout_v.x = -h * inout_v.x / w;
		}
	}

	return result;
}

template <class T> inline bool aabrTestCollisionNrm(const __AABR<T>& in_a, const __AABR<T>& in_b, __vec2<T>& inout_v)
{
	__AABR<T> aabr(0, 0, 0, 0);
	aabrUnion(aabr, in_a, in_b);

	T w = aabrWidth(aabr) * sign(inout_v.x);
	T h = aabrHeight(aabr) * sign(inout_v.y);
	bool result = w * h != 0;

	if(result && (inout_v.x != 0 || inout_v.y != 0))
	{
		if(abs(inout_v.x) > abs(inout_v.y))
		{
			inout_v.x = w;
			inout_v.y = 0;
		}
		else
		{
			inout_v.y = h;
			inout_v.x = 0;
		}
	}

	return result;
}

template <class T> inline bool aabrTestCollision(const __AABR<T>& in_a, const __AABR<T>& in_b)
{
	return 
		((in_a.left <= in_b.left && in_b.left < in_a.right) || (in_b.left <= in_a.left && in_a.left < in_b.right)) &&
		((in_a.top <= in_b.top && in_b.top < in_a.bottom) || (in_b.top <= in_a.top && in_a.top < in_b.bottom));
}

} //namespace sparrow
