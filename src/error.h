/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "std.h"
#include <string>
#include <stdexcept>

/**
 * For handling generic errors where error codes < 0
 */
struct Error : public std::exception {
	int err_ = 0;
	std::string msg_;

	/**
	 * Create an error where ok() returns true
	 */
	Error() noexcept = default;

	/**
	 * The same as set()
	 */
	PRINTF(3, 4) Error(int err, const char *format, ...) noexcept;

	/**
	 * Set the error iff err < 0
	 */
	PRINTF(3, 4)
	void set(int err, const char *format, ...) noexcept;

	/**
	 * Set the error iff err < 0
	 */
	PRINTF(3, 0)
	void vset(int err, const char *format, va_list args) noexcept;

	/**
	 * If no error occurred
	 */
	inline bool ok() noexcept
	{
		return this->err_ >= 0;
	}

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
	OSError(int err, const char *format, ...) noexcept;
};
