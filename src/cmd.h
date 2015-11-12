/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "error.h"
#include "std.h"
#include "vault.h"

class Cmd
{
public:
	virtual ~Cmd()
	{
	}

	virtual std::string name() const = 0;
	virtual std::string desc() const = 0;
	virtual Error run(std::vector<std::string> argv) = 0;
};

/**
 * Run from any set of arbitrary arguments
 */
Error cmd(std::vector<std::string> argv);
