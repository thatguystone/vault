/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "crypt.h"

namespace vault
{
namespace crypt
{

sp<Creds>
FormatOp::run(const fs::Dev &, const std::string &, const std::string &)
{
	return nullptr;
}

void FormatOp::undo() noexcept
{
}

fs::Dev OpenOp::run(const fs::Dev &, const std::string &, sp<Creds>)
{
	return fs::Dev("");
}

void OpenOp::undo() noexcept
{
}

void UpdateCredsOp::run(const fs::Dev &, sp<Creds>, sp<NewCreds>)
{
}

void UpdateCredsOp::undo() noexcept
{
}
}
}
