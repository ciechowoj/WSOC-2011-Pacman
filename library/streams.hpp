#pragma once
#include "config.hpp"
namespace sparrow {

enum StreamOrigin
{
	StreamOriginBegin = 0,
	StreamOriginCurrent = 1,
	StreamOriginEnd = 2,
};

enum StreamMode
{
	StreamModeRead = 0,
	StreamModeWrite = 1,
	StreamModeBoth = 2,
};

enum FileMode
{
	FileModeOpenExisting = 0,
	FileModeOpenAlways = 1,
	FileModeCreateAlways = 2,
};

class DataStream
{
public:
	virtual				~DataStream() = 0;
	virtual void		read(void* buffer, u32 size) const = 0;
	virtual void		write(const void* buffer, u32 size) = 0;
	virtual void		flush() = 0;
	u32					size() const;
	u32					sptr() const;
	virtual void		seek(u32 position) const = 0;
	virtual void		seek(s32 delta, StreamOrigin position) const = 0;
	bool				seekable() const;
	bool				readable() const;
	bool				writeable() const;
	bool				opened() const;
	string				name() const;
protected:
	enum { _Readable = 1u, _Writeable = 2u, _Seekable = 4u };
	void				_setup(u32 flags);
	void				_cleanup();
	void				_setName(const string& name);
	void				_setSize(u32 size) const;
	void				_setFptr(u32 fptr) const;

						DataStream();
private:
	string				_name;
	u32					_flag;
	mutable u32			_size;
	mutable s32			_fptr;
};

class FileStream : public DataStream
{
public:
						FileStream();
						FileStream(const string& file_path, StreamMode stream_mode = StreamModeRead, FileMode file_mode = FileModeOpenExisting);
	virtual				~FileStream();

	void				open(const string& file_path, StreamMode stream_mode = StreamModeRead, FileMode file_mode = FileModeOpenExisting);
	void				close();
	virtual void		read(void* buffer, u32 size) const;
	virtual void		write(const void* buffer, u32 size);
	virtual void		flush();
	virtual void		seek(u32 position) const;
	virtual void		seek(s32 delta, StreamOrigin origin) const;
private:
	mutable void*		_handle;
};

void serialize(u64 object, DataStream* stream);
void deserialize(u64& object, const DataStream* stream);
void serialize(u32 object, DataStream* stream);
void deserialize(u32& object, const DataStream* stream);
void serialize(u16 object, DataStream* stream);
void deserialize(u16& object, const DataStream* stream);
void serialize(u8 object, DataStream* stream);
void deserialize(u8& object, const DataStream* stream);
void serialize(s64 object, DataStream* stream);
void deserialize(s64& object, const DataStream* stream);
void serialize(s32 object, DataStream* stream);
void deserialize(s32& object, const DataStream* stream);
void serialize(s16 object, DataStream* stream);
void deserialize(u16& object, const DataStream* stream);
void serialize(s8 object, DataStream* stream);
void deserialize(u8& object, const DataStream* stream);
void serialize(f64 object, DataStream* stream);
void deserialize(f64& object, const DataStream* stream);
void serialize(f32 object, DataStream* stream);
void deserialize(f32& object, const DataStream* stream);
void serialize(bool object, DataStream* stream);
void deserialize(bool& object, const DataStream* stream);
void serialize(const string& object, DataStream* stream);
void deserialize(string& object, const DataStream* stream);

template <class T> void serialize(const __vec2<T>& object, DataStream* stream);
template <class T> void serialize(const __vec3<T>& object, DataStream* stream);
template <class T> void serialize(const __vec4<T>& object, DataStream* stream);
template <class T> void deserialize(__vec2<T>& object, const DataStream* stream);
template <class T> void deserialize(__vec3<T>& object, const DataStream* stream);
template <class T> void deserialize(__vec4<T>& object, const DataStream* stream);

void save(const string& text, DataStream* stream, bool write_bom);
void load(string& text, const DataStream* stream);

namespace impl {
void serialize__vec2(const void* object, u32 size, DataStream* stream);
void serialize__vec3(const void* object, u32 size, DataStream* stream);
void serialize__vec4(const void* object, u32 size, DataStream* stream);
void deserialize__vec2(void* object, u32 size, const DataStream* stream);
void deserialize__vec3(void* object, u32 size, const DataStream* stream);
void deserialize__vec4(void* object, u32 size, const DataStream* stream);
} // namespace impl

//- inline impls --------------------------------------------------------------------------------------------------------------
inline DataStream::~DataStream() 
{ }

inline u32 DataStream::size() const 
{ 
	return _size;
}

inline u32 DataStream::sptr() const 
{
	return _fptr; 
}

inline bool DataStream::seekable() const 
{ 
	return (_flag & _Seekable) != 0;
}

inline bool DataStream::readable() const 
{ 
	return (_flag & _Readable) != 0;
}

inline bool DataStream::writeable() const 
{ 
	return (_flag & _Writeable) != 0;
}

inline bool DataStream::opened() const 
{
	return readable() || writeable();
}

inline string DataStream::name() const
{
	return _name;
}

inline void DataStream::_setup(u32 flags)
{
	_flag = flags;
}

inline void DataStream::_cleanup()
{
	_flag = 0;
	_size = 0;
	_fptr = 0;
}

inline void DataStream::_setName(const string& name)
{
	_name = name;
}

inline void DataStream::_setSize(u32 size) const
{
	_size = size;
}

inline void DataStream::_setFptr(u32 fptr) const
{
	_fptr = fptr;
}

inline DataStream::DataStream() 
	: _flag(0)
	, _size(0)
	, _fptr(0) 
{ }

template <class T> inline void serialize(const __vec2<T>& object, DataStream* stream)
{
	impl::serialize__vec2(&object, sizeof __vec2<T>, stream);
}

template <class T> inline void serialize(const __vec3<T>& object, DataStream* stream)
{
	impl::serialize__vec3(&object, sizeof __vec3<T>, stream);
}

template <class T> inline void serialize(const __vec4<T>& object, DataStream* stream)
{
	impl::serialize__vec4(&object, sizeof __vec4<T>, stream);
}

template <class T> inline void deserialize(__vec2<T>& object, const DataStream* stream)
{
	impl::deserialize__vec2(&object, sizeof __vec2<T>, stream);
}

template <class T> inline void deserialize(__vec3<T>& object, const DataStream* stream)
{
	impl::deserialize__vec3(&object, sizeof __vec3<T>, stream);
}

template <class T> inline void deserialize(__vec4<T>& object, const DataStream* stream)
{
	impl::deserialize__vec4(&object, sizeof __vec4<T>, stream);
}

} //namespace sparrow
