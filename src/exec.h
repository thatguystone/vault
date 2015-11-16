/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#pragma once
#include <string>
#include <vector>

namespace vault
{

/**
 * Run the named command, returning combined stdout and stderr
 */
std::string exec(const std::string &cmd, std::vector<std::string> args);
}
