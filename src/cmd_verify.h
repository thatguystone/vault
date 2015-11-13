/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

namespace vault
{
namespace cmd
{

class Verify : public Cmd
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

	void run(std::vector<std::string>)
	{
	}
};
}
}
