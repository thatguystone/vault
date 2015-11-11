/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "args.h"
#include "cmd.h"
#include "cmd_close.h"
#include "cmd_new.h"
#include "cmd_open.h"
#include "cmd_resize.h"
#include "cmd_verify.h"
#include "std.h"
#include "vault.h"

static up<Cmd> _cmds[] = {
	std::make_unique<CloseCmd>(),  std::make_unique<NewCmd>(),
	std::make_unique<OpenCmd>(),   std::make_unique<ResizeCmd>(),
	std::make_unique<VerifyCmd>(),
};

class Usage : public ArgsOut
{
protected:
	virtual std::string extraUsage()
	{
		return "<action>";
	}

	virtual void printExtras()
	{
		printf("Actions:\n");

		for (auto &cmd : _cmds) {
			printf("   %-8s %s\n", cmd->name().c_str(), cmd->desc().c_str());
		}

		printf("\n");
	}

public:
	[[noreturn]] void showHelp(std::vector<std::string> argv)
	{
		if (argv.size() == 1 || argv[1][0] != '-') {
			argv = std::vector<std::string>({ argv[0], "-h" });
		} else {
			argv = std::vector<std::string>({ argv[0], argv[1] });
		}

		while (true) {
			Args(this).parse(argv);
		}
	}
};

int vault(std::vector<std::string> argv)
{
	if (argv.size() > 1) {
		for (auto &cmd : _cmds) {
			if (cmd->name() == argv[1]) {
				argv[0] += " " + argv[1];
				argv.erase(++argv.begin());

				auto err = cmd->run(std::move(argv));
				if (!err.ok()) {
					printf("Error: %s\n", err.msg_.c_str());
					return 1;
				}

				return 0;
			}
		}
	}

	if (argv.size() > 1 && argv[1] != "help" && argv[1][0] != '-') {
		fprintf(stderr, "Unrecognized action: %s\n\n", argv[1].c_str());
	}

	Usage().showHelp(std::move(argv));
}
