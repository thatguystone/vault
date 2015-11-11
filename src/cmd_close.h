/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

class CloseCmd : public Cmd
{
public:
	std::string name() const
	{
		return "close";
	}

	std::string desc() const
	{
		return "close an opened vault";
	}

	Error run(std::vector<std::string>)
	{
		return Error();
	}
};
