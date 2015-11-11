/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

class OpenCmd : public Cmd
{
public:
	std::string name() const
	{
		return "open";
	}

	std::string desc() const
	{
		return "open an existing vault";
	}

	Error run(std::vector<std::string>)
	{
		return Error();
	}
};
