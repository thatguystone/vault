/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "exec.h"
#include "fs.h"

namespace vault
{
namespace fs
{

Dev LoMountOp::run(const std::string &path)
{
	auto out = exec("losetup", { "--show", "-f", path });

	this->dev_ = Dev(out);

	return this->dev_;
}

void LoMountOp::undo() noexcept
{
}

void FormatOp::run(const Dev &, const std::string &)
{
}

void FormatOp::undo() noexcept
{
}

void MountOp::run(const Dev &, const std::string &)
{
}

void MountOp::undo() noexcept
{
}
}
}
