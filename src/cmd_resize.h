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

class Resize : public Cmd
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

	void run(std::vector<std::string>)
	{
	}
};
}
}
