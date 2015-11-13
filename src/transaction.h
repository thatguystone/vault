/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <vector>
#include <type_traits>
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
	 * Add something that can be rolled back
	 */
	template <typename T, typename... Args> auto run(Args &&... args)
	{
		auto op = std::make_unique<T>();
		auto p = op.get();

		this->ops_.push_back(std::move(op));

		return p->run(std::forward<Args>(args)...);
	}

	/**
	 * Run the series
	 */
	void commit();
};
}
