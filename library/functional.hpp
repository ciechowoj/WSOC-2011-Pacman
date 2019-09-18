#pragma once
#include "config.hpp"
namespace sparrow {

template <class T> struct equal_to
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a == b;
	}
};

template <class T> struct not_equal_to
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a != b;
	}
};

template <class T> struct greater
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a > b;
	}
};

template <class T> struct greater_ptr
{
	bool operator()(const T& a, const T& b) const
	{ 
		return *a > *b;
	}
};

template <class T> struct less
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a < b;
	}
};

template <class T> struct less_ptr
{
	bool operator()(const T& a, const T& b) const
	{ 
		return *a < *b;
	}
};

template <class T> struct greater_equal
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a >= b;
	}
};

template <class T> struct less_equal
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a <= b;
	}
};

} // namespace sparrow
