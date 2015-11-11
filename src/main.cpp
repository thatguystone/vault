/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "vault.h"

int main(int argc, char **argv)
{
	int i;
	std::vector<std::string> args(argc);

	for (i = 0; i < argc; i++) {
		args[i] = argv[i];
	}

	return vault(std::move(args));
}
