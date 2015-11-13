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

namespace vault
{
namespace cmd
{

static up<Cmd> _cmds[] = {
	std::make_unique<Close>(),  std::make_unique<New>(),
	std::make_unique<Open>(),   std::make_unique<Resize>(),
	std::make_unique<Verify>(),
};

class Usage : public args::StdOutput
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
			args::Args(this).parse(argv);
		}
	}
};

void cmd(std::vector<std::string> argv)
{
	if (argv.size() > 1) {
		for (auto &cmd : _cmds) {
			if (cmd->name() == argv[1]) {
				argv[0] += " " + argv[1];
				argv.erase(++argv.begin());

				cmd->run(std::move(argv));
				return;
			}
		}
	}

	if (argv.size() > 1 && argv[1] != "help" && argv[1][0] != '-') {
		fprintf(stderr, "Unrecognized action: %s\n\n", argv[1].c_str());
	}

	Usage().showHelp(std::move(argv));
}
}
}
