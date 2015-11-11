/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "error.h"

Error::Error(int err, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	this->vset(err, format, args);
	va_end(args);
}

void Error::set(int err, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	this->vset(err, format, args);
	va_end(args);
}

void Error::vset(int err, const char *format, va_list args)
{
	if (err >= 0) {
		return;
	}

	this->err_ = err;

	char buff[8192];
	vsnprintf(buff, sizeof(buff), format, args);
	this->msg_ = buff;
}
