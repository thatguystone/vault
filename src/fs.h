/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "transaction.h"

namespace vault
{
namespace fs
{

/**
 * Untyped strings are confusing. This refers to a specific device in /dev/
 */
class Dev
{
	std::string dev_;

public:
	Dev(std::string dev) : dev_(std::move(dev))
	{
	}

	inline operator const std::string &() const
	{
		return this->dev_;
	}
};

/**
 * Mount a file via /dev/loop
 */
class LoMountOp : public Transaction::Op
{
public:
	Dev run(const std::string &path);

	void undo() noexcept;
};

/**
 * Put a filesystem into a vault
 */
class FormatOp : public Transaction::Op
{
public:
	void run(const Dev &dev, const std::string &fs);
	void undo() noexcept;
};

/**
 * Mount a device at a path
 */
class MountOp : public Transaction::Op
{
public:
	void run(const Dev &dev, const std::string &at);

	void undo() noexcept;
};
}
}
