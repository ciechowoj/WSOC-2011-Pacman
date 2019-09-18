#pragma once
#include "vector.hpp"
namespace sparrow {

bool iswhite(c16 c);
bool isdec(c16 c);
bool ishex(c16 c);
bool isbin(c16 c);
bool issign(c16 c);
bool isalpha(c16 c);
bool ishexpref(c16 c);
bool isbinpref(c16 c);
bool isscipref(c16 c);
bool ispunct(c16 c);
s32 hextodig(c16 c);
c16 digtohex(s32 a);

u64 strtou64(const string::iterator& first, const string::iterator& last);
u32 strtou32(const string::iterator& first, const string::iterator& last);
s64 strtos64(const string::iterator& first, const string::iterator& last);
s32 strtos32(const string::iterator& first, const string::iterator& last);
u64 dectou64(const string::iterator& first, const string::iterator& last);
u32 dectou32(const string::iterator& first, const string::iterator& last);
s64 dectos64(const string::iterator& first, const string::iterator& last);
s32 dectos32(const string::iterator& first, const string::iterator& last);
u64 hextou64(const string::iterator& first, const string::iterator& last);
u32 hextou32(const string::iterator& first, const string::iterator& last);
s64 hextos64(const string::iterator& first, const string::iterator& last);
s32 hextos32(const string::iterator& first, const string::iterator& last);
f64 strtof64(const string::iterator& first, const string::iterator& last);
f32 strtof32(const string::iterator& first, const string::iterator& last);

u64 strtou64(const string& str);
u32 strtou32(const string& str);
s64 strtos64(const string& str);
s32 strtos32(const string& str);
u64 dectou64(const string& str);
u32 dectou32(const string& str);
s64 dectos64(const string& str);
s32 dectos32(const string& str);
u64 hextou64(const string& str);
u32 hextou32(const string& str);
s64 hextos64(const string& str);
s32 hextos32(const string& str);
f64 strtof64(const string& str);
f32 strtof32(const string& str);

string u64tostr(u64 val);
string u32tostr(u32 val);
string s64tostr(s64 val);
string s32tostr(s32 val);
string f64tostr(f64 val);
string f32tostr(f32 val);
string u64tohex(u64 val);
string u32tohex(u32 val);
string s64tohex(s64 val);
string s32tohex(s32 val);
string u64tobin(u64 val);
string u32tobin(u32 val);
string s64tobin(s64 val);
string s32tobin(s32 val);

string uppercase(const string& str);
string lowercase(const string& str);

bool strncmp(string::iterator itr, const string& str);

enum Lexem
{
	LexemFinished,
	LexemUnknown,
	LexemLabel,
	LexemString,
	LexemCharacter,
	LexemFloating,
	LexemInteger,
	LexemOperator,
	LexemKeyword,
};

class StringLexer
{
public:
						StringLexer();
						~StringLexer();

	void				setSource(const string& source);
	void				setOperatorMap(const vector_map<string, u32>& operator_map);
	void				setKeywordMap(const vector_map<string, u32>& keyword_map);
	void				setComments(const string& inline_comment, const string& multiline_comment_begin, const string& multiline_comment_end);

	Lexem				getLexem(string& out);
	u32					getLine() const;
private:
	vector_map<string, u32> _operatorMap;
	vector_map<string, u32> _keywordMap;
	string				_commentInline;
	string				_commentBegin;
	string				_commentEnd;
	string				_source;
	string::iterator	_itr;
	u32					_line;
};

//- inline impls --------------------------------------------------------------------------------------------------------------
inline bool iswhite(c16 c)
{
	return c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r' || c == '\f';
}

inline bool isdec(c16 c)
{
	return 0x2f < c && c < 0x3a;
}

inline bool ishex(c16 c)
{
	return (c > 0x2f && c < 0x3a) || (c > 0x40 && c < 0x47) || (c > 0x60 && c < 0x67);
}

inline bool isbin(c16 c)
{
	return c == '1' || c == '0';
}

inline bool issign(c16 c)
{
	return c == '+' || c == '-';
}

inline bool isalpha(c16 c)
{
	return (0x40 < c && c < 0x5b) || (0x60 < c && c < 0x7b);
}

inline bool ispunct(c16 c)
{
	return  (0x20 < c && c < 0x30) || (0x39 < c && c < 0x41) || (0x5a < c && c < 0x61) || (0x7a < c && c < 0x7f);
}

inline bool ishexpref(c16 c)
{
	return c == 'x' || c == 'X';
}

inline bool isbinpref(c16 c)
{
	return c == 'n' || c == 'N';
}

inline bool isscipref(c16 c)
{
	return c == 'e' || c == 'E';
}

inline s32 hextodig(c16 c)
{
	s32 result = 0;
	if(c > 0x40 && c < 0x47)
		result += c - 0x37;
	else if(c > 0x60 && c < 0x67)
		result += c - 0x57;
	else
		result += c - 0x30;

	return result;
}

inline c16 digtohex(s32 a)
{
	c16 result = 0;
	if(a < 10)
		result = a + 0x30;
	else
		result = a + 0x37;
	return result;
}

} // namespace sparrow
