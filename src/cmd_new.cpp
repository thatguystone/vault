/**
 * @copyright 2015 Andrew Stone <a@stoney.io>
 * @license MIT
 */

#include <tclap/CmdLine.h>
#include <fcntl.h>
#include "args.h"
#include "cmd_new.h"

std::string NewCmd::name() const
{
	return "new";
}

std::string NewCmd::desc() const
{
	return "create a new vault";
}

Error NewCmd::createFile(const std::string &path, off_t len)
{
	int ierr;
	Error err;

	int fd = open(path.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	err = OSError(fd, "failed to create vault at %s", path.c_str());

	if (err.ok()) {
		ierr = ftruncate(fd, len);
		err = OSError(ierr, "failed to size vault");
	}

	close(fd);

	return err;
}

Error NewCmd::run(std::vector<std::string> argv)
{
	Error err;

	Args args;
	CredentialArgs cred_args(&args);
	SizeArg size_arg(&args);
	FileArg file_arg(&args);

	args.parse(argv);

	err = this->createFile(file_arg.getValue(), size_arg.getValue().bytes_);

	if (err.ok()) {
	}

	return err;
}
