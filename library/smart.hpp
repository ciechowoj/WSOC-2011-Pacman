#include "config.hpp"
namespace sparrow {

class RefCtr
{
public:
						RefCtr();
						~RefCtr();
	void				acquire() const;
	bool				release() const;
private:
	mutable u32			_refCtr;
};

class RefCtrMT
{
public:
						RefCtrMT();
						~RefCtrMT();
	void				acquire() const;
	bool				release() const;
private:
	mutable u32			_refCtr;
};

template <class T> class shared
{
public:
						shared(const shared<T>& that);
						shared(T* that);
						~shared();

						operator T*();
	shared<T>&			operator=(const shared<T>& that);

	T*					operator->() const;
	T&					operator*() const;
private:
	T*					_object;
	template <class Y> friend void swap(shared<Y>& a, shared<Y>& b);
};

//- inline impls --------------------------------------------------------------------------------------------------------------
inline RefCtr::RefCtr()
	: _refCtr(0)
{ }

inline RefCtr::~RefCtr()
{ }

inline void RefCtr::acquire() const
{
	++_refCtr;
}

inline bool RefCtr::release() const
{
	return --_refCtr == 0;
}

inline RefCtrMT::RefCtrMT()
	: _refCtr(0)
{ }

inline RefCtrMT::~RefCtrMT()
{ }

template <class T> inline shared<T>::shared(const shared<T>& that)
	: _object(that._object)
{
	_object->acquire();
}

template <class T> inline shared<T>::shared(T* that)
	: _object(that)
{
	runtime_assert(that != nullptr, "dupa");
	_object->acquire();	
}

template <class T> inline shared<T>::~shared()
{
	_object->release();
}

template <class T> inline shared<T>::operator T*()
{
	return _object;
}

template <class T> inline shared<T>& shared<T>::operator=(const shared<T>& that)
{
	_object->release();
	_object = that;
	_object->acquire();
	return *this;
}

template <class T> inline T* shared<T>::operator->() const
{
	return _object;
}

template <class T> inline T& shared<T>::operator*() const
{
	return *_object;
}

template <class Y> inline void swap(shared<Y>& a, shared<Y>& b)
{
	Y* temp = a._object;
	a._object = b._object;
	b._object = temp;
}


} // namespace sparrow
