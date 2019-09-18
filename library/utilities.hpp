#pragma once
#include "vector.hpp"
namespace sparrow {

u32 bitcount(u32 value);
u32 bitcount(u64 value);
void leftshift(u32* vec, u32 size, u32 shift);
void rightshift(u32* vec, u32 size, u32 shift);
u32 msbindex(u32 value);
u32 msbindex(u64 value);

double timef64();
float timef32();
u64 ntimeu64();
u32 utimeu32();
void sleep(u32 ms);

void deleteFile(const string& path);

const u32 kiB = 1024;
const u32 MiB = 1024 * 1024;

} //namespace sparrow
