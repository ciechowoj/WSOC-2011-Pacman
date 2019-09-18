#include "config.hpp"
#ifdef SIRIUS_TARGET_WINDOWS
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <process.h>
#endif

namespace Sirius {

//-----------------------------------------------------------------------------------------------------------------------------
//- string --------------------------------------------------------------------------------------------------------------------

class _StringAllocator
{
public:
	void*				alloc(u32 size);
	void				free(void* memptr, u32 size);

	struct _Chunk
	{
		void				init(u32 block_size, u32 block_count);
		void				quit();
		void*				allocate(u32 block_size);
		void				deallocate(void* ptr, u32 block_size);
		bool				contain(void* ptr, u32 size_mul_count);
		u32*				_data;
		u32*				_firstAvailable;
		u32					_availableCount;
	};

	struct _FixedBase
	{
		void				init(u32 block_size);
		void				quit();
		void*				allocate();
		void				deallocate(void* ptr);
		_Chunk*				_chunks;
		u32					_size;
		u32					_capacity;
		u32					_blockSize;
		u32					_blockCount;
		_Chunk*				_allocChunk;
		_Chunk*				_deallocChunk;
		Mutex				_mutex;
	};

	template <u32 size> struct _Fixed : public _FixedBase
	{
							_Fixed();
							~_Fixed();
	};

	_Fixed<16>			_fixed16;
	_Fixed<32>			_fixed32;
	_Fixed<64>			_fixed64;
	_Fixed<128>			_fixed128;
} GlobalStringAllocator;

u32 _stringHash(const u8* ptr)
{
	u32 c, hash = 5381;
	if(ptr)
		while(c = *ptr++)
			hash = ((hash << 5) + hash) + c;
	else
		hash = 0;
	return hash;
}

string::iterator& string::iterator::operator+=(s32 offset)
{
	u32 a, b, c;
	if(offset < 0)
	{
		operator-=(-offset);
	}
	else while(offset != 0)
	{
		c = *_ptr;
		a = c << 24 >> 31;
		b = c << 25 >> 31;
		c = c << 26 >> 31;
		_ptr += 1 + a * (b + c);
		--offset;
	}
	return *this;
}

string::iterator& string::iterator::operator-=(s32 offset)
{
	u32 a, b;
	if(offset < 0)
	{
		operator+=(-offset);
	}
	else while(offset != 0)
	{
		--_ptr;
		a = *_ptr;
		b = ~a;
		a = a << 25 >> 31;
		b = b << 24 >> 31;
		offset -= a | b;
	}
	return *this;
}

string::iterator& string::iterator::operator++()
{
	u32 a, b, c;
	c = *_ptr;
	a = c << 24 >> 31;
	b = c << 25 >> 31;
	c = c << 26 >> 31;
	_ptr += 1 + a * (b + c);
	return *this;
}

string::iterator& string::iterator::operator--()
{
	u32 a, b, o = 1;
	while(o != 0)
	{
		--_ptr;
		a = *_ptr;
		b = ~a;
		a = a << 25 >> 31;
		b = b << 24 >> 31;
		o -= a | b;
	}
	return *this;
}

string::iterator string::iterator::operator++(int)
{
	string::iterator prev = *this;
	u32 a, b, c;
	c = (u8)*_ptr;
	a = c << 24 >> 31;
	b = c << 25 >> 31;
	c = c << 26 >> 31;
	_ptr += 1 + a * (b + c);
	return prev;
}

string::iterator string::iterator::operator--(int)
{
	string::iterator prev = *this;
	u32 a, b, o = 1;
	while(o != 0)
	{
		--_ptr;
		a = (u8)*_ptr;
		b = ~a;
		a = a << 25 >> 31;
		b = b << 24 >> 31;
		o -= a | b;
	}
	return prev;
}

s32 string::iterator::operator-(string::iterator itr) const
{
	s32 result = 0;
	while(*this < itr)
	{
		--itr;
		--result;
	}
	while(*this > itr)
	{
		++itr;
		++result;
	}
	return result;
}

utf32 string::iterator::operator*() const
{
	u32 a = (u8)*_ptr & 0xf0;
	u32 r = a;	

	if(a <= 0x70)
	{
		r = (u8)*_ptr; 
	}
	else if(a <= 0xd0)
	{
		r = ((u8)*_ptr & 0x1f) << 6;
		r |= ((u8)*(_ptr + 1) & 0x3f);
	}
	else
	{
		r = ((u8)*_ptr & 0x0f) << 12;
		r |= ((u8)*(_ptr + 1) & 0x3f) << 6;
		r |= ((u8)*(_ptr + 2) & 0x3f);
	}
	return (utf32)r;
}



u32 _utf8count(const u16* data)
{
	const u16* ptr = data;
	u32 result = 0;

	while(*ptr != 0)
	{
		if(*ptr > 0x7f)
			result += *ptr > 0x7FF ? 2 : 1;
		++ptr;
	}

	result += ptr - (const u16*)data;
	return result;
}

u32 _utf8count(const u16* first, const u16* last)
{
	const u16* ptr = (const u16*)first;
	u32 result = 0;

	while(ptr != (const u16*)last)
	{
		if(*ptr > 0x7f)
			result += *ptr > 0x7FF ? 2 : 1;
		++ptr;
	}

	result += ptr - (const u16*)first;
	return result;
}

void _utf8copy(u8* dest, const u16* data)
{
	const u8 Sequence = 0x8 << 4, Sequence1 = 0xC << 4, Sequence2 = 0xE << 4;
	const u16 Mask = 0x3f, Mask1 = 0x1f, Mask2 = 0x0f;

	const u16* pSrc = (const u16*)data;
	u8* pDest = (u8*)dest;

	while(*pSrc != 0)
	{
		if(*pSrc <= 0x007fu)
		{
			*pDest = (u8)*pSrc;
		}
		else if(*pSrc <= 0x07ffu)
		{
			*pDest = Sequence1 | u8(*pSrc >> 6 & Mask1); ++pDest;
			*pDest = Sequence | u8(*pSrc & Mask);
		}
		else
		{
			*pDest = Sequence2 | u8(*pSrc >> 12 & Mask2); ++pDest;
			*pDest = Sequence | u8(*pSrc >> 6 & Mask); ++pDest;
			*pDest = Sequence | u8(*pSrc & Mask);
		}
		++pDest;
		++pSrc;
	}
}

void _utf8copy(u8* dest, const u16* first, const u16* last)
{
	const u8 Sequence = 0x8 << 4, Sequence1 = 0xC << 4, Sequence2 = 0xE << 4;
	const u16 Mask = 0x3f, Mask1 = 0x1f, Mask2 = 0x0f;

	const u16* pSrc = (const u16*)first, *pEnd = (const u16*)last;
	u8* pDest = (u8*)dest;

	while(pSrc != pEnd)
	{
		if(*pSrc <= 0x007fu)
		{
			*pDest = (u8)*pSrc;
		}
		else if(*pSrc <= 0x07ffu)
		{
			*pDest = Sequence1 | u8(*pSrc >> 6 & Mask1); ++pDest;
			*pDest = Sequence | u8(*pSrc & Mask);
		}
		else
		{
			*pDest = Sequence2 | u8(*pSrc >> 12 & Mask2); ++pDest;
			*pDest = Sequence | u8(*pSrc >> 6 & Mask); ++pDest;
			*pDest = Sequence | u8(*pSrc & Mask);
		}
		++pDest;
		++pSrc;
	}
}

string::string() 
	: _data(nullptr)
	, _hash(0)
	, _size(0)
{ }
						
string::string(const char* that)
	: _data(nullptr)
	, _hash(0)
	, _size(0)
{
	u32 size = ::strlen(that);
	if(size != 0)
	{
		_data = (u8*)GlobalStringAllocator.alloc(size);
		_size = size;
		::memcpy(_data, that, size);
		_hash = _stringHash(_data);
		u32* refctr = (u32*)(_data - 4);
		*refctr = 1;
	}
}

string::string(const char* first, const char* last)
	: _data(nullptr)
	, _hash(0)
	, _size(0)
{
	u32 size = last - first;
	if(size != 0)
	{
		_data = (u8*)GlobalStringAllocator.alloc(size);
		_size = size;
		::memcpy(_data, first, size);
		_hash = _stringHash(_data);
		u32* refctr = (u32*)(_data - 4);
		*refctr = 1;
	}
}

string::string(iterator first, iterator last)
	: _data(nullptr)
	, _hash(0)
	, _size(0)
{
	u32 size = last._ptr - first._ptr;
	if(size != 0)
	{
		_data = (u8*)GlobalStringAllocator.alloc(size);
		_size = size;
		::memcpy(_data, first._ptr, size);
		_hash = _stringHash(_data);
		u32* refctr = (u32*)(_data - 4);
		*refctr = 1;
	}
}

string::string(const string& that)
	: _data(that._data)
	, _hash(that._hash)
	, _size(that._size)
{
	if(_data != nullptr)
	{
		u32* refctr = (u32*)(_data - 4);
		interlockedIncrementU32(refctr);
	}
}

string::~string()
{
	if(_data != nullptr)
	{
		u32* refctr = (u32*)(_data - 4);
		if(interlockedDecrementU32(refctr) == 0)
			GlobalStringAllocator.free(_data, _size);
	}
}

string& string::operator=(const string& that)
{
	if(_data != that._data)
	{
		u32* refctr = nullptr;
		if(_data != nullptr)
		{
			refctr = (u32*)(_data - 4);
			if(interlockedDecrementU32(refctr) == 0)
				GlobalStringAllocator.free(_data, _size);
		}

		_data = that._data;
		_hash = that._hash;
		_size = that._size;

		if(_data != nullptr)
		{
			refctr = (u32*)(_data - 4);
			interlockedIncrementU32(refctr);
		}
	}
	return *this;
}

string& string::operator+=(const string& that)
{
	u32 size = _size + that._size;
	if(size != 0)
	{
		u32* refctr = nullptr;
		if(_data != nullptr)
		{
			u8* data = (u8*)GlobalStringAllocator.alloc(size);
			::memcpy(data, _data, _size);
			::memcpy(data + _size, that._data, that._size);
			refctr = (u32*)(data - 4);
			*refctr = 1;

			refctr = (u32*)(_data - 4);
			if(interlockedDecrementU32(refctr) == 0)
				GlobalStringAllocator.free(_data, _size);

			_data = data;
			_size = size;
			_hash = _stringHash(_data);
		}
		else
		{
			_data = that._data;
			_hash = that._hash;
			_size = that._size;

			if(_data != nullptr)
			{
				refctr = (u32*)(_data - 4);
				interlockedIncrementU32(refctr);
			}
		}
	}
	return *this;
}

string& string::operator+=(const char* that)
{
	u32 strl = ::strlen(that);
	u32 size = _size + strl;
	if(size != 0)
	{
		u32* refctr = nullptr;
		u8* data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(data, _data, _size);
		::memcpy(data + _size, that, strl);
		refctr = (u32*)(data - 4);
		*refctr = 1;

		if(_data != nullptr)
		{
			refctr = (u32*)(_data - 4);
			if(interlockedDecrementU32(refctr) == 0)
				GlobalStringAllocator.free(_data, _size);
		}
		
		_data = data;
		_size = size;
		_hash = _stringHash(_data);
	}
	return *this;
}

string& string::operator+=(char that)
{
	u32 size = _size + 1;
	if(size != 0)
	{
		u32* refctr = nullptr;
		u8* data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(data, _data, _size);
		data[size - 1] = that;
		refctr = (u32*)(data - 4);
		*refctr = 1;

		if(_data != nullptr)
		{
			refctr = (u32*)(_data - 4);
			if(interlockedDecrementU32(refctr) == 0)
				GlobalStringAllocator.free(_data, _size);
		}
		
		_data = data;
		_size = size;
		_hash = _stringHash(_data);
	}
	return *this;
}

string& string::operator-=(u32 count)
{
	string::iterator itr = end() - count;
	u32 size = (u8*)itr._ptr - _data;
	if(size != 0)
	{
		u32* refctr = nullptr;
		u8* data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(data, _data, size);
		refctr = (u32*)(data - 4);
		*refctr = 1;

		if(_data != nullptr)
		{
			refctr = (u32*)(_data - 4);
			if(interlockedDecrementU32(refctr) == 0)
				GlobalStringAllocator.free(_data, _size);
		}
		
		_data = data;
		_size = size;
		_hash = _stringHash(_data);
	}
	return *this;
}

wchar_t string::operator[](u32 index) const
{
	return *(begin() + index);
}

bool string::operator==(const string& that) const
{
	bool result = false;
	if(_hash == that._hash && _size == that._size)
	{
		if(_data != nullptr)
			result = ::strcmp((char*)_data, (char*)that._data) == 0;
		else
			result = true;
	}
	return result;
}

bool string::operator!=(const string& that) const
{
	bool result = true;
	if(_hash == that._hash && _size == that._size)
	{
		if(_data != nullptr)
			result = ::strcmp((char*)_data, (char*)that._data) != 0;
		else
			result = false;
	}
	return result;
}

bool string::operator==(const char* that) const
{
	return _data ? ::strcmp((char*)_data, that) == 0 : false;
}

bool string::operator!=(const char* that) const
{
	return _data ? ::strcmp((char*)_data, that) != 0 : true;
}

bool string::operator<(const string& that) const
{
	bool result = false;
	if(_hash < that._hash)
	{
		result = true;
	}
	else if(_hash == that._hash)
	{
		if(_size < that._size)
		{
			result = true;
		}
		else if(_size == that._size && _data != nullptr)
		{
			result = ::strcmp((const char*)_data, (const char*)that._data) < 0;
		}
	}
	return result;
}

bool string::operator>(const string& that) const
{
	bool result = false;
	if(_hash > that._hash)
	{
		result = true;
	}
	else if(_hash == that._hash)
	{
		if(_size > that._size)
		{
			result = true;
		}
		else if(_size == that._size && _data != nullptr)
		{
			result = ::strcmp((const char*)_data, (const char*)that._data) > 0;
		}
	}
	return result;
}

bool string::operator<=(const string& that) const
{
	bool result = false;
	if(_hash < that._hash)
	{
		result = true;
	}
	else if(_hash == that._hash)
	{
		if(_size < that._size)
		{
			result = true;
		}
		else if(_size == that._size)
		{
			result = _data ? ::strcmp((const char*)_data, (const char*)that._data) <= 0 : true;
		}
	}
	return result;
}

bool string::operator>=(const string& that) const
{
	bool result = false;
	if(_hash > that._hash)
	{
		result = true;
	}
	else if(_hash == that._hash)
	{
		if(_size > that._size)
		{
			result = true;
		}
		else if(_size == that._size)
		{
			result = _data ? ::strcmp((const char*)_data, (const char*)that._data) >= 0 : true;
		}
	}
	return result;
}

string operator+(const string& a, const string& b)
{
	string result;
	u32 size = a._size + b._size;
	if(size != 0)
	{
		result._size = size;
		result._data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(result._data, a._data, a._size);
		::memcpy(result._data + a._size, b._data, b._size);
		result._hash = _stringHash(result._data);
		u32* refctr = (u32*)(result._data - 4);
		*refctr = 1;
	}
	return result;
}

string operator+(const char* a, const string& b)
{
	string result;
	u32 strl = ::strlen(a);
	u32 size = strl + b._size;
	if(size != 0)
	{
		result._size = size;
		result._data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(result._data, a, strl);
		::memcpy(result._data + strl, b._data, b._size);
		result._hash = _stringHash(result._data);
		u32* refctr = (u32*)(result._data - 4);
		*refctr = 1;
	}
	return result;
}

string operator+(const string& a, const char* b)
{
	string result;
	u32 strl = ::strlen(b);
	u32 size = a._size + strl;
	if(size != 0)
	{
		result._size = size;
		result._data = (u8*)GlobalStringAllocator.alloc(size);
		::memcpy(result._data, a._data, a._size);
		::memcpy(result._data + a._size, b, strl);
		result._hash = _stringHash(result._data);
		u32* refctr = (u32*)(result._data - 4);
		*refctr = 1;
	}
	return result;
}

void _StringAllocator::_Chunk::init(u32 block_size, u32 block_count)
{
	block_size >>= 2;
	_data = ::new u32[block_size * block_count];

	if(_data == nullptr)
		TOGGLE_BREAKPOINT

	_firstAvailable = _data;
	_availableCount = block_count;

	u32* end = _data + block_count * block_size;
	u32* ptr = _data;
	while(ptr < end)
	{
		*ptr = reinterpret_cast<u32>(ptr + block_size);
		ptr += block_size;
	}
}

void _StringAllocator::_Chunk::quit()
{
	::delete[] _data;
}

void* _StringAllocator::_Chunk::allocate(u32 block_size)
{
	block_size >>= 2;
	u32* result = nullptr;
	if(_availableCount > 0)
	{
		result = _firstAvailable;
		_firstAvailable = reinterpret_cast<u32*>(*result);
		--_availableCount;
	}
	return result;
}

void _StringAllocator::_Chunk::deallocate(void* ptr, u32 block_size)
{
	block_size >>= 2;
	u32* rel_ptr = static_cast<u32*>(ptr);
	*rel_ptr = reinterpret_cast<u32>(_firstAvailable);
	_firstAvailable = rel_ptr;
	++_availableCount;
}

bool _StringAllocator::_Chunk::contain(void* ptr, u32 size_mul_count)
{
	return _data <= ptr && ptr < _data + (size_mul_count >> 2);
}

void _StringAllocator::_FixedBase::init(u32 block_size)
{
	_blockSize = block_size;
	_blockCount = 255;
	_chunks = ::new _Chunk[16];

	if(_chunks == nullptr)
		TOGGLE_BREAKPOINT

	_capacity = 16;
	_size = 1;
	_chunks->init(block_size, _blockCount);
	_allocChunk = _chunks;
	_deallocChunk = _chunks;
}

void _StringAllocator::_FixedBase::quit()
{
	for(u32 i = 0; i < _size; ++i)
		_chunks[i].quit();
	::delete[] _chunks;
}

void* _StringAllocator::_FixedBase::allocate()
{
	_mutex.lock();

	void* result = _allocChunk->allocate(_blockSize);

	u32 i = 0, s = _size;
	while(i < s && result == nullptr)
	{
		_allocChunk = &_chunks[i];
		result = _allocChunk->allocate(_blockSize);
		++i;
	}

	if(i == s)
	{
		if(_size == _capacity)
		{
			_capacity *= 2;
			_Chunk* chunks = ::new _Chunk[_capacity];
			if(_chunks == nullptr)
				TOGGLE_BREAKPOINT
			::memcpy(chunks, _chunks, sizeof _Chunk * _size);
			::delete[] _chunks;
			_chunks = chunks;
		}
		_chunks[_size].init(_blockSize, _blockCount);
		_allocChunk = &_chunks[_size++];
		result = _allocChunk->allocate(_blockSize);
	}

	_mutex.unlock();
	return result;
}

void _StringAllocator::_FixedBase::deallocate(void* ptr)
{
	_mutex.lock();

	u32 size_mul_count = _blockCount * _blockSize;
	if(_deallocChunk->contain(ptr, size_mul_count))
	{
		_deallocChunk->deallocate(ptr, _blockSize);
	}
	else
	{
		_deallocChunk = &_chunks[0];
		while(!_deallocChunk->contain(ptr, size_mul_count))
			++_deallocChunk;
		_deallocChunk->deallocate(ptr, _blockSize);
	}

	_mutex.unlock();
}

template <u32 size> _StringAllocator::_Fixed<size>::_Fixed()
{
	init(size);
}

template <u32 size> _StringAllocator::_Fixed<size>::~_Fixed()
{
	quit();
}

void* _StringAllocator::alloc(u32 size)
{
	const u32 RefCounterSize = 4;
	const u32 TerminatorSize = 1;
	void* memptr = nullptr;
	if(size != 0)
	{
		size = size + RefCounterSize + TerminatorSize;

		if(size <= 16)
			memptr = _fixed16.allocate();
		else if(size <= 32)
			memptr = _fixed32.allocate();
		else if(size <= 64)
			memptr = _fixed64.allocate();
		else if(size <= 128)
			memptr = _fixed128.allocate();
		else
		{
			memptr = ::new u8[size];
			if(memptr == nullptr)
				TOGGLE_BREAKPOINT
		}

		((u8*)memptr)[size - 1] = 0;
		memptr = ((u8*)memptr) + RefCounterSize;
	}
	return memptr;
}

void _StringAllocator::free(void* memptr, u32 size)
{
	const u32 RefCounterSize = 4;
	const u32 TerminatorSize = 1;
	if(size != 0)
	{
		size = size + RefCounterSize + TerminatorSize;
		memptr = ((u8*)memptr) - RefCounterSize;

		if(size <= 16)
			_fixed16.deallocate(memptr);
		else if(size <= 32)
			_fixed32.deallocate(memptr);
		else if(size <= 64)
			_fixed64.deallocate(memptr);
		else if(size <= 128)
			_fixed128.deallocate(memptr);
		else
			::delete[] (u8*)memptr;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
//- interlocked functions -----------------------------------------------------------------------------------------------------

s64 interlockedIncrementS64(volatile s64* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedIncrement64(value);
#endif
}

u64 interlockedIncrementU64(volatile u64* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedIncrement(value);
#endif
}

s32 interlockedIncrementS32(volatile s32* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedIncrement((volatile LONG*)value);
#endif
}

u32 interlockedIncrementU32(volatile u32* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedIncrement(value);
#endif
}

s64 interlockedDecrementS64(volatile s64* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedDecrement64(value);
#endif
}

u64 interlockedDecrementU64(volatile u64* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedDecrement(value);
#endif
}

s32 interlockedDecrementS32(volatile s32* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedDecrement((volatile LONG*)value);
#endif
}

u32 interlockedDecrementU32(volatile u32* value)
{
#ifdef SIRIUS_TARGET_WINDOWS
	return InterlockedDecrement(value);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------
//- Thread --------------------------------------------------------------------------------------------------------------------

u32 _TlsThreadInfo;
const u32 _ThreadFlagAlive = 1;
const u32 _ThreadFlagSuspended = _ThreadFlagAlive << 1;

#ifdef SIRIUS_TARGET_WINDOWS
unsigned __stdcall thread_main(void* ptr)
{
	Thread* _this = reinterpret_cast<Thread*>(ptr);
	_this->_flags = _ThreadFlagAlive | _ThreadFlagSuspended;
	TlsSetValue(_TlsThreadInfo, reinterpret_cast<void*>(_this));
	_this->entry();
	_this->_flags = 0;
	return 0;
}
#endif

Thread::Thread(const string& name) 
	: _name(name)
	, _handle(0)
	, _flags(0)
{
#ifdef SIRIUS_TARGET_WINDOWS
	_handle = reinterpret_cast<void*>(_beginthreadex(nullptr, 0, thread_main, reinterpret_cast<void*>(this), CREATE_SUSPENDED, nullptr));
	if(_handle == 0)
		TOGGLE_BREAKPOINT
#endif
}

Thread::~Thread()
{
#ifdef SIRIUS_TARGET_WINDOWS
	if(_handle != 0)
	{
		wait();
		CloseHandle(_handle);
	}
	else
	{
		TlsFree(_TlsThreadInfo);
	}
#endif
}

void Thread::resume()
{
#ifdef SIRIUS_TARGET_WINDOWS
	_flags &= (~_ThreadFlagSuspended);
	ResumeThread(_handle);
#endif
}

void Thread::suspend()
{
#ifdef SIRIUS_TARGET_WINDOWS
	_flags |= _ThreadFlagSuspended;
	SuspendThread(_handle);
#endif
}

void Thread::wait() 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	WaitForSingleObject(_handle, INFINITE); 
#endif
}

void Thread::wait(u32 ms) 
{
#ifdef SIRIUS_TARGET_WINDOWS
	WaitForSingleObject(_handle, ms); 
#endif
}

bool Thread::isAlive() const 
{
#ifdef SIRIUS_TARGET_WINDOWS
	return (_flags & _ThreadFlagAlive) != 0; 
#endif
}

bool Thread::isRunning() const 
{
#ifdef SIRIUS_TARGET_WINDOWS
	return (_flags & _ThreadFlagSuspended) != 0; 
#endif
}

void Thread::setPriority(ThreadPriority priority)
{
#ifdef SIRIUS_TARGET_WINDOWS
	s32 map[] = {
		THREAD_PRIORITY_LOWEST,				//ThreadPriorityLowest
		THREAD_PRIORITY_BELOW_NORMAL,		//ThreadPriorityLow
		THREAD_PRIORITY_NORMAL,				//ThreadPriorityNormal
		THREAD_PRIORITY_ABOVE_NORMAL,		//ThreadPriorityHigh,
		THREAD_PRIORITY_HIGHEST,			//ThreadPriorityHighest
	};

	SetThreadPriority(_handle, map[priority]);
#endif
}

ThreadPriority Thread::getPriority() const
{
#ifdef SIRIUS_TARGET_WINDOWS
	ThreadPriority result = ThreadPriorityNormal;
	switch(GetThreadPriority(_handle))
	{
	case THREAD_PRIORITY_IDLE: 
		result = ThreadPriorityLowest;
		break;
	case THREAD_PRIORITY_LOWEST: 
		result = ThreadPriorityLowest;
		break;
	case THREAD_PRIORITY_BELOW_NORMAL:
		result = ThreadPriorityLow;
		break;
	case THREAD_PRIORITY_NORMAL:
		result = ThreadPriorityNormal;
		break;
	case THREAD_PRIORITY_ABOVE_NORMAL:
		result = ThreadPriorityHigh;
		break;
	case THREAD_PRIORITY_HIGHEST:
		result = ThreadPriorityHighest;
		break;
	case THREAD_PRIORITY_TIME_CRITICAL:
		result = ThreadPriorityHighest;
		break;
	}
	return result;
#endif
}

const string& Thread::getName() const
{
	return _name;
}

void Thread::entry()
{ }

void Thread::sleep(u32 ms) 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	Sleep(ms); 
#endif
}

void Thread::yeld() 
{
#ifdef SIRIUS_TARGET_WINDOWS
	SwitchToThread(); 
#endif
}

Thread::Thread()
	: _name("main")
	, _handle(0)
	, _flags(_ThreadFlagAlive | _ThreadFlagSuspended)
{
#ifdef SIRIUS_TARGET_WINDOWS
	_TlsThreadInfo = TlsAlloc();
	TlsSetValue(_TlsThreadInfo, this);
#endif
}

Thread* Thread::thread()
{
#ifdef SIRIUS_TARGET_WINDOWS
	return (Thread*)TlsGetValue(_TlsThreadInfo);
#endif
}

Thread Thread::_main;

//-----------------------------------------------------------------------------------------------------------------------------
//- Mutex ---------------------------------------------------------------------------------------------------------------------

Mutex::Mutex() 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	static_assert(sizeof CRITICAL_SECTION == sizeof _object, "Correct _object size!");
	CRITICAL_SECTION* critical_section = reinterpret_cast<CRITICAL_SECTION*>(&_object);
	InitializeCriticalSection(critical_section); 
#endif
}

Mutex::~Mutex() 
{
#ifdef SIRIUS_TARGET_WINDOWS
	CRITICAL_SECTION* critical_section = reinterpret_cast<CRITICAL_SECTION*>(&_object);
	DeleteCriticalSection(critical_section); 
#endif
}

void Mutex::lock() 
{
#ifdef SIRIUS_TARGET_WINDOWS
	CRITICAL_SECTION* critical_section = reinterpret_cast<CRITICAL_SECTION*>(&_object);
	EnterCriticalSection(critical_section); 
#endif
}

void Mutex::unlock() 
{
#ifdef SIRIUS_TARGET_WINDOWS
	CRITICAL_SECTION* critical_section = reinterpret_cast<CRITICAL_SECTION*>(&_object);
	LeaveCriticalSection(critical_section);
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------
//- Event ---------------------------------------------------------------------------------------------------------------------

Event::Event(bool manual_reset) 
{
#ifdef SIRIUS_TARGET_WINDOWS
	_handle = CreateEventW(nullptr, (manual_reset ? TRUE : FALSE), FALSE, nullptr);
#endif
}

Event::~Event() 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	CloseHandle(_handle); 
#endif
}

void Event::signal() 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	SetEvent(_handle); 
#endif
}

void Event::reset() 
{ 
#ifdef SIRIUS_TARGET_WINDOWS
	ResetEvent(_handle); 
#endif
}

void Event::wait() 
{
#ifdef SIRIUS_TARGET_WINDOWS
	WaitForSingleObject(_handle, INFINITE); 
#endif
}

//-----------------------------------------------------------------------------------------------------------------------------
//- debug info ----------------------------------------------------------------------------------------------------------------

const u32 _DebugInfoRingSize = 1024;
IDebugInfoHandler* _GlobalDebugInfoHandler = nullptr;
bool _GlobalDebugInfoEnabled = true;

struct _DebugInfoQueue
{
public:
						_DebugInfoQueue()
		: _head(0)
		, _tail(0)
	{ }

						~_DebugInfoQueue()
	{ }

	void				push(const string& message)
	{
		_mutex.lock();
		_ring[_tail] = message;
		_tail = ++_tail % _DebugInfoRingSize;
		if(_tail == _head) ++_head;
		_mutex.unlock();
	}

	string				pop()
	{
		_mutex.lock();
		if(_head == _tail)
		{
			return string();
		}
		else
		{
			u32 head = _head;
			_head = ++_head % _DebugInfoRingSize;
			return _ring[head];
		}
		_mutex.unlock();
	}

	u32					size()
	{
		u32 size = 0;
		_mutex.lock();
		size = _head < _tail ? _tail - _head : _head - _tail;
		_mutex.unlock();
		return size;
	}

	Mutex				_mutex;
	string				_ring[_DebugInfoRingSize];
	u32					_head;
	u32					_tail;
} GlobalDebugInfoQueue;

void _debugInfo(const char* format, const char* function, ...)
{
	GlobalDebugInfoQueue.push(string(format) + " Function: " + function + ". Thread: " + Thread::thread()->getName() + ".");
}

IDebugInfoHandler::~IDebugInfoHandler()
{ }

void IDebugInfoHandler::poolDebugInfo()
{
#ifdef SIRIUS_DEBUG
	if(_GlobalDebugInfoHandler != nullptr && _GlobalDebugInfoEnabled)
	{
		while(GlobalDebugInfoQueue.size() != 0)
			_GlobalDebugInfoHandler->handleDebugInfo(GlobalDebugInfoQueue.pop());
	}
#endif
}

void IDebugInfoHandler::setDebugInfoHandler(IDebugInfoHandler* debug_info_handler)
{
	_GlobalDebugInfoHandler = debug_info_handler;
}

void IDebugInfoHandler::enableDebugInfo()
{
	_GlobalDebugInfoEnabled = true;
}

void IDebugInfoHandler::disableDebugInfo()
{
	_GlobalDebugInfoEnabled = false;
}

bool IDebugInfoHandler::isDebugInfoEnalbed()
{
#ifdef SIRIUS_DEBUG
	return _GlobalDebugInfoEnabled;
#else
	return false;
#endif
}

} // namespace String
