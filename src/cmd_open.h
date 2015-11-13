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

class Open : public Cmd
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

	void run(std::vector<std::string>)
	{
	}
};
}
}
