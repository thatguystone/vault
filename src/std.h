/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <inttypes.h>
#include <limits>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <string>
#include <unistd.h>

#define __STR(s) #s
#define STR(s) __STR(s)
#define NELS(a) (sizeof(a) / sizeof((a)[0]))
#define LIKELY(cond) (__builtin_expect(!!(cond), 1))
#define UNLIKELY(cond) (__builtin_expect(!!(cond), 0))
#define PRINTF(f, a) __attribute__((__format__(__printf__, f, a)))
#define CHECK_C_ALLOC(val)                                                     \
	if (UNLIKELY((val) == NULL)) {                                             \
		throw std::bad_alloc();                                                \
	}

template <typename T> using sp = std::shared_ptr<T>;
template <typename T> using up = std::unique_ptr<T>;

// namespace std
// {
// template <class T> struct _Unique_if {
// 	typedef unique_ptr<T> _Single_object;
// };

// template <class T> struct _Unique_if<T[]> {
// 	typedef unique_ptr<T[]> _Unknown_bound;
// };

// template <class T, size_t N> struct _Unique_if<T[N]> {
// 	typedef void _Known_bound;
// };

// template <class T, class... Args>
// typename _Unique_if<T>::_Single_object make_unique(Args &&... args)
// {
// 	return unique_ptr<T>(new T(std::forward<Args>(args)...));
// }

// template <class T> typename _Unique_if<T>::_Unknown_bound make_unique(size_t
// n)
// {
// 	typedef typename remove_extent<T>::type U;
// 	return unique_ptr<T>(new U[n]());
// }

// template <class T, class... Args>
// typename _Unique_if<T>::_Known_bound make_unique(Args &&...) = delete;
// }
