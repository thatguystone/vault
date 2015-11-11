/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "std.h"
#include <string>

struct Error {
	int err_ = 0;
	std::string msg_;

	Error() = default;

	PRINTF(3, 4)
	Error(int err, const char *format, ...);

	PRINTF(3, 4)
	void set(int err, const char *format, ...);

	PRINTF(3, 0)
	void vset(int err, const char *format, va_list args);

	inline bool ok()
	{
		return this->err_ >= 0;
	}
};

struct OSError : public Error {
	PRINTF(3, 4)
	OSError(int err, const char *format, ...)
	{
		if (err >= 0) {
			this->err_ = err;
		} else {
			err = errno;

			va_list args;
			va_start(args, format);
			this->vset(-err, format, args);
			va_end(args);

			this->msg_.append(": ");
			this->msg_.append(strerror(err));
		}
	}
};
