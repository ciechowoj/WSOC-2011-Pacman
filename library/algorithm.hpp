#pragma once
#include <algorithm>
#include "stlbase.hpp"
namespace sparrow {

template <class SrcItr, class DestItr> struct _plain_old_data
{
	typedef typename typename iterator_traits<SrcItr>::value_type src_type;
	typedef typename typename iterator_traits<DestItr>::value_type dest_type;

	template <bool pod> __forceinline static DestItr uninitialized_copy(SrcItr first, SrcItr last, DestItr result)
	{
		while(first < last)
		{
			::new (&*result) dest_type(*first);
			++first;
			++result;
		}
		return result;
	}

	template <> __forceinline static DestItr uninitialized_copy<true>(SrcItr first, SrcItr last, DestItr result)
	{
		if(first < last)
			std::memcpy(&*result, &*first, (last - first) * sizeof(dest_type));
		return result + (last - first);
	}
	
	template <bool pod> __forceinline static DestItr uninitialized_copy_n(SrcItr first, u32 n, DestItr result)
	{
		while(n > 0)
		{
			::new (&*result) dest_type(*first);
			++result;
			++first;
			--n;
		}
		return result;
	}
	
	template <> __forceinline static DestItr uninitialized_copy_n<true>(SrcItr first, u32 n, DestItr result)
	{
		if(n != 0)
			std::memcpy(&*result, &*first, n * sizeof(dest_type));
		return result + n;
	}

	template <bool arithmetic, class T> __forceinline static DestItr uninitialized_fill(DestItr first, DestItr last, const T& x)
	{
		while(first < last)
		{
			::new (&*first) dest_type(x);
			++first;
		}
		return first;
	}

	template <bool arithmetic, class T> __forceinline static DestItr uninitialized_fill_n(DestItr first, u32 n, const T& x)
	{
		while(n != 0)
		{
			::new (&*first) dest_type(x);
			++first;
			--n;
		}
		return first;
	}

	template <bool arithmetic> __forceinline static void construct(DestItr first, DestItr last)
	{
		while(first < last)
		{
			::new (&*first) dest_type();
			++first;
		}
	}

	template <> __forceinline static void construct<true>(DestItr first, DestItr last)
	{ 
		if(first < last)
			std::memset(&*first, 0, (last - first) * sizeof(dest_type));
	}

	template <bool destructible> __forceinline static void construct_n(DestItr first, u32 n)
	{
		DestItr last = first + n;
		while(first != last)
		{
			::new (&*first) dest_type();
			++first;
		}
	}

	template <> __forceinline static void construct_n<true>(DestItr first, u32 n)
	{ 
		std::memset(&*first, 0, n * sizeof(dest_type));
	}

	template <bool destructible> __forceinline static void destruct(DestItr first, DestItr last)
	{
		while(first < last)
		{
			(&*first)->~dest_type();
			++first;
		}
	}

	template <> __forceinline static void destruct<true>(DestItr first, DestItr last)
	{ }

	template <bool destructible> __forceinline static void destruct_n(DestItr first, u32 n)
	{
		DestItr last = first + n;
		while(first != last)
		{
			(&*first)->~dest_type();
			++first;
		}
	}

	template <> __forceinline static void destruct_n<true>(DestItr first, u32 n)
	{ }
};

template <class SrcItr, class DestItr> inline DestItr uninitialized_copy(SrcItr first, SrcItr last, DestItr result)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	return _plain_old_data<SrcItr, DestItr>::uninitialized_copy<is_trivially_copyable<T>::value>(first, last, result);
}

template <class SrcItr, class Size, class DestItr> inline DestItr uninitialized_copy_n(SrcItr first, Size n, DestItr result)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	return _plain_old_data<SrcItr, DestItr>::uninitialized_copy_n<is_trivially_copyable<T>::value>(first, static_cast<u32>(n), result);
}

template <class DestItr, class T> inline void uninitialized_fill(DestItr first, DestItr last, const T& x)
{
	typedef typename iterator_traits<DestItr>::value_type T1;
	_plain_old_data<DestItr, DestItr>::uninitialized_fill<is_trivially_copyable<T1>::value, T1>(first, last, x);
}

template <class DestItr, class Size, class T> inline DestItr uninitialized_fill_n(DestItr first, Size n, const T& x)
{
	typedef typename iterator_traits<DestItr>::value_type T1;
	return _plain_old_data<DestItr, DestItr>::uninitialized_fill_n<is_trivially_copyable<T1>::value, T1>(first, static_cast<u32>(n), x);
}

template <class DestItr, class T> inline void construct(DestItr first, DestItr last, const T& x)
{
	typedef typename iterator_traits<DestItr>::value_type T1;
	_plain_old_data<DestItr, DestItr>::uninitialized_fill<is_trivially_copyable<T1>::value, T1>(first, last, x);
}

template <class DestItr, class Size, class T> inline DestItr construct_n(DestItr first, Size n, const T& x)
{
	typedef typename iterator_traits<DestItr>::value_type T1;
	return _plain_old_data<DestItr, DestItr>::uninitialized_fill_n<is_trivially_copyable<T1>::value, T1>(first, static_cast<u32>(n), x);
}

template <class DestItr> inline void construct(DestItr first, DestItr last)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	_plain_old_data<DestItr, DestItr>::construct<is_trivially_copyable<T>::value>(first, last);
}

template <class DestItr, class T> inline void construct(DestItr first, const T& x)
{
	typedef typename iterator_traits<DestItr>::value_type T1;
	new (&*first) T1(x);
}

template <class DestItr> inline void construct(DestItr first)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	new (&*first) T;
}

template <class DestItr, class Size> inline void construct_n(DestItr first, Size n)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	_plain_old_data<DestItr, DestItr>::construct_n<is_arithmetic<T>::value>(first, static_cast<u32>(n));
}

template <class DestItr> inline void destruct(DestItr first, DestItr last)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	_plain_old_data<DestItr, DestItr>::destruct<is_trivially_destructible<T>::value>(first, last);
}

template <class DestItr> inline void destruct(DestItr first)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	(&*first)->~T();
}

template <class DestItr, class Size> inline void destruct_n(DestItr first, Size n)
{
	typedef typename iterator_traits<DestItr>::value_type T;
	_plain_old_data<DestItr, DestItr>::destruct_n<is_trivially_destructible<T>::value>(first, static_cast<u32>(n));
}

template<class Iterator, class T> inline Iterator lower_bound(Iterator begin, Iterator end, const T& value)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(*half < value)
			begin = half + 1;
		else
			end = half;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template<class Iterator, class T, class Compare> inline Iterator lower_bound(Iterator begin, Iterator end, const T& value, Compare comp)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(comp(*half, value))
			begin = half + 1;
		else
			end = half;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template<class Iterator, class T> inline Iterator upper_bound(Iterator begin, Iterator end, const T& value)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(!(value < *half))
			begin = half + 1;
		else
			end = half;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template<class Iterator, class T, class Compare> inline Iterator upper_bound(Iterator begin, Iterator end, const T& value, Compare comp)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(!comp(value, *half))
			begin = half + 1;
		else
			end = half;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template <class Iterator, class T> inline pair<Iterator, Iterator> equal_range(Iterator first, Iterator last, const T& value)
{
	return pair<Iterator, Iterator>(lower_bound(first, last, value), upper_bound(first, last, value));
}

template <class Iterator, class T, class Compare> inline pair<Iterator, Iterator> equal_range(Iterator first, Iterator last, const T& value, Compare comp)
{
	return pair<Iterator, Iterator>(lower_bound(first, last, value, comp), upper_bound(first, last, value, comp));
}

template <class Iterator, class T> inline Iterator binary_search_i(Iterator begin, Iterator end, const T& value)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(*half < value)
			begin = half + 1;
		else if(value < *half)
			end = half;
		else
			break;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template <class Iterator, class T, class Compare> inline Iterator binary_search_i(Iterator begin, Iterator end, const T& value, Compare comp)
{
	Iterator half = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(comp(*half, value))
			begin = half + 1;
		else if(comp(value, *half))
			end = half;
		else
			break;
		half = begin + (end - begin) / 2;
	}
	return half;
}

template <class IteratorA, class IteratorB> inline IteratorB copy_forward(IteratorA first, IteratorA last, IteratorB result)
{
	  while(first < last)
	  {
		  *result = *first;
		  ++result;
		  ++first;
	  }
	  return result;
}

template <class IteratorA, class IteratorB> inline IteratorB copy(IteratorA first, IteratorA last, IteratorB result)
{
	return copy_forward(first, last, result);
}

template <class IteratorA, class IteratorB> inline IteratorB copy_backward(IteratorA first, IteratorA last, IteratorB result)
{
	  while(first < last)
	  {
		  --result;
		  --last;
		  *result = *last;
	  }
	  return result;
}

template <class Iterator, class Compare> inline void quicksort(Iterator begin, Iterator end, Compare compare)
{
	std::sort(begin, end, compare);
}

template <class Iterator> inline void quicksort(Iterator begin, Iterator end)
{
	std::sort(begin, end);
}

template <class IteratorA> inline IteratorA rotate(IteratorA first, IteratorA middle, IteratorA last)
{
	iterator_traits<IteratorA>::value_type temp;
	IteratorA begin, end;

	begin = first;
	end = middle;
	while(begin < end)
	{
		--end;
		temp = *begin;
		*begin = *end;
		*end = temp;
		++begin;
	}

	begin = middle;
	end = last;
	while(begin < end)
	{
		--end;
		temp = *begin;
		*begin = *end;
		*end = temp;
		++begin;
	}

	begin = first;
	end = last;
	while(begin < end)
	{
		--end;
		temp = *begin;
		*begin = *end;
		*end = temp;
		++begin;
	}
	return first + (last - middle);
}

} //namespace sparrow
