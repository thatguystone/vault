/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "error.h"

namespace vault
{

#define CHECK()                                                                \
	if (err >= 0) {                                                            \
		return;                                                                \
	}                                                                          \
	va_list args;                                                              \
	char buff[8192];                                                           \
	va_start(args, format);                                                    \
	vsnprintf(buff, sizeof(buff), format, args);                               \
	va_end(args);                                                              \
	this->err_ = err;                                                          \
	this->msg_ = buff;                                                         \
	if (extra != nullptr) {                                                    \
		this->msg_.append(": ");                                               \
		this->msg_.append(extra);                                              \
	}                                                                          \
	throw * this;

Error::Error(int err, const char *format, ...)
{
	const char *extra = nullptr;
	CHECK();
}

OSError::OSError(int err, const char *format, ...)
{
	const char *extra = nullptr;
	if (err < 0) {
		extra = strerror(errno);
		err = -errno;
	}

	CHECK();
}

Assert::Assert(bool a, const char *format, ...)
{
	int err = a ? 0 : -1;
	const char *extra = nullptr;
	CHECK();
}
}
