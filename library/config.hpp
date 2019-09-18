#pragma once
#include <new>

#ifdef __GNUC__
#define SIRIUS_COMPILER_GCC (__GNUC__ * 100 + __GNUC_MINOR__)
#define SIRIUS_COMPILER_VCPP 0
#define SIRIUS_TARGET_ANDROID
#define TOGGLE_BREAKPOINT { asm(int $0x03) }
#endif //__GNUC__

#ifdef _MSC_VER
#define SIRIUS_COMPILER_GCC 0
#define SIRIUS_COMPILER_VCPP _MSC_VER
#define SIRIUS_TARGET_WINDOWS
#define TOGGLE_BREAKPOINT { __asm { int 3 } }
#endif //_MSC_VER

#ifdef SIRIUS_DEBUG
#define runtime_assert(cond, msg) { if(!(cond)) TOGGLE_BREAKPOINT }
#define DEBUG_INFO(condition, format, ...) if(condition) _debugInfo(format, __FUNCTION__, __VA_ARGS__);
#else
#define runtime_assert(cond, msg)
#define DEBUG_INFO(condition, format, ...)
#endif //SIRIUS_DEBUG

#define DISALLOW_COPY_AND_ASSIGN(type) \
	private: \
	type(const type& that) { } \
	type& operator=(const type& that) { return *this; }

#define DEFINE_VECTOR_TYPE(type) \
	typedef __vec2<type> type##x2; \
	typedef __vec3<type> type##x3; \
	typedef __vec4<type> type##x4; \
	typedef __vec2<type##x2> type##x2x2; \
	typedef __vec3<type##x2> type##x3x2; \
	typedef __vec4<type##x2> type##x4x2; \
	typedef __vec2<type##x3> type##x2x3; \
	typedef __vec3<type##x3> type##x3x3; \
	typedef __vec4<type##x3> type##x4x3; \
	typedef __vec2<type##x4> type##x2x4; \
	typedef __vec3<type##x4> type##x3x4; \
	typedef __vec4<type##x4> type##x4x4; 

typedef void (*unspecified_bool)(void);
const unspecified_bool unspecified_true = reinterpret_cast<unspecified_bool>(1);
const unspecified_bool unspecified_false = reinterpret_cast<unspecified_bool>(0);

#if SIRIUS_COMPILER_GCC < 406 && SIRIUS_COMPILER_VCPP < 1600
const class null_type
{
public:
	template <class T> operator T*() const { return 0; }
	template <class C, class T> operator T C::*() const { return 0; }
	operator unspecified_bool() { return unspecified_false; }
private:
	void operator&() const;
} nullptr;

#define static_assert(cond, msg)
#endif

template <class T, class U> inline T& absolute_cast(U& src) { return reinterpret_cast<T&>(src); }
template <class T, class U> inline const T& absolute_cast(const U& src) { return reinterpret_cast<const T&>(src); }

namespace sparrow {

template <class T> class __vec2;
template <class T> class __vec3;
template <class T> class __vec4;
template <class T> struct __vector_type { typedef T type; };
template <class T> struct __vector_type< __vec2<T> > { typedef typename __vec2<T>::value_type type; };
template <class T> struct __vector_type< __vec3<T> > { typedef typename __vec3<T>::value_type type; };
template <class T> struct __vector_type< __vec4<T> > { typedef typename __vec4<T>::value_type type; };

template <class T> class __vec2
{
public:
	typedef typename __vector_type<T>::type value_type;
	T					x, y;

	inline 				__vec2() { }
	inline 				__vec2(const T& x_, const T& y_) : x(x_), y(y_) { }
	explicit inline 	__vec2(const T v[]) : x(v[0]), y(v[1]) { }
	template <typename A> inline __vec2(const __vec2<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }
	template <typename A> inline __vec2(const __vec3<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }
	template <typename A> inline __vec2(const __vec4<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) { }
	explicit inline		__vec2(const value_type& s) : x(s), y(s) { }

	inline __vec2		operator+() const { return *this; }
	inline __vec2		operator-() const { return __vec2(-x, -y); }
	inline __vec2		operator+(const __vec2& v) const { return __vec2(x + v.x, y + v.y); }
	inline __vec2		operator-(const __vec2& v) const { return __vec2(x - v.x, y - v.y); }
	inline __vec2		operator*(const __vec2& v) const { return __vec2(x * v.x, y * v.y); }
	inline __vec2		operator/(const __vec2& v) const { return __vec2(x / v.x, y / v.y); }
	inline __vec2		operator*(const value_type& s) const { return __vec2(x * s, y * s); }
	inline __vec2		operator/(const value_type& s) const { return __vec2(x / s, y / s); }
	inline __vec2&		operator+=(const __vec2& v) { x += v.x; y += v.y; return *this; }
	inline __vec2&		operator-=(const __vec2& v) { x -= v.x; y -= v.y; return *this; }
	inline __vec2&		operator*=(const __vec2& v) { x *= v.x; y *= v.y; return *this; }
	inline __vec2&		operator/=(const __vec2& v) { x /= v.x; y /= v.y; return *this; }
	inline __vec2&		operator*=(const value_type& s) { x *= s; y *= s; return *this; }
	inline __vec2&		operator/=(const value_type& s) { x /= s; y /= s; return *this; }
	inline const T&		operator[](unsigned i) const { return reinterpret_cast<const T*>(this)[i]; }
	inline T&			operator[](unsigned i) { return reinterpret_cast<T*>(this)[i]; }

	inline bool			operator==(const __vec2& v) const { return x == v.x && y == v.y; }
	inline bool			operator!=(const __vec2& v) const { return x != v.x || y != v.y; }
	inline bool			operator<=(const __vec2& v) const { return x <= v.x && y <= v.y; }
	inline bool			operator>=(const __vec2& v) const { return x >= v.x && y >= v.y; }
	inline bool			operator<(const __vec2& v) const { return x < v.x && y < v.y; }
	inline bool			operator>(const __vec2& v) const { return x > v.x && y > v.y; }
};

template <class T> class __vec3
{
public:
	typedef typename __vector_type<T>::type value_type;
	T					x, y, z;

	inline 				__vec3() { }
	inline 				__vec3(const T& _x, const T& _y, const T& _z) : x(_x), y(_y), z(_z) { }
	explicit inline 	__vec3(const T v[]) : x(v[0]), y(v[1]), z(v[2]) { }
	inline				__vec3(const __vec2<T>& v, const T& z = T(0)) : x(v.x), y(v.y), z(z) { }
	template <typename A> inline __vec3(const __vec2<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(0)) { }
	template <typename A> inline __vec3(const __vec3<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) { }
	template <typename A> inline __vec3(const __vec4<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)) { }
	
	explicit inline		__vec3(const value_type& s) : x(s), y(s), z(s) { }

	inline __vec3		operator+() const { return *this; }
	inline __vec3		operator-() const { return __vec3(-x, -y, -z); }
	inline __vec3		operator+(const __vec3& v) const { return __vec3(x + v.x, y + v.y, z + v.z); }
	inline __vec3		operator-(const __vec3& v) const { return __vec3(x - v.x, y - v.y, z - v.z); }
	inline __vec3		operator*(const __vec3& v) const { return __vec3(x * v.x, y * v.y, z * v.z); }
	inline __vec3		operator/(const __vec3& v) const { return __vec3(x / v.x, y / v.y, z / v.z); }
	inline __vec3		operator*(const value_type& s) const { return __vec3(x * s, y * s, z * s); }
	inline __vec3		operator/(const value_type& s) const { return __vec3(x / s, y / s, z / s); }
	inline __vec3&		operator+=(const __vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	inline __vec3&		operator-=(const __vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	inline __vec3&		operator*=(const __vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
	inline __vec3&		operator/=(const __vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }
	inline __vec3&		operator*=(const value_type& s) { x *= s; y *= s; z *= s; return *this; }
	inline __vec3&		operator/=(const value_type& s) { x /= s; y /= s; z /= s; return *this; }
	inline const T&	operator[](unsigned i) const { return reinterpret_cast<const T*>(this)[i]; }
	inline T&			operator[](unsigned i) { return reinterpret_cast<T*>(this)[i]; }
						
	inline bool			operator==(const __vec3& v) const { return x == v.x && y == v.y && z == v.z; }
	inline bool			operator!=(const __vec3& v) const { return x != v.x || y != v.y || z != v.z; }
	inline bool			operator<=(const __vec3& v) const { return x <= v.x && y <= v.y && z <= v.z; }
	inline bool			operator>=(const __vec3& v) const { return x >= v.x && y >= v.y && z >= v.z; }
	inline bool			operator<(const __vec3& v) const { return x < v.x && y < v.y && z < v.z; }
	inline bool			operator>(const __vec3& v) const { return x > v.x && y > v.y && z > v.z; }
};

template <typename T> class __vec4
{
public:
	typedef typename __vector_type<T>::type value_type;
	T					x, y, z, w;

	inline 				__vec4() { }
	inline 				__vec4(const T& _x, const T& _y, const T& _z, const T& _w) : x(_x), y(_y), z(_z), w(_w) { }
	explicit inline 	__vec4(const T v[]) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) { }
	template <typename A> inline __vec4(const __vec2<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(0)), w(static_cast<T>(1)) { }
	template <typename A> inline __vec4(const __vec3<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(1)) { }
	template <typename A> inline __vec4(const __vec4<A>& v) : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w)) { }
	explicit inline	__vec4(const value_type& s) : x(s), y(s), z(s), w(s) { }

	inline __vec4		operator+() const { return *this; }
	inline __vec4		operator-() const { return __vec4(-x, -y, -z, -w); }
	inline __vec4		operator+(const __vec4& v) const { return __vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline __vec4		operator-(const __vec4& v) const { return __vec4(x - v.x, y - v.y, z - v.z, w - v.w); }
	inline __vec4		operator*(const __vec4& v) const { return __vec4(x * v.x, y * v.y, z * v.z, w * v.w); }
	inline __vec4		operator/(const __vec4& v) const { return __vec4(x / v.x, y / v.y, z / v.z, w / v.w); }
	inline __vec4		operator*(const value_type& s) const { return __vec4(x * s, y * s, z * s, w * s); }
	inline __vec4		operator/(const value_type& s) const { return __vec4(x / s, y / s, z / s, w * s); }
	inline __vec4&		operator+=(const __vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
	inline __vec4&		operator-=(const __vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
	inline __vec4&		operator*=(const __vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
	inline __vec4&		operator/=(const __vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }
	inline __vec4&		operator*=(const value_type& s) { x *= s; y *= s; z *= s; w *= s; return *this; }
	inline __vec4&		operator/=(const value_type& s) { x /= s; y /= s; z /= s; w /= s; return *this; }
	inline const T&		operator[](unsigned i) const { return reinterpret_cast<const T*>(this)[i]; }
	inline T&			operator[](unsigned i) { return reinterpret_cast<T*>(this)[i]; }

	inline bool			operator==(const __vec4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
	inline bool			operator!=(const __vec4& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
	inline bool			operator<=(const __vec4& v) const { return x <= v.x && y <= v.y && z <= v.z && w <= v.w; }
	inline bool			operator>=(const __vec4& v) const { return x >= v.x && y >= v.y && z >= v.z && w >= v.w; }
	inline bool			operator<(const __vec4& v) const { return x < v.x && y < v.y && z < v.z && w < v.w; }
	inline bool			operator>(const __vec4& v) const { return x > v.x && y > v.y && z > v.z && w > v.w; }
};

template <class T> inline __vec2<T> operator*(const typename __vec2<T>::value_type& s, const __vec2<T>& v) { return __vec2<T>(s * v.x, s * v.y); }
template <class T> inline __vec2<T> operator/(const typename __vec2<T>::value_type& s, const __vec2<T>& v) { return __vec2<T>(s / v.x, s / v.y); }
template <class T> inline __vec3<T> operator*(const typename __vec3<T>::value_type& s, const __vec3<T>& v) { return __vec3<T>(s * v.x, s * v.y, s * v.z); }
template <class T> inline __vec3<T> operator/(const typename __vec3<T>::value_type& s, const __vec3<T>& v) { return __vec3<T>(s / v.x, s / v.y, s / v.z); }
template <class T> inline __vec4<T> operator*(const typename __vec4<T>::value_type& s, const __vec4<T>& v) { return __vec4<T>(s * v.x, s * v.y, s * v.z, s * v.w); }
template <class T> inline __vec4<T> operator/(const typename __vec4<T>::value_type& s, const __vec4<T>& v) { return __vec4<T>(s / v.x, s / v.y, s / v.z, s / v.w); }

typedef long long		s64;
typedef unsigned long long u64;
typedef int				s32;
typedef unsigned		u32;
typedef short			s16;
typedef unsigned short	u16;
typedef signed char		s8;
typedef unsigned char	u8;
typedef double			f64;
typedef float			f32;
typedef u32				utf32;
typedef u16				utf16;
typedef u8				utf8;
typedef char			c8;
typedef short			c16;

DEFINE_VECTOR_TYPE(s64); static_assert(sizeof s64 == 8, "Invalid typedef!");
DEFINE_VECTOR_TYPE(u64); static_assert(sizeof u64 == 8, "Invalid typedef!");
DEFINE_VECTOR_TYPE(s32); static_assert(sizeof s32 == 4, "Invalid typedef!");
DEFINE_VECTOR_TYPE(u32); static_assert(sizeof u32 == 4, "Invalid typedef!");
DEFINE_VECTOR_TYPE(s16); static_assert(sizeof s16 == 2, "Invalid typedef!");
DEFINE_VECTOR_TYPE(u16); static_assert(sizeof u16 == 2, "Invalid typedef!");
DEFINE_VECTOR_TYPE(s8); static_assert(sizeof s8 == 1, "Invalid typedef!");
DEFINE_VECTOR_TYPE(u8); static_assert(sizeof u8 == 1, "Invalid typedef!");
DEFINE_VECTOR_TYPE(f64); static_assert(sizeof f64 == 8, "Invalid typedef!");
DEFINE_VECTOR_TYPE(f32); static_assert(sizeof f32 == 4, "Invalid typedef!");

class string
{
public:
	class iterator
	{
	public:
							iterator();
							iterator(const iterator& that);
							~iterator();

		iterator&			operator=(const iterator& that);
		iterator&			operator+=(s32 offset);
		iterator&			operator-=(s32 offset);
		iterator&			operator++();
		iterator&			operator--();
		iterator			operator++(int);
		iterator			operator--(int);
		iterator			operator+(s32 index) const;
		iterator			operator-(s32 index) const;
		s32					operator-(iterator itr) const;
		utf32				operator*() const;

		bool				operator==(const iterator& that) const;
		bool				operator!=(const iterator& that) const;
		bool				operator<=(const iterator& that) const;
		bool				operator>=(const iterator& that) const;
		bool				operator<(const iterator& that) const;
		bool				operator>(const iterator& that) const;
	private:
							iterator(u8* ptr);
		u8*					_ptr;
		friend class		string;
	};

						string();											///< Creates empty string.
						string(const char* that);							///< Creates string from array of chars. Array MUST consist of ASCII characters!
						string(const char* first, const char* last);		///< Creates string from range. Array MUST consist of ASCII characters!
						string(iterator first, iterator last);				///< Creates string from range.
						string(const string& that);							///< Copy constructor.
						~string();

	string&				operator=(const string& that);						///< Assigns one string to other string.
	string&				operator+=(const string& that);						///< Appends one string at the end of another.
	string&				operator+=(const char* that);						///< Appends one string at the end of another.
	string&				operator+=(char that);								///< Appends one character at end of string.
	string&				operator-=(u32 count);								///< Erases count of characters from string's end.
	wchar_t				operator[](u32 index) const;						///< Index operator. It is rather slow (due to utf8 ;P), use iterators instead it.

	u32					size() const;										///< Returns count of characters. Currently it must go throught whole array at every invocation so it is slow.
	u32					bytes() const;										///< Returns amount of bytes taken by string. It returns without any addinational operations.
	const void*			data() const;										///< Returns pointer to UTF-8 data.
	u32					hash() const;										///< Returns string hash.

	const char*			c_str() const;										///< Return c-style array of ASCII chars. You shouldn't store returned pointer.

	iterator			begin() const;
	iterator			end() const;

	bool				operator==(const string& that) const;				///< Very fast comparison operator.
	bool				operator!=(const string& that) const;				///< Very fast comparison operator.
	bool				operator==(const char* that) const;					///< This operator is as fast as strcmp(...).
	bool				operator!=(const char* that) const;					///< This operator is as fast as strcmp(...).
	bool				operator<(const string& that) const;				///< Compares string's hash values.
	bool				operator>(const string& that) const;
	bool				operator<=(const string& that) const;
	bool				operator>=(const string& that) const;
private:
	u8*					_data;
	u32					_hash;
	u32					_size;

	friend string operator+(const string& a, const string& b);
	friend string operator+(const char* a, const string& b);
	friend string operator+(const string& a, const char* b);
};

enum ErrorCode
{
	NoError,
	InvalidArg,
	InvalidCall,
	OutOfMemory,
	DataCorruption,
	OperationFailed,
};

enum ThreadPriority
{
	ThreadPriorityLowest,
	ThreadPriorityLow,
	ThreadPriorityNormal,
	ThreadPriorityHigh,
	ThreadPriorityHighest,
};

s64 interlockedIncrementS64(volatile s64* value);
u64 interlockedIncrementU64(volatile u64* value);
s32 interlockedIncrementS32(volatile s32* value);
u32 interlockedIncrementU32(volatile u32* value);
s64 interlockedDecrementS64(volatile s64* value);
u64 interlockedDecrementU64(volatile u64* value);
s32 interlockedDecrementS32(volatile s32* value);
u32 interlockedDecrementU32(volatile u32* value);

class Thread
{
public:
						Thread(const string& name);
						~Thread();
	void				resume();
	void				suspend();
	void				wait();
	void				wait(u32 ms);
	bool				isAlive() const;
	bool				isRunning() const;
	void				setPriority(ThreadPriority priority);
	ThreadPriority		getPriority() const;
	const string&		getName() const;
	virtual void		entry();

	static void			sleep(u32 ms);
	static void			yeld();
	static Thread*		thread();
private:
						Thread();
	string				_name;
	void*				_handle;
	u32					_flags;
	friend unsigned __stdcall thread_main(void* ptr);
	static Thread		_main;
};

class Mutex
{
public:
						Mutex();
						~Mutex();
	void				lock();
	void				unlock();
private:
	u8					_object[24];
};

class Event
{
public:
						Event(bool manual_reset = true);
						~Event();
	void				signal();
	void				reset();
	void				wait();
private:
	void*				_handle;
};

class IDebugInfoHandler
{
public:
	virtual				~IDebugInfoHandler();
	virtual void		handleDebugInfo(const string& debug_info) = 0;
	static void			poolDebugInfo();
	static void			setDebugInfoHandler(IDebugInfoHandler* debug_info_handler);
	static void			enableDebugInfo();
	static void			disableDebugInfo();
	static bool			isDebugInfoEnalbed();
};

void _debugInfo(const char* format, const char* function, ...);

//- inline impls --------------------------------------------------------------------------------------------------------------

inline string::iterator::iterator() 
	: _ptr(nullptr) 
{ }

inline string::iterator::iterator(const iterator& that)
	: _ptr(that._ptr)
{ }

inline string::iterator::~iterator()
{ }

inline string::iterator& string::iterator::operator=(const string::iterator& that)
{
	_ptr = that._ptr;
	return *this;
}

inline string::iterator string::iterator::operator+(s32 offset) const
{
	string::iterator t = *this;
	t += offset;
	return t;
}

inline string::iterator string::iterator::operator-(s32 offset) const
{
	string::iterator t = *this;
	t -= offset;
	return t;
}

inline bool string::iterator::operator==(const string::iterator& that) const
{
	return _ptr == that._ptr;
}

inline bool string::iterator::operator!=(const string::iterator& that) const
{
	return _ptr != that._ptr;
}

inline bool string::iterator::operator<=(const string::iterator& that) const
{
	return _ptr <= that._ptr;
}

inline bool string::iterator::operator>=(const string::iterator& that) const
{
	return _ptr >= that._ptr;
}

inline bool string::iterator::operator<(const string::iterator& that) const
{
	return _ptr < that._ptr;
}

inline bool string::iterator::operator>(const string::iterator& that) const
{
	return _ptr > that._ptr;
}

inline string::iterator::iterator(u8* ptr)
	: _ptr(ptr)
{ }

inline string::iterator operator+(s32 offset, const string::iterator& that)
{
	return that + offset;
}

inline u32 string::size() const
{
	return _data ? end() - begin() : 0;
}

inline u32 string::bytes() const
{
	return _size;
}

inline const void* string::data() const
{
	return _data; 
}

inline u32 string::hash() const
{
	return _hash;
}

inline const char* string::c_str() const 
{
	return (const char*)_data; 
}

inline string::iterator string::begin() const
{
	return iterator((u8*)_data);
}

inline string::iterator string::end() const
{
	return iterator((u8*)_data + _size);
}

inline bool operator==(const char* a, const string& b)
{ 
	return b == a; 
}

inline bool operator!=(const char* a, const string& b)
{ 
	return b == a; 
}

} //namespace sparrow
