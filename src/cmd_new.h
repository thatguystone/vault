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

class New : public Cmd
{
public:
	std::string name() const;
	std::string desc() const;
	void run(std::vector<std::string> args);
};
}
}
