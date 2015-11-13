/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "fs.h"

namespace vault
{
namespace fs
{

Dev LoMountOp::run(const std::string &)
{
	return Dev("");
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
