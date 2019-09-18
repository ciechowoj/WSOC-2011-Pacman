#include "vector.hpp"
#include <cstdlib>
namespace sparrow {
namespace impl {

bool vec_serialize_header(u32 size, DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector<T>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector<T>, stream isn't opened for writing. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		serialize(size, stream);
	}
	return result;
}

bool vec_deserialize_header(u32& size, const DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector<T>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->readable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector<T>, stream isn't opened for reading. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		deserialize(size, stream);
	}
	return result;
}


bool vecset_serialize_header(u32 size, DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_set<T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_set<T, C>, stream isn't opened for writing. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		serialize(size, stream);
	}
	return result;
}


bool vecset_deserialize_header(u32& size, const DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_set<T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->readable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_set<T, C>, stream isn't opened for reading. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		deserialize(size, stream);
	}
	return result;
}

bool vecmultiset_serialize_header(u32 size, DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_multiset<T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_multiset<T, C>, stream isn't opened for writing. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		serialize(size, stream);
	}
	return result;
}


bool vecmultiset_deserialize_header(u32& size, const DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_multiset<T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->readable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_multiset<T, C>, stream isn't opened for reading. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		deserialize(size, stream);
	}
	return result;
}

bool vecmap_serialize_header(u32 size, DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_map<K, T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->writeable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot serialize vector_map<K, T, C>, stream isn't opened for writing. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		serialize(size, stream);
	}
	return result;
}

bool vecmap_deserialize_header(u32& size, const DataStream* stream)
{
	bool result = true;
	if(stream == nullptr)
	{
		setLastError(InvalidCall);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_map<K, T, C>, \'stream\' is nullptr.");
		result = false;
	}
	else if(!stream->readable())
	{
		setLastError(InvalidArg);
		DEBUG_INFO(true, "ERROR: Cannot deserialize vector_map<K, T, C>, stream isn't opened for reading. Stream name: %s.", stream->name());
		result = false;
	}
	else
	{
		deserialize(size, stream);
	}
	return result;
}

void vec_move(void* dest, const void* src, u32 size)
{
	memmove(dest, src, size);
}

void vec_copy(void* dest, const void* src, u32 size)
{
	memcpy(dest, src, size);
}

} // namespace impl
} // namespace sparrow
