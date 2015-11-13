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

class Close : public Cmd
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

	void run(std::vector<std::string>)
	{
	}
};
}
}
