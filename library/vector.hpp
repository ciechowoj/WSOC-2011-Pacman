#pragma once
#include "type_traits.hpp"
#include "functional.hpp"
#include "memory.hpp"
#include "streams.hpp"
namespace sparrow {
template <class T> class vector;
template <class T, class C> class vector_set;
template <class T, class C> class vector_multiset;
template <class T, class K, class C> class vector_map;
template <class T, class K, class C> class vector_multimap;
namespace impl {

template <class T> class vec_rev_itr
{
public:
	typedef s32			diff_type;

						vec_rev_itr();
	template <class U>	vec_rev_itr(const vec_rev_itr<U>& that);
	template <class U>	vec_rev_itr& operator=(const vec_rev_itr<U>& that);

	T&					operator*() const;
	T*					operator->() const;
	vec_rev_itr&		operator++();
	vec_rev_itr			operator++(s32);
	vec_rev_itr&		operator--();
	vec_rev_itr			operator--(s32);

	vec_rev_itr			operator+(diff_type n) const;
	vec_rev_itr			operator-(diff_type n) const;
	vec_rev_itr&		operator+=(diff_type n);
	vec_rev_itr&		operator-=(diff_type n);
	T&					operator[](diff_type n) const;

	template <class U> bool operator==(const vec_rev_itr<U>& that) const;
	template <class U> bool operator!=(const vec_rev_itr<U>& that) const;
	template <class U> bool operator<(const vec_rev_itr<U>& that) const;
	template <class U> bool operator>(const vec_rev_itr<U>& that) const;
	template <class U> bool operator<=(const vec_rev_itr<U>& that) const;
	template <class U> bool operator>=(const vec_rev_itr<U>& that) const;
private:
	T*					_current;
	explicit			vec_rev_itr(T* that);
	template <class A, class B> friend s32 operator-(const vec_rev_itr<A>& x, const vec_rev_itr<B>& y);
	template <class U> friend vec_rev_itr<U> operator+(typename vec_rev_itr<U>::diff_type n, const vec_rev_itr<U>& i);
	friend class sparrow::vector<T>;
};

bool vec_serialize_header(u32 size, DataStream* stream);
bool vec_deserialize_header(u32& size, const DataStream* stream);
bool vecset_serialize_header(u32 size, DataStream* stream);
bool vecset_deserialize_header(u32& size, const DataStream* stream);
bool vecmultiset_serialize_header(u32 size, DataStream* stream);
bool vecmultiset_deserialize_header(u32& size, const DataStream* stream);
bool vecmap_serialize_header(u32 size, DataStream* stream);
bool vecmap_deserialize_header(u32& size, const DataStream* stream);

void vec_move(void* dest, const void* src, u32 size);
void vec_copy(void* dest, const void* src, u32 size);

template <class T, bool pod> class vec_base
{
protected:
	static void			_constructCopy(T* dest, const T* src, s32 size);
	static void			_constructInit(T* dest, s32 size, const T& object);
	static void			_constructInit(T* dest, const T& object);
	template <class I> static void _constructItr(T* dest, I first, I last);
	static void			_constructDef(T* dest, s32 size);
	static void			_constructDef(T* dest);
	static void			_destruct(const T* data, s32 size);
	static void			_destruct(const T* data);
	static void			_move(T* dest, const T* src, s32 size);
	static void			_copy(T* dest, const T* src, s32 size);
	static void			_serialize(u32 size, const T* data, DataStream* stream);
	static void			_deserialize(u32 size, T* data, const DataStream* stream);
};

template <class T> class vec_base<T, true>
{
protected:
	static void			_constructCopy(T* dest, const T* src, s32 size);
	static void			_constructInit(T* dest, s32 size, const T& object);
	static void			_constructInit(T* dest, const T& object);
	template <class I> static void _constructItr(T* dest, I first, I last);
	static void			_constructDef(T* dest, s32 size);
	static void			_constructDef(T* dest);
	static void			_destruct(const T* data, s32 size);
	static void			_destruct(const T* data);
	static void			_move(T* dest, const T* src, s32 size);
	static void			_copy(T* dest, const T* src, s32 size);
	static void			_serialize(u32 size, const T* data, DataStream* stream);
	static void			_deserialize(u32 size, T* data, const DataStream* stream);
};

template <class T> struct def_less_comp
{
	bool operator()(const T& a, const T& b) const
	{ 
		return a < b;
	}
};

} // namespace impl

template <class T> class vector : public impl::vec_base<T, is_trivially_copyable<T>::value>
{
public:
	typedef T			value_type;
	typedef u32			size_type;
	typedef s32			diff_type;

	typedef T*			iterator;
	typedef const T*	const_iterator;
	typedef impl::vec_rev_itr<T> reverse_iterator;
	typedef impl::vec_rev_itr<const T> const_reverse_iterator;

	explicit			vector(u32 reserved = 0);
	explicit			vector(const shared<Allocator>& allocator, u32 reserved = 0);
						vector(const vector<T>& that, u32 reserved = 0);
						vector(const vector<T>& that, const shared<Allocator>& allocator, u32 reserved = 0);
						~vector();

	vector<T>&			operator=(const vector<T>& that);

	shared<Allocator>&	get_allocator() const;
	void				set_allocator(const shared<Allocator>& allocator);

	iterator			begin();
	const_iterator		begin() const;
	iterator			end();
	const_iterator		end() const;
	reverse_iterator	rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator	rend();
	const_reverse_iterator rend() const;
	const_iterator		cbegin() const;
	const_iterator		cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	size_type			size() const;
	size_type			capacity() const;
	size_type			reserved() const;
	bool				empty() const;

	void				resize(size_type new_size, const T& object);
	void				resize(size_type new_size);
	void				reserve(size_type to_reserve);
	void				shrink_to_fit();

	T&					operator[](size_type index);
	const T&			operator[](size_type index) const;
	T&					at(size_type index);
	const T&			at(size_type index) const;
	T&					front();
	const T&			front() const;
	T&					back();
	const T&			back() const;
	T*					data();
	const T*			data() const;
	
	void				assign(size_type count, const T& object);
	void				assign(size_type count);

	void				push_back(size_type count, const T& object);
	void				push_back_def(size_type count);
	void				push_back(const T& object);
	void				push_back_def();
	void				pop_back(size_type count);
	void				pop_back();

	iterator			insert(const_iterator position, size_type count, const T& object);
	iterator			insert_def(const_iterator position, size_type count);
	iterator			insert(const_iterator position, const T& object);
	iterator			insert_def(const_iterator position);
	
	iterator			erase(const_iterator first, const_iterator last);
	iterator			erase(const_iterator position);
	iterator			erase_unordered(const_iterator first, const_iterator last);
	iterator			erase_unordered(const_iterator position);
	

	template <class Iterator> iterator insert(const_iterator position, Iterator first, Iterator last);
	template <class Iterator> void assign(Iterator first, Iterator last);

	void				swap(vector& that);
	void				clear();
	void				reset();

	template <class Y> friend void serialize(const vector<Y>& object, DataStream* stream);
	template <class Y> friend void deserialize(vector<Y>& object, const DataStream* stream);
	template <class X, class Y> friend void serialize(const vector_set<X, Y>& object, DataStream* stream);
	template <class X, class Y> friend void deserialize(vector_set<X, Y>& object, const DataStream* stream);
	template <class X, class Y> friend void serialize(const vector_multiset<X, Y>& object, DataStream* stream);
	template <class X, class Y> friend void deserialize(vector_multiset<X, Y>& object, const DataStream* stream);
	template <class X, class Y, class Z> friend void serialize(const vector_map<X, Y, Z>& object, DataStream* stream);
	template <class X, class Y, class Z> friend void deserialize(vector_map<X, Y, Z>& object, const DataStream* stream);
private:
	T*					_data;
	size_type			_size;
	size_type			_capacity;
	size_type			_reserved;
	mutable shared<Allocator> _allocator;
};

template <class T, class C = less<T>> class vector_set 
{
public:
	typedef T			value_type;
	typedef C			comp_type;
	typedef u32			size_type;
	typedef s32			diff_type;

	typedef typename vector<T>::iterator iterator;
	typedef typename vector<T>::const_iterator const_iterator;
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::const_reverse_iterator const_reverse_iterator;

	explicit			vector_set(u32 reserved = 0);
	explicit			vector_set(const shared<Allocator>& allocator, u32 reserved = 0);
						vector_set(const vector_set<T>& that, u32 reserved = 0);
						vector_set(const vector_set<T>& that, const shared<Allocator>& allocator, u32 reserved = 0);
						~vector_set();

	vector_set&			operator=(const vector_set& that);

	shared<Allocator>&	get_allocator() const;
	void				set_allocator(const shared<Allocator>& allocator);

	iterator			begin();
	const_iterator		begin() const;
	iterator			end();
	const_iterator		end() const;
	reverse_iterator	rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator	rend();
	const_reverse_iterator rend() const;
	const_iterator		cbegin() const;
	const_iterator		cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
	size_type			size() const;
	size_type			capacity() const;
	size_type			reserved() const;
	bool				empty() const;
	const C&			comp() const;

	void				reserve(size_type to_reserve);

	T&					front();
	const T&			front() const;
	T&					back();
	const T&			back() const;

	iterator			insert(const T& object);
	iterator			insert_def();
	iterator			insert(const T& object, const_iterator hint);
	iterator			insert_def(const_iterator hint);

	iterator			erase(const_iterator first, const_iterator last);
	iterator			erase(const_iterator position);
	size_type			erase(const T& object);
	
	template <class Iterator> void insert_set(Iterator first, Iterator last);
	template <class Iterator> void erase_set(Iterator first, Iterator last);
	template <class Iterator> void assign_set(Iterator first, Iterator last);

	void				swap(vector_set& that);
	void				clear();
	void				reset();

	iterator			find(const T& object);
	const_iterator		find(const T& object) const;
	size_type			count(const T& object) const;

	template <class X, class Y> friend void serialize(const vector_set<X, Y>& object, DataStream* stream);
	template <class X, class Y> friend void deserialize(vector_set<X, Y>& object, const DataStream* stream);
private:
	vector<T>			_vector;
	C					_compare;
};

template <class T, class C = less<T>> class vector_multiset 
{
public:
	typedef T			value_type;
	typedef C			comp_type;
	typedef u32			size_type;
	typedef s32			diff_type;

	typedef typename vector<T>::iterator iterator;
	typedef typename vector<T>::const_iterator const_iterator;
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::const_reverse_iterator const_reverse_iterator;

	explicit			vector_multiset(u32 reserved = 0);
	explicit			vector_multiset(const shared<Allocator>& allocator, u32 reserved = 0);
						vector_multiset(const vector_multiset<T>& that, u32 reserved = 0);
						vector_multiset(const vector_multiset<T>& that, const shared<Allocator>& allocator, u32 reserved = 0);
						~vector_multiset();

	vector_multiset&			operator=(const vector_multiset& that);

	shared<Allocator>&	get_allocator() const;
	void				set_allocator(const shared<Allocator>& allocator);

	iterator			begin();
	const_iterator		begin() const;
	iterator			end();
	const_iterator		end() const;
	reverse_iterator	rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator	rend();
	const_reverse_iterator rend() const;
	const_iterator		cbegin() const;
	const_iterator		cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
	
	size_type			size() const;
	size_type			capacity() const;
	size_type			reserved() const;
	bool				empty() const;
	const C&			comp() const;

	void				reserve(size_type to_reserve);

	T&					front();
	const T&			front() const;
	T&					back();
	const T&			back() const;

	iterator			insert(const T& object);
	iterator			insert_def();
	iterator			insert(const T& object, const_iterator hint);
	iterator			insert_def(const_iterator hint);

	iterator			erase(const_iterator first, const_iterator last);
	iterator			erase(const_iterator position);
	size_type			erase(const T& object);
	
	template <class Iterator> void insert_set(Iterator first, Iterator last);
	template <class Iterator> void erase_set(Iterator first, Iterator last);
	template <class Iterator> void assign_set(Iterator first, Iterator last);

	void				swap(vector_multiset& that);
	void				clear();
	void				reset();

	iterator			find(const T& object);
	const_iterator		find(const T& object) const;
	size_type			count(const T& object) const;

	template <class X, class Y> friend void serialize(const vector_multiset<X, Y>& object, DataStream* stream);
	template <class X, class Y> friend void deserialize(vector_multiset<X, Y>& object, const DataStream* stream);
private:
	vector<T>			_vector;
	C					_compare;
};


template <class K, class T, class C = less<K>> class vector_map 
{
public:
	typedef K			key_type;
	typedef T			value_type;
	typedef C			comp_type;
	typedef u32			size_type;
	typedef s32			diff_type;

	typedef typename vector<T>::iterator iterator;
	typedef typename vector<T>::const_iterator const_iterator;
	typedef typename vector<T>::reverse_iterator reverse_iterator;
	typedef typename vector<T>::const_reverse_iterator const_reverse_iterator;

	explicit			vector_map(u32 reserved = 0);
	explicit			vector_map(const shared<Allocator>& allocator, u32 reserved = 0);
						vector_map(const vector_map& that, u32 reserved = 0);
						vector_map(const vector_map& that, const shared<Allocator>& allocator, u32 reserved = 0);
						~vector_map();

	vector_map&			operator=(const vector_map& that);

	shared<Allocator>&	get_allocator() const;
	void				set_allocator(const shared<Allocator>& allocator);

	iterator			begin();
	const_iterator		begin() const;
	iterator			end();
	const_iterator		end() const;
	reverse_iterator	rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator	rend();
	const_reverse_iterator rend() const;
	const_iterator		cbegin() const;
	const_iterator		cend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	size_type			size() const;
	size_type			capacity() const;
	size_type			reserved() const;
	bool				empty() const;
	const C&			comp() const;

	void				reserve(size_type to_reserve);

	T&					operator[](const K& key);
	const T&			operator[](const K& key) const;
	T&					at(const K& key);
	const T&			at(const K& key) const;
	T&					at(const K& key, T& def_object);
	const T&			at(const K& key, const T& def_object) const;
	const K&			key_at(const_iterator itr) const;
	T&					front();
	const T&			front() const;
	T&					back();
	const T&			back() const;

	iterator			insert(const K& key, const T& object);
	iterator			insert_def(const K& key);
	iterator			insert(const K& key, const T& object, const_iterator hint);
	iterator			insert_def(const K& key, const_iterator hint);

	iterator			erase(const_iterator first, const_iterator last);
	iterator			erase(const_iterator position);
	size_type			erase(const K& key);
	
	void				swap(vector_map& that);
	void				clear();
	void				reset();

	iterator			find(const K& key);
	const_iterator		find(const K& key) const;
	size_type			count(const K& key) const;

	template <class X, class Y, class Z> friend void serialize(const vector_map<X, Y, Z>& object, DataStream* stream);
	template <class X, class Y, class Z> friend void deserialize(vector_map<X, Y, Z>& object, const DataStream* stream);
private:
	vector<K>			_kVector;
	vector<T>			_tVector;
	C					_compare;
};

//- inline impls --------------------------------------------------------------------------------------------------------------
namespace impl {

template <class T> inline vec_rev_itr<T>::vec_rev_itr() 
{ }

template <class T> template <class U> inline vec_rev_itr<T>::vec_rev_itr(const vec_rev_itr<U>& that)
	: _current(that._current)
{ }

template <class T> template <class U> inline vec_rev_itr<T>& vec_rev_itr<T>::operator=(const vec_rev_itr<U>& that)
{
	_current = that._current;
	return *this;
}

template <class T> inline T& vec_rev_itr<T>::operator*() const
{
	return *_current;
}

template <class T> inline T* vec_rev_itr<T>::operator->() const
{
	return _current;
}

template <class T> inline vec_rev_itr<T>& vec_rev_itr<T>::operator++()
{
	--_current;
	return *this;
}

template <class T> inline vec_rev_itr<T> vec_rev_itr<T>::operator++(s32)
{
	vec_rev_itr<T> temp = *this;
	--_current;
	return temp;
}

template <class T> inline vec_rev_itr<T>& vec_rev_itr<T>::operator--()
{
	++_current;
	return *this;
}

template <class T> inline vec_rev_itr<T> vec_rev_itr<T>::operator--(s32)
{
	vec_rev_itr<T> temp = *this;
	++_current;
	return temp;
}

template <class T> inline vec_rev_itr<T> vec_rev_itr<T>::operator+(diff_type n) const
{
	return vec_rev_itr<T>(_current - n);
}

template <class T> inline vec_rev_itr<T> vec_rev_itr<T>::operator-(diff_type n) const
{
	return vec_rev_itr<T>(_current + n);
}

template <class T> inline vec_rev_itr<T>& vec_rev_itr<T>::operator+=(diff_type n)
{
	_current -= n;
	return *this;
}

template <class T> inline vec_rev_itr<T>& vec_rev_itr<T>::operator-=(diff_type n)
{
	_current += n;
	return *this;
}

template <class T> inline T& vec_rev_itr<T>::operator[](diff_type n) const
{
	return _current[-n];
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator==(const vec_rev_itr<U>& that) const
{
	return _current == that._current;
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator!=(const vec_rev_itr<U>& that) const
{
	return _current != that._current;
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator<(const vec_rev_itr<U>& that) const
{
	return _current > that._current;
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator>(const vec_rev_itr<U>& that) const
{
	return _current < that._current;
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator<=(const vec_rev_itr<U>& that) const
{
	return _current >= that._current;
}

template <class T> template <class U> inline bool vec_rev_itr<T>::operator>=(const vec_rev_itr<U>& that) const
{
	return _current <= that._current;
}

template <class T> inline vec_rev_itr<T>::vec_rev_itr(T* that)
	: _current(that)
{ }

template <class A, class B> inline s32 operator-(const vec_rev_itr<A>& x, const vec_rev_itr<B>& y)
{
	return y._current - x._current;
}

template <class U> inline vec_rev_itr<U> operator+(typename vec_rev_itr<U>::diff_type n, const vec_rev_itr<U>& i)
{
	return vec_rev_itr<U>(i._current - n);
}

template <class T, bool pod> inline void vec_base<T, pod>::_constructCopy(T* dest, const T* src, s32 size)
{
	for(s32 i = 0; i < size; ++i)
		::new (dest + i) T(src[i]);
}

template <class T, bool pod> inline void vec_base<T, pod>::_constructInit(T* dest, s32 size, const T& object)
{
	for(s32 i = 0; i < size; ++i)
		::new (dest + i) T(object);
}

template <class T, bool pod> inline void vec_base<T, pod>::_constructInit(T* dest, const T& object)
{
	::new (dest) T(object);
}

template <class T, bool pod> template <class I> inline void vec_base<T, pod>::_constructItr(T* dest, I first, I last)
{
	while(first < last)
	{
		::new (dest) T(*first);
		++dest;
		++first;
	}
}

template <class T, bool pod> inline void vec_base<T, pod>::_constructDef(T* dest, s32 size)
{
	for(s32 i = 0; i < size; ++i)
		::new (dest + i) T();
}

template <class T, bool pod> inline void vec_base<T, pod>::_constructDef(T* dest)
{
	::new (dest) T();
}

template <class T, bool pod> inline void vec_base<T, pod>::_destruct(const T* data, s32 size)
{
	for(s32 i = 0; i < size; ++i)
		(data + i)->~T();

}

template <class T, bool pod> inline void vec_base<T, pod>::_destruct(const T* data)
{
	data->~T();
}

template <class T, bool pod> inline void vec_base<T, pod>::_move(T* dest, const T* src, s32 size)
{
	for(s32 i = size - 1; i >= 0; --i)
		dest[i] = src[i];
}

template <class T, bool pod> inline void vec_base<T, pod>::_copy(T* dest, const T* src, s32 size)
{
	for(s32 i = 0; i < size; ++i)
		dest[i] = src[i];
}

template <class T, bool pod> inline void vec_base<T, pod>::_serialize(u32 size, const T* data, DataStream* stream)
{
	for(u32 i = 0; i < size; ++i)
		serialize(data[i], stream);
}

template <class T, bool pod> inline void vec_base<T, pod>::_deserialize(u32 size, T* data, const DataStream* stream)
{
	for(u32 i = 0; i < size; ++i)
	{
		::new (data + i) T();
		deserialize(data[i], stream);
	}
}

template <class T> inline void vec_base<T, true>::_constructCopy(T* dest, const T* src, s32 size)
{
	vec_copy(dest, src, size * sizeof T);
}

template <class T> inline void vec_base<T, true>::_constructInit(T* dest, s32 size, const T& object)
{
	for(s32 i = 0; i < size; ++i)
		dest[i] = object;
}

template <class T> inline void vec_base<T, true>::_constructInit(T* dest, const T& object)
{
	*dest = object;
}

template <class T> template <class I> inline void vec_base<T, true>::_constructItr(T* dest, I first, I last)
{
	s32 size = last - first;
	if(size > 0)
		vec_copy(dest, first, size * sizeof T);
}

template <class T> inline void vec_base<T, true>::_constructDef(T* dest, s32 size)
{ }

template <class T> inline void vec_base<T, true>::_constructDef(T* dest)
{ }

template <class T> inline void vec_base<T, true>::_destruct(const T* data, s32 size)
{ }

template <class T> inline void vec_base<T, true>::_destruct(const T* data)
{ }

template <class T> inline void vec_base<T, true>::_move(T* dest, const T* src, s32 size)
{
	vec_move(dest, src, size * sizeof T);
}

template <class T> inline void vec_base<T, true>::_copy(T* dest, const T* src, s32 size)
{
	vec_copy(dest, src, size * sizeof T);
}

template <class T> inline void vec_base<T, true>::_serialize(u32 size, const T* data, DataStream* stream)
{
	stream->write(data, size * sizeof T);
}

template <class T> inline void vec_base<T, true>::_deserialize(u32 size, T* data, const DataStream* stream)
{
	stream->read(data, size * sizeof T);
}

} // namespace impl

template <class T> 
inline vector<T>::vector(u32 reserved)
	: _data(nullptr)
	, _size(0)
	, _capacity(0)
	, _reserved(reserved)
	, _allocator(obtainGlobalAllocator())
{ }

template <class T> 
inline vector<T>::vector(const shared<Allocator>& allocator, u32 reserved)
	: _data(nullptr)
	, _size(0)
	, _capacity(0)
	, _reserved(reserved)
	, _allocator(allocator)
{ }

template <class T> 
inline vector<T>::vector(const vector<T>& that, u32 reserved)
	: _data(nullptr)
	, _size(that._size)
	, _capacity(0)
	, _reserved(reserved)
	, _allocator(obtainGlobalAllocator())
{
	_capacity = _reserved < _size ? _size : _reserved;
	_data = (T*)_allocator->alloc(_capacity * sizeof T);
	_constructCopy(_data, that._data, _size);
}

template <class T> 
inline vector<T>::vector(const vector<T>& that, const shared<Allocator>& allocator, u32 reserved)
	: _data(nullptr)
	, _size(x._size)
	, _capacity(0)
	, _reserved(reserved)
	, _allocator(allocator)
{
	_capacity = _reserved < _size ? _size : _reserved;
	_data = (T*)_allocator->alloc(_capacity * sizeof T);
	_constructCopy(_data, that._data, _size);
}

template <class T> 
inline vector<T>::~vector()
{
	_destruct(_data, _size);
	_allocator->free(_data, _capacity * sizeof T);
}

template <class T> 
inline vector<T>& vector<T>::operator=(const vector<T>& that)
{
	if(&that != this)
	{
		_destruct(_data, _size);

		u32 cap_limit = that._size * 2;
		cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
		if(_capacity < that._size || _capacity > cap_limit)
		{
			_allocator->free(_data, _capacity * sizeof T);
			_capacity = 0;
			if(that._size != 0)
			{
				_capacity = _reserved < that._size ? that._size : _reserved;
				_data = (T*)_allocator->alloc(_capacity * sizeof T);
			}
		}

		_size = that._size;
		_constructCopy(_data, that._data, _size);
	}
	return *this;
}

template <class T> 
inline shared<Allocator>& vector<T>::get_allocator() const
{
	return _allocator;
}

template <class T> 
inline void vector<T>::set_allocator(const shared<Allocator>& allocator)
{
	if(_allocator != allocator)
	{
		T* data = (T*)allocator->alloc(_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_allocator = allocator;
	}
}

template <class T> 
inline typename vector<T>::iterator vector<T>::begin()
{
	return _data;
}

template <class T> 
inline typename vector<T>::const_iterator vector<T>::begin() const
{
	return _data;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::end()
{
	return _data + _size;
}

template <class T> 
inline typename vector<T>::const_iterator vector<T>::end() const
{
	return _data + _size;
}

template <class T> 
inline typename vector<T>::reverse_iterator vector<T>::rbegin()
{
	return reverse_iterator(_data + _size - 1);
}

template <class T> 
inline typename vector<T>::const_reverse_iterator vector<T>::rbegin() const
{
	return const_reverse_iterator(_data + _size - 1);
}

template <class T> 
inline typename vector<T>::reverse_iterator vector<T>::rend()
{
	return reverse_iterator(_data - 1);
}

template <class T> 
inline typename vector<T>::const_reverse_iterator vector<T>::rend() const
{
	return const_reverse_iterator(_data - 1);
}

template <class T> 
inline typename vector<T>::const_iterator vector<T>::cbegin() const
{
	return begin();
}

template <class T> 
inline typename vector<T>::const_iterator vector<T>::cend() const
{
	return end();
}

template <class T> 
inline typename vector<T>::const_reverse_iterator vector<T>::crbegin() const
{
	return rbegin();
}

template <class T> 
inline typename vector<T>::const_reverse_iterator vector<T>::crend() const
{
	return rend();
}

template <class T> 
inline typename vector<T>::size_type vector<T>::size() const
{
	return _size;
}

template <class T> 
inline typename vector<T>::size_type vector<T>::capacity() const
{
	return _capacity;
}

template <class T> 
inline typename vector<T>::size_type vector<T>::reserved() const
{
	return _reserved;
}

template <class T> 
inline bool vector<T>::empty() const
{
	return _size == 0;
}

template <class T> 
inline void vector<T>::resize(size_type new_size, const T& object)
{
	if(_size != new_size)
	{
		u32 cap_limit = new_size * 2;
		cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
		if(_capacity < new_size || _capacity > cap_limit)
		{
			u32 new_capacity = new_size < _reserved ? _reserved : new_size;
			T* data = (T*)_allocator->alloc(new_capacity * sizeof T);

			if(_size < new_size)
			{
				_constructCopy(data, _data, _size);
				_constructInit(data + _size, new_size - _size, object);
			}
			else
			{
				_constructCopy(data, _data, new_size);
			}

			_destruct(_data, _size);
			_allocator->free(_data, _capacity * sizeof T);
			_data = data;
			_capacity = new_capacity;
		}
		else
		{
			_destruct(_data + new_size, _size - new_size);
			_constructInit(_data + _size, new_size - _size, object);
		}
		_size = new_size;
	}
}

template <class T> 
inline void vector<T>::resize(size_type new_size)
{
	if(_size != new_size)
	{
		u32 cap_limit = new_size * 2;
		cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
		if(_capacity < new_size || _capacity > cap_limit)
		{
			u32 new_capacity = new_size < _reserved ? _reserved : new_size;
			T* data = (T*)_allocator->alloc(new_capacity * sizeof T);

			if(_size < new_size)
			{
				_constructCopy(data, _data, _size);
				_constructDef(data + _size, new_size - _size);
			}
			else
			{
				_constructCopy(data, _data, new_size);
			}

			_destruct(_data, _size);
			_allocator->free(_data, _capacity * sizeof T);
			_data = data;
			_capacity = new_capacity;
		}
		else
		{
			_destruct(_data + new_size, _size - new_size);
			_constructDef(_data + _size, new_size - _size);
		}
		_size = new_size;
	}
}

template <class T> 
inline void vector<T>::reserve(size_type to_reserve)
{
	_reserved = to_reserve;
}

template <class T> 
inline void vector<T>::shrink_to_fit()
{
	if(_size != _capacity && _capacity > _reserved)
	{
		u32 new_capacity = _size < _reserved ? _reserved : _size;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
}

template <class T> 
inline T& vector<T>::operator[](size_type index)
{
	return _data[index];
}

template <class T> 
inline const T& vector<T>::operator[](size_type index) const
{
	return _data[index];
}

template <class T> 
inline T& vector<T>::at(size_type index)
{
	return _data[index];
}

template <class T> 
inline const T& vector<T>::at(size_type index) const
{
	return _data[index];
}

template <class T> 
inline T& vector<T>::front()
{
	return *_data;
}

template <class T> 
inline const T& vector<T>::front() const
{
	return *_data;
}

template <class T> 
inline T& vector<T>::back()
{
	return *(_data + _size - 1);
}

template <class T> inline const T& vector<T>::back() const
{
	return *(_data + _size - 1);
}

template <class T> 
inline T* vector<T>::data()
{
	return _data;
}

template <class T> 
inline const T* vector<T>::data() const
{
	return _data;
}

template <class T> 
inline void vector<T>::assign(size_type count, const T& object)
{
	_destruct(_data, _size);
	u32 cap_limit = count + count;
	cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
	if(_capacity < count || _capacity > cap_limit)
	{
		_allocator->free(_data, _capacity * sizeof T);
		if(count != 0)
		{
			_capacity = count < _reserved ? _reserved : count;
			_data = (T*)_allocator->alloc(_capacity * sizeof T);
		}
		else
		{
			_capacity = 0;
		}
	}
	_constructInit(_data, count, object);
}

template <class T> 
inline void vector<T>::assign(size_type count)
{
	_destruct(_data, _size);
	u32 cap_limit = count + count;
	cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
	if(_capacity < count || _capacity > cap_limit)
	{
		_allocator->free(_data, _capacity * sizeof T);
		if(count != 0)
		{
			_capacity = count < _reserved ? _reserved : count;
			_data = (T*)_allocator->alloc(_capacity * sizeof T);
		}
		else
		{
			_capacity = 0;
		}
	}
	_constructDef(_data, count);
}

template <class T> 
inline void vector<T>::push_back(size_type count, const T& object)
{
	u32 new_size = _size + count;
	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_constructInit(_data + _size, count, object);
	_size = new_size;
}

template <class T> 
inline void vector<T>::push_back_def(size_type count)
{
	u32 new_size = _size + count;
	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_constructDef(_data + _size, count);
	_size = new_size;
}

template <class T> 
inline void vector<T>::push_back(const T& object)
{
	u32 new_size = _size + 1;
	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_constructInit(_data + _size, object);
	_size = new_size;
}

template <class T> 
inline void vector<T>::push_back_def() 
{
	u32 new_size = _size + 1;
	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, _size);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_constructDef(_data + _size);
	_size = new_size;
}

template <class T> 
inline void vector<T>::pop_back(size_type count)
{
	u32 new_size = _size - count;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;
	_destruct(_data + new_size, count);
	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, new_size);
		_destruct(_data, new_size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_size = new_size;
}

template <class T> 
inline void vector<T>::pop_back()
{
	u32 new_size = _size - 1;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;
	_destruct(_data + new_size);
	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, new_size);
		_destruct(_data, new_size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	_size = new_size;
}


template <class T> 
inline typename vector<T>::iterator vector<T>::insert(const_iterator position, size_type count, const T& object)
{
	u32 new_size = _size + count;
	u32 index = position - _data;

	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructInit(data + index, count, object);
		_constructCopy(data + index + count, _data + index, _size - index);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_constructDef(_data + _size, count);
		_move(_data + index + count, _data + index, _size - index);
		_constructInit(_data + index, count, object);
	}

	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::insert_def(const_iterator position, size_type count)
{
	u32 new_size = _size + count;
	u32 index = position - _data;

	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructDef(data + index, count);
		_constructCopy(data + index + count, _data + index, _size - index);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_constructDef(_data + _size, count);
		_move(_data + index + count, _data + index, _size - index);
		_constructDef(_data + index, count);
	}

	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::insert(const_iterator position, const T& object)
{
	u32 new_size = _size + 1;
	u32 index = position - _data;

	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructInit(data + index, object);
		_constructCopy(data + index + 1, _data + index, _size - index);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_constructDef(_data + _size);
		_move(_data + index + 1, _data + index, _size - index);
		_constructInit(_data + index, object);
	}

	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::insert_def(const_iterator position)
{
	u32 new_size = _size + 1;
	u32 index = position - _data;

	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructDef(data + index);
		_constructCopy(data + index + 1, _data + index, _size - index);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_constructDef(_data + _size);
		_move(_data + index + 1, _data + index, _size - index);
		_constructDef(_data + index);
	}

	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::erase(const_iterator first, const_iterator last)
{
	u32 count = last - first;
	u32 new_size = _size - count;
	u32 index = first - _data;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;

	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructCopy(data + index, _data + index + count, _size - index - count);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_copy(_data + index, _data + index + count, _size - index - count);
		_destruct(_data + new_size, count);
	}
	
	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::erase(const_iterator position)
{
	u32 new_size = _size - 1;
	u32 index = position - _data;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;

	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructCopy(data + index, _data + index + 1, _size - index - 1);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_copy(_data + index, _data + index + 1, _size - index - 1);
		_destruct(_data + new_size);
	}
	
	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::erase_unordered(const_iterator first, const_iterator last)
{
	u32 count = last - first;
	u32 new_size = _size - count;
	u32 index = first - _data;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;

	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructCopy(data + index, _data + index + count, _size - index - count);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_copy(_data + index, _data + _size - count, count);
		_destruct(_data + _size - count, count);
	}
	
	_size = new_size;
	return _data + index;
}

template <class T> 
inline typename vector<T>::iterator vector<T>::erase_unordered(const_iterator position)
{
	u32 new_size = _size - 1;
	u32 index = position - _data;
	u32 cap_limit = new_size * 2 < _reserved ? _reserved : new_size * 2;

	if(_capacity > cap_limit)
	{
		u32 new_capacity = (new_size >> 2) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructCopy(data + index, _data + index + 1, _size - index - 1);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_copy(_data + index, _data + _size - 1, 1);
		_destruct(_data + _size - 1);
	}
	
	_size = new_size;
	return _data + index;
}

template <class T> 
template <class Iterator> inline typename vector<T>::iterator vector<T>::insert(const_iterator position, Iterator first, Iterator last)
{
	u32 count = last - first;
	u32 new_size = _size + count;
	u32 index = position - _data;

	if(new_size > _capacity)
	{
		u32 new_capacity = (new_size >> 1) + new_size;
		new_capacity = new_capacity < _reserved ? _reserved : new_capacity;
		T* data = (T*)_allocator->alloc(new_capacity * sizeof T);
		_constructCopy(data, _data, index);
		_constructItr(data + index, first, last);
		_constructCopy(data + index + count, _data + index, _size - index);
		_destruct(_data, _size);
		_allocator->free(_data, _capacity * sizeof T);
		_data = data;
		_capacity = new_capacity;
	}
	else
	{
		_constructDef(_data + _size, count);
		_move(_data + index + count, _data + index, _size - index);
		_constructItr(_data + index, first, last);
	}

	_size = new_size;
	return _data + index;
}

template <class T> 
template <class Iterator> inline void vector<T>::assign(Iterator first, Iterator last)
{
	_destruct(_data, _size);
	u32 count = last - first;
	u32 cap_limit = count + count;
	cap_limit = cap_limit < _reserved ? _reserved : cap_limit;
	if(_capacity < count || _capacity > cap_limit)
	{
		_allocator->free(_data, _capacity * sizeof T);
		if(count != 0)
		{
			_capacity = count < _reserved ? _reserved : count;
			_data = (T*)_allocator->alloc(_capacity * sizeof T);
		}
		else
		{
			_capacity = 0;
		}
	}
	_size = count;
	_constructItr(_data, first, last);
}

template <class T> 
inline void vector<T>::swap(vector<T>& that)
{
	sparrow::swap(_allocator, that._allocator);
	T* ptr; u32 val;
	ptr = _data;
	_data = that._data;
	that._data = ptr;

	val = _size;
	_size = that._size;
	that._size = val;

	val = _capacity;
	_capacity = that._size;
	that._capacity = val;

	val = _reserved;
	_reserved = that._reserved;
	that._reserved = val;
}

template <class T> 
inline void vector<T>::clear()
{
	_destruct(_data, _size);
	_size = 0;

	if(_capacity > _reserved)
	{
		_allocator->free(_data, _capacity * sizeof T);
		_capacity = 0;
		_data = nullptr;
	}
}

template <class T> 
inline void vector<T>::reset()
{
	_destruct(_data, _size);
	_allocator->free(_data, _capacity * sizeof T);
	_data = nullptr;
	_size = 0;
	_capacity = 0;
	_reserved = 0;
}

template <class Y>
void serialize(const vector<Y>& object, DataStream* stream)
{
	if(impl::vec_serialize_header(object._size, stream))
	{
		object._serialize(object._size, object._data, stream);
	}
}

template <class Y> 
void deserialize(vector<Y>& object, const DataStream* stream)
{
	object.clear();
	u32 new_size = 0;
	if(impl::vec_deserialize_header(new_size, stream))
	{
		if(object._capacity < new_size)
		{
			object._allocator->free(object._data, object._capacity * sizeof Y);
			object._data = (Y*)object._allocator->alloc(new_size * sizeof Y);
			object._capacity = new_size;
		}
		object._deserialize(new_size, object._data, stream);
		object._size = new_size;
	}
}

template <class T, class C> inline vector_set<T, C>::vector_set(u32 reserved)
	: _vector(reserved)
{ }

template <class T, class C> 
inline vector_set<T, C>::vector_set(const shared<Allocator>& allocator, u32 reserved)
	: _vector(allocator, reserved)
{ }

template <class T, class C> 
inline vector_set<T, C>::vector_set(const vector_set<T>& that, u32 reserved)
	: _vector(that._vector, reserved)
	, _compare(that._compare)
{ }

template <class T, class C> 
inline vector_set<T, C>::vector_set(const vector_set<T>& that, const shared<Allocator>& allocator, u32 reserved)
	: _vector(that._vector, allocator, reserved)
	, _compare(that._compare)
{ }

template <class T, class C>
inline vector_set<T, C>::~vector_set()
{ }

template <class T, class C> 
inline vector_set<T, C>& vector_set<T, C>::operator=(const vector_set& that)
{
	_vector = that._vector;
	_compare = that._compare;
	return *this;
}

template <class T, class C> 
inline shared<Allocator>& vector_set<T, C>::get_allocator() const
{
	return _vector.get_allocator();
}

template <class T, class C> 
inline void vector_set<T, C>::set_allocator(const shared<Allocator>& allocator)
{
	_vector.set_allocator(allocator);
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::begin()
{
	return _vector.begin();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_iterator vector_set<T, C>::begin() const
{
	return _vector.begin();
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::end()
{
	return _vector.end();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_iterator vector_set<T, C>::end() const
{
	return _vector.end();
}

template <class T, class C> 
inline typename vector_set<T, C>::reverse_iterator vector_set<T, C>::rbegin()
{
	return _vector.rbegin();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_reverse_iterator vector_set<T, C>::rbegin() const
{
	return _vector.rbegin();
}

template <class T, class C> 
inline typename vector_set<T, C>::reverse_iterator vector_set<T, C>::rend()
{
	return _vector.rend();
}

template <class T, class C>
inline typename vector_set<T, C>::const_reverse_iterator vector_set<T, C>::rend() const
{
	return _vector.rend();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_iterator vector_set<T, C>::cbegin() const
{
	return _vector.cbegin();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_iterator vector_set<T, C>::cend() const
{
	return _vector.cend();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_reverse_iterator vector_set<T, C>::crbegin() const
{
	return _vector.crbegin();
}

template <class T, class C> 
inline typename vector_set<T, C>::const_reverse_iterator vector_set<T, C>::crend() const
{
	return _vector.crend();
}
	
template <class T, class C> 
inline typename vector_set<T, C>::size_type vector_set<T, C>::size() const
{
	return _vector.size();
}

template <class T, class C> 
inline typename vector_set<T, C>::size_type vector_set<T, C>::capacity() const
{
	return _vector.capacity();
}

template <class T, class C> 
inline typename vector_set<T, C>::size_type vector_set<T, C>::reserved() const
{
	return _vector.reserved();
}

template <class T, class C> 
inline bool vector_set<T, C>::empty() const
{
	return _vector.empty();
}

template <class T, class C> 
inline const C& vector_set<T, C>::comp() const
{
	return _compare;
}

template <class T, class C> 
inline void vector_set<T, C>::reserve(size_type to_reserve)
{
	_vector.reserve(to_reserve);
}

template <class T, class C> 
inline T& vector_set<T, C>::front()
{
	return _vector.front();
}

template <class T, class C> 
inline const T& vector_set<T, C>::front() const
{
	return _vector.front();
}

template <class T, class C> 
inline T& vector_set<T, C>::back()
{
	return _vector.back();
}

template <class T, class C> 
inline const T& vector_set<T, C>::back() const
{
	return _vector.back();
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::insert(const T& object)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _vector.end() || _compare(object, *itr))
		itr = _vector.insert(itr, object);
	return itr;
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::insert_def()
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _vector.end() || _compare(object, *itr))
		itr = _vector.insert_def(itr);
	return itr;
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::insert(const T& object, const_iterator hint)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = (iterator)hint;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _vector.end() || _compare(object, *itr))
		itr = _vector.insert(itr, object);
	return itr;
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::insert_def(const_iterator hint)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = (iterator)hint;
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _vector.end() || _compare(object, *itr))
		itr = _vector.insert_def(itr);
	return itr;
}


template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::erase(const_iterator first, const_iterator last)
{
	return _vector.erase(first, last);
}

template <class T, class C> 
inline typename vector_set<T, C>::iterator vector_set<T, C>::erase(const_iterator position)
{
	return _vector.erase(position);
}

template <class T, class C> 
inline typename vector_set<T, C>::size_type vector_set<T, C>::erase(const T& object)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;
	size_type count = 0;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _vector.end() && !_compare(object, *itr))
	{
		itr = _vector.erase(itr);
		count = 1;
	}	
	return count;
}
	
template <class T, class C> template <class Iterator> inline void vector_set<T, C>::insert_set(Iterator first, Iterator last)
{
	iterator hint = _vector.begin() + (_vector.end() - _vector.begin()) / 2;
	while(first < last)
	{
		hint = insert(*first, hint);
		++first;
	}
}

template <class T, class C> template <class Iterator> inline void vector_set<T, C>::erase_set(Iterator first, Iterator last)
{
	iterator hint = _vector.begin() + (_vector.end() - _vector.begin()) / 2;
	while(first < last)
	{
		erase(*first);
		++first;
	}
}

template <class T, class C> template <class Iterator> inline void vector_set<T, C>::assign_set(Iterator first, Iterator last)
{
	_vector.clean();
	insert_set(first, last);
}

template <class T, class C> inline void vector_set<T, C>::swap(vector_set& that)
{
	_vector.swap(that._vector);
	C compare = that._compare;
	that._compare = _compare;
	_compare = compare;
}

template <class T, class C> inline void vector_set<T, C>::clear()
{
	_vector.clear();
}

template <class T, class C> inline void vector_set<T, C>::reset()
{
	_vector.reset();
}

template <class T, class C> inline typename vector_set<T, C>::iterator vector_set<T, C>::find(const T& object)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _vector.end() && _compare(object, *itr))
		itr = _vector.end();
	return itr;
}

template <class T, class C> inline typename vector_set<T, C>::const_iterator vector_set<T, C>::find(const T& object) const
{
	const_iterator begin = _vector.begin();
	const_iterator end = _vector.end();
	const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _vector.end() && _compare(object, *itr))
		itr = _vector.end();
	return itr;
}

template <class T, class C> inline typename vector_set<T, C>::size_type vector_set<T, C>::count(const T& object) const
{
	const_iterator begin = _vector.begin();
	const_iterator end = _vector.end();
	const_iterator itr = begin + (end - begin) / 2;
	size_type result = 1;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _vector.end() || _compare(object, *itr))
		result = 0;

	return itr;
}

template <class X, class Y>
void serialize(const vector_set<X, Y>& object, DataStream* stream)
{
	if(impl::vecset_serialize_header(object._vector._size, stream))
	{
		object._vector._serialize(object._vector._size, object._vector._data, stream);
	}
}

template <class X, class Y> 
void deserialize(vector_set<X, Y>& object, const DataStream* stream)
{
	object.clear();
	u32 new_size = 0;
	if(impl::vecset_deserialize_header(new_size, stream))
	{
		if(object._vector._capacity < new_size)
		{
			object._vector._allocator->free(object._vector._data, object._vector._capacity * sizeof X);
			object._vector._data = (X*)object._vector._allocator->alloc(new_size * sizeof X);
			object._vector._capacity = new_size;
		}
		object._vector._deserialize(new_size, object._vector._data, stream);
		object._vector._size = new_size;
	}
}

template <class T, class C> inline vector_multiset<T, C>::vector_multiset(u32 reserved)
	: _vector(reserved)
{ }

template <class T, class C> 
inline vector_multiset<T, C>::vector_multiset(const shared<Allocator>& allocator, u32 reserved)
	: _vector(allocator, reserved)
{ }

template <class T, class C> 
inline vector_multiset<T, C>::vector_multiset(const vector_multiset<T>& that, u32 reserved)
	: _vector(that._vector, reserved)
	, _compare(that._compare)
{ }

template <class T, class C> 
inline vector_multiset<T, C>::vector_multiset(const vector_multiset<T>& that, const shared<Allocator>& allocator, u32 reserved)
	: _vector(that._vector, allocator, reserved)
	, _compare(that._compare)
{ }

template <class T, class C>
inline vector_multiset<T, C>::~vector_multiset()
{ }

template <class T, class C> 
inline vector_multiset<T, C>& vector_multiset<T, C>::operator=(const vector_multiset& that)
{
	_vector = that._vector;
	_compare = that._compare;
	return *this;
}

template <class T, class C> 
inline shared<Allocator>& vector_multiset<T, C>::get_allocator() const
{
	return _vector.get_allocator();
}

template <class T, class C> 
inline void vector_multiset<T, C>::set_allocator(const shared<Allocator>& allocator)
{
	_vector.set_allocator(allocator);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::begin()
{
	return _vector.begin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_iterator vector_multiset<T, C>::begin() const
{
	return _vector.begin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::end()
{
	return _vector.end();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_iterator vector_multiset<T, C>::end() const
{
	return _vector.end();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::reverse_iterator vector_multiset<T, C>::rbegin()
{
	return _vector.rbegin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_reverse_iterator vector_multiset<T, C>::rbegin() const
{
	return _vector.rbegin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::reverse_iterator vector_multiset<T, C>::rend()
{
	return _vector.rend();
}

template <class T, class C>
inline typename vector_multiset<T, C>::const_reverse_iterator vector_multiset<T, C>::rend() const
{
	return _vector.rend();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_iterator vector_multiset<T, C>::cbegin() const
{
	return _vector.cbegin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_iterator vector_multiset<T, C>::cend() const
{
	return _vector.cend();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_reverse_iterator vector_multiset<T, C>::crbegin() const
{
	return _vector.crbegin();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::const_reverse_iterator vector_multiset<T, C>::crend() const
{
	return _vector.crend();
}
	
template <class T, class C> 
inline typename vector_multiset<T, C>::size_type vector_multiset<T, C>::size() const
{
	return _vector.size();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::size_type vector_multiset<T, C>::capacity() const
{
	return _vector.capacity();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::size_type vector_multiset<T, C>::reserved() const
{
	return _vector.reserved();
}

template <class T, class C> 
inline bool vector_multiset<T, C>::empty() const
{
	return _vector.empty();
}

template <class T, class C> 
inline const C& vector_multiset<T, C>::comp() const
{
	return _compare;
}

template <class T, class C> 
inline void vector_multiset<T, C>::reserve(size_type to_reserve)
{
	_vector.reserve(to_reserve);
}

template <class T, class C> 
inline T& vector_multiset<T, C>::front()
{
	return _vector.front();
}

template <class T, class C> 
inline const T& vector_multiset<T, C>::front() const
{
	return _vector.front();
}

template <class T, class C> 
inline T& vector_multiset<T, C>::back()
{
	return _vector.back();
}

template <class T, class C> 
inline const T& vector_multiset<T, C>::back() const
{
	return _vector.back();
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::insert(const T& object)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	return _vector.insert(itr, object);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::insert_def()
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	return _vector.insert_def(itr);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::insert(const T& object, const_iterator hint)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = (iterator)hint;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	return _vector.insert(itr, object);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::insert_def(const_iterator hint)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = (iterator)hint;
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	return _vector.insert_def(itr);
}


template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::erase(const_iterator first, const_iterator last)
{
	return _vector.erase(first, last);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::erase(const_iterator position)
{
	return _vector.erase(position);
}

template <class T, class C> 
inline typename vector_multiset<T, C>::size_type vector_multiset<T, C>::erase(const T& object)
{
	iterator begin, end;
	
	begin = _vector.begin();
	end = _vector.end();
	iterator lower = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(_compare(*lower, object))
			begin = lower + 1;
		else
			end = lower;
		lower = begin + (end - begin) / 2;
	}

	begin = _vector.begin();
	end = _vector.end();
	iterator upper = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(!_compare(object, *upper))
			begin = upper + 1;
		else
			end = upper;
		upper = begin + (end - begin) / 2;
	}

	_vector.erase(lower, upper);	

	return upper - lower;
}
	
template <class T, class C> template <class Iterator> inline void vector_multiset<T, C>::insert_set(Iterator first, Iterator last)
{
	iterator hint = _vector.begin() + (_vector.end() - _vector.begin()) / 2;
	while(first < last)
	{
		hint = insert(*first, hint);
		++first;
	}
}

template <class T, class C> template <class Iterator> inline void vector_multiset<T, C>::erase_set(Iterator first, Iterator last)
{
	iterator hint = _vector.begin() + (_vector.end() - _vector.begin()) / 2;
	while(first < last)
	{
		erase(*first);
		++first;
	}
}

template <class T, class C> template <class Iterator> inline void vector_multiset<T, C>::assign_set(Iterator first, Iterator last)
{
	_vector.clean();
	insert_set(first, last);
}

template <class T, class C> inline void vector_multiset<T, C>::swap(vector_multiset& that)
{
	_vector.swap(that._vector);
	C compare = that._compare;
	that._compare = _compare;
	_compare = compare;
}

template <class T, class C> inline void vector_multiset<T, C>::clear()
{
	_vector.clear();
}

template <class T, class C> inline void vector_multiset<T, C>::reset()
{
	_vector.reset();
}

template <class T, class C> inline typename vector_multiset<T, C>::iterator vector_multiset<T, C>::find(const T& object)
{
	iterator begin = _vector.begin();
	iterator end = _vector.end();
	iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _vector.end() && _compare(object, *itr))
		itr = _vector.end();
	return itr;
}

template <class T, class C> inline typename vector_multiset<T, C>::const_iterator vector_multiset<T, C>::find(const T& object) const
{
	const_iterator begin = _vector.begin();
	const_iterator end = _vector.end();
	const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _vector.end() && _compare(object, *itr))
		itr = _vector.end();
	return itr;
}

template <class T, class C> inline typename vector_multiset<T, C>::size_type vector_multiset<T, C>::count(const T& object) const
{
	const_iterator begin, end;

	begin = _vector.begin();
	end = _vector.end();
	const_iterator lower = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(_compare(*lower, object))
			begin = lower + 1;
		else
			end = lower;
		lower = begin + (end - begin) / 2;
	}

	begin = _vector.begin();
	end = _vector.end();
	const_iterator upper = begin + (end - begin) / 2;
	while(begin < end)
	{
		if(!_compare(object, *upper))
			begin = upper + 1;
		else
			end = upper;
		upper = begin + (end - begin) / 2;
	}

	return upper - lower;
}

template <class X, class Y>
void serialize(const vector_multiset<X, Y>& object, DataStream* stream)
{
	if(impl::vecmultiset_serialize_header(object._vector._size, stream))
	{
		object._vector._serialize(object._vector._size, object._vector._data, stream);
	}
}

template <class X, class Y> 
void deserialize(vector_multiset<X, Y>& object, const DataStream* stream)
{
	object.clear();
	u32 new_size = 0;
	if(impl::vecmultiset_deserialize_header(new_size, stream))
	{
		if(object._vector._capacity < new_size)
		{
			object._vector._allocator->free(object._vector._data, object._vector._capacity * sizeof X);
			object._vector._data = (X*)object._vector._allocator->alloc(new_size * sizeof X);
			object._vector._capacity = new_size;
		}
		object._vector._deserialize(new_size, object._vector._data, stream);
		object._vector._size = new_size;
	}
}

template <class K, class T, class C> 
inline vector_map<K, T, C>::vector_map(u32 reserved)
	: _kVector(reserved)
	, _tVector(reserved)
{ }

template <class K, class T, class C> 
inline vector_map<K, T, C>::vector_map(const shared<Allocator>& allocator, u32 reserved)
	: _kVector(allocator, reserved)
	, _tVector(allocator, reserved)
{ }
						
template <class K, class T, class C> 
inline vector_map<K, T, C>::vector_map(const vector_map& that, u32 reserved)
	: _kVector(that._kVector, reserved)
	, _tVector(that._tVector, reserved)
	, _compare(that._compare)
{ }
						
template <class K, class T, class C> 
inline vector_map<K, T, C>::vector_map(const vector_map& that, const shared<Allocator>& allocator, u32 reserved)
	: _kVector(that._kVector, allocator, reserved)
	, _tVector(that._tVector, allocator, reserved)
	, _compare(that._compare)
{ }
						
template <class K, class T, class C> 
inline vector_map<K, T, C>::~vector_map()
{ }

template <class K, class T, class C> 
inline vector_map<K, T, C>& vector_map<K, T, C>::operator=(const vector_map& that)
{
	_kVector = that._kVector;
	_tVector = that._tVector;
	_compare = that._compare;
	return *this;
}

template <class K, class T, class C> 
inline shared<Allocator>& vector_map<K, T, C>::get_allocator() const
{
	return _kVector.get_allocator();
}

template <class K, class T, class C> 
inline void vector_map<K, T, C>::set_allocator(const shared<Allocator>& allocator)
{
	_kVector.set_allocator(allocator);
	_tVector.set_allocator(allocator);
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::begin()
{
	return _tVector.begin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_iterator vector_map<K, T, C>::begin() const
{
	return _tVector.begin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::end()
{
	return _tVector.end();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_iterator vector_map<K, T, C>::end() const
{
	return _tVector.end();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::reverse_iterator vector_map<K, T, C>::rbegin()
{
	return _tVector.rbegin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_reverse_iterator vector_map<K, T, C>::rbegin() const
{
	return _tVector.rbegin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::reverse_iterator vector_map<K, T, C>::rend()
{
	return _tVector.rend();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_reverse_iterator vector_map<K, T, C>::rend() const
{
	return _tVector.rend();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_iterator vector_map<K, T, C>::cbegin() const
{
	return _tVector.begin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_iterator vector_map<K, T, C>::cend() const
{
	return _tVector.end();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_reverse_iterator vector_map<K, T, C>::crbegin() const
{
	return _tVector.rbegin();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_reverse_iterator vector_map<K, T, C>::crend() const
{
	return _tVector.rend();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::size_type vector_map<K, T, C>::size() const
{
	return _kVector.size();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::size_type vector_map<K, T, C>::capacity() const
{
	return _kVector.capacity();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::size_type vector_map<K, T, C>::reserved() const
{
	return _kVector.reserved();
}

template <class K, class T, class C> 
inline bool vector_map<K, T, C>::empty() const
{
	return _kVector.empty();
}

template <class K, class T, class C> 
inline const C& vector_map<K, T, C>::comp() const
{
	return _compare;
}

template <class K, class T, class C> 
inline void vector_map<K, T, C>::reserve(size_type to_reserve)
{
	_kVector.reserve(to_reserve);
	_tVector.reserve(to_reserve);
}

template <class K, class T, class C> 
inline T& vector_map<K, T, C>::operator[](const K& key)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	runtime_assert(itr != _kVector.end() && !_compare(key, *itr), "dupa");

	return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline const T& vector_map<K, T, C>::operator[](const K& key) const
{
	vector<K>::const_iterator begin = _kVector.begin();
	vector<K>::const_iterator end = _kVector.end();
	vector<K>::const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	runtime_assert(itr != _kVector.end() && !_compare(key, *itr), "dupa");

	return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline T& vector_map<K, T, C>::at(const K& key)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	runtime_assert(itr != _kVector.end() && !_compare(key, *itr), "dupa");

	return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline const T& vector_map<K, T, C>::at(const K& key) const
{
	vector<K>::const_iterator begin = _kVector.begin();
	vector<K>::const_iterator end = _kVector.end();
	vector<K>::const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	runtime_assert(itr != _kVector.end() && !_compare(key, *itr), "dupa");

	return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline T& vector_map<K, T, C>::at(const K& key, T& def_object)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, object))
			begin = itr + 1;
		else if(_compare(object, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || !_compare(key, *itr))
		return def_object;
	else
		return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline const T& vector_map<K, T, C>::at(const K& key, const T& def_object) const
{
	vector<K>::const_iterator begin = _kVector.begin();
	vector<K>::const_iterator end = _kVector.end();
	vector<K>::const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || !_compare(key, *itr))
		return def_object;
	else
		return *(_tVector.begin() +  (itr - _kVector.begin()));
}

template <class K, class T, class C> 
inline const K& vector_map<K, T, C>::key_at(const_iterator itr) const
{
	return *(_kVector.begin() + (itr - _tVector.begin()));
}

template <class K, class T, class C> 
inline T& vector_map<K, T, C>::front()
{
	return _tVector.front();
}

template <class K, class T, class C> 
inline const T& vector_map<K, T, C>::front() const
{
	return _tVector.front();
}

template <class K, class T, class C> 
inline T& vector_map<K, T, C>::back()
{
	return _tVector.back();
}

template <class K, class T, class C> 
inline const T& vector_map<K, T, C>::back() const
{
	return _tVector.back();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::insert(const K& key, const T& object)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;
	vector<T>::iterator result = _tVector.end();

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || _compare(key, *itr))
	{
		result = _tVector.begin() + (itr - _kVector.begin());
		result = _tVector.insert(result, object);
		_kVector.insert(itr, key);
	}

	return result;
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::insert_def(const K& key)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;
	vector<T>::iterator result = _tVector.end();
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || _compare(key, *itr))
	{
		result = _tVector.begin() + (itr - _kVector.begin());
		result = _tVector.insert_def(result);
		_kVector.insert(itr, key);
	}

	return result;
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::insert(const K& key, const T& object, const_iterator hint)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = _kVector.begin() + (hint - _tVector.begin());
	vector<T>::iterator result = _tVector.end();

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || _compare(key, *itr))
	{
		result = _tVector.begin() + (itr - _kVector.begin());
		result = _tVector.insert(result, object);
		_kVector.insert(itr, key);
	}

	return result;
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::insert_def(const K& key, const_iterator hint)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = _kVector.begin() + (hint - _tVector.begin());
	vector<T>::iterator result = _tVector.end();
	T object = T();

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || _compare(key, *itr))
	{
		result = _tVector.begin() + (itr - _kVector.begin());
		result = _tVector.insert_def(result);
		_kVector.insert(itr, key);
	}

	return result;
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::erase(const_iterator first, const_iterator last)
{
	vector<K>::iterator begin = _kVector.begin() + (first - _tVector.begin());
	vector<K>::iterator end = _kVector.begin() + (last - _tVector.begin());
	_kVector.erase(begin, end);
	return _tVector.erase(first, last);
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::erase(const_iterator position)
{
	vector<K>::iterator itr = _kVector.begin() + (position - _tVector.begin());
	_kVector.erase(itr);
	return _tVector.erase(position);
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::size_type vector_map<K, T, C>::erase(const K& key)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;
	size_type count = 0;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _kVector.end() && !_compare(key, *itr))
	{
		vector<T>::iterator tItr = _tVector.begin() + (itr - _kVector.begin());
		_kVector.erase(itr);
		_tVector.erase(tItr);
		count = 1;
	}	
	return count;
}
	
template <class K, class T, class C> 
inline void	 vector_map<K, T, C>::swap(vector_map& that)
{
	_kVector.swap(that._kVector);
	_tVector.swap(that._tVector);

	C compare = _compare;
	_compare = that._compare;
	that._compare = compare;
}

template <class K, class T, class C> 
inline void vector_map<K, T, C>::clear()
{
	_kVector.clear();
	_tVector.clear();
}

template <class K, class T, class C> 
inline void vector_map<K, T, C>::reset()
{
	_kVector.reset();
	_tVector.reset();
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::iterator vector_map<K, T, C>::find(const K& key)
{
	vector<K>::iterator begin = _kVector.begin();
	vector<K>::iterator end = _kVector.end();
	vector<K>::iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _kVector.end() && _compare(key, *itr))
		itr = _kVector.end();

	return _tVector.begin() + (itr - _kVector.begin());
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::const_iterator vector_map<K, T, C>::find(const K& key) const
{
	vector<K>::const_iterator begin = _kVector.begin();
	vector<K>::const_iterator end = _kVector.end();
	vector<K>::const_iterator itr = begin + (end - begin) / 2;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr != _kVector.end() && _compare(key, *itr))
		itr = _kVector.end();

	return _tVector.begin() + (itr - _kVector.begin());
}

template <class K, class T, class C> 
inline typename vector_map<K, T, C>::size_type vector_map<K, T, C>::count(const K& key) const
{
	vector<K>::const_iterator begin = _kVector.begin();
	vector<K>::const_iterator end = _kVector.end();
	vector<K>::const_iterator itr = begin + (end - begin) / 2;
	size_type count = 1;

	while(begin < end)
	{
		if(_compare(*itr, key))
			begin = itr + 1;
		else if(_compare(key, *itr))
			end = itr;
		else
			break;
		itr = begin + (end - begin) / 2;
	}

	if(itr == _kVector.end() || _compare(key, *itr))
		count = 0;

	return count;
}

template <class X, class Y, class Z>
void serialize(const vector_map<X, Y, Z>& object, DataStream* stream)
{
	if(impl::vecmap_serialize_header(object._kVector._size, stream))
	{
		object._kVector._serialize(object._kVector._size, object._kVector._data, stream);
		object._tVector._serialize(object._tVector._size, object._tVector._data, stream);
	}
}

template <class X, class Y, class Z>
void deserialize(vector_map<X, Y, Z>& object, const DataStream* stream)
{
	object.clear();
	u32 new_size = 0;
	if(impl::vecmap_deserialize_header(new_size, stream))
	{
		if(object._kVector._capacity < new_size)
		{
			object._kVector._allocator->free(object._kVector._data, object._kVector._capacity * sizeof X);
			object._kVector._data = (X*)object._kVector._allocator->alloc(new_size * sizeof X);
			object._kVector._capacity = new_size;

			object._tVector._allocator->free(object._tVector._data, object._tVector._capacity * sizeof Y);
			object._tVector._data = (Y*)object._tVector._allocator->alloc(new_size * sizeof Y);
			object._tVector._capacity = new_size;
		}
		object._kVector._deserialize(new_size, object._kVector._data, stream);
		object._kVector._size = new_size;

		object._tVector._deserialize(new_size, object._tVector._data, stream);
		object._tVector._size = new_size;
	}
}

} //namespace sparrow
