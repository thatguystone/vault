/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

class VerifyCmd : public Cmd
{
public:
	std::string name() const
	{
		return "verify";
	}

	std::string desc() const
	{
		return "verify vault integrity";
	}

	Error run(std::vector<std::string>)
	{
		return Error();
	}
};
