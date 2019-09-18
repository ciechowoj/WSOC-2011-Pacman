#pragma once
#include "config.hpp"
namespace sparrow {

template <class T> class __AABR
{
public:
	T					left, top, right, bottom;

						__AABR()
	{ }
						__AABR(const __vec2<T>& leftTop, const __vec2<T>& rightBottom)
		: left(leftTop.x)
		, top(leftTop.y)
		, right(rightBottom.x)
		, bottom(rightBottom.y)
	{ }

						__AABR(const T& l, const T& t, const T& r, const T& b)
		: left(l)
		, top(t)
		, right(r)
		, bottom(b)
	{ }

	explicit			__AABR(const __vec4<T>& that)
		: left(that.x)
		, top(that.y)
		, right(that.z)
		, bottom(that.w)
	{ }

	template <class A>  __AABR(const __AABR<A>& that)
		: left(that.left)
		, top(that.top)
		, right(that.right)
		, bottom(that.bottom)
	{ }

	template <class A> explicit __AABR(const __vec4<A>& that)
		: left((T)that.x)
		, top((T)that.y)
		, right((T)that.z)
		, bottom((T)that.w)
	{ }

	const __vec2<T>&	leftTop() const
	{
		return ((const __vec2<T>*)(this))[0];
	}

	__vec2<T>			rightTop() const
	{
		return __vec2<T>(right, top);
	}

	__vec2<T>			leftBottom() const
	{
		return __vec2<T>(left, bottom);
	}

	const __vec2<T>&	rightBottom() const
	{
		return ((const __vec2<T>*)(this))[1];
	}

	void				leftTop(const __vec2<T>& left_top)
	{
		left = left_top.x;
		top = left_top.y;
	}

	void				rightTop(const __vec2<T>& right_top)
	{
		right = right_top.x;
		top = right_top.y;
	}

	void				leftBottom(const __vec2<T>& left_bottom)
	{
		left = left_bottom.x;
		bottom = left_bottom.y;
	}

	void				rightBottom(const __vec2<T>& right_bottom)
	{
		right = right_bottom.x;
		bottom = right_bottom.y;
	}

	bool				operator==(const __AABR& that) const
	{
		return left = that.left && top == that.top && right == that.right && bottom == that.bottom;
	}

	bool				operator!=(const __AABR& that) const
	{
		return left != that.left || top != that.top || right != that.right || bottom != that.bottom;
	}

	bool				operator<(const __AABR& that) const
	{
		return left < that.left && top < that.top && right < that.right && bottom < that.bottom;
	}

	bool				operator>(const __AABR& that) const
	{
		return left > that.left && top > that.top && right > that.right && bottom > that.bottom;
	}

	bool				operator<=(const __AABR& that) const
	{
		return left <= that.left && top <= that.top && right <= that.right && bottom <= that.bottom;
	}

	bool				operator>=(const __AABR& that) const
	{
		return left >= that.left && top >= that.top && right >= that.right && bottom => that.bottom;
	}
};

typedef __AABR<float> AABRf;
typedef __AABR<short> AABRs;
typedef __AABR<s32> AABRi;

} //namespace sparrow
