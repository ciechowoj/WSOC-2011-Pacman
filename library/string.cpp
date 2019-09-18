#include "string.hpp"
#include <math.h>
#include <stdio.h>
namespace sparrow {

//- numeric conversions from range --------------------------------------------------------------------------------------------
u64 strtou64(const string::iterator& first, const string::iterator& last)
{
	u64 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(isbinpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}
	return result;
}

u32 strtou32(const string::iterator& first, const string::iterator& last)
{
	u32 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(isbinpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}
	return result;
}


s64 strtos64(const string::iterator& first, const string::iterator& last)
{
	s64 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}

	return result * sign;
}

s32 strtos32(const string::iterator& first, const string::iterator& last)
{
	s32 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}

	return result * sign;
}

u64 dectou64(const string::iterator& first, const string::iterator& last)
{
	u64 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result;
}

u32 dectou32(const string::iterator& first, const string::iterator& last)
{
	u32 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result;
}

s64 dectos64(const string::iterator& first, const string::iterator& last)
{
	s64 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result * sign;
}

s32 dectos32(const string::iterator& first, const string::iterator& last)
{
	s32 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result * sign;
}

u64 hextou64(const string::iterator& first, const string::iterator& last)
{
	u64 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result;
}

u32 hextou32(const string::iterator& first, const string::iterator& last)
{
	u32 result = 0;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result;
}

s64 hextos64(const string::iterator& first, const string::iterator& last)
{
	s64 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result * sign;
}

s32 hextos32(const string::iterator& first, const string::iterator& last)
{
	s32 result = 0, sign = 1;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((issign(*ptr) || iswhite(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0' && ptr < end)
	{
		++ptr;
		if(ishexpref(*ptr) && ptr < end)
		{
			++ptr;
			while(ishex(*ptr) && ptr < end)
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result * sign;
}

f64 strtof64(const string::iterator& first, const string::iterator& last)
{
	f64 result = 0.f, sign = 1.f, exp_st = 0.f;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((iswhite(*ptr) || issign(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10.f;
		result += *ptr - 0x30;
		++ptr;
	}

	if(*ptr == '.' && ptr < end)
	{
		++ptr;
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10.f;
			result += *ptr - 0x30;
			exp_st -= 1.f;
			++ptr;
		}
	}

	if(isscipref(*ptr) && ptr < end)
	{
		++ptr;
		f64 sign = 1.f, exp_nd = 0.f;
		if(*ptr == L'-' && ptr < end)
		{
			sign = -1.f; 
			++ptr;
		}
		else if(*ptr == L'+' && ptr < end)
		{
			++ptr;
		}
		while(isdec(*ptr) && ptr < end)
		{
			exp_nd *= 10.f;
			exp_nd += *ptr - 0x30;
			++ptr;
		}
		exp_st += exp_nd * sign;
	}

	return result * (f64)pow(10.0, (double)exp_st) * sign;
}


f32 strtof32(const string::iterator& first, const string::iterator& last)
{
	f32 result = 0.f, sign = 1.f, exp_st = 0.f;

	c8 *ptr, *end;
	ptr = absolute_cast<c8*>(first);
	end = absolute_cast<c8*>(last);

	while((iswhite(*ptr) || issign(*ptr)) && ptr < end)
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr) && ptr < end)
	{
		result *= 10.f;
		result += *ptr - 0x30;
		++ptr;
	}

	if(*ptr == '.' && ptr < end)
	{
		++ptr;
		while(isdec(*ptr) && ptr < end)
		{
			result *= 10.f;
			result += *ptr - 0x30;
			exp_st -= 1.f;
			++ptr;
		}
	}

	if(isscipref(*ptr) && ptr < end)
	{
		++ptr;
		f32 sign = 1.f, exp_nd = 0.f;
		if(*ptr == L'-' && ptr < end)
		{
			sign = -1.f; 
			++ptr;
		}
		else if(*ptr == L'+' && ptr < end)
		{
			++ptr;
		}
		while(isdec(*ptr) && ptr < end)
		{
			exp_nd *= 10.f;
			exp_nd += *ptr - 0x30;
			++ptr;
		}
		exp_st += exp_nd * sign;
	}

	return result * (f32)pow(10.0, (double)exp_st) * sign;
}

//- numeric conversions from string -------------------------------------------------------------------------------------------
u64 strtou64(const string& str)
{
	u64 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	if(*ptr == '0')
	{
		++ptr;
		if(isbinpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr))
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}
	return result;
}

u32 strtou32(const string& str)
{
	u32 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	if(*ptr == '0')
	{
		++ptr;
		if(isbinpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr))
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}
	return result;
}


s64 strtos64(const string& str)
{
	s64 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while((issign(*ptr) || iswhite(*ptr)))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr))
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}

	return result * sign;
}

s32 strtos32(const string& str)
{
	s32 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while((issign(*ptr) || iswhite(*ptr)))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}
	else
	{
		while(isdec(*ptr))
		{
			result *= 10;
			result += *ptr - 0x30;
			++ptr;
		}
	}

	return result * sign;
}

u64 dectou64(const string& str)
{
	u64 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while(isdec(*ptr))
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result;
}

u32 dectou32(const string& str)
{
	u32 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while(isdec(*ptr))
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result;
}

s64 dectos64(const string& str)
{
	s64 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while(issign(*ptr) || iswhite(*ptr))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr))
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result * sign;
}

s32 dectos32(const string& str)
{
	s32 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while(issign(*ptr) || iswhite(*ptr))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr))
	{
		result *= 10;
		result += *ptr - 0x30;
		++ptr;
	}

	return result * sign;
}

u64 hextou64(const string& str)
{
	u64 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result;
}

u32 hextou32(const string& str)
{
	u32 result = 0;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result;
}

s64 hextos64(const string& str)
{
	s64 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while((issign(*ptr) || iswhite(*ptr)))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result * sign;
}

s32 hextos32(const string& str)
{
	s32 result = 0, sign = 1;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while((issign(*ptr) || iswhite(*ptr)))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	if(*ptr == '0')
	{
		++ptr;
		if(ishexpref(*ptr))
		{
			++ptr;
			while(ishex(*ptr))
			{
				result *= 16;
				result += hextodig(*ptr);
				++ptr;
			}
		}
	}

	return result * sign;
}

f64 strtof64(const string& str)
{
	f64 result = 0.f, sign = 1.f, exp_st = 0.f;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while((iswhite(*ptr) || issign(*ptr)))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr))
	{
		result *= 10.f;
		result += *ptr - 0x30;
		++ptr;
	}

	if(*ptr == '.')
	{
		++ptr;
		while(isdec(*ptr))
		{
			result *= 10.f;
			result += *ptr - 0x30;
			exp_st -= 1.f;
			++ptr;
		}
	}

	if(isscipref(*ptr))
	{
		++ptr;
		f64 sign = 1.f, exp_nd = 0.f;
		if(*ptr == L'-')
		{
			sign = -1.f; 
			++ptr;
		}
		else if(*ptr == L'+')
		{
			++ptr;
		}
		while(isdec(*ptr))
		{
			exp_nd *= 10.f;
			exp_nd += *ptr - 0x30;
			++ptr;
		}
		exp_st += exp_nd * sign;
	}

	return result * (f64)pow(10.0, (double)exp_st) * sign;
}


f32 strtof32(const string& str)
{
	f32 result = 0.f, sign = 1.f, exp_st = 0.f;

	c8 *ptr = absolute_cast<c8*>(str.begin());

	while(iswhite(*ptr) || issign(*ptr))
	{
		if(*ptr == '-')
			sign *= -1;
		++ptr;
	}

	while(isdec(*ptr))
	{
		result *= 10.f;
		result += *ptr - 0x30;
		++ptr;
	}

	if(*ptr == '.')
	{
		++ptr;
		while(isdec(*ptr))
		{
			result *= 10.f;
			result += *ptr - 0x30;
			exp_st -= 1.f;
			++ptr;
		}
	}

	if(isscipref(*ptr))
	{
		++ptr;
		f32 sign = 1.f, exp_nd = 0.f;
		if(*ptr == L'-')
		{
			sign = -1.f; 
			++ptr;
		}
		else if(*ptr == L'+')
		{
			++ptr;
		}
		while(isdec(*ptr))
		{
			exp_nd *= 10.f;
			exp_nd += *ptr - 0x30;
			++ptr;
		}
		exp_st += exp_nd * sign;
	}

	return result * (f32)pow(10.0, (double)exp_st) * sign;
}

string u64tostr(u64 val)
{
	c8 buffer[21];
	buffer[20] = 0;
	c8* ptr = buffer + 20;

	if(val == 0)
	{
		*--ptr = '0';
	}
	else
	{
		while(val != 0)
		{
			*--ptr = val % 10 + 0x30;
			val /= 10;
		}
	}
	return string(ptr, buffer + 20);
}

string u32tostr(u32 val)
{
	c8 buffer[11];
	buffer[10] = 0;
	c8* ptr = buffer + 10;

	if(val == 0)
	{
		*--ptr = '0';
	}
	else
	{
		while(val != 0)
		{
			*--ptr = val % 10 + 0x30;
			val /= 10;
		}
	}
	return string(ptr, buffer + 10);
}

string s64tostr(s64 val)
{
	c8 buffer[22];
	buffer[21] = 0;
	c8* ptr = buffer + 21;
	s64 sign = val < 0 ? -1 : 1;
	val *= sign;

	if(val == 0)
	{
		*--ptr = '0';
	}
	else
	{
		while(val != 0)
		{
			*--ptr = val % 10 + 0x30;
			val /= 10;
		}

		if(sign == -1)
			*--ptr = '-';
	}
	return string(ptr, buffer + 21);
}

string s32tostr(s32 val)
{
	c8 buffer[12];
	buffer[11] = 0;
	c8* ptr = buffer + 11;
	s32 sign = val < 0 ? -1 : 1;
	val *= sign;

	if(val == 0)
	{
		*--ptr = '0';
	}
	else
	{
		while(val != 0)
		{
			*--ptr = val % 10 + 0x30;
			val /= 10;
		}

		if(sign == -1)
			*--ptr = '-';
	}
	return string(ptr, buffer + 11);
}

string f64tostr(f64 val)
{
	c8 buffer[128];
	sprintf_s(buffer, 128, "%g", val);
	return string(buffer);
}

string f32tostr(f32 val)
{
	c8 buffer[64];
	sprintf_s(buffer, 64, "%g", val);
	return string(buffer);
}

//- string manipultion --------------------------------------------------------------------------------------------------------

bool strncmp(string::iterator itr, const string& str)
{
	return ::strncmp(absolute_cast<c8*>(itr), absolute_cast<c8*>(str.begin()), str.bytes()) == 0; 
}


//- class StringLexer ---------------------------------------------------------------------------------------------------------
StringLexer::StringLexer()
	: _line(0)
	, _commentInline("//")
	, _commentBegin("/*")
	, _commentEnd("*/")
{ }

StringLexer::~StringLexer()
{ }

void StringLexer::setSource(const string& source)
{
	_source = source;
	_itr = source.begin();
	_line = 1;
}

void StringLexer::setOperatorMap(const vector_map<string, u32>& operator_map)
{
	_operatorMap = operator_map;
}

void StringLexer::setKeywordMap(const vector_map<string, u32>& keyword_map)
{
	_keywordMap = keyword_map;
}

void StringLexer::setComments(const string& inline_comment, const string& multiline_comment_begin, const string& multiline_comment_end)
{
	_commentInline = inline_comment;
	_commentBegin = multiline_comment_begin;
	_commentEnd = multiline_comment_end;
}

Lexem StringLexer::getLexem(string& out)
{
	setLastError(NoError);
	Lexem lexem = LexemFinished;
	string::iterator end = _source.end();
	string::iterator itr = _itr;
	string::iterator first, last;
	bool operators = false;

	if(itr < end)
	{
		bool whitespace = true;
		while(whitespace)
		{
			// set whitespace flag
			whitespace = false;

			// avoid whitespaces
			while(iswhite(*itr))
			{
				if(*itr == '\n')
					++_line;
				++itr;
			}

			// avoid inline comment
			while(strncmp(itr, _commentInline))
			{
				while(*itr != '\n' && itr < end)
					++itr;
				whitespace = true;
			}

			// avoid multiline comment
			while(strncmp(itr, _commentBegin) && _commentBegin.size() != 0)
			{
				do
				{
					if(*itr == '\n')
						++_line;
					++itr;
				} while(strncmp(itr, _commentEnd) && itr < end);
				itr += _commentEnd.size();
				whitespace = true;
			}
		}

		// label _abcABC123
		if(isalpha(*itr) || *itr == '_')
		{
			first = itr;
			while(isalpha(*itr) || isdec(*itr) || *itr == '_')
				++itr;
			last = itr;

			out = string(first, last);
			lexem = _keywordMap.count(out) == 1 ? LexemKeyword : LexemLabel;
		}
		// string "string"
		else if(*itr == '"')
		{
			first = ++itr;
			while(*itr != '"')
			{
				if(*itr == '\n')
					setLastError(InvalidArg);
				if(itr == end)
					setLastError(InvalidArg);
				else
					++itr;
			} 
			last = itr != end ? itr++ : end;

			out = string(first, last);
			lexem = LexemString;
		}
		// character 'c'
		else if(*itr == '\'')
		{
			first = ++itr;
			while(*itr != '\'')
			{
				if(*itr == '\n')
					setLastError(InvalidArg);
				if(itr == end)
					setLastError(InvalidArg);
				else
					++itr;
			}
			last = itr != end ? itr++ : end;

			out = string(first, last);
			lexem = LexemCharacter;
		}
		// digit beginning with +-0...9
		else if(isdec(*itr) || issign(*itr))
		{
			first = itr;
			while(issign(*itr) || iswhite(*itr)) ++itr;
			if(isdec(*itr) || *itr == '.')
			{
				if(*itr == 0)
				{
					if(*++itr == '.')
					{
						while(isdec(*++itr));
						if(isscipref(*itr))
						{
							++itr;
							while(issign(*itr) || iswhite(*itr)) ++itr;
							while(isdec(*itr)) ++itr;
						}
						lexem = LexemFloating;
					}
					else if(ishexpref(*itr))
					{
						while(ishex(*++itr))
						lexem = LexemInteger;
					}
					else
					{
						lexem = LexemInteger;
					}
				}
				else if(isdec(*itr))
				{
					while(isdec(*++itr));
					if(*itr == '.')
					{
						++itr;
						while(isdec(*++itr));
						if(isscipref(*itr))
						{
							++itr;
							while(issign(*itr) || iswhite(*itr)) ++itr;
							while(isdec(*itr)) ++itr;
						}
						lexem = LexemFloating;
					}
					lexem = lexem == LexemFloating ? LexemFloating : LexemInteger;
				}
				else if(*itr == '.')
				{
					if(isdec(*++itr))
					{
						while(isdec(*++itr));
						if(isscipref(*itr))
						{
							++itr;
							while(issign(*itr) || iswhite(*itr)) ++itr;
							while(isdec(*itr)) ++itr;
						}
						lexem = LexemFloating;
					}
					else
					{
						itr = first;
						operators = true;
					}
				}
				if(operators == false)
				{
					last = itr;
					out = string(first, last);
				}
			}
			else
			{
				itr = first;
				operators = true;
			}
		}
		else
		{
			operators = true;
		}
			
		if(operators == true)
		{
			if(ispunct(*itr))
			{
				first = itr;
				while(ispunct(*++itr));
				last = itr;
				
				string op[3];
				for(u32 i = 0; i < 3 && (first + i) != last; ++i)
					op[i] = string(first, first + i + 1);

				s32 i = 2;
				while(i >= 0)
				{
					if(_operatorMap.count(op[i]) == 1)
					{
						out = op[i];
						itr = first + i + 1;
						break;
					}
					--i;
				}

				if(i == -1)
				{
					setLastError(InvalidArg);
					lexem = LexemUnknown;
				}
				else
				{
					lexem = LexemOperator;
				}
			}
			else 
			{
				if(itr < end)
				{
					setLastError(InvalidArg);
					lexem = LexemUnknown;
				}
				else
				{
					lexem = LexemFinished;
				}
			}

		}
	}

	_itr = itr;
	return lexem;
}

u32 StringLexer::getLine() const
{
	return _line;
}

} // namespace sparrow
