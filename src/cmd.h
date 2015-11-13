/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "error.h"
#include "std.h"
#include "vault.h"

namespace vault
{
namespace cmd
{

class Cmd
{
public:
	virtual ~Cmd()
	{
	}

	virtual std::string name() const = 0;
	virtual std::string desc() const = 0;
	virtual void run(std::vector<std::string> argv) = 0;
};

/**
 * Run from any set of arbitrary arguments
 */
void cmd(std::vector<std::string> argv);
}
}
