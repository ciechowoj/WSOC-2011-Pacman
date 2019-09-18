#pragma once
#include "config.hpp"
namespace sparrow {


template <class T, T v> struct integral_constant
{
	static const T		value = v;
	typedef T			value_type;
	typedef integral_constant<T,v> type;
						operator value_type() { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template <class T> struct is_void : public false_type { };
template <> struct is_void<void> : public true_type { };

template <class T> struct is_integral : public false_type { };
template <> struct is_integral<signed char> : public true_type { };
template <> struct is_integral<unsigned char> : public true_type { };
template <> struct is_integral<signed short> : public true_type { };
template <> struct is_integral<unsigned short> : public true_type { };
template <> struct is_integral<signed int> : public true_type { };
template <> struct is_integral<unsigned int> : public true_type { };
template <> struct is_integral<signed long long> : public true_type { };
template <> struct is_integral<unsigned long long> : public true_type { };
template <> struct is_integral<bool> : public true_type { };
template <> struct is_integral<wchar_t> : public true_type { };

template <class T> struct is_floating_point : public false_type { };
template <> struct is_floating_point<float> : public true_type { };
template <> struct is_floating_point<double> : public true_type { };

template <class T> struct is_vector : public false_type { };
template <class T> struct is_vector<__vec2<T>> : public true_type { };
template <class T> struct is_vector<__vec3<T>> : public true_type { };
template <class T> struct is_vector<__vec4<T>> : public true_type { };

template <class T> struct is_array : public false_type { };
template <class T> struct is_array<T[]> : public true_type { };
template <class T, u32 n> struct is_array<T[n]> : public true_type { };

template <class T> struct is_pointer : public false_type { };
template <class T> struct is_pointer<T*> : public true_type { };

template <class T> struct is_enum : public integral_constant<bool, __is_enum(T)> { };
template <class T> struct is_union : public integral_constant<bool, __is_union(T)> { };
template <class T> struct is_class : public integral_constant<bool, __is_class(T)> { };

template <class T> struct is_reference : public false_type { };
template <class T> struct is_reference<T&> : public false_type { };

template <class T> struct is_arithmetic : public integral_constant<bool, is_integral<T>::value || is_floating_point<T>::value || is_vector<T>::value> { };
template <class T> struct is_fundamental : public integral_constant<bool, is_arithmetic<T>::value || is_void<T>::value> { };

template <class T> struct is_const : public false_type { };
template <class T> struct is_const<const T> : public true_type { };

template <class T> struct is_volatile : public false_type { };
template <class T> struct is_volatile<volatile T> : public true_type { };

template <class T> struct is_trivially_copyable : public integral_constant<bool, (__has_trivial_assign(T) && __has_trivial_copy(T) && __has_trivial_destructor(T)) || is_pointer<T>::value || is_arithmetic<T>::value || is_enum<T>::value> { };
template <class T> struct is_standard_layout : public integral_constant<bool, __is_pod(T) || is_pointer<T>::value || is_arithmetic<T>::value || is_enum<T>::value> { };

template <class T> struct is_pod : public integral_constant<bool, is_standard_layout<T>::value && is_trivially_copyable<T>::value> { };
//template <class T> struct is_literal_type;
template <class T> struct is_empty : public integral_constant<bool, __is_empty(T)> { };
//template <class T> struct is_polymorphic;
//template <class T> struct is_abstract;
//template <class T> struct is_signed;
//template <class T> struct is_unsigned;

//template <class T> struct is_default_constructible;
//template <class T> struct is_copy_constructible;
//template <class T> struct is_move_constructible;

template <class T> struct is_trivially_destructible : public integral_constant<bool, __has_trivial_destructor(T) || is_pointer<T>::value || is_arithmetic<T>::value || is_enum<T>::value> { };

} // namespace sparrow
