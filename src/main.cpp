/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "cmd.h"

int main(int argc, char **argv)
{
	int i;
	std::vector<std::string> args(argc);

	for (i = 0; i < argc; i++) {
		args[i] = argv[i];
	}

	try {
		auto err = cmd(std::move(args));
		if (!err.ok()) {
			throw err;
		}
	} catch (Error e) {
		printf("Error: %s\n", err.msg_.c_str());
		return 1;
	}

	return 0;
}
