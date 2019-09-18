#pragma once
#include "utilities.hpp"
namespace sparrow {
namespace impl {

template <class This, u32 Size> class bitset_base
{
public:
						bitset_base()
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] = 0;
	}

						bitset_base(u32 val)
	{
		_set[0] = val;
		for(u32 i = 1; i < _Capacity; ++i)
			_set[i] = 0;
		_set[_Capacity - 1] &= This::_Mask;
	}

						bitset_base(u64 val)
	{
		_set[0] = static_cast<u32>(val);
		_set[1] = static_cast<u32>(val >> 32);
		for(u32 i = 2; i < _Capacity; ++i)
			_set[i] = 0;
		_set[_Capacity - 1] &= This::_Mask;
	}

	This&				operator&=(const This& rhs)
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] &= rhs._set[i];
		return _this();
	}

	This&				operator|=(const This& rhs)
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] |= rhs._set[i];
		return _this();
	}

	This&				operator^=(const This& rhs)
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] ^= rhs._set[i];
		return _this();
	}

	This&				operator<<=(u32 shift)
	{
		leftshift(_set, _Capacity, shift);
		_set[_Capacity - 1] &= This::_Mask;
		return _this();
	}

	This&				operator>>=(u32 shift)
	{
		rightshift(_set, _Capacity, shift);
		_set[_Capacity - 1] &= This::_Mask;
		return _this();
	}

	This&				set()
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] = 0xffffffff;
		_set[_Capacity - 1] &= This::_Mask;
		return _this();
	}

	This&				set(u32 pos)
	{
		u32 hShift = pos >> 5u;
		u32 lShift = pos - (hShift << 5u);
		_set[hShift] |= 1u << lShift;
		return _this();
	}

	This&				set(u32 pos, bool val)
	{
		u32 hShift = pos >> 5u;
		u32 lShift = pos - (hShift << 5u);
		u32 bit = static_cast<u32>(val);
		_set[hShift] &= ~(1u << lShift);
		_set[hShift] |= bit << lShift;
		return _this();
	}

	This&				reset()
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] = 0;
		return _this();
	}

	This&				reset(u32 pos)
	{
		u32 hShift = pos >> 5u;
		u32 lShift = pos - (hShift << 5u);
		_set[hShift] &= ~(1u << lShift);
		return _this();
	}

	This				operator~() const
	{
		This bitset = _this();
		return bitset.flip();
	}

	This&				flip()
	{
		for(u32 i = 0; i < _Capacity; ++i)
			_set[i] = ~_set[i];
		_set[_Capacity - 1] &= This::_Mask;
		return _this();
	}

	This&				flip(u32 pos)
	{
		u32 hShift = pos >> 5u;
		u32 lShift = pos - (hShift << 5u);
		_set[hShift] ^= 1u << lShift;
		return _this();
	}

	u32					to_u32() const
	{
		return _set[0];
	}

	u64					to_u64() const
	{
		return static_cast<u64>(_set[1]) << 32 | static_cast<u64>(_set[0]);
	}

	u32					count() const
	{
		u32 result = bitcount(_set[0]);
		for(u32 i = 1; i < _Capacity; ++i)
			result += bitcount(_set[i]);
		return result;
	}

	bool				operator==(const This& rhs) const
	{
		u32 i = 0;
		while(i < _Capacity && _set[i] == rhs._set[i])
			++i;
		return i == _Capacity;
	}

	bool				operator!=(const This& rhs) const
	{
		u32 i = 0;
		while(i < _Capacity && _set[i] == rhs._set[i])
			++i;
		return i != _Capacity;
	}

	bool				test(u32 pos) const
	{
		u32 hShift = pos >> 5u;
		u32 lShift = pos - (hShift << 5u);
		return (_set[hShift] & 1u << lShift) != 0;
	}

	bool				all() const
	{
		u32 result = 0xffffffff;
		for(u32 i = 0; i < _Capacity - 1; ++i)
			result &= _set[i];
		return result == 0xffffffff && _set[_Capacity - 1] == This::_Mask;
	}

	bool				any() const
	{
		u32 result = 0;
		for(u32 i = 0; i < _Capacity; ++i)
			result |= _set[i];
		return result != 0;
	}

	bool				none() const
	{
		u32 result = 0;
		for(u32 i = 0; i < _Capacity; ++i)
			result |= _set[i];
		return result == 0;
	}

	void				serialize(DataStream* stream) const
	{
		stream->write(_set, Size);
	}

	void				deserialize(const DataStream* stream)
	{
		stream->read(_set, Size);
	}
private:
	This&				_this()
	{
		return reinterpret_cast<This&>(*this);
	}

	const This&			_this() const
	{
		return reinterpret_cast<const This&>(*this);
	}

	static const u32	_Capacity = (Size + 3) / 4;
	u32					_set[_Capacity];	
};

template <class This> class bitset_base<This, 4>
{
public:
						bitset_base()
		: _set(0)
	{ }

						bitset_base(u32 val)
		: _set(val)
	{ 
		_set &= This::_Mask;			
	}

						bitset_base(u64 val)
		: _set(static_cast<u32>(val))
	{ 
		_set &= This::_Mask;			
	}

	This&				operator&=(const This& rhs)
	{
		_set &= rhs._set;
		return _this();
	}

	This&				operator|=(const This& rhs)
	{
		_set |= rhs._set;
		return _this();
	}

	This&				operator^=(const This& rhs)
	{
		_set ^= rhs._set;
		return _this();
	}

	This&				operator<<=(u32 pos)
	{
		_set <<= rhs._set;
		_set &= This::_Mask;
		return _this();
	}

	This&				operator>>=(u32 pos)
	{
		_set >>= rhs._set;
		return _this();
	}
	
	This&				set()
	{
		_set = This::_Mask;
		return _this();
	}

	This&				set(u32 pos)
	{
		_set |= 1u << pos;
		return _this();
	}

	This&				set(u32 pos, bool val)
	{
		u32 bit = static_cast<u32>(val);
		_set &= ~(bit << pos);
		_set |= bit << pos;
		return _this();
	}

	This&				reset()
	{
		_set = 0;
		return _this();
	}

	This&				reset(u32 pos)
	{
		_set &= ~(1u << pos);
		return _this();
	}

	This				operator~() const
	{
		return This(~_set);
	}

	This&				flip()
	{
		_set = ~_set;
		return _this();
	}

	This&				flip(u32 pos)
	{
		_set ^= 1u << pos;
		return _this();
	}

	u32					to_u32() const
	{
		return _set;
	}

	u64					to_u64() const
	{
		return _set;
	}

	u32					count() const
	{
		return bitcount(_set);
	}

	bool				operator==(const This& rhs) const
	{
		return _set == rhs._set;
	}

	bool				operator!=(const This& rhs) const
	{
		return _set != rhs._set;
	}

	bool				test(u32 pos) const
	{
		return (_set & 1u << pos) != 0;
	}

	bool				all() const
	{
		return _set == This::_Mask;
	}

	bool				any() const
	{
		return _set != 0;
	}

	bool				none() const
	{
		return _set == 0;
	}

	void				serialize(DataStream* stream) const
	{
		stream->write(&_set, Size);
	}

	void				deserialize(const DataStream* stream)
	{
		stream->read(&_set, Size);
	}
private:
	This&				_this()
	{
		return reinterpret_cast<This&>(*this);
	}

	const This&			_this() const
	{
		return reinterpret_cast<const This&>(*this);
	}

	u32					_set;
};

template <class This> class bitset_base<This, 3>
{
public:
						bitset_base()
		: _set(0)
	{ }

						bitset_base(u32 val)
		: _set(val)
	{ 
		_set &= This::_Mask;			
	}

						bitset_base(u64 val)
		: _set(static_cast<u32>(val))
	{ 
		_set &= This::_Mask;			
	}

	This&				operator&=(const This& rhs)
	{
		_set &= rhs._set;
		return _this();
	}

	This&				operator|=(const This& rhs)
	{
		_set |= rhs._set;
		return _this();
	}

	This&				operator^=(const This& rhs)
	{
		_set ^= rhs._set;
		return _this();
	}

	This&				operator<<=(u32 pos)
	{
		_set <<= rhs._set;
		_set &= This::_Mask;
		return _this();
	}

	This&				operator>>=(u32 pos)
	{
		_set >>= rhs._set;
		return _this();
	}
	
	This&				set()
	{
		_set = This::_Mask;
		return _this();
	}

	This&				set(u32 pos)
	{
		_set |= 1u << pos;
		return _this();
	}

	This&				set(u32 pos, bool val)
	{
		u32 bit = static_cast<u32>(val);
		_set &= ~(bit << pos);
		_set |= bit << pos;
		return _this();
	}

	This&				reset()
	{
		_set = 0;
		return _this();
	}

	This&				reset(u32 pos)
	{
		_set &= ~(1u << pos);
		return _this();
	}

	This				operator~() const
	{
		return This(~_set);
	}

	This&				flip()
	{
		_set = ~_set;
		return _this();
	}

	This&				flip(u32 pos)
	{
		_set ^= 1u << pos;
		return _this();
	}

	u32					to_u32() const
	{
		return _set;
	}

	u64					to_u64() const
	{
		return _set;
	}

	u32					count() const
	{
		return bitcount(_set);
	}

	bool				operator==(const This& rhs) const
	{
		return _set == rhs._set;
	}

	bool				operator!=(const This& rhs) const
	{
		return _set != rhs._set;
	}

	bool				test(u32 pos) const
	{
		return (_set & 1u << pos) != 0;
	}

	bool				all() const
	{
		return _set == This::_Mask;
	}

	bool				any() const
	{
		return _set != 0;
	}

	bool				none() const
	{
		return _set == 0;
	}

	void				serialize(DataStream* stream) const
	{
		stream->write(&_set, Size);
	}

	void				deserialize(const DataStream* stream)
	{
		stream->read(&_set, Size);
	}
private:
	This&				_this()
	{
		return reinterpret_cast<This&>(*this);
	}

	const This&			_this() const
	{
		return reinterpret_cast<const This&>(*this);
	}

	u32					_set;
};

template <class This> class bitset_base<This, 2>
{
public:
						bitset_base()
		: _set(0)
	{ }

						bitset_base(u32 val)
		: _set(val)
	{ 
		_set &= This::_Mask;			
	}

						bitset_base(u64 val)
		: _set(static_cast<u32>(val))
	{ 
		_set &= This::_Mask;			
	}

	This&				operator&=(const This& rhs)
	{
		_set &= rhs._set;
		return _this();
	}

	This&				operator|=(const This& rhs)
	{
		_set |= rhs._set;
		return _this();
	}

	This&				operator^=(const This& rhs)
	{
		_set ^= rhs._set;
		return _this();
	}

	This&				operator<<=(u32 pos)
	{
		_set <<= rhs._set;
		_set &= This::_Mask;
		return _this();
	}

	This&				operator>>=(u32 pos)
	{
		_set >>= rhs._set;
		return _this();
	}
	
	This&				set()
	{
		_set = This::_Mask;
		return _this();
	}

	This&				set(u32 pos)
	{
		_set |= 1u << pos;
		return _this();
	}

	This&				set(u32 pos, bool val)
	{
		u32 bit = static_cast<u32>(val);
		_set &= ~(bit << pos);
		_set |= bit << pos;
		return _this();
	}

	This&				reset()
	{
		_set = 0;
		return _this();
	}

	This&				reset(u32 pos)
	{
		_set &= ~(1u << pos);
		return _this();
	}

	This				operator~() const
	{
		return This(~_set);
	}

	This&				flip()
	{
		_set = ~_set;
		return _this();
	}

	This&				flip(u32 pos)
	{
		_set ^= 1u << pos;
		return _this();
	}

	u32					to_u32() const
	{
		return _set;
	}

	u64					to_u64() const
	{
		return _set;
	}

	u32					count() const
	{
		return bitcount(_set);
	}

	bool				operator==(const This& rhs) const
	{
		return _set == rhs._set;
	}

	bool				operator!=(const This& rhs) const
	{
		return _set != rhs._set;
	}

	bool				test(u32 pos) const
	{
		return (_set & 1u << pos) != 0;
	}

	bool				all() const
	{
		return _set == This::_Mask;
	}

	bool				any() const
	{
		return _set != 0;
	}

	bool				none() const
	{
		return _set == 0;
	}

	void				serialize(DataStream* stream) const
	{
		stream->write(&_set, Size);
	}

	void				deserialize(const DataStream* stream)
	{
		stream->read(&_set, Size);
	}
private:
	This&				_this()
	{
		return reinterpret_cast<This&>(*this);
	}

	const This&			_this() const
	{
		return reinterpret_cast<const This&>(*this);
	}

	u16					_set;
};

template <class This> class bitset_base<This, 1>
{
public:
						bitset_base()
		: _set(0)
	{ }

						bitset_base(u32 val)
		: _set(val)
	{ 
		_set &= This::_Mask;			
	}

						bitset_base(u64 val)
		: _set(static_cast<u32>(val))
	{ 
		_set &= This::_Mask;			
	}

	This&				operator&=(const This& rhs)
	{
		_set &= rhs._set;
		return _this();
	}

	This&				operator|=(const This& rhs)
	{
		_set |= rhs._set;
		return _this();
	}

	This&				operator^=(const This& rhs)
	{
		_set ^= rhs._set;
		return _this();
	}

	This&				operator<<=(u32 pos)
	{
		_set <<= rhs._set;
		_set &= This::_Mask;
		return _this();
	}

	This&				operator>>=(u32 pos)
	{
		_set >>= rhs._set;
		return _this();
	}
	
	This&				set()
	{
		_set = This::_Mask;
		return _this();
	}

	This&				set(u32 pos)
	{
		_set |= 1u << pos;
		return _this();
	}

	This&				set(u32 pos, bool val)
	{
		u32 bit = static_cast<u32>(val);
		_set &= ~(bit << pos);
		_set |= bit << pos;
		return _this();
	}

	This&				reset()
	{
		_set = 0;
		return _this();
	}

	This&				reset(u32 pos)
	{
		_set &= ~(1u << pos);
		return _this();
	}

	This				operator~() const
	{
		return This(~_set);
	}

	This&				flip()
	{
		_set = ~_set;
		return _this();
	}

	This&				flip(u32 pos)
	{
		_set ^= 1u << pos;
		return _this();
	}

	u32					to_u32() const
	{
		return _set;
	}

	u64					to_u64() const
	{
		return _set;
	}

	u32					count() const
	{
		return bitcount(_set);
	}

	bool				operator==(const This& rhs) const
	{
		return _set == rhs._set;
	}

	bool				operator!=(const This& rhs) const
	{
		return _set != rhs._set;
	}

	bool				test(u32 pos) const
	{
		return (_set & 1u << pos) != 0;
	}

	bool				all() const
	{
		return _set == This::_Mask;
	}

	bool				any() const
	{
		return _set != 0;
	}

	bool				none() const
	{
		return _set == 0;
	}
private:
	This&				_this()
	{
		return reinterpret_cast<This&>(*this);
	}

	const This&			_this() const
	{
		return reinterpret_cast<const This&>(*this);
	}

	u8					_set;
};

template <u32 N> class metaMSB
{
	template <u32 M> class _impl { public: static const u32 value = 1 + _impl<(M >> 1)>::value; };
	template <> class _impl<0> { public: static const u32 value = 0; };
public:
	static const s32 value = static_cast<s32>(_impl<N>::value) - 1;
};

template <u32 N> class metaLBitMask // creates bitmask of (32 - N)x0 and Nx1, e.g. for N = 25 0n00000001111111111111111111111111
{
	template <s32 M> class _impl { public: static const u32 value = _impl<(M - 1)>::value << 1 | 1u; };
	template <> class _impl<0> { public: static const u32 value = 1; };
	template <> class _impl<-1> { public: static const u32 value = 0; };
public:
	static const u32 value = static_cast<u32>(_impl<N - 1>::value);
};

} //namespace impl

template <u32 Size> class bitset : public impl::bitset_base<bitset<Size>, (Size + 7) / 8>
{
public:
	class reference 
	{
							reference(u32 pos, bitset* ref)
			: _pos(pos)
			, _ref(ref)
		{ }
	public:
							~reference()
		{ }

		reference&			operator=(bool x)
		{
			_ref->set(_pos, x);
			return *this;
		}

		reference&			operator=(const reference& that)
		{
			_ref->set(_pos, that);
			return *this;
		}

		bool				operator~() const
		{
			return !_ref->test(_pos);
		}

		operator			bool() const
		{
			return _ref->test(_pos);
		}

		reference&			flip()
		{
			_ref->flip(_pos);
			return *this;
		}
	private:
		u32					_pos;
		bitset*				_ref;
	friend class bitset;
	};


						bitset()
	{ }

						bitset(u32 val)
		: impl::bitset_base<bitset<Size>, (Size + 7) / 8>(val)
	{ }
						bitset(u64 val)
		: impl::bitset_base<bitset<Size>, (Size + 7) / 8>(val)
	{ }

	bool				operator[](u32 pos) const
	{
		return test(pos);
	}

	reference			operator[](u32 pos)
	{
		return reference(pos, this);
	}

	u32					size()
	{
		return Size;
	}

	bitset				operator<<(u32 shift) const
	{
		bitset x = *this;
		return x <<= pos;
	}

	bitset				operator>>(u32 shift) const
	{
		bitset x = *this;
		return x >>= pos;
	}
private:
	static const u32 _Mask = impl::metaLBitMask<Size % 32>::value; 
	friend class impl::bitset_base<bitset<Size>, (Size + 7) / 8>;
};

} // namespace sparrow
