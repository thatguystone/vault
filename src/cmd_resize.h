/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

class ResizeCmd : public Cmd
{
public:
	std::string name() const
	{
		return "resize";
	}

	std::string desc() const
	{
		return "resize an existing vault";
	}

	Error run(std::vector<std::string>)
	{
		return Error();
	}
};
