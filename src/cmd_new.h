/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include "cmd.h"

class NewCmd : public Cmd
{
	Error createFile(const std::string &path, off_t len);

public:
	std::string name() const;
	std::string desc() const;
	Error run(std::vector<std::string> args);
};
