#include "memory.hpp"
namespace sparrow {

Allocator::Allocator(_Alloc alloc_func, _Free free_func, _Destruct destructor, const c8* name)
	: _alloc(alloc_func)
	, _free(free_func)
	, _destruct(destructor)
{ 
	u32 i = 0;
	while(i < 31 && name[i] != 0)
	{
		_name[i] = name[i];
		++i;
	}
	_name[i] = 0;
}

namespace impl {

class _GlobalAllocator : public Allocator
{
public:
						_GlobalAllocator();
						~_GlobalAllocator();
	static void*		_alloc(Allocator* object, u32 size, u32 align);
	static void			_free(Allocator* object, void* memptr, u32 size);
	static void			_destruct(const Allocator* object);
	friend class Library;
} GlobalAllocator;

_GlobalAllocator::_GlobalAllocator()
	: Allocator(_alloc, _free, _destruct, "global")
{ 
	acquire();
}

_GlobalAllocator::~_GlobalAllocator()
{ 
	release();
}

void* _GlobalAllocator::_alloc(Allocator* object, u32 size, u32 align)
{
	return size == 0 ? nullptr : ::malloc(size);
}

void _GlobalAllocator::_free(Allocator* object, void* memptr, u32 size)
{
	::free(memptr);
}

void _GlobalAllocator::_destruct(const Allocator* object)
{ }

} // namespace impl

shared<Allocator> obtainGlobalAllocator()
{
	return &impl::GlobalAllocator;
}

} // namespace sparrow
