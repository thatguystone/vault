/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include "args.h"
#include "cmd_new.h"

namespace vault
{
namespace cmd
{

std::string New::name() const
{
	return "new";
}

std::string New::desc() const
{
	return "create a new vault";
}

void New::run(std::vector<std::string>)
{
	// Error err;
	// Args args;

	// NewCredentialsArgs cred(&args, false);
	// TCLAP::SwitchArg randomize("r", "randomize", "Randomize the file", args,
	// 						   true);
	// FileArg file(&args);
	// SizeArg size(&args);

	// args.parse(argv);

	// Vault v(file.getValue(), Credentials());

	// return v.create(size.getValue().bytes_, cred.asNewCredentials(),
	// 				randomize.getValue());
}
}
}
