/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "std.h"
#include <string>
#include <stdexcept>

namespace vault
{

/**
 * For handling generic errors where error codes < 0. This is THE ONLY
 * exception thrown. If something fails, it throws itself.
 *
 * Normally I would object to exceptions, but given the simplicity of this
 * tool, I just don't see a reason to have detailed error handling and
 * recovery. Here, if something fails, you just give up, and exceptions are
 * pretty good at that.
 */
class Error : public std::exception
{
protected:
	Error() = default;

public:
	int err_ = 0;
	std::string msg_;

	/**
	 * The same as set()
	 */
	PRINTF(3, 4) Error(int err, const char *format, ...);

	/**
	 * Get a string description of the error.
	 */
	const char *what() const noexcept
	{
		return this->msg_.c_str();
	}
};

/**
 * For handling OS error codes
 */
struct OSError : public Error {
	/**
	 * Set the error, formatted with strerror(errno), iff err < 0
	 */
	PRINTF(3, 4)
	OSError(int err, const char *format, ...);
};

/**
 * For handling basic assertions. Rather than calling assert (which could
 * leave secrets in memory), use this.
 */
struct Assert : public Error {
	/**
	 * Set the error, formatted with strerror(errno), iff err < 0
	 */
	PRINTF(3, 4)
	Assert(bool a, const char *format, ...);
};
}
