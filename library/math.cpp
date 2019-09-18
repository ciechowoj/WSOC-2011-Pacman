#include "math.hpp"
#include <math.h>
namespace sparrow {

f32 floor(f32 x) 
{ 
	return ::floor(x); 
}

f32 ceil(f32 x) 
{ 
	return ::ceil(x); 
}

f32 round(f32 x) 
{ 
	return floor(x + 0.5f); 
}

f32 vectorLength(const f32x4& a)
{
	return sqrt(vectorLengthSq(a));
}

f32 vectorLength(const f32x3& a)
{
	return sqrt(vectorLengthSq(a));
}

f32 vectorLength(const f32x2& a)
{
	return sqrt(vectorLengthSq(a));
}

/*f32 vectorLength(const f32x1& a)
{
	return sqrt(vectorLengthSq(a));
}*/

template <s32 R, s32 C> f32 _minor(const f32x4x4& a)
{
    return a[R > 0 ? 0 : 1][C > 0 ? 0 : 1] * a[R > 1 ? 1 : 2][C > 1 ? 1 : 2] * a[R > 2 ? 2 : 3][C > 2 ? 2 : 3] +
           a[R > 0 ? 0 : 1][C > 1 ? 1 : 2] * a[R > 1 ? 1 : 2][C > 2 ? 2 : 3] * a[R > 2 ? 2 : 3][C > 0 ? 0 : 1] +
           a[R > 0 ? 0 : 1][C > 2 ? 2 : 3] * a[R > 1 ? 1 : 2][C > 0 ? 0 : 1] * a[R > 2 ? 2 : 3][C > 1 ? 1 : 2] -
           a[R > 2 ? 2 : 3][C > 0 ? 0 : 1] * a[R > 1 ? 1 : 2][C > 1 ? 1 : 2] * a[R > 0 ? 0 : 1][C > 2 ? 2 : 3] -
           a[R > 2 ? 2 : 3][C > 1 ? 1 : 2] * a[R > 1 ? 1 : 2][C > 2 ? 2 : 3] * a[R > 0 ? 0 : 1][C > 0 ? 0 : 1] -
           a[R > 2 ? 2 : 3][C > 2 ? 2 : 3] * a[R > 1 ? 1 : 2][C > 0 ? 0 : 1] * a[R > 0 ? 0 : 1][C > 1 ? 1 : 2];
}

typedef f32 (*_minorptr)(const f32x4x4&);

f32x4x4& matrixMultiply(f32x4x4& out, const f32x4x4& a, const f32x4x4& b)
{
	out[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0] + a[0][2] * b[2][0] + a[0][3] * b[3][0];
	out[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1] + a[0][2] * b[2][1] + a[0][3] * b[3][1];
	out[0][2] = a[0][0] * b[0][2] + a[0][1] * b[1][2] + a[0][2] * b[2][2] + a[0][3] * b[3][2];
	out[0][3] = a[0][0] * b[0][3] + a[0][1] * b[1][3] + a[0][2] * b[2][3] + a[0][3] * b[3][3];

	out[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0] + a[1][2] * b[2][0] + a[1][3] * b[3][0];
	out[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1] + a[1][2] * b[2][1] + a[1][3] * b[3][1];
	out[1][2] = a[1][0] * b[0][2] + a[1][1] * b[1][2] + a[1][2] * b[2][2] + a[1][3] * b[3][2];
	out[1][3] = a[1][0] * b[0][3] + a[1][1] * b[1][3] + a[1][2] * b[2][3] + a[1][3] * b[3][3];

	out[2][0] = a[2][0] * b[0][0] + a[2][1] * b[1][0] + a[2][2] * b[2][0] + a[2][3] * b[3][0];
	out[2][1] = a[2][0] * b[0][1] + a[2][1] * b[1][1] + a[2][2] * b[2][1] + a[2][3] * b[3][1];
	out[2][2] = a[2][0] * b[0][2] + a[2][1] * b[1][2] + a[2][2] * b[2][2] + a[2][3] * b[3][2];
	out[2][3] = a[2][0] * b[0][3] + a[2][1] * b[1][3] + a[2][2] * b[2][3] + a[2][3] * b[3][3];

	out[3][0] = a[3][0] * b[0][0] + a[3][1] * b[1][0] + a[3][2] * b[2][0] + a[3][3] * b[3][0];
	out[3][1] = a[3][0] * b[0][1] + a[3][1] * b[1][1] + a[3][2] * b[2][1] + a[3][3] * b[3][1];
	out[3][2] = a[3][0] * b[0][2] + a[3][1] * b[1][2] + a[3][2] * b[2][2] + a[3][3] * b[3][2];
	out[3][3] = a[3][0] * b[0][3] + a[3][1] * b[1][3] + a[3][2] * b[2][3] + a[3][3] * b[3][3];
	return out;
}

f32x4x4& matrixInverse(f32x4x4& out, f32 det, const f32x4x4& a)
{
	runtime_assert(det != 0.f);

	out[0][0] = _minor<0, 0>(a);
	out[1][0] = -_minor<0, 1>(a);
	out[2][0] = _minor<0, 2>(a);
	out[3][0] = -_minor<0, 3>(a);

	out[0][1] = -_minor<1, 0>(a);
	out[1][1] = _minor<1, 1>(a);
	out[2][1] = -_minor<1, 2>(a);
	out[3][1] = _minor<1, 3>(a);

	out[0][2] = _minor<2, 0>(a);
	out[1][2] = -_minor<2, 1>(a);
	out[2][2] = _minor<2, 2>(a);
	out[3][2] = -_minor<2, 3>(a);

	out[0][3] = -_minor<3, 0>(a);
	out[1][3] = _minor<3, 1>(a);
	out[2][3] = -_minor<3, 2>(a);
	out[3][3] = _minor<3, 3>(a);

    det = 1.0f / det;
    out *= det;

	return out;
}


f32x4x4& matrixInverse(f32x4x4& out, const f32x4x4& a)
{
	out[0][0] = _minor<0, 0>(a);
	out[1][0] = -_minor<0, 1>(a);
	out[2][0] = _minor<0, 2>(a);
	out[3][0] = -_minor<0, 3>(a);

	out[0][1] = -_minor<1, 0>(a);
	out[1][1] = _minor<1, 1>(a);
	out[2][1] = -_minor<1, 2>(a);
	out[3][1] = _minor<1, 3>(a);

	out[0][2] = _minor<2, 0>(a);
	out[1][2] = -_minor<2, 1>(a);
	out[2][2] = _minor<2, 2>(a);
	out[3][2] = -_minor<2, 3>(a);

	out[0][3] = -_minor<3, 0>(a);
	out[1][3] = _minor<3, 1>(a);
	out[2][3] = -_minor<3, 2>(a);
	out[3][3] = _minor<3, 3>(a);

	f32 det = a[0][0] * out[0][0] + a[0][1] * out[1][0] + a[0][2] * out[2][0] + a[0][3] * out[3][0]; 
    runtime_assert(det != 0.f);

    det = 1.0f / det;
    out *= det;

	return out;
}

f32x4x4& matrixInverse(f32x4x4& out)
{
	f32x4x4 temp;
	
	out[0][0] = _minor<0, 0>(out);
	out[1][0] = -_minor<0, 1>(out);
	out[2][0] = _minor<0, 2>(out);
	out[3][0] = -_minor<0, 3>(out);

	out[0][1] = -_minor<1, 0>(out);
	out[1][1] = _minor<1, 1>(out);
	out[2][1] = -_minor<1, 2>(out);
	out[3][1] = _minor<1, 3>(out);

	out[0][2] = _minor<2, 0>(out);
	out[1][2] = -_minor<2, 1>(out);
	out[2][2] = _minor<2, 2>(out);
	out[3][2] = -_minor<2, 3>(out);

	out[0][3] = -_minor<3, 0>(out);
	out[1][3] = _minor<3, 1>(out);
	out[2][3] = -_minor<3, 2>(out);
	out[3][3] = _minor<3, 3>(out);

	f32 det = out[0][0] * temp[0][0] + out[0][1] * temp[1][0] + out[0][2] * temp[2][0] + out[0][3] * temp[3][0];
	runtime_assert(det != 0.f);

    det = 1.0f / det;
    out *= det;

	return out;
}

f32x4x4& matrixTranspose(f32x4x4& out, const f32x4x4& a)
{
	out[0][0] = a[0][0];
	out[0][1] = a[1][0];
	out[0][2] = a[2][0];
	out[0][3] = a[3][0];

	out[1][0] = a[0][1];
	out[1][1] = a[1][1];
	out[1][2] = a[2][1];
	out[1][3] = a[3][1];

	out[2][0] = a[0][2];
	out[2][1] = a[1][2];
	out[2][2] = a[2][2];
	out[2][3] = a[3][2];

	out[3][0] = a[0][3];
	out[3][1] = a[1][3];
	out[3][2] = a[2][3];
	out[3][3] = a[3][3];

	return out;
}

f32x4x4& matrixTranspose(f32x4x4& out)
{
	f32 t;

	t = out[0][1]; out[0][1] = out[1][0]; out[1][0] = t;
	t = out[0][2]; out[0][2] = out[2][0]; out[2][0] = t;
	t = out[0][3]; out[0][2] = out[3][0]; out[3][0] = t;

	t = out[1][0]; out[0][2] = out[0][1]; out[0][1] = t;
	t = out[1][2]; out[0][2] = out[2][1]; out[2][1] = t;
	t = out[1][3]; out[0][2] = out[3][1]; out[3][1] = t;

	t = out[2][0]; out[0][2] = out[0][2]; out[0][2] = t;
	t = out[2][1]; out[0][2] = out[1][2]; out[1][2] = t;
	t = out[2][3]; out[0][2] = out[3][2]; out[3][2] = t;

	t = out[3][0]; out[0][2] = out[0][3]; out[0][3] = t;
	t = out[3][1]; out[0][2] = out[1][3]; out[1][3] = t;
	t = out[3][2]; out[0][2] = out[2][3]; out[2][3] = t;

	return out;
}


f32 matrixDeterminant(const f32x4x4& a)
{
	f32 t0 = a[2][2] * a[3][3];
	f32 t1 = a[1][2] * a[2][3];
	f32 t2 = a[1][3] * a[3][2];
	f32 t3 = a[2][2] * a[1][3];
	f32 t4 = a[3][2] * a[2][3];
	f32 t5 = a[3][3] * a[1][2];

	f32 m0 = 
		a[1][1] * t0 +
        t1 * a[3][1] +
        t2 * a[2][1] -
        a[3][1] * t3 -
        t4 * a[1][1] -
        t5 * a[2][1];

	f32 m1 =
		a[1][0] * t0 +
        t1 * a[3][0] +
        t2 * a[2][0] -
        a[3][0] * t3 -
        t4 * a[1][0] -
        t5 * a[2][0];

	t0 = a[1][0] * a[2][1];
	t1 = a[1][1] * a[3][0];
	t2 = a[2][0] * a[3][1];
	t3 = a[3][0] * a[2][1];
	t4 = a[3][1] * a[1][0];
	t5 = a[2][0] * a[1][1];

	f32 m2 = 
		a[3][3] * t0 +
        a[2][3] * t1 +
        a[1][3] * t2 -
        t3 * a[1][3] -
        t4 * a[2][3] -
        a[3][3] * t5;

	f32 m3 =
		a[3][2] * t0 +
        a[2][2] * t1 +
        a[1][2] * t2 -
        t3 * a[1][2] -
        t4 * a[2][2] -
        a[3][2] * t5;

	t0 = 0;
	t0 += a[0][0] * m0;
	t0 += a[0][1] * -m1; 
	t0 += a[0][2] * m2;
	t0 += a[0][3] * -m3;

	return t0;
}

f32 matrixCofactor(const f32x4x4& a, s32 row, s32 col)
{
	static const _minorptr mtable[4][4] = {
		{ &_minor<0,0>, &_minor<0,1>, &_minor<0,2>, &_minor<0,3> },
		{ &_minor<1,0>, &_minor<1,1>, &_minor<1,2>, &_minor<1,3> },
		{ &_minor<2,0>, &_minor<2,1>, &_minor<2,2>, &_minor<2,3> },
		{ &_minor<3,0>, &_minor<3,1>, &_minor<3,2>, &_minor<3,3> }, };

	return ((row + col) % 2 ? -1.f : 1.f) * mtable[row][col](a);
}

f32 matrixMinor(const f32x4x4& a, s32 row, s32 col)
{
	static const _minorptr mtable[4][4] = {
		{ &_minor<0,0>, &_minor<0,1>, &_minor<0,2>, &_minor<0,3> },
		{ &_minor<1,0>, &_minor<1,1>, &_minor<1,2>, &_minor<1,3> },
		{ &_minor<2,0>, &_minor<2,1>, &_minor<2,2>, &_minor<2,3> },
		{ &_minor<3,0>, &_minor<3,1>, &_minor<3,2>, &_minor<3,3> }, };
	return mtable[row][col](a);
}

f32x4x4& matrixIdentity(f32x4x4& out)
{
	for(s32 i = 0; i < 16; ++i)
		(&out[0][0])[i] = 0.f;
	for(s32 i = 0; i < 4; ++i)
		out[i][i] = 1.f;
	return out;
}

f32x4x4& matrixClear(f32x4x4& out)
{
	for(s32 i = 0; i < 16; ++i)
		(&out[0][0])[i] = 0.f;
	return out;
}

} //namespace sparrow
