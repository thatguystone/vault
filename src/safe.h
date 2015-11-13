/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <libcryptsetup.h>
#include "error.h"
#include "std.h"

namespace vault
{

/**
 * Adapted from libcrypsetup's safe_alloc functions.
 */
class Safe
{
	uint64_t c_ = 0;
	void *m_ = nullptr;

	void dispose(void *m, uint64_t c)
	{
		volatile uint8_t *p = (volatile uint8_t *)m;

		while (c--) {
			*p++ = 0;
		}

		free(m);
	}

public:
	/**
	 * Create with no backing store
	 */
	Safe()
	{
		int locked = crypt_memory_lock(nullptr, 1);
		if (!locked) {
			throw Error(-1, "Failed to lock memory. Are you running as root?");
		}
	}

	/**
	 * Create with a buffer capable of holding the given size
	 */
	Safe(uint64_t size)
	{
		this->resize(size);
	}

	/**
	 * Don't make copies. The more copies, the higher the chances of doing
	 * something wrong.
	 */
	Safe(const Safe &) = delete;

	/**
	 * Moving is fine. Still only 1 copy around.
	 */
	Safe(Safe &&o)
	{
		this->c_ = o.c_;
		this->m_ = o.m_;

		o.c_ = 0;
		o.m_ = nullptr;
	}

	~Safe()
	{
		this->dispose(this->m_, this->c_);
		crypt_memory_lock(nullptr, 0);
	}

	void resize(uint64_t c)
	{
		void *m = malloc(c);
		CHECK_C_ALLOC(m);

		memcpy(m, this->m_, std::min(this->c_, c));
		this->dispose(this->m_, this->c_);

		this->m_ = m;
		this->c_ = c;
	}
};
}
