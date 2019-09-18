#pragma once
#include "smart.hpp"
namespace sparrow {

class Allocator
{
public:
						~Allocator(); 
	void*				alloc(u32 size, u32 align = 4);
	void				free(void* memptr, u32 size);
	const char*			name() const;

	void				acquire() const;
	void				release() const;
protected:
	typedef void* (*_Alloc)(Allocator* object, u32 size, u32 align);
	typedef void (*_Free)(Allocator* object, void* memptr, u32 size);
	typedef void (*_Destruct)(const Allocator* object);
						Allocator(_Alloc alloc_func, _Free free_func, _Destruct destructor, const c8* name);	
private:
	char				_name[32];
	mutable u32			_refcount;
	_Destruct			_destruct;
	_Alloc				_alloc;
	_Free				_free;
	DISALLOW_COPY_AND_ASSIGN(Allocator)
};

shared<Allocator> obtainGlobalAllocator();

//- inline impls --------------------------------------------------------------------------------------------------------------
inline Allocator::~Allocator()
{ }

inline void* Allocator::alloc(u32 size, u32 align)
{
	return _alloc(this, size, align);
}

inline void Allocator::free(void* memptr, u32 size)
{
	_free(this, memptr, size);
}

inline const c8* Allocator::name() const
{
	return _name;
}

inline void Allocator::acquire() const
{
	++_refcount;
}

inline void Allocator::release() const
{
	if(--_refcount == 0)
		_destruct(this);
}

} // namespace sparrow
