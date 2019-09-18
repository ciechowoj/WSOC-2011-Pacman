#include "streams.hpp"
#ifdef WINDOWS32
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#endif
#ifdef ANDROID
#error "Unimplemented!"
#endif

namespace sparrow {

FileStream::FileStream() 
	: _handle(INVALID_HANDLE_VALUE) 
{ }

FileStream::FileStream(const string& file_path, StreamMode stream_mode, FileMode file_mode)
	 : _handle(INVALID_HANDLE_VALUE) 
{ 
	open(file_path, stream_mode, file_mode);
}

FileStream::~FileStream() 
{ 
	if(opened()) 
		close(); 
}

void FileStream::open(const string& file_path, StreamMode stream_mode, FileMode file_mode)
{
	const DWORD dwDesiredAccess[3] = { GENERIC_READ, GENERIC_WRITE, GENERIC_READ | GENERIC_WRITE };
	const DWORD dwCreationDisposition[3] = { OPEN_EXISTING, OPEN_ALWAYS, CREATE_ALWAYS };
	const DWORD dwFlagsAndAttributes[3] = { FILE_FLAG_SEQUENTIAL_SCAN, FILE_FLAG_SEQUENTIAL_SCAN, FILE_FLAG_RANDOM_ACCESS };

	setLastError(NoError);

	close();

	_handle = CreateFileW(
		file_path.c_wstr(), 
		dwDesiredAccess[stream_mode], 
		0, 
		nullptr, 
		dwCreationDisposition[file_mode], 
		dwFlagsAndAttributes[stream_mode], 
		nullptr);

	if(_handle != INVALID_HANDLE_VALUE)
	{
		u32 flag = _Seekable;
		flag |= (dwDesiredAccess[stream_mode] & GENERIC_READ) != 0 ? _Readable : 0;
		flag |= (dwDesiredAccess[stream_mode] & GENERIC_WRITE) != 0 ? _Writeable : 0;
		_setup(flag);
		_setSize(GetFileSize(_handle, nullptr));
		_setFptr(0);
	}
	else
	{
		setLastError(OperationFailed);
		DEBUG_INFO(true, "WARNING: FileStream \"%s\" hasn't been opened.", file_path);
		_cleanup();
	}

	_setName(file_path);
}

void FileStream::close()
{
	if(_handle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(_handle);
		_handle = INVALID_HANDLE_VALUE;
	}
	_cleanup();
}

void FileStream::read(void* buffer, u32 size) const
{
	DEBUG_INFO(!readable(), "WARNING: A try of reading from stream without a such right. Stream name: \"%s\".", name());

	DWORD read = 0;
	ReadFile(_handle, buffer, size, &read, nullptr);
	_setFptr(fptr() + read);
}

void FileStream::write(const void* buffer, u32 size)
{
	DEBUG_INFO(!writeable(), "WARNING: A try of writing to stream without a such right. Stream name: \"%s\".", name());

	DWORD written = 0;
	WriteFile(_handle, buffer, size, &written, nullptr);
	_setFptr(fptr() + written);
	_setSize(GetFileSize(_handle, nullptr));
}

void FileStream::flush()
{
	DEBUG_INFO(!writeable(), "WARNING: A try of flushing stream without a write right. Stream name: \"%s\".", name());

	FlushFileBuffers(_handle);
}

void FileStream::seek(u32 position) const
{
	DEBUG_INFO(!opened(), "WARNING: A try of flushing the unopened stream. Stream name: \"%s\".", name());

	LARGE_INTEGER pos, result;
	pos.HighPart = 0;
	pos.LowPart = position;
	SetFilePointerEx(_handle, pos, &result, FILE_BEGIN);
	_setFptr(result.LowPart);
}

void FileStream::seek(s32 delta, StreamOrigin origin) const
{
	DEBUG_INFO(!seekable(), "WARNING: A try of seeking the unseekable stream. Stream name: \"%s\".", name());

	_setFptr(SetFilePointer(_handle, delta, nullptr, origin));
}

//- built-in types serialization ----------------------------------------------------------------------------------------------
void serialize(u64 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize u64, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize u64, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof u64);
	}
}

void deserialize(u64& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u64, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u64, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof u64);
	}
}

void serialize(u32 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize u32, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize u32, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof u32);
	}
}

void deserialize(u32& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u32, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u32, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof u32);
	}
}

void serialize(u16 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize u16, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize u16, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof u16);
	}
}

void deserialize(u16& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u16, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u16, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof u16);
	}
}

void serialize(u8 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize u8, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize u8, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof u8);
	}
}

void deserialize(u8& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u8, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize u8, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof u8);
	}
}

void serialize(s64 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize s64, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize s64, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof s64);
	}
}

void deserialize(s64& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s64, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s64, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof s64);
	}
}

void serialize(s32 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize s32, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize s32, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof s32);
	}
}

void deserialize(s32& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s32, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s32, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof s32);
	}
}

void serialize(s16 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize s16, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize s16, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof s16);
	}
}

void deserialize(s16& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s16, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s16, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof s16);
	}
}

void serialize(s8 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize s8, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize s8, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof s8);
	}
}

void deserialize(s8& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s8, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize s8, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof s8);
	}
}

void serialize(f32 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize f32, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize f32, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof f32);
	}
}

void deserialize(f32& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize f32, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize f32, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof f32);
	}
}

void serialize(f64 object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize f64, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize f64, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(&object, sizeof f64);
	}
}

void deserialize(f64& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize f64, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize f64, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(&object, sizeof f64);
	}
}

void serialize(bool object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize bool, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize bool, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		u32 temp = object;
		stream->write(&temp, sizeof u32);
	}
}

void deserialize(bool& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize bool, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize bool, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		u32 temp = 0;
		stream->read(&object, sizeof u32);
		object = temp == 1;
	}
}

namespace impl {

void serialize__vec2(const void* object, u32 size, DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec2<T>, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec2<T>, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(object, size);
	}
}

void serialize__vec3(const void* object, u32 size, DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec3<T>, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec3<T>, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(object, size);
	}
}

void serialize__vec4(const void* object, u32 size, DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec4<T>, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize __vec4<T>, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		stream->write(object, size);
	}
}

void deserialize__vec2(void* object, u32 size, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec2<T>, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec2<T>, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(object, size);
	}
}

void deserialize__vec3(void* object, u32 size, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec3<T>, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec3<T>, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(object, size);
	}
}

void deserialize__vec4(void* object, u32 size, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = 0;
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec4<T>, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = 0;
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize __vec4<T>, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		stream->read(object, size);
	}
}

} // namespace impl

//- string serialization ------------------------------------------------------------------------------------------------------
namespace impl {

extern u32 _stringHash(const u8* ptr);
extern u8* _stringAlloc(u32 bytes);
extern void _stringFree(u32 bytes, u8* data);
extern u32 _utf8count(const c16* first, const c16* last);
extern void _utf8copy(c8* dest, const c16* first, const c16* last);

struct _string
{
	u8*					_data;
	u32					_hash;
	u32					_size;
};

} // namespace impl

void serialize(const string& object, DataStream* stream)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize string, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize string, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		u32 bytes = object.size();
		u32 hash = object.hash();
		stream->write(&bytes, sizeof bytes);
		stream->write(&hash, sizeof hash);
		stream->write(object.data(), bytes);
	}
}

void deserialize(string& object, const DataStream* stream)
{
	if(stream == nullptr)
	{
		object = string();
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize string, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		object = string();
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize string, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		impl::_string result;
		u32 hash = 0;
		stream->read(&result._size, sizeof result._size);
		stream->read(&hash, sizeof hash);

		result._data = impl::_stringAlloc(result._size);

		stream->read(result._data, result._size);
		result._hash = impl::_stringHash(result._data);
		if(result._hash != hash)
		{
			object = string();
			impl::_stringFree(result._size, result._data);
			setLastError(DataCorruption);
			DEBUG_INFO(true, "ERROR: Cannot deserialize string due to data corruption. Stream name: %s.", stream->name());
		}
		else if(result._data != nullptr)
		{
			u32* refctr = (u32*)(result._data - 4);
			*refctr = 0;
			object = *(string*)(&result);
		}
	}
}

//- saving/loading text files -------------------------------------------------------------------------------------------------
void save(const string& text, DataStream* stream, bool write_bom)
{
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot save string, \'stream\' is nullptr.");
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot save string, stream isn't opened for writing. Stream name: %s.", stream->name());
	}
	else
	{
		if(write_bom)
		{
			u32 header = 0x00bfbbef;
			stream->write(&header, 3);
		}

		stream->write(text.data(), text.bytes());
	}
}

void load(string& text, const DataStream* stream)
{
	const u32 UTF8ByteOrderMark = 0x00bfbbef;
	const u32 UTF8ByteOrderMarkMask = 0x00ffffff;
	const u32 UTF8ByteOrderMarkSize = 3;
	const u32 UCS16ByteOrderMarkMask = 0x0000ffff;
	const u32 UCS16LEByteOrderMark = 0x0000feff;
	const u32 UCS16BEByteOrderMark = 0x0000fffe;
	const u32 USC16ByteOrderMarkSize = 2;
	
	const u32 BitsInByte = 8;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot load string, \'stream\' is nullptr.");
	}
	else if(!stream->readable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot load string, stream isn't opened for reading. Stream name: %s.", stream->name());
	}
	else
	{
		u32 header = 0;
		u32 toread = stream->size() < sizeof header ? stream->size() : sizeof header;
		impl::_string result;
		stream->read(&header, toread);

		result._size = 0;
		result._hash = 0;
		result._data = nullptr;

		if((header & UTF8ByteOrderMarkMask) == UTF8ByteOrderMark && toread != UTF8ByteOrderMarkSize)
		{
			result._size = stream->size() - UTF8ByteOrderMarkSize;
			result._data = impl::_stringAlloc(result._size);
			result._data[0] = header >> (UTF8ByteOrderMarkSize * BitsInByte);
			stream->read(result._data + 1, result._size - 1);
			result._hash = impl::_stringHash(result._data);
			u32* refctr = (u32*)(result._data - 4);
			*refctr = 0;
		}
		else if((header & UCS16ByteOrderMarkMask) == UCS16LEByteOrderMark && toread != USC16ByteOrderMarkSize)
		{
			u32 size = stream->size() - USC16ByteOrderMarkSize;
			c16* data = (c16*)impl::_stringAlloc(size);
			data[0] = header >> USC16ByteOrderMarkSize * BitsInByte;
			stream->read(data + 1, size - 2);

			result._size = impl::_utf8count(data, data + size / 2);
			result._data = impl::_stringAlloc(result._size);
			impl::_utf8copy((c8*)result._data, data, data + size / 2);
			result._hash = impl::_stringHash(result._data);
			u32* refctr = (u32*)(result._data - 4);
			*refctr = 0;

			impl::_stringFree(size, (u8*)data);
		}
		else if((header & UCS16ByteOrderMarkMask) == UCS16BEByteOrderMark && toread != USC16ByteOrderMarkSize)
		{
			u32 size = stream->size() - USC16ByteOrderMarkSize;
			c16* data = (c16*)impl::_stringAlloc(size);
			data[0] = header >> USC16ByteOrderMarkSize * BitsInByte;
			stream->read(data + 1, size - 2);

			for(u32 i = 0, s = size / 2; i < s; ++i)
				(u16&)(data[i]) = (u16)(data[i]) >> 8 | (u16)(data[i]) << 8;

			result._size = impl::_utf8count(data, data + size / 2);
			result._data = impl::_stringAlloc(result._size);
			impl::_utf8copy((c8*)result._data, data, data + size / 2);
			result._hash = impl::_stringHash(result._data);
			u32* refctr = (u32*)(result._data - 4);
			*refctr = 0;

			impl::_stringFree(size, (u8*)data);
		}
		else if(toread != 0)
		{
			result._size = stream->size();
			result._data = impl::_stringAlloc(result._size);

			for(u32 i = 0; i < toread; ++i)
			{
				(u8&)(result._data[i]) = header; 
				header >>= BitsInByte;
			}

			stream->read(result._data + toread, result._size - toread);
			result._hash = impl::_stringHash(result._data);
			u32* refctr = (u32*)(result._data - 4);
			*refctr = 0;
		}

		text = *(string*)(&result);
	}
}

} // namespace sparrow
