/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <vector>
#include <type_traits>
#include "error.h"
#include "std.h"

namespace vault
{

/**
 * Provides for an atomic series of operations with rollbacks. Rollbacks run
 * in reverse so that things can be torn down properly.
 */
class Transaction
{
public:
	class Op
	{
	public:
		virtual ~Op()
		{
		}

		virtual void undo() noexcept = 0;
	};

private:
	bool ok_ = false;
	std::vector<up<Op>> ops_;

public:
	~Transaction();

	/**
	 * Add something that can be rolled back. If the operation fails,
	 * the operation's corresponding undo() function will not be called.
	 */
	template <typename T, typename... Args> auto run(Args &&... args)
	{
		auto op = std::make_unique<T>();
		this->ops_.push_back(std::move(op));

		try {
			return op->run(std::forward<Args>(args)...);
		} catch (Error e) {
			// I'm not a fan of this block, but I'm not sure what else to do
			this->ops_.pop_back();
			throw e;
		}
	}

	/**
	 * Mark the series as "ok"
	 */
	void commit();
};
}
